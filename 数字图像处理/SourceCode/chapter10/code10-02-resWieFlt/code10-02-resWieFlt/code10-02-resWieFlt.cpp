/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code10-02-resWieFlt.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��19��
  ��  ��: ʵ��wiener�˲�ͼ��ԭ
  ��  ��:
  �����б�:
    1. void creKerTemp() �������ģ��
	2  void creNoiTemp() ��������ģ��
    3. void fftConImg()  Ƶ���н��и���ҶΪ��
    4. void wieFilt() Ƶ���н������˲�����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���ͷ�   2014/04/18   1.0   �������ļ�
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

void creKerTemp(Mat &dst,int nHeight, int nWidth, int nTemSize);
void creNoiTemp(Mat &dst,int nHeight, int nWidth);
void fftConImg(Mat *imgDFT,Mat *kerTempDFT, Mat *conImg);
void wieFilt(Mat *imgDFT,Mat *kerTempDFT, Mat *noiTempDFT,Mat *dst);

int main( int argc, char** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	// ת��Ϊ�Ҷ�ͼ��
	if (img.channels()>=3)
		cvtColor(img,img,CV_RGB2GRAY);

	int nHeight=img.rows;
	int nWidth=img.cols;

	namedWindow("Original Image",CV_WINDOW_AUTOSIZE);
	imshow("Original Image",img);  // ��ʾԭʼͼ��
	waitKey();


	// ����ģ��ͼ��
	int nkerSize=9;
	Mat kerTemp;
	creKerTemp(kerTemp,nHeight,nWidth,nkerSize);

	// ��ԭͼ����и���Ҷ�任
	Mat img2;
	img2=img.clone();
	img2=Mat_<float>(img2);
	
	// ����һ��һά���飬����Ԫ�أ�ÿ��Ԫ�ص�������Mat
	Mat planesImg[]={img2,Mat::zeros(img2.size(),img2.type())};
	Mat complexImg;
	merge(planesImg,2,complexImg);
	dft(complexImg,complexImg);	
	split(complexImg, planesImg);  
     
	// ��ģ����и���Ҷ�任
	Mat planesKerTemp[]={kerTemp,Mat::zeros(kerTemp.size(),kerTemp.type())};
	Mat complexKerTemp;
	merge(planesKerTemp,2,complexKerTemp);
	dft(complexKerTemp,complexKerTemp);
	split(complexKerTemp,planesKerTemp);

	// ��Ƶ��ͼ��ľ�����
	Mat conImg;
	Mat planesTmp[]={Mat::zeros(kerTemp.size(),kerTemp.type()),Mat::zeros(kerTemp.size(),kerTemp.type())};
	fftConImg(planesImg,planesKerTemp,planesTmp);	
    merge(planesTmp, 2, complexImg);
	idft(complexImg,complexImg);
	split(complexImg,planesTmp);

	// ��������ģ��
	Mat noiTemp;
	creNoiTemp(noiTemp,nHeight,nWidth);  // ��������ģ��

	add(planesTmp[0],noiTemp,img2);  // �˲�������������	
	normalize(img2, img2, 0, 1, CV_MINMAX); // �淶��ͼ��
	namedWindow("Convulution+noise Image",CV_WINDOW_AUTOSIZE);
	imshow("Convulution+noise Image",img2);  // ��ʾͼ����ģ����Ľ��
	waitKey();

	// �Ծ��ģ����������ͼ����и���Ҷ�任
	planesImg[0]=img2;
	planesImg[1]=Mat::zeros(img2.size(),img2.type());
	merge(planesImg,2,complexImg);
	dft(complexImg,complexImg);	
	split(complexImg, planesImg);

	// ������ģ����и���Ҷ�任
	Mat planesNoiTemp[]={noiTemp,Mat::zeros(noiTemp.size(),noiTemp.type())};
	Mat complexNoiTemp;
	merge(planesNoiTemp,2,complexNoiTemp);
	dft(complexNoiTemp,complexNoiTemp);
	split(complexNoiTemp,planesNoiTemp);


	// ����ά���˲���������ͼ��ԭ
	Mat planesWieFilt[]={Mat::zeros(noiTemp.size(),noiTemp.type()),Mat::zeros(noiTemp.size(),noiTemp.type())};
	wieFilt(planesImg,planesKerTemp,planesNoiTemp,planesWieFilt);  // wiener�˲����planesInvFilt��ΪƵ��

	merge(planesWieFilt, 2, complexImg); // ���ݺϲ���������и���Ҷ�任
	idft(complexImg,complexImg);         // �渵��Ҷ�任
	split(complexImg,planesWieFilt);

	normalize(planesWieFilt[0], img2, 0, 1, CV_MINMAX); // �淶��ͼ��

	namedWindow("Wiener Filter Image Restoration",CV_WINDOW_AUTOSIZE);
	imshow("Wiener Filter Image Restoration",img2);  // ��ʾͼ����ģ����Ľ��

    waitKey();
	return 0;
}

/*************************************************
  ��    ��: void creKerTemp(Mat &dst,int nHeight, int nWidth��int nTemSize)
  ��    ��: ����һ��ģ��ͼ��
  ���ú���: ��
  ��������: ������
  �������: nHeight--> ͼ��߶�
			nWidth --> ͼ����  
			nTemSize --> ģ��ߴ�
  �������: dst--> ���ͼ��
  ��    ��: ��
  ��    ��: ��
  ��    �ڣ�2014��4��20��
*************************************************/
void creKerTemp(Mat &dst,int nHeight, int nWidth,int nkerSize)
{
	int i,j;
	float nCoef = 1.0/(nkerSize*nkerSize);
	dst = Mat::zeros(nHeight,nWidth, CV_32FC1);

	for (i=0;i<nkerSize;i++)
	{
		for(j=0;j<nkerSize;j++)
		{
			dst.at<float>(i,j)=nCoef;
		}
	}	

}

/*************************************************
  ��    ��: void fftConImg(Mat *imgDFT,Mat *kerTempDFT, Mat *conImg)
  ��    ��: ��Ƶ���ͼ����о���������ΪƵ��
  ���ú���: ��
  ��������: ������
  �������: imgDFT --> ͼ��ĸ���Ҷ�任
            kerTempDFT --> ģ��ĸ���Ҷ�任
  �������: conImg --> Ƶ�������
  ��    ��: ��
  ��    ��: ��
  ��    �ڣ�2014��4��20��
*************************************************/
void fftConImg(Mat *imgDFT,Mat *kerTempDFT, Mat *conImg)
{
	int nHeight,nWidth;
	nHeight=imgDFT[0].rows;
	nWidth=imgDFT[0].cols;

	//conImg=Mat::zeros(nHeight,nWidth,CV_32F);
	double a,b,c,d;
	int i,j;
	for (i=0;i<nHeight;i++)
	{
		for (j=0;j<nWidth;j++)
		{
			a=imgDFT[0].at<float>(i,j);             // ͼ����Ҷ�任�����ʵ��
			b=imgDFT[1].at<float>(i,j);  
			c=kerTempDFT[0].at<float>(i,j);         // ģ�帵��Ҷ�任�����ʵ�����鲿
			d=kerTempDFT[1].at<float>(i,j);

			conImg[0].at<float>(i,j)=a*c-b*d;
			conImg[1].at<float>(i,j)=a*d+b*c;
		}
	}

}

/*************************************************
  ��    ��: void wieFilt(Mat *imgDFT,Mat *kerTempDFT,Mat *noiTempDFT, Mat *dst)
  ��    ��: ��Ƶ���ͼ��������˲��������ΪƵ��
  ���ú���: ��
  ��������: ������
  �������: imgDFT --> ͼ��ĸ���Ҷ�任
            kerTempDFT --> ģ��ĸ���Ҷ�任
            noiTempDFT --> �����ĸ���Ҷ�任
  �������: dst --> ά���˲���Ƶ������
  ��    ��: ��
  ��    ��: ��
  ��    �ڣ�2014��4��20��
*************************************************/
void wieFilt(Mat *imgDFT,Mat *kerTempDFT, Mat *noiTempDFT,Mat *dst)
{
	int nHeight,nWidth;
	nHeight=imgDFT[0].rows;
	nWidth=imgDFT[0].cols;

	double a,b,c,d,e,f,multi;
	int i,j;
	for (i=0;i<nHeight;i++)
	{
		for (j=0;j<nWidth;j++)
		{
			a=imgDFT[0].at<float>(i,j);                 // ͼ����Ҷ�任��ʵ�����鲿
			b=imgDFT[1].at<float>(i,j);  
			c=kerTempDFT[0].at<float>(i,j);             // ģ�帵��Ҷ�任��ʵ�����鲿
			d=kerTempDFT[1].at<float>(i,j);
			e=noiTempDFT[0].at<float>(i,j);             // ��������Ҷ�任��ʵ�����鲿
			f=noiTempDFT[1].at<float>(i,j);

			multi=(a*a+b*b)/(a*a+b*b-e*e-f*f);
			if(c*c+d*d>0.003)
			{	
				dst[0].at<float>(i,j)=(a*c+b*d)/(c*c+d*d)/multi;
				dst[1].at<float>(i,j)=(b*c-a*d)/(c*c+d*d)/multi;
			}
			else
			{
				dst[0].at<float>(i,j)=a;
				dst[1].at<float>(i,j)=b;
			}
		}
	}
}
/*************************************************
  ��    ��: void creNoiTemp(Mat &dst,int nHeight, int nWidth)
  ��    ��: ����һ������ͼ��
  ���ú���: ��
  ��������: ������
  �������: nHeight--> ͼ��߶�
			nWidth --> ͼ����
  �������: dst--> ���ͼ��
  ��    ��: ��
  ��    ��: ��
  ��    �ڣ�2014��4��20��
*************************************************/
void creNoiTemp(Mat &dst,int nHeight, int nWidth)
{
	int i,j;
	dst = Mat::zeros(nHeight,nWidth, CV_32FC1);

	for (i=0;i<nHeight;i++)
	{
		for(j=0;j<nWidth;j++)
		{
			if((i+j)==((int) (i+j)/8)*8)
			{
				dst.at<float>(i,j)=-16;
			}
		}
	}	

}