/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-11-highBoostFiltering.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��19��
  ��  ��: ʵ�ָ�Ƶ��ǿ�˲���
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/19    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	const string srcWinName = "Source image";
	const string dstWinName = "High-Boosted image";
	Mat srcImg, dstImg;

	/// ����ԭʼͼ��
	string srcImgName;
	cout << "Enter the source image file name: ";
	cin >> srcImgName;
	srcImg = imread(srcImgName);
	cvtColor(srcImg, srcImg, CV_RGB2GRAY);

	/// ��ʾԭʼͼ��
	imshow("Source Image", srcImg);

	///// ����ԭʼͼ�����Ƶͼ��ļ�Ȩϵ��
	//float srcImgCoef = 1.0f, highPassCoef = 0.0f;
	//cout << "Enter the coefficient of source image(>=0): ";
	//cin >> srcImgCoef;
	//cout << "Enter the coefficient of high-pass image(0-1):";
	//cin >> highPassCoef;

	/// ������˹��˹�˲�
	float laplacianMaskData[][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
	Mat laplacianMask(3, 3, CV_32F, &laplacianMaskData[0]);
	filter2D(srcImg, dstImg, CV_8U, laplacianMask);
	imshow("Laplacian Sharpen Image", dstImg);

	/// ������˹��Ƶ��ǿ�˲�
	//dstImg = dstImg + srcImg;
	/** ����ʹ�������ģ���ԭʼͼ�����ģ���� **/
	laplacianMask.at<float>(1, 1) = 9;
	filter2D(srcImg, dstImg, CV_8U, laplacianMask);
	imshow("Laplacian High-Boost Image", dstImg);

	/// ��˹ƽ����Ƶ��ǿ�˲�
	// ��˹ƽ��
	cv::GaussianBlur(srcImg, dstImg, Size(5, 5), 1.5, 1.5);
	// �����˲�
	dstImg = srcImg - dstImg;
	// ��Ƶ��ǿ�˲�
	dstImg = srcImg + 0.6*dstImg;
	imshow("Gaussian High-Boost Image", dstImg);

	cv::waitKey(0);

	return 0;
}