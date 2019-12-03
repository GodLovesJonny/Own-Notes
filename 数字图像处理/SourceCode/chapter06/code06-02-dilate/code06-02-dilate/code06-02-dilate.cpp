/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: // code06-02-dilate
  ��  ��: // ���پ����ζ�������骵�
  ��  ��: // 2014��8��4��
  ��  ��: // ��ֵͼ�����������OpenCVʵ��
  �����б�:
    1. void Dilation( int, void* ); //�Զ�ֵͼ�������������
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

// ȫ�ֱ���
Mat srcImg, binaryImg, dilationDst;

int dilationElem = MORPH_RECT;
int dilationSize = 1;
int const maxElem = 2;
int const maxKernelSize = 21;

void Dilation( int, void* )
{
  int elemType = MORPH_RECT;
  if( dilationElem == 0 ){ elemType = MORPH_RECT; }
  else if( dilationElem == 1 ){ elemType = MORPH_CROSS; }
  else if( dilationElem == 2) { elemType = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( elemType,
                                       Size( 2*dilationSize + 1, 2*dilationSize+1 ),
                                       Point( dilationSize, dilationSize ) );
  //���Ͳ���
  dilate( binaryImg, dilationDst, element );
  imwrite("..\\figures\\figure6_9_c.bmp", dilationDst);
  imshow( "Dilation", dilationDst );
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

  // ������ʾ����
  namedWindow( "Dilation", CV_WINDOW_AUTOSIZE );

  // ����������
  createTrackbar( "�ṹԪ��", "Dilation",
                  &dilationElem, maxElem,
                  Dilation );

  createTrackbar( "��С", "Dilation",
                  &dilationSize, maxKernelSize,
                  Dilation );

  Dilation( 0, 0 );

  waitKey(0);
  return 0;
}
