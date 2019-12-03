/************************************************************
Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
�ļ���: // code06-08-findCorners
��  ��: // ���پ����ζ�������骵�
��  ��: // 2014��8��4��
��  ��: // �Ҷ�ͼ��ǵ���
�����б�:
1. void drawCircles(const Mat& maskImg, Mat &dstImg); //��Բ���ƶ˵㵽Ŀ��ͼ��
2. Mat findCorners(const Mat &inImg); //��ȡ�Ҷ�ͼ���еĽǵ�
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

//****************************************************************************
//�������ƣ�void drawCircles(const Mat& maskImg, Mat &dstImg)
//�������ܣ���maskImg�еĽǵ���Ƶ�ͼ��dstImg
//����˵��������1Ϊ��ȡ�Ľǵ�ͼ�񣻲���2ΪĿ��ͼ��
//�� �� ֵ����
//****************************************************************************
void drawCircles(const Mat& maskImg, Mat &dstImg)
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
//�������ƣ�Mat findCorners(const Mat &binaryImg)
//�������ܣ���ȡ�Ҷ�ͼ���еĽǵ�
//����˵��������Ҷ�ͼ��
//�� �� ֵ�����ذ����ǵ�Ķ�ֵͼ��
//****************************************************************************
Mat findCorners(const Mat &inImg)
{
	int i, elemSize = 5;
	Mat dstImg1, dstImg2, diffImg;

	//����ʮ���νṹԪ��
	Mat crossStruct = getStructuringElement( MORPH_CROSS,
									  Size(elemSize, elemSize ),
									  Point(elemSize/2, elemSize/2));
	//���þ��νṹԪ��
	Mat rectStruct = getStructuringElement( MORPH_RECT,
									  Size(elemSize, elemSize ),
									  Point(elemSize/2, elemSize/2));
	//�Զ���X�νṹԪ��
	Mat xStruct(elemSize, elemSize, CV_8U, Scalar(0));
	for(i=0; i<elemSize; i++)
	{
		xStruct.at<uchar>(i, i) = 1;
		xStruct.at<uchar>(4-i, i) = 1;
	}
	//�Զ������νṹԪ��
	Mat diamondStruct(elemSize, elemSize, CV_8U, Scalar(1));
	diamondStruct.at<uchar>(0, 0) = 0;
	diamondStruct.at<uchar>(0, 1) = 0;
	diamondStruct.at<uchar>(1, 0) = 0;
	diamondStruct.at<uchar>(4, 4) = 0;
	diamondStruct.at<uchar>(3, 4) = 0;
	diamondStruct.at<uchar>(4, 3) = 0;
	diamondStruct.at<uchar>(4, 0) = 0;
	diamondStruct.at<uchar>(4, 1) = 0;
	diamondStruct.at<uchar>(3, 0) = 0;
	diamondStruct.at<uchar>(0, 4) = 0;
	diamondStruct.at<uchar>(0, 3) = 0;
	diamondStruct.at<uchar>(1, 4) = 0;

	//��ʮ���νṹԪ������ͼ��
	dilate(inImg, dstImg1, crossStruct);
	//�����νṹԪ�ظ�ʴͼ��
	erode(dstImg1, dstImg1, diamondStruct);

	//��X�νṹԪ������ͼ��
	dilate(inImg, dstImg2, xStruct);
	//�þ��νṹԪ�ظ�ʴͼ��
	erode(dstImg2, dstImg2, rectStruct);

	absdiff(dstImg2, dstImg1, diffImg);

	threshold(diffImg, dstImg1, 60, 255, THRESH_BINARY);

	dstImg2 = inImg;

	drawCircles(dstImg1, dstImg2);

	return dstImg2;
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

	Mat grayImg (srcImg.size(), CV_8U), diaImg;

	//��Դͼ��ת��Ϊ�Ҷ�ͼ��
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	binaryImg = grayImg;

	dstImg = findCorners(binaryImg);

	imshow("Find corners", dstImg);

	waitKey(0);
	return 0;
}
