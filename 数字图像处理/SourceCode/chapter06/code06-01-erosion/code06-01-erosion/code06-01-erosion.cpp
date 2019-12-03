/************************************************************
Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
�ļ���: // code06-01-erosion
��  ��: // ���پ����ζ�������骵�
��  ��: // 2014��8��4��
��  ��: // ��ֵͼ��ʴ�����OpenCVʵ��
�����б�:
1. void Erosion( int, void* ); //�Զ�ֵͼ����и�ʴ����
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat srcImg, binaryImg, erosionDst;

int erosionElem = MORPH_RECT;
int erosionSize = 1;
int const maxElem = 2;
int const maxKernelSize = 21;

void Erosion( int, void* )
{
	int elemType = MORPH_RECT;

	if( erosionElem == 0 ){ elemType = MORPH_RECT; }
	else if( erosionElem == 1 ){ elemType = MORPH_CROSS; }
	else if( erosionElem == 2) { elemType = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( elemType,
		Size( 2*erosionSize + 1, 2*erosionSize+1 ),
		Point( erosionSize, erosionSize ) );

	// ��ʴ����
	erode( binaryImg, erosionDst, element);
	imwrite("..\\figures\\figure6_7_b.bmp", erosionDst);
	imshow( "Erosion", erosionDst );
}

int main( int argc, char** argv )
{
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

	// ������ʾ����
	namedWindow( "Erosion", CV_WINDOW_AUTOSIZE );

	// ����������
	createTrackbar( "�ṹԪ��", "Erosion",
		&erosionElem, maxElem,
		Erosion );

	createTrackbar( "��С", "Erosion",
		&erosionSize, maxKernelSize,
		Erosion );

	Erosion( 0, 0 );

	waitKey(0);
	return 0;
}
