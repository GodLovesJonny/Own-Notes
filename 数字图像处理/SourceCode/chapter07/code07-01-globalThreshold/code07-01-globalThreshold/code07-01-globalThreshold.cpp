/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-01-globalThreshold.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��24��
  ��  ��: ʵ��ȫ����ֵ�ָ����������ֵ����Otsu��ֵ����p������ֵ����
  �����б�:
    1. double iterativeThreshold() ������ֵ��
    2. double otsuThreshold() Otsu��ֵ��
    3. double pTileThreshold() p������ֵ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/24     1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// ������ֵ
double iterativeThreshold(const Mat &grayImg);

/// Otsu��ֵ
double otsuThreshold(const Mat &grayImg);

/// p������ֵ��
double pTileThreshold(const Mat &grayImg, double objRatio);

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;
	
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// ����ԭʼͼ��
	srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	///// ������ֵ��������ֵ����
	//double th = iterativeThreshold(srcImg);

	/// ������ֵ��Otsu��ֵ����
	//double th = otsuThreshold(srcImg);

	/// ������ֵ��p-������ֵ����
	double th = pTileThreshold(srcImg, 0.65);

	cout << "�ָ���ֵ��" << th << endl; 

	/// ��ֵ�ָ�
	threshold(srcImg, dstImg, th, 255, THRESH_BINARY);
	
	/// ��ʾԭʼͼ��
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// ��ʾ�ָ�ͼ��
	namedWindow("Segmented Image");
	imshow("Segmented Image", dstImg);

	waitKey();

	return 0;
}

/**
 * ���õ�����ֵ������Ҷ�ͼ�����ֵ
 * @param grayImg ��ͨ���Ҷ�ͼ��
 * @return �Ҷ���ֵ
 */
double iterativeThreshold(const Mat &grayImg)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format." << endl;
		return 0;
	}

	/// ����Ҷ�ֱ��ͼ
	const int histSize[] = {256};
	float grayRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges};
	int histChannels[] = {0};
	Mat_<float> histImg;
	calcHist(&grayImg, 1, histChannels, Mat(), histImg, 1, histSize, histRanges, true, false);

	/// ����ͼ���е���С�����Ҷ�ֵ
	int minGray = 0, maxGray = 255;
	for(minGray = 0; minGray < 256 && histImg(minGray) <= 0; minGray++);
	for(maxGray = 255; maxGray > minGray && histImg(maxGray) <= 0; maxGray--);

	/// ���ͼ��ֻ��1�ֻ�2�ֻҶȣ���ֱ�ӷ�����С�Ҷ�
	if(maxGray - minGray < 2)
	{
		return minGray;
	}

	/// ����С�����Ҷ�ֵ���м�ֵ��Ϊ��ʼ��ֵ
	double thOld = (minGray + maxGray) / 2;

	/// ���������ֵ
	double thNew = thOld;
	double leftMean, rightMean, leftSum, rightSum;
	bool bDone = false;
	while(!bDone)
	{
		// ������һ������ֵ��ͼ��ָ�Ϊ�����ֲ���������ֵĻҶȾ�ֵ
		leftMean = 0;
		leftSum = 0;
		int i = minGray;
		for(; i <= thOld; i++)
		{
			leftSum += histImg(i);
			leftMean += histImg(i) * i;
		}
		leftMean /= leftSum;

		rightMean = 0;
		rightSum = 0;
		for(; i <= maxGray; i++)
		{
			rightSum += histImg(i);
			rightMean += histImg(i) * i;
		}
		rightMean /= rightSum;

		// ���������ֵĻҶȾ�ֵ������ֵ
		thNew = (leftMean + rightMean) / 2.0;

		bDone = abs(thNew - thOld) < 1.0;
		thOld = thNew;
	}

	return thOld;
}

/**
 * ����Otsu��ֵ���������䷽�����򷨣�����Ҷ�ͼ�����ֵ
 * @param grayImg ��ͨ���Ҷ�ͼ��
 * @return �Ҷ���ֵ
 */
double otsuThreshold(const Mat &grayImg)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format." << endl;
		return 0;
	}

	/// ����Ҷ�ֱ��ͼ
	const int histSize[] = {256};
	float grayRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges};
	int histChannels[] = {0};
	Mat_<float> histImg;
	calcHist(&grayImg, 1, histChannels, Mat(), histImg, 1, histSize, histRanges, true, false);

	/// ֱ��ͼ��һ������������ػҶȵĳ��ָ��ʣ�
	int totPixels = grayImg.rows * grayImg.cols;
	histImg /= totPixels;

	/// ����ͼ���е���С�����Ҷ�ֵ
	int minGray = 0, maxGray = 255;
	for(minGray = 0; minGray < 256 && histImg(minGray) <= 0; minGray++);
	for(maxGray = 255; maxGray > minGray && histImg(maxGray) <= 0; maxGray--);

	/// ���ͼ��ֻ��1�ֻ�2�ֻҶȣ���ֱ�ӷ�����С�Ҷ�
	if(maxGray - minGray < 2)
	{
		return minGray;
	}

	/// �������ͼ���ƽ���Ҷ�ֵ
	double ut = 0;
	for(int i = minGray; i <= maxGray; i++)
	{
		ut += i * histImg(i);
	}

	/// ����ȡ[minGray, maxGray]�е�ֵΪ�Ҷ���ֵ�����ʹ��䷽��������ֵ
	double u0 = 0, u1 = 0, w0 = 0, w1 = 0, sigma = 0, maxSigma = 0, th = minGray;
	for(int i = minGray; i < maxGray; i++)
	{
		// ��1���ֵĻҶȸ��ʺͻҶȾ�ֵ��Ϊ�򻯼��㣬�˴��ĻҶȾ�ֵû�г��ԻҶȸ��ʣ�
		w0 += histImg(i);
		u0 += histImg(i) * i;

		// ��2���ֵĻҶȸ��ʺͻҶȾ�ֵ��Ϊ�򻯼��㣬�˴��ĻҶȾ�ֵû�г��ԻҶȸ��ʣ�
		w1 = 1.0 - w0;
		u1 = ut - u0;

		// ������䷽��
		sigma = w0 * w1 * pow(u0/w0 - u1/w1, 2);

		// ��������䷽����Ӧ�ĻҶ���ֵ
		if(sigma > maxSigma)
		{
			maxSigma = sigma;
			th = i;
		}
	}

	return th;
}

/**
 * ����p-����������Ҷ�ͼ�����ֵ
 * @param grayImg ��ͨ���Ҷ�ͼ��
 * @param objPercent ǰ�����أ��ٶ�Ϊ�߻Ҷȣ�ռ���������ı�����0��1��
 * @return �Ҷ���ֵ
*/
double pTileThreshold(const Mat &grayImg, double objRatio)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format." << endl;
		return 0;
	}

	/// ����Ҷ�ֱ��ͼ
	const int histSize[] = {256};
	float grayRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges};
	int histChannels[] = {0};
	Mat_<float> histImg;
	calcHist(&grayImg, 1, histChannels, Mat(), histImg, 1, histSize, histRanges, true, false);

	/// ͼ����������
	int totPixels = grayImg.rows * grayImg.cols;

	/// �����������ռ�ȴﵽָ��ֵ���򷵻ص�ǰ�Ҷ�ֵΪ�ָ���ֵ
	double backRatio = 1.0 - objRatio;
	double backPixels = 0;
	int i;
	for(i = 0; i < 255 && (backPixels <= backRatio * totPixels); i++)
	{
		backPixels += histImg(i);
	}

	return i;
}
