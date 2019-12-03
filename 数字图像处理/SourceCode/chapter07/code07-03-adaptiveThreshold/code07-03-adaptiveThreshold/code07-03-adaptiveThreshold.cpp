/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-03-adaptiveThreshold.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��24��
  ��  ��: �ֿ�����Ӧ��ֵ�ָ��̬��ֵ����
  �����б�:
    1. double otsuThreshold() Otsu��ֵ��
    2. void blockAdaptiveThreshold() �ֿ�����Ӧ��ֵ�ָ�
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

/// Otsu��ֵ
double otsuThreshold(const Mat &grayImg);

/// �ֿ�����Ӧ��ֵ�ָ�
void blockAdaptiveThreshold(const Mat &srcImg, Mat& dstImg, Size blockSize = Size(64, 64));

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

	/// ����Ӧ��ֵ�ָ�
	/**
		void adaptiveThreshold(InputArray src, OutputArray dst, double maxValue, int adaptiveMethod,
				int thresholdType, int blockSize, double C)
	*/
	//adaptiveThreshold(srcImg, dstImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);
	//adaptiveThreshold(srcImg, dstImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 5);

	/// �ֿ�����Ӧ��ֵ�ָ�
	blockAdaptiveThreshold(srcImg, dstImg);

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
 * �ֿ�����Ӧ��ֵ�ָ�
 * @param srcImg ԭʼͼ��
 * @param dstImg �ָ���
 * @param blockSize �ֿ�ߴ�
 */
void blockAdaptiveThreshold(const Mat &srcImg, Mat& dstImg, Size blockSize)
{
	dstImg = srcImg.clone();

	/// ����ֿ���Ŀ
	int xBlocks = (srcImg.cols + blockSize.width - 1) / blockSize.width;
	int yBlocks = (srcImg.rows + blockSize.height - 1) / blockSize.height;

	/// �����ֵ�ָ�
	for(int y = 0; y < yBlocks-1; y++)
	{
		for(int x = 0; x < xBlocks-1; x++)
		{
			// ��÷ֿ�����
			Mat blockImg = dstImg(Rect(x * blockSize.width, y * blockSize.height, blockSize.width, blockSize.height));

			// ����ֿ���ֵ
			double t = otsuThreshold(blockImg);

			// ʵ�ַֿ�ָ�
			threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
		}
	}
	for(int x = 0; x < xBlocks-1; x++)
	{// �²�ֿ�
		// ��÷ֿ�����
		Mat blockImg = dstImg(Rect(x * blockSize.width, (yBlocks - 1) * blockSize.height, blockSize.width, dstImg.rows - (yBlocks - 1) * blockSize.height));

		// ����ֿ���ֵ
		double t = otsuThreshold(blockImg);

		// ʵ�ַֿ�ָ�
		threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
	}
	for(int y = 0; y < yBlocks-1; y++)
	{// �Ҳ�ֿ�
		// ��÷ֿ�����
		Mat blockImg = dstImg(Rect((xBlocks - 1) * blockSize.width, y * blockSize.height, dstImg.cols - (xBlocks - 1) * blockSize.width, blockSize.height));

		// ����ֿ���ֵ
		double t = otsuThreshold(blockImg);

		// ʵ�ַֿ�ָ�
		threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
	}
	{// ���½Ƿֿ�
		// ��÷ֿ�����
		Mat blockImg = dstImg(Rect((xBlocks - 1) * blockSize.width, (yBlocks - 1) * blockSize.height,
			dstImg.cols - (xBlocks - 1) * blockSize.width, dstImg.rows - (yBlocks - 1) * blockSize.height));

		// ����ֿ���ֵ
		double t = otsuThreshold(blockImg);

		// ʵ�ַֿ�ָ�
		threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
	}
}
