/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: // code06-04-closing
  ��  ��: // ���پ����ζ�������骵�
  ��  ��: // 2014��8��4��
  ��  ��: // ��ֵͼ��������OpenCVʵ��
  �����б�:
    1. void morphologyClosing( int, void* ); //�Զ�ֵͼ��ִ�б�����
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

char* windowName = "Morphology Closing";

void morphologyClosing( int, void* )
{
	// �趨�ṹԪ��
	Mat element = getStructuringElement( elemType, Size( 2*morphSize + 1, 2*morphSize+1 ),
																Point( morphSize, morphSize ) );
	// ִ�б�����
	morphologyEx( binaryImg, dstImg, MORPH_CLOSE, element );

	imwrite("..\\figures\\figure6_12_b.bmp", dstImg);
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
							morphologyClosing );

	createTrackbar( "��С", windowName,
							&morphSize, maxKernelSize,
							morphologyClosing );

	morphologyClosing( 0, 0 );

	waitKey(0);
	return 0;
}
