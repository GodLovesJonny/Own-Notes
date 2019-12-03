/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-06-histogramSpecification.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��3��
  ��  ��: ʵ��ֱ��ͼ�涨����
  ��  ��: ��ӳ�����ӳ�����Ӧ��ʾ��
 	Mat srcHist = Mat::zeros(8, 1, CV_32FC1);
	Mat dstHist = Mat::zeros(8, 1, CV_32FC1);

	// ԭʼֱ��ͼ
	srcHist.at<float>(0) = 0.19;
	srcHist.at<float>(1) = 0.25;
	srcHist.at<float>(2) = 0.21;
	srcHist.at<float>(3) = 0.16;
	srcHist.at<float>(4) = 0.08;
	srcHist.at<float>(5) = 0.06;
	srcHist.at<float>(6) = 0.03;
	srcHist.at<float>(7) = 0.02;

	// �涨ֱ��ͼ
	dstHist.at<float>(3) = 0.2;
	dstHist.at<float>(5) = 0.5;
	dstHist.at<float>(7) = 0.3;

	vector<uchar> grayMap;
	
	// ��ӳ�����
	singleMapping(srcHist, dstHist, grayMap);
	// ��ӳ�����
	groupMapping(srcHist, dstHist, grayMap);
  �����б�:
    1. void HistogramSpecification() ֱ��ͼ�涨��
    2. void createGrayHistogram() �����Ҷ�ֱ��ͼ
    3. void drawHistogram() ���ƻҶ�ֱ��ͼ
    4. void singleMapping() ��ӳ�����
	5. void groupMapping() ��ӳ�����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/03    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// ֱ��ͼ�涨��
void HistogramSpecification(Mat& grayImg, const Mat& dstHist, int mapRule = 0);

/// �����Ҷ�ֱ��ͼ
void createGrayHistogram(Mat srcImg, Mat& grayHist);

/// ���ƻҶ�ֱ��ͼ
void drawHistogram(const string &histWinName, Mat &grayHist);

/// ��ӳ�����
void singleMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap);

/// ��ӳ�����
void groupMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap);

int main(int argc, char** argv)
{
	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// ת��Ϊ�Ҷ�ͼ��
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// �涨ֱ��ͼ
	Mat dstHist = Mat::zeros(256, 1, CV_32FC1);
	for(int i = 0; i < 256; i++)
	{
		dstHist.at<float>(i) = float(i);
	}

	/// ֱ��ͼ�涨��
	Mat dstImg = srcImg.clone();
	HistogramSpecification(dstImg, dstHist);

	/// ��ʾͼ��
	const string srcWinName = "Source image";
	const string dstWinName = "Result image";
	namedWindow(srcWinName);
	imshow(srcWinName, srcImg);
	namedWindow(dstWinName);
	imshow(dstWinName, dstImg);

	/// ��ʾֱ��ͼ
	drawHistogram("Desired histogram", dstHist);
	Mat rstHist;
	createGrayHistogram(dstImg, rstHist);
	drawHistogram("Resulted histogram", rstHist);

	waitKey(0);

	return 0;
}


/**
 * ֱ��ͼ�涨��
 * @param grayImg �涨��ǰΪԭʼ�Ҷ�ͼ�񣬹涨����Ϊ��ǿ�Ҷ�ͼ�񣨵�ͨ���Ҷ�ͼ��
 * @param dstHist ָ����ֱ��ͼ��Ҫ����ԭʼͼ��ĻҶȼ���һ��
 * @param mapRule ӳ�����0-��ӳ�����1-��ӳ�����
 */
void HistogramSpecification(Mat& grayImg, const Mat& dstHist, int mapRule)
{
	if(grayImg.empty() || grayImg.channels() != 1)
	{
		cerr << "Invalid image format." << endl;
		return;
	}

	/// ����ԭʼͼ��ĻҶ�ֱ��ͼ
	Mat srcHist;
	int histSize = 256;			// ֱ��ͼ�������
	float range[] = {0, 255};
	const float* histRange = {range};	// ֱ��ͼ��ά��������ֵ��Χ
	calcHist(&grayImg, 1, 0, Mat(), srcHist, 1, &histSize, &histRange, true, false);

	/// ����ҶȵĶ�Ӧ��ϵ
	vector<uchar> grayMap;
	switch (mapRule)
	{
	case 0:
		singleMapping(srcHist, dstHist, grayMap);
		break;
	case 1:
		groupMapping(srcHist, dstHist, grayMap);
		break;
	default:
		break;
	}

	/// ���лҶ�ӳ��
	uchar *pGray = NULL;
	for(int r = 0; r < grayImg.rows; r++)
	{
		pGray = grayImg.ptr<uchar>(r);
		for(int c = 0; c < grayImg.cols; c++)
		{
			pGray[c] = grayMap[pGray[c]];
		}
	}
}


/**
 * �����Ҷ�ֱ��ͼ
 * @param srcImg ԭʼͼ��
 * @param grayHist �Ҷ�ֱ��ͼ
 */
void createGrayHistogram(Mat srcImg, Mat& grayHist)
{
	///// ת��Ϊ�Ҷ�ͼ��
	//cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// ����Ҷ�ֱ��ͼ
	int histSize = 256;			// ֱ��ͼ�������
	float range[] = {0, 255};
	const float* histRange = {range};	// ֱ��ͼ��ά��������ֵ��Χ
	calcHist(&srcImg, 1, 0, Mat(), grayHist, 1, &histSize, &histRange, true, false);
}

/**
 * ���ƻҶ�ֱ��ͼ
 * @param histWinName ������ʾ�Ҷ�ֱ��ͼ�Ĵ�������
 * @param grayHist �Ҷ�ֱ��ͼ����
 */
void drawHistogram(const string &histWinName, Mat &grayHist)
{
	/// ����ֱ��ͼͼ��
	int histWidth = 512;	// ֱ��ͼ���
	int histHeight = 400;	// ֱ��ͼ�߶�
	int binWidth = cvRound((double)histWidth / grayHist.rows);	// �����
	Mat histImg(histHeight, histWidth, CV_8UC3, Scalar(0,0,0));

	/// �淶��ֱ��ͼ
	normalize(grayHist, grayHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());

	/// ����ֱ��ͼ
	//for(int i = 1; i < grayHist.rows; i++)
	//{
	//	line(histImg, Point(binWidth*(i-1), histHeight-cvRound(grayHist.at<float>(i-1))),
	//		Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
	//		Scalar(128, 128, 128), 2, 8, 0);
	//}
	for(int i = 0; i < grayHist.rows; i++)
	{
		line(histImg, Point(binWidth*(i), histHeight),
			Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	/// ��ʾֱ��ͼ
	imshow(histWinName, histImg);
}

/**
 * ֱ��ͼ�涨���еĵ�ӳ�����
 * @param srcHist ԭʼֱ��ͼ
 * @param dstHist �涨ֱ��ͼ
 * @param grayMap �Ҷ�ӳ���ӳ������
 * @note Ҫ��ԭʼֱ��ͼ��涨ֱ��ͼ������ͬ�ĻҶȼ���Ĭ��Ϊ256��
 */
void singleMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap)
{
	assert(srcHist.rows == dstHist.rows);

	/// ֱ��ͼ���������Ĭ��Ϊ256��
	int histSize = srcHist.rows;

	/// �淶��ֱ��ͼ
	Scalar srcHistTot, dstHistTot;
	srcHistTot = cv::sum(srcHist);
	srcHist /= srcHistTot[0];
	dstHistTot = cv::sum(dstHist);
	dstHist /=  dstHistTot[0];
	
	/// �����ۻ�ֱ��ͼ
	vector<double> srcCumHist(histSize, 0);
	vector<double> dstCumHist(histSize, 0);
	double srcTotVal = 0, dstTotVal = 0;
	for(int i = 0; i < histSize; i++)
	{
		srcTotVal += srcHist.at<float>(i);
		dstTotVal += dstHist.at<float>(i);

		srcCumHist[i] = srcTotVal;
		dstCumHist[i] = dstTotVal;
	}

	/// ����ҶȵĶ�Ӧ��ϵ
	grayMap.assign(histSize, 0);
	double minVal = 0, absDiff = 0;
	int mapVal = 0;
	for(int i = 0; i < histSize; i++)
	{
		minVal = 1.0;
		for(int j = mapVal; j < histSize; j++)
		{
			absDiff = std::abs(dstCumHist[j] - srcCumHist[i]);
			if(absDiff < minVal)
			{
				minVal = std::abs(dstCumHist[j] - srcCumHist[i]);
				mapVal = j;
			}
			else if(absDiff > minVal)
			{
				break;
			}
		}

		grayMap[i] = mapVal;
	}
}

/**
 * ֱ��ͼ�涨���е���ӳ�����
 * @param srcHist ԭʼֱ��ͼ
 * @param dstHist �涨ֱ��ͼ
 * @param grayMap �Ҷ�ӳ���ӳ������
 * @note Ҫ��ԭʼֱ��ͼ��涨ֱ��ͼ������ͬ�ĻҶȼ���Ĭ��Ϊ256��
 */
void groupMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap)
{
	assert(srcHist.rows == dstHist.rows);

	/// ֱ��ͼ���������Ĭ��Ϊ256��
	int histSize = srcHist.rows;

	/// �淶��ֱ��ͼ
	Scalar srcHistTot, dstHistTot;
	srcHistTot = cv::sum(srcHist);
	srcHist /= srcHistTot[0];
	dstHistTot = cv::sum(dstHist);
	dstHist /=  dstHistTot[0];
	
	/// �����ۻ�ֱ��ͼ
	vector<double> srcCumHist(histSize, 0);
	vector<double> dstCumHist(histSize, 0);
	double srcTotVal = 0, dstTotVal = 0;
	for(int i = 0; i < histSize; i++)
	{
		srcTotVal += srcHist.at<float>(i);
		dstTotVal += dstHist.at<float>(i);

		srcCumHist[i] = srcTotVal;
		dstCumHist[i] = dstTotVal;
	}

	/// ����ҶȵĶ�Ӧ��ϵ
	grayMap.assign(histSize, 0);
	double minVal = 0, absDiff = 0;
	int lastIdx = -1, minIdx = 0;
	for(int j = 0; j < histSize; j++)
	{
		if(dstHist.at<float>(j) <= 0)
		{
			continue;
		}

		/// �ҳ�������������СI(l)
		minVal = 1.0;
		for(int i = minIdx; i < histSize; i++)
		{
			absDiff = std::abs(dstCumHist[j] - srcCumHist[i]);
			if(absDiff < minVal)
			{
				minVal = std::abs(dstCumHist[j] - srcCumHist[i]);
				minIdx = i;
			}
			else if(absDiff > minVal)
			{
				break;
			}
		}

		/// ȷ��ӳ���ϵ��[I(l-1), I(l)] --> zl
		for(int k = lastIdx + 1; k <= minIdx; k++)
		{
			grayMap[k] = j;
		}

		/// ����I(l-1)
		lastIdx = minIdx;
	}
}