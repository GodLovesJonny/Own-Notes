/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code08-01-harrisCorDet.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��20��
  ��  ��: ʵ��Harris�ǵ����㷨��
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

using namespace cv;
using namespace std;

/** @function main */
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

	Mat dst, dstNorm,dstNorSca;
	dst = Mat::zeros( img.size(), CV_32FC1 );

	/// Global variables
	int thresh = 120;

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarris( img, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

	/// Normalizing
	normalize( dst, dstNorm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs( dstNorm, dstNorSca );

	/// Drawing a circle around corners
	for( int i = 0; i < dstNorm.rows ; i++ )
	{ 
		for( int j = 0; j < dstNorm.cols; j++ )
		{
			if( (int) dstNorm.at<float>(i,j) > thresh )
			{
				circle( dstNorSca, Point(j, i ), 5,  Scalar(0), 2, 8, 0 );
			}
		}
	}

	imshow("Harris corner detect",dstNorSca);
	imshow("Original Image",img);

	waitKey(0);
	return(0);
}