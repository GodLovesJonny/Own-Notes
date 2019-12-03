/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code08-02-susanCorDet.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��20��
  ��  ��: ʵ��susan�ǵ����㷨��
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���ͷ�  2014/04/20    1.0     �������ļ�
***********************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

void susanArea(Mat src, Mat &dst,int k,int t, int g);
void localMaxDet(Mat src, Mat &dst, int k);

int main( int argc, char** argv )
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName);
	if (img.empty()||img.data==NULL)
	{
		cout << "Failed to load image: " << srcFileName << endl;
		return 1;
	}

	// ת��Ϊ�Ҷ�ͼ��
	if (img.channels()>=3)
		cvtColor(img,img,CV_RGB2GRAY);

	namedWindow("Original Image", CV_WINDOW_AUTOSIZE );
	imshow("Original Image", img );

	int nRad=3;  // ���뾶
	int t=15;    // ���ƶ���ֵ
	int g=20;    // susan�����ֵ

	Mat susImg,susLocMax;
	//dst=Mat::zeros(img.rows,img.cols,CV_32F);
	susanArea(img,susImg,nRad,t,g);       // ���Ǳ�ڵ�suan��ѡ��
	//imwrite("f:\\susImg.bmp",susImg);
	localMaxDet(susImg,susLocMax,nRad);   // �ֲ�����ֵ���

	namedWindow("Susan Image", CV_WINDOW_AUTOSIZE );
	imshow("Susan Image", susImg );

	// �ڽǵ���Χ�����
	for( int i = 0; i < susLocMax.rows ; i++ )
	{ 
		for( int j = 0; j < susLocMax.cols; j++ )
		{
			if( susLocMax.at<char>(i,j) > 0 )
			{
				circle(img, Point( j, i ), 5, Scalar(0), 2, 8, 0 );
			}
		}
	}
	imshow("Susan Corner Image", img );
	waitKey(0);
  return(0);
}

void susanArea(Mat src, Mat &dst,int k,int t, int g)
{
	int nHeight=src.rows,nWidth=src.cols;
	int i,j,m,n;

	// susan�㷨��������Χ
	dst=Mat::zeros(nHeight,nWidth,CV_32F);

	// ����ͼ���ÿһ���㣬������susan���
	for (i=k;i<nHeight-k;i++)
	{
		for (j=k;j<nWidth-k;j++)
		{
			for (m=-k;m<=k;m++)
			{
				for (n=-k;n<=k;n++)
				{
					if ((m*m+n*n<=k*k) && abs(src.at<char>(i+m,j+n)-src.at<char>(i,j))<=t)
						dst.at<float>(i,j)=dst.at<float>(i,j)+1;	
				}
			}
		}	
	}
	// ���susan���С��һ����ֵ������Ϊ���ǽǵ��ѡ�㣬������Ϊ0
	for (i=k;i<nHeight-k;i++)
	{
		for (j=k;j<nWidth-k;j++)
		{
			if (dst.at<float>(i,j)<=g && dst.at<float>(i,j)>0)
				dst.at<float>(i,j)=g-dst.at<float>(i,j);
			else
				dst.at<float>(i,j)=0;
		}
	}
}

void localMaxDet(Mat src, Mat &dst,int k)
{
	int i,j,flag;
	int m,n;
	dst=Mat::zeros(src.rows,src.cols,CV_8UC1);
	for (i=k;i<src.rows-k;i++)
	{
		for (j=k;j<src.cols-k;j++)
		{
			// ȡ����ǰ���ֵ
			if (src.at<float>(i,j)<=13)
				continue;
			flag=1;
			
			// ������ǰ���3*3���򣬼�����Ƿ��Ǽ���ֵ
			for (m=-2;m<=2;m++)
			{
				for (n=-2;n<=2;n++)
				{
					if (!(m==0 && n==0) && src.at<float>(i+m,j+n)>=src.at<float>(i,j))
					{
						flag=0;
						break;
					}
				}
				if (flag==0)
					break;
			}

			// �����־δ�仯��������õ��Ǽ���ֵ��
			if (flag==1)
				dst.at<char>(i,j)=1;
		}
	}
	imwrite("f:\\susImg3.bmp",dst);
}