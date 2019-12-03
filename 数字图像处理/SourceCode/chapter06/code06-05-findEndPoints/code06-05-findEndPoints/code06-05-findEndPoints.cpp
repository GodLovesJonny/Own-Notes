/************************************************************
Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
�ļ���: // code06-05-findEndPoints
��  ��: // ���پ����ζ�������骵�
��  ��: // 2014��8��4��
��  ��: // ��ֵ�Ǽ�ͼ��Ķ˵���ȡ
�����б�:
1. void drawEndPoints(const Mat& maskImg, Mat &dstImg); //��Բ���ƶ˵㵽Ŀ��ͼ��
2. Mat findEndPoints(const Mat &binaryImg); //��ȡ��ֵ�Ǽ�ͼ�����ж˵�
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

//****************************************************************************
//�������ƣ�void drawEndPoints(const Mat& maskImg, Mat &dstImg)
//�������ܣ���maskImg�еĶ˵���Ƶ�ͼ��dstImg
//����˵��������1Ϊ��ȡ�Ķ˵��ֵͼ�񣻲���2ΪĿ��ͼ��
//�� �� ֵ����
//****************************************************************************
void drawEndPoints(const Mat& maskImg, Mat &dstImg)
{
	uchar pixel;
	for(int i=0; i<maskImg.rows; i++)
		for(int j=0; j<maskImg.cols; j++)
		{
			pixel = maskImg.at<uchar>(i, j);
			if(pixel==255)
				circle(dstImg, Point(j, i), 5, Scalar(255,255,255));
		}
}

//****************************************************************************
//�������ƣ�Mat findEndPoints(const Mat &binaryImg)
//�������ܣ����û��л�����������ȡ��ֵ�Ǽ�ͼ�����ж˵�
//����˵���������ֵͼ�񣬸�ͼ���б���ɫΪ0��ǰ��ɫ(Ŀ��)Ϊ255
//�� �� ֵ�����ذ������ж˵�Ķ�ֵͼ��
//****************************************************************************
Mat findEndPoints(const Mat &binaryImg)
{
	int i, elemSize = 3;
	Mat dstImg;
	Mat maskImg = Mat::zeros(binaryImg.size(), CV_8UC1);
	/**���ֽṹԪ��
	  1 1 1		1 1 0	0 0 0	0 1 1
	  1 0 1		1 0 0	1 0 1	0 0 1
	  0 0 0		1 1 0	1 1 1	0 1 1  **/
	//�����϶˵�3��3�ṹԪ��S2
	Mat elemStruct(elemSize, elemSize, CV_8U, Scalar(1));
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(2, i) = 0;
	dstImg = 255 - binaryImg;
	erode(dstImg, dstImg, elemStruct);
	maskImg = binaryImg&dstImg;

	//������˵�3��3�ṹԪ��S2
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(i, 2) = 0;
	dstImg = 255 - binaryImg;
	erode( dstImg, dstImg, elemStruct);
	maskImg |= binaryImg&dstImg;

	//�����¶˵�3��3�ṹԪ��S2
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(0, i) = 0;
	dstImg = 255 - binaryImg;
	erode( dstImg, dstImg, elemStruct);
	maskImg |= binaryImg&dstImg;

	//�����Ҷ˵�3��3�ṹԪ��S2
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(i, 0) = 0;
	dstImg = 255 - binaryImg;
	erode( dstImg, dstImg, elemStruct);
	maskImg |= binaryImg&dstImg;

	dstImg = binaryImg;
	drawEndPoints(maskImg, dstImg);

	dstImg = 255-dstImg;

	return dstImg;
}

int main( int argc, char** argv )
{
	Mat srcImg, binaryImg, dstImg;

	// װ��ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	Mat grayImg (srcImg.size(), CV_8U);

	//��Դͼ��ת��Ϊ�Ҷ�ͼ��
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	binaryImg = grayImg;

	//��ֵ������
	threshold(grayImg, binaryImg, 100, 255, THRESH_BINARY);

	binaryImg = 255 - binaryImg;

	dstImg = findEndPoints(binaryImg);

	imshow("Find end points", dstImg);

	waitKey(0);
	return 0;
}
