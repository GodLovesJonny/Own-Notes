/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: // code06-03-opening
  ��  ��: // ���پ����ζ�������骵�
  ��  ��: // 2014��8��4��
  ��  ��: // ��ֵͼ�������OpenCVʵ��
  �����б�:
    1. void morphologyOpening( int, void* ); //�Զ�ֵͼ��ִ�п�����
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat srcImg, binaryImg, dstImg;

int elemType = MORPH_ELLIPSE;
int morphSize = 8;
const int maxType = MORPH_ELLIPSE;
const int maxKernelSize = 20;

char* windowName = "Morphology Opening";

void morphologyOpening( int, void* )
{
	// �趨�ṹԪ��
	Mat element = getStructuringElement( elemType, Size( 2*morphSize + 1, 2*morphSize+1 ),
																Point( morphSize, morphSize ) );
	// ִ�п�����
	morphologyEx( binaryImg, dstImg, MORPH_OPEN, element );

	imwrite("..\\figures\\figure6_12_a.bmp", dstImg);
	imshow( windowName, dstImg );
}

int main( int argc, char** argv )
{
	/// װ��ͼ��
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

	namedWindow( windowName, CV_WINDOW_AUTOSIZE );

	createTrackbar( "�ṹ����", windowName,
		&elemType, maxType,
		morphologyOpening );

	createTrackbar( "��С", windowName,
		&morphSize, maxKernelSize,
		morphologyOpening );

	morphologyOpening( 0, 0 );

	waitKey(0);
	return 0;
}
