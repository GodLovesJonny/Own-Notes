/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-09-imageSmoothing.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��12��
  ��  ��: ʵ��ͼ��ƽ����������ֵ�˲���˫���˲�����˹�˲�����ֵ�˲�����
  ��  ��:
  �����б�:
    1. void onKernelSizeTrackbar() �������Ļص�����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/12    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// ��������
const string szWinName = "Smoothing Demo";

// ƽ�������뾶
const int maxKernelRadius = 15;

// ƽ���˰뾶
int kernelRadius = 1;

// ƽ������
char smoothMethod = 'M';

/// ƽ���˳ߴ�Ļص�����
void onKernelSizeTrackbar(int iVal, void* pUserData);

int main(int argc, char** argv)
{
	cout << "�÷���H - ��ֵ�˲���B - ˫���˲���G - ��˹�˲���M - ��ֵ�˲���Esc - �˳���" << endl;

	/// ����ԭʼͼ��
	Mat srcImg;
	string srcImgName;
	cout << "Enter the source image file name: ";
	cin >> srcImgName;
	srcImg = imread(srcImgName);
	if(srcImg.empty())
	{
		cerr << "Failed to load source image." << endl;
		return -1;
	}

	/// ��ʾԭʼͼ��
	namedWindow(szWinName);
	imshow(szWinName, srcImg);

	/// ����ƽ���˳ߴ�Ļ���
	createTrackbar("ƽ���˰뾶", szWinName, &kernelRadius, maxKernelRadius, onKernelSizeTrackbar, &srcImg);

	/// ���ݰ����趨ƽ������
	int keyVal = 0;
	while(1)
	{
		keyVal = waitKey(0);
		if(keyVal == 27)
		{
			break;
		}

		switch (keyVal)
		{
		case 'H':
		case 'h':
			smoothMethod = 'H';
			break;
		case 'B':
		case 'b':
			smoothMethod = 'B';
			break;
		case 'G':
		case 'g':
			smoothMethod = 'G';
			break;
		case 'M':
		case 'm':
			smoothMethod = 'M';
			break;
		default:
			smoothMethod = 'H';
			break;
		}

		/// ��ʾƽ�����
		onKernelSizeTrackbar(kernelRadius, &srcImg);
	}

	return 0;
}

/**
* ƽ���˰뾶����Ļص�����
* @param iVal ����λ�ã���0��ʼ��������ƽ���˰뾶
* @param pUserData �����¼����û����ݣ��˴�����ԭͼ�����
*/
void onKernelSizeTrackbar(int iVal, void* pUserData)
{
	/// �ӻ����¼����û������л�ȡԭͼ������
	Mat* pSrcImg = (Mat*)pUserData;
	if(pSrcImg == NULL)
	{
		return;
	}

	/// ����ԭͼ�����ݴ���Ŀ��ͼ��
	Mat dstImg = pSrcImg->clone();

	/// ƽ���˴��ڳߴ�
	int kernelSize = 2 * iVal + 1;

	/// ���ݰ���ѡ����Ӧƽ������ʵ��ͼ��ƽ��
	switch (smoothMethod)
	{
	case 'H':	// Homogeneous Blur����ֵ�˲���
		blur(*pSrcImg, dstImg, Size(kernelSize, kernelSize), Point(-1, -1));
		putText(dstImg, "Homogeneous Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break;
	case 'B':	// Bilateral Blur��˫���˲���
		bilateralFilter(*pSrcImg, dstImg, kernelSize, kernelSize*2, kernelSize/2); 
		putText(dstImg, "Bilateral Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break; 
	case 'G':	// Gaussian Blur����˹ƽ����
		GaussianBlur(*pSrcImg, dstImg, Size(kernelSize, kernelSize), 0, 0);
		putText(dstImg, "Gaussian Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break;
	case 'M':	// Median Blur����ֵ�˲���
		medianBlur(*pSrcImg, dstImg, kernelSize);
		putText(dstImg, "Median Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break;
	}

	/// ��ʾƽ�����
	imshow(szWinName, dstImg);
}