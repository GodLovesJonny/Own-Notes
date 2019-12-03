/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-04-createAndDrawHistogram.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��1��
  ��  ��: ��������ʾ�Ҷ�ֱ��ͼ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/01     1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the image file name: ";
	cin >> srcFileName;
	Mat srcImg;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// ת��Ϊ�Ҷ�ͼ��
	Mat grayImg;
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);

	/// ����ͨ��
	vector<Mat> bgrImg;
	split(srcImg, bgrImg);


	/**
	void calcHist(const Mat* images, int nimages, const int* channels,
	              InputArray mask, OutputArray	hist,
				  int dims, const int* histSize, const float** ranges,
				  bool uniform=true, bool accumulate=false )
	* images - Դͼ������
	* nimages - Դͼ����Ŀ
	* channels - ���ڼ���ֱ��ͼ��ͨ���б���0��ʼ��ţ�
	* mask - ��ģͼ�����0Ԫ�ض�Ӧ��ͼ�����ؽ����ڼ���ֱ��ͼ��
	* hist - ֱ��ͼ��dimsά����
	* dims - ֱ��ͼ��ά�������ֵΪCV_MAX_DIMS��ĿǰΪ32��
	* histSize - ֱ��ͼ��ά�Ĵ�С���������
	* ranges - ֱ��ͼ��ά��������ֵ��Χ���������ȷֲ�����ÿάֻ������ֵ��
	* uniform - ֱ��ͼ����Ƿ���ȷֲ��ı�־
	* accumulate - �Ƿ��ۼ���ǰ��ֱ��ͼ
	*/
	/// ����ֱ��ͼ
	Mat bHist, gHist, rHist, grayHist;	// ֱ��ͼ����
	int histSize = 256;					// ֱ��ͼ�������
	float range[] = {0, 255};
	const float* histRange = {range};	// ֱ��ͼ��ά��������ֵ��Χ
	calcHist(&grayImg, 1, 0, Mat(), grayHist, 1, &histSize, &histRange, true, false);	// ����ֱ��ͼ
	//calcHist(&bgrImg[0], 1, 0, Mat(), bHist, 1, &histSize, &histRange, true, false);	// ��ɫͨ��ֱ��ͼ
	//calcHist(&bgrImg[1], 1, 0, Mat(), gHist, 1, &histSize, &histRange, true, false);	// ��ɫͨ��ֱ��ͼ
	//calcHist(&bgrImg[2], 1, 0, Mat(), rHist, 1, &histSize, &histRange, true, false);	// ��ɫͨ��ֱ��ͼ

	// Ҳ���������������졢�̡���ͨ����ֱ��ͼ���������ͨ��
	const int channels[] = {0, 1, 2}; // ����Ϊ�����̡������ɫͨ������
	calcHist(&srcImg, 1, &channels[0], Mat(), bHist, 1, &histSize, &histRange, true, false);	// ��ɫͨ��ֱ��ͼ
	calcHist(&srcImg, 1, &channels[1], Mat(), gHist, 1, &histSize, &histRange, true, false);	// ��ɫͨ��ֱ��ͼ
	calcHist(&srcImg, 1, &channels[2], Mat(), rHist, 1, &histSize, &histRange, true, false);	// ��ɫͨ��ֱ��ͼ

	/// ����ֱ��ͼͼ��
	int histWidth = 512;	// ֱ��ͼ���
	int histHeight = 400;	// ֱ��ͼ�߶�
	int binWidth = cvRound((double)histWidth / histSize);	// �����
	Mat histImg(histHeight, histWidth, CV_8UC3, Scalar(0,0,0));

	/// �淶��ֱ��ͼ
	normalize(grayHist, grayHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// ����ֱ��ͼ
	normalize(bHist, bHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// ��ɫͨ��ֱ��ͼ
	normalize(gHist, gHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// ��ɫͨ��ֱ��ͼ
	normalize(rHist, rHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// ��ɫͨ��ֱ��ͼ

	/// ����ֱ��ͼ
	for(int i = 1; i < histSize; i++)
	{
		// ����ֱ��ͼ
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(grayHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
			Scalar(128, 128, 128), 2, 8, 0);

		// ��ɫͨ��ֱ��ͼ
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(bHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(bHist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

		// ��ɫͨ��ֱ��ͼ
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(gHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(gHist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);

		// ��ɫͨ��ֱ��ͼ
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(rHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(rHist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// ��ʾԭʼͼ���ֱ��ͼ
	const string srcWinName = "Source image";
	const string histWinName = "Gray histogram";
	namedWindow(srcWinName);
	imshow(srcWinName, srcImg);
	namedWindow(histWinName);
	imshow(histWinName, histImg);

	waitKey(0);

	return 0;
}