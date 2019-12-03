/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-08-regionLabel.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��7��
  ��  ��: ����OpenCV��findContours����ʵ�ֶ�ֵͼ���������ȡ��
          ����drawContours�������Ʋ�����������Ӷ�ʵ�������ǹ��ܡ�
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/07    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{
	/// ����ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << srcFileName << endl;
		return -1;
	}

	/// ��ֵ�ָ�
	Mat bwImg;
	cv::threshold(srcImg, bwImg, 50, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	namedWindow("Source image");
	imshow("Source image", bwImg);

	/// ������ȡ
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( bwImg, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	/// �������ƣ���䷽ʽ��
	Mat dstImg = Mat::zeros(bwImg.size(), CV_8UC3);
	if( !contours.empty() && !hierarchy.empty() )
	{
		// ���������������������ɫ���ƣ���䷽ʽ��������
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] )
		{
			Scalar color((rand()&255), (rand()&255), (rand()&255));
			drawContours(dstImg, contours, idx, color, CV_FILLED, 8, hierarchy);
		}
	}
	namedWindow("Labeled image");
	imshow("Labeled image", dstImg);

	waitKey(0);
	return 0;
}
