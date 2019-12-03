/**
 * 
 * @file HistogramEqualization.cpp
 * @author ������(longmansheng@126.com)
 * @date 2014-7-1
 * @note ʾ������ֻ��ԻҶ�ͼ�����ֱ��ͼ���⻯����ϣ���Բ�ɫͼ�����ֱ��ͼ���⻯��
 * ���������split����������ɫͨ��������equalizeHist�����ֱ�Ը�����ɫͨ������ֱ��ͼ
 * ���⻯��Ȼ����merge����������ͨ���ϲ�Ϊ��ɫͼ��
 */
/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-05-histgromEqualization.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��1��
  ��  ��: ʵ��ֱ��ͼ���⻯
  ��  ��:
  �����б�:
    1. void createGrayHistogram() �����Ҷ�ֱ��ͼ
    2. void drawHistogram  ���ƻҶ�ֱ��ͼ
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/01    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

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

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;

	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load source image." << endl;
		return -1;
	}

	/// ת��Ϊ�Ҷ�ͼ��
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// ֱ��ͼ���⻯
	equalizeHist(srcImg, dstImg);

	/// ��ʾԭʼͼ������⻯���
	const string srcWinName = "Source image";
	const string dstWinName = "Equalized image";
	namedWindow(srcWinName);
	namedWindow(dstWinName);
	imshow(srcWinName, srcImg);
	imshow(dstWinName, dstImg);

	/// �������⻯ǰ��ĻҶ�ֱ��ͼ
	Mat srcGrayHist, dstGrayHist;
	createGrayHistogram(srcImg, srcGrayHist);
	createGrayHistogram(dstImg, dstGrayHist);

	/// ��ʾԭʼͼ������⻯�����ֱ��ͼ
	const string srcHistWinName = "Histogram of source image";
	const string dstHistWinName = "Histogram of equalized image";
	namedWindow(srcHistWinName);
	namedWindow(dstHistWinName);
	drawHistogram(srcHistWinName, srcGrayHist);
	drawHistogram(dstHistWinName, dstGrayHist);

	waitKey(0);

	return 0;
}