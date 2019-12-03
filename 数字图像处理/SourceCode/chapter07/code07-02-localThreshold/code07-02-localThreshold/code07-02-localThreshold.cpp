/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-02-localThreshold.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��24��
  ��  ��: ʵ�־ֲ���ֵ�ָ�����Ҷȣ��ݶ�ɢ��ͼ�ļ��㡢����
          �Ҷȼ�ƽ���ݶȱ任ֱ��ͼ��
  �����б�:
    1. Mat grayGradient() �����ݶȷ���
    2. Mat grayGradientScatter() ����Ҷȣ��ݶ�ɢ��ͼ
    3. Mat histDivByAverGrad() ���ڻҶȼ�ƽ���ݶȱ任ֱ��ͼ
    4. double otsuThreshold() Otsu��ֵ��
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

/// �����ݶȷ���
Mat grayGradient(const Mat &grayImg);

/// ����Ҷȣ��ݶ�ɢ��ͼ
Mat grayGradientScatter(const Mat &grayImg);

/// ���ڻҶȼ�ƽ���ݶȱ任ֱ��ͼ
Mat histDivByAverGrad(const Mat &scatImg);

/// ����Otsu��ֵ���������䷽�����򷨣�����Ҷ�ͼ�����ֵ
double otsuThreshold(const Mat &histImg);

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

	/// ��ʾԭʼͼ��
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// ����Ҷȣ��ݶ�ɢ��ͼ
	Mat scatImg = grayGradientScatter(srcImg);

	///// ��ʾ�Ҷȣ��ݶ�ɢ��ͼ
	//scatImg.convertTo(dstImg, CV_8U);
	//cv::imshow("Gray-Gradient Scatter Image", dstImg);

	/// ���ڻҶȼ�ƽ���ݶȱ任ֱ��ͼ
	Mat newHist = histDivByAverGrad(scatImg);
	for(int r = 0; r < newHist.rows; r++)
	{
		for(int c = 0; c < newHist.cols; c++)
		{
			cout << newHist.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	cout << endl;

	/// ��ֵ�ָ�
	double th = otsuThreshold(newHist);
	threshold(srcImg, dstImg, th, 255, THRESH_BINARY);
	
	/// ��ʾ�ָ�ͼ��
	namedWindow("Segmented Image");
	imshow("Segmented Image", dstImg);

	waitKey();

	return 0;
}

/**
 * ����Ҷ��ݶȷ���
 * @param grayImg ��ͨ���Ҷ�ͼ��
 * @return �Ҷ��ݶȷ��ȣ������ͣ�
 */
Mat grayGradient(const Mat& grayImg)
{
	double dx, dy;
	int r, c, rows = grayImg.rows - 1, cols = grayImg.cols - 1;

	Mat gradMag = Mat::zeros(grayImg.size(), CV_32F);
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
			dx = grayImg.at<uchar>(r, c+1) - grayImg.at<uchar>(r, c);
			dy = grayImg.at<uchar>(r+1, c) - grayImg.at<uchar>(r, c);

			gradMag.at<float>(r, c) = (float)sqrt(dx * dx + dy * dy);
		}
	}

	return gradMag;
}

/**
 * ����Ҷȣ��ݶȷ���ɢ��ͼ
 * @param grayImg ��ͨ���Ҷ�ͼ��
 * @return �Ҷȣ��ݶȷ���ɢ��ͼ�������ͣ��з���Ϊ�Ҷ��������з���Ϊ�ݶȷ���������
 * @note ��Ҫ��ʾɢ��ͼ����Ӧ����convertToת��ΪCV_8U
 */
Mat grayGradientScatter(const Mat &grayImg)
{
	/// ����Ҷ��ݶȷ���[0-255]
	Mat gradMag = grayGradient(grayImg);

	///// ������С����ݶȷ���
	//double minGrad, maxGrad;
	//cv::minMaxLoc(gradMag, &minGrad, &maxGrad);

	/// ����Ҷȣ��ݶȷ���ֱ��ͼ��ɢ��ͼ��
	Mat images[] = {Mat_<float>(grayImg), gradMag};
	const int histSize[] = {256, 256};
	float grayRanges[] = {0, 256};
	float gradRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges, gradRanges};
	int histChannels[] = {0, 1};
	Mat histImg;
	calcHist(images, 2, histChannels, Mat(), histImg, 2, histSize, histRanges, true, false);

	return histImg;
}

/**
 * ���ڻҶȼ�ƽ���ݶȱ任ֱ��ͼ
 * @param scatImg �Ҷȣ��ݶȷ���ɢ��ͼ�������ͣ��з���Ϊ�Ҷ��������з���Ϊ�ݶȷ���������
 * @return ���ԻҶ�ƽ���ݶȵĻҶ�ֱ��ͼ
 */
Mat histDivByAverGrad(const Mat &scatImg)
{
	/// ������ֱ��ͼ
	Mat histImg = Mat::zeros(scatImg.rows, 1, CV_32F);

	/// ���ڻҶȣ��ݶ�ɢ��ͼ����Ҷ�ƽ���ݶȱ任���ֱ��ͼ
	for(int r = 0; r < scatImg.rows; r++)
	{
		int curGrayPixels = 0;		// ��ǰ�Ҷȵ���������
		float averGrayGrad = 0;		// ��ǰ�Ҷȵ�ƽ���ݶ�
		float pixelVal;
		for(int c = 0; c < scatImg.cols; c++)
		{
			pixelVal = scatImg.at<float>(r, c);

			if(pixelVal > 0)
			{
				curGrayPixels += pixelVal;
				averGrayGrad += c * pixelVal;
			}
		}

		if(curGrayPixels > 0)
		{
			averGrayGrad /= curGrayPixels;	// �Ҷȼ�ƽ���ݶ�

			histImg.at<float>(r) = float(curGrayPixels / (averGrayGrad + 0.1f)); // �����ĸΪ0
		}
	}

	return histImg;
}

/**
 * ����Otsu��ֵ���������䷽�����򷨣�����Ҷ�ͼ�����ֵ
 * @param histImg �Ҷ�ֱ��ͼ
 * @return �Ҷ���ֵ
 */
double otsuThreshold(const Mat &histImg)
{
	/// ֱ��ͼ��һ������������ػҶȵĳ��ָ��ʣ�
	Scalar totPixels = cv::sum(histImg);
	histImg /= totPixels(0);

	/// ����ͼ���е���С�����Ҷ�ֵ
	int minGray = 0, maxGray = 255;
	for(minGray = 0; minGray < 256 && histImg.at<float>(minGray) <= 0; minGray++);
	for(maxGray = 255; maxGray > minGray && histImg.at<float>(maxGray) <= 0; maxGray--);

	/// ���ͼ��ֻ��1�ֻ�2�ֻҶȣ���ֱ�ӷ�����С�Ҷ�
	if(maxGray - minGray < 2)
	{
		return minGray;
	}

	/// �������ͼ���ƽ���Ҷ�ֵ
	double ut = 0;
	for(int i = minGray; i <= maxGray; i++)
	{
		ut += i * histImg.at<float>(i);
	}

	/// ����ȡ[minGray, maxGray]�е�ֵΪ�Ҷ���ֵ�����ʹ��䷽��������ֵ
	double u0 = 0, u1 = 0, w0 = 0, w1 = 0, sigma = 0, maxSigma = 0, th = minGray;
	for(int i = minGray; i < maxGray; i++)
	{
		// ��1���ֵĻҶȸ��ʺͻҶȾ�ֵ��Ϊ�򻯼��㣬�˴��ĻҶȾ�ֵû�г��ԻҶȸ��ʣ�
		w0 += histImg.at<float>(i);
		u0 += histImg.at<float>(i) * i;

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