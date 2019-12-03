/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-11-meanshiftSegmentation.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��9��
  ��  ��: ����Meanshift�㷨��ͼ��ķָ
  ��  ��:
  �ο�����:
    1. OpenCV�������룺samples/cpp/meanshift_segmentation.cpp
  �����б�:
    1. void floodFillSegmentation() ����Meanshift�㷨�����н���������㣩���ͼ������
    2. void meanShiftSegmentation() ����Ļص�������ʵ��Meanshift�ָ�����ʾ�ָ���
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/09    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

/// ����ռ�ĺ˺������ڰ뾶
int spatialRadius = 10;

/// ��ɫ�ռ�ĺ˺������ڰ뾶
int colorRadius = 10;

/// �ָ��������������
int maxPyrLevel = 1;

/// ����ͼ��
Mat srcImg;

/// ���ͼ��
Mat dstImg;

/// ��������
string winName = "Meanshift";

/**
 * ����Meanshift�㷨�����н���������㣩���ͼ������
 * @param img Meanshift�㷨��������
 * @param colorDiff �������ʱ�������ص���ɫ������
 */
static void floodFillSegmentation(Mat& img, const Scalar& colorDiff = Scalar::all(1))
{
	CV_Assert(!img.empty());
	RNG rng = theRNG();
	Mat mask(img.rows+2, img.cols+2, CV_8UC1, Scalar::all(0));
	for(int y = 0; y < img.rows; y++)
	{
		for(int x = 0; x < img.cols; x++)
		{
			if(mask.at<uchar>(y+1, x+1) == 0)
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(img, mask, Point(x,y), newVal, 0, colorDiff, colorDiff);
			}
		}
	}
}

/**
 * ����Ļص�������ʵ��Meanshift�ָ�����ʾ�ָ���
 */
static void meanShiftSegmentation(int, void*)
{
	cout << "spatialRad=" << spatialRadius << "; "
		 << "colorRad=" << colorRadius << "; "
		 << "maxPyrLevel=" << maxPyrLevel << endl;

	pyrMeanShiftFiltering(srcImg, dstImg, spatialRadius, colorRadius, maxPyrLevel);
	floodFillSegmentation(dstImg, Scalar::all(2));

	imshow(winName, dstImg);
}

int main(int argc, char** argv)
{
	/// ����ͼ��
	string fileName;
	cout << "Enter the source image file name: " << endl;
	cin >> fileName;
	srcImg = imread(fileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << fileName << endl;
		return -1;
	}
	imshow("Original image", srcImg);

	/// ��������
	namedWindow(winName);

	/// ������������������Ӧ�Ļص�����
	createTrackbar("spatialRad", winName, &spatialRadius, 80, meanShiftSegmentation);
	createTrackbar("colorRad", winName, &colorRadius, 60, meanShiftSegmentation);
	createTrackbar("maxPyrLevel", winName, &maxPyrLevel, 5, meanShiftSegmentation);

	/// ��ʼ�ָ�
	meanShiftSegmentation(0, 0);

	waitKey();

	return 0;
}