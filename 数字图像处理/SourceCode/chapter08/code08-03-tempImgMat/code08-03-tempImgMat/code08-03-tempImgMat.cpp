/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code08-03-tempImgMat.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��20��
  ��  ��: ʵ��ģ��ͼ��ƥ�䡣
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

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
	Mat img; Mat temp; Mat result;

	int max_Trackbar = 5;

	// װ��ԭͼ���ģ��ͼ��
	string firstFile, secondFile;
	cout << "������ԭͼ���ļ���: ";
	cin >> firstFile;
	cout << "������ģ��ͼ���ļ���: ";
	cin >> secondFile;
	
	img = imread(firstFile);
	if(img.empty())
	{
		cerr << "Failed to load image " << firstFile << endl;
		return -1;
	}

	temp = imread(secondFile);
	if(tmp.empty())
	{
		cerr << "Failed to load image " << secondFile << endl;
		return -1;
	}

	// ת��Ϊ�Ҷ�ͼ��
	if (img.channels()>=3)
		cvtColor(img,img,CV_RGB2GRAY);
	if (temp.channels()>=3)
		cvtColor(temp,temp,CV_RGB2GRAY);

	// ��������
	namedWindow( "Source Image", CV_WINDOW_AUTOSIZE );
	namedWindow( "Result window", CV_WINDOW_AUTOSIZE );	// ������ʾ��ԭͼ��

	Mat imgDis;

	// �����������ľ���
	int resultCols =  img.cols - temp.cols + 1;
	int resultRows = img.rows - temp.rows + 1;

	result.create( resultCols, resultRows, CV_32FC1 );

	/// ����ƥ��ͱ�׼��
	int i;   // i:ģ��ƥ�䷽��
	for (i=0;i<5;i++)
	{
		matchTemplate( img, temp, result, i );
		normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

		// ͨ������ minMaxLoc ��λ��ƥ���λ��
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;

		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		// ���ڷ��� SQDIFF �� SQDIFF_NORMED, ȡ��Сֵ��Ϊƥ��������������������, ���ֵ��Ϊƥ����
		if( i  == CV_TM_SQDIFF || i == CV_TM_SQDIFF_NORMED )
		{
			matchLoc = minLoc; 
		}
		else
		{ 
			matchLoc = maxLoc; 
		}

		img.copyTo( imgDis );

		// ��ʾ���ս��
		rectangle( imgDis, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y + temp.rows ), Scalar::all(255), 2, 8, 0 );
		rectangle( result, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y + temp.rows ), Scalar::all(255), 2, 8, 0 );

		imshow( "Source Image", imgDis );
		imshow( "Result window", result );

		waitKey(0);
	}
	return 0;
}