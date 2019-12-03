/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-07-imageNoise.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��11��
  ��  ��: ��ͼ������Ӽ��Խ����������˹������
  ��  ��:
  �����б�:
    1. void SaltAndPepperNoise() ��ӽ�������
    2. void GaussianNoise() ��Ӹ�˹����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/11    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <ctime>

using namespace std;
using namespace cv;

/// ��ӽ�������
void SaltAndPepperNoise(Mat& ioImg, double noiseDensity = 0.05);

/// ��Ӹ�˹����
void GaussianNoise(Mat& ioImg, double mean = 0.0, double sigma = 5.0);

int main()
{
	/// ָ��ͼ���ļ�
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// ����ԭʼͼ��
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}
	//cvtColor(srcImg, srcImg, CV_RGB2GRAY);

	/// ��ʾԭʼͼ��
	imshow("Original Image", srcImg);

	/// ������������
	Mat saltpepperImg = srcImg.clone();
	SaltAndPepperNoise(saltpepperImg, 0.02);

	/// ��ʾ��������ͼ��
	imshow("Salt & Pepper Noise Image", saltpepperImg);

	/// ������˹����
	Mat gaussianImg = srcImg.clone();
	GaussianNoise(gaussianImg, 0, 25.0);

	/// ��ʾ��˹����ͼ��
	imshow("Gaussian Noise Image", gaussianImg);

	waitKey(0);

	return 0;
}

/**
* ��ͼ����ӽ�������
* @param ioImg ����ǰΪԭʼͼ�񣬴����Ϊ����ͼ��
* @param noiseDensity �����ܶȣ�������ռ�������ı�����Ĭ��Ϊ0.05��
*/
void SaltAndPepperNoise(Mat& ioImg, double noiseDensity)
{
	if(ioImg.empty())
	{
		cerr << "Invalid image format." << endl;
		return;
	}

	/// ���������;�������ͼ��[0, 1.0)
	Mat noiseImg = Mat::zeros(ioImg.size(), CV_32F);
	randu(noiseImg, 0.0f, 1.0f);

	/// ���������ܶȷֱ𴴽��ڵ�ģ��Ͱ׵�ģ��
	Mat blackMask = noiseImg < (noiseDensity / 2.0f);
	Mat whiteMask = noiseImg >= (1.0f - noiseDensity / 2.0f);

	/// ���ݺڵ�ģ��Ͱ׵�ģ��ֱ𽫶�Ӧ������Ϊ��ɫ�������������Ͱ�ɫ����������
	ioImg.setTo(0, blackMask);
	ioImg.setTo(255, whiteMask);

	/**
	����ʹ������Ĵ�����ƴ�������Ĵ��룬��RNG::uniform����������������
	*/

/*
	/// Ϊ�˼򻯴��룬��ʱֻ�����޷����ַ���ͼ��
	if(ioImg.empty() || ioImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return;
	}

	/// ��ʼ�������
	RNG rng(time(NULL));

	/// ��ӽ�������������������
	int r, c;
	double randVal, halfVal = noiseDensity / 2.0;
	switch (ioImg.channels())
	{
	case 1:
		for(r = 0; r < ioImg.rows; r++)
		{
			for(c = 0; c < ioImg.cols; c++)
			{
				// ����[0,1)֮��ľ�������
				randVal = rng.uniform(0.0f, 1.0f);

				if(randVal < halfVal)
				{
					// ��������
					ioImg.at<uchar>(r, c) = 0;
				}
				else if(randVal >= 1.0f - halfVal)
				{
					// ������
					ioImg.at<uchar>(r, c) = 255;
				}
			}
		}
		break;
	case 3:
		for(r = 0; r < ioImg.rows; r++)
		{
			for(c = 0; c < ioImg.cols; c++)
			{
				// ����[0,1)֮��ľ�������
				randVal = rng.uniform(0.0, 1.0);

				if(randVal < halfVal)
				{
					// ��������
					ioImg.at<cv::Vec3b>(r, c) = cv::Vec3b(0, 0, 0);
				}
				else if(randVal < noiseDensity)
				{
					// ������
					ioImg.at<cv::Vec3b>(r, c) = cv::Vec3b(255, 255, 255);
				}
			}
		}
		break;
	default:
		break;
	}
*/
}

/**
* ��ͼ����Ӹ�˹����
* @param ioImg ����ǰΪԭʼͼ�񣬴����Ϊ����ͼ��
* @param mean ������ֵ
* @param sigma ������׼��
*/
void GaussianNoise(Mat& ioImg, double mean, double sigma)
{
	if(ioImg.empty())
	{
		cerr << "Invalid image format." << endl;
		return;
	}

	/// ����ԭʼͼ�񴴽�������ͼ��
	Mat dstImg;
	ioImg.convertTo(dstImg, CV_MAKE_TYPE(CV_64F, ioImg.channels()));

/*
	// ������δ���Ϊ��ͨ���ֱ������˹������������ڶ�ͨ���������ǲ�ɫ��
	/// ָ��������ͨ���ľ�ֵ�ͱ�׼��
	Mat meanMat = Mat::zeros(ioImg.channels(), 1, CV_64FC1);
	Mat sigmaMat = Mat::ones(ioImg.channels(), 1, CV_64FC1);
	meanMat += mean;
	sigmaMat *= sigma;

	/// ���ɸ�˹����
	Mat noiseImg(dstImg.size(), dstImg.type());
	randn(noiseImg, meanMat, sigmaMat);

	/// ����������ͼ��
	dstImg += noiseImg;
*/

	//==============================================
	// ������δ���Ϊ��ͨ��������ͬ�ĸ�˹������������ڶ�ͨ���������ǻ�ɫ��
	/// ���ɸ�˹����
	Mat noiseImg(dstImg.size(), CV_64F);
	randn(noiseImg, mean, sigma);

	/// ����������ͼ���ͨ��
	std::vector<Mat> noiseVec;
	cv::split(dstImg, noiseVec);
	for(int i = 0; i < dstImg.channels(); i++)
	{
		noiseVec[i] += noiseImg;
	}
	cv::merge(noiseVec, dstImg);
	//----------------------------------------------

	/// ת����Ŀ��ͼ��
	dstImg.convertTo(ioImg, ioImg.type());

	/**
	����ʹ������Ĵ�����ƴ�������Ĵ��룬��RNG::gaussian��������˹������
	*/

/*
	/// ����ԭʼͼ�񴴽�������ͼ��
	Mat dstImg;
	ioImg.convertTo(dstImg, CV_MAKE_TYPE(CV_64F, ioImg.channels()));

	/// ��ʼ�������
	RNG rng(time(NULL));

	/// ��Ӹ�˹����
	int y, x;
	double randVal;
	double* pData = NULL;
	switch (dstImg.channels())
	{
	case 1:
		for(y = 0; y < dstImg.rows; y++)
		{
			pData = dstImg.ptr<double>(y);
			for(x = 0; x < dstImg.cols; x++)
			{
				/// ���ɸ�˹����
				randVal = rng.gaussian(sigma);

				/// ���Ӹ�˹����
				pData[x] += mean + randVal;
			}
		}
		break;
	case 3:
		for(y = 0; y < dstImg.rows; y++)
		{
			pData = dstImg.ptr<double>(y);
			for(x = 0; x < dstImg.cols; x++)
			{
				/// ���ɸ�˹����
				randVal = rng.gaussian(sigma);

				/// ���Ӹ�˹����
				randVal += mean;
				pData[0] += randVal;
				pData[1] += randVal;
				pData[2] += randVal;

				pData += 3;
			}
		}
		break;
	default:
		break;
	}

	/// ת����Ŀ��ͼ��
	dstImg.convertTo(ioImg, ioImg.type());
*/
}