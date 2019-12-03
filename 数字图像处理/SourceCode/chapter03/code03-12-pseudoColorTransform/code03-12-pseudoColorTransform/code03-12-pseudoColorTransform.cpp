/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-12-pseudoColorTransform.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��19��
  ��  ��: ���ûҶȱ任��ʵ��α��ɫ�任��
  ��  ��:
  �����б�:
    1. void pseudoColorTransform() ���ûҶȱ任��ʵ��α��ɫ�任
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/19    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * ���ûҶȱ任��ʵ��α��ɫ�任
 * @param grayImg �任ǰ��8λ�Ҷ�ͼ��
 * @param colorImg �任���8λα��ɫͼ��
 * ��ɫ�任��if x<L/2, then y=0; else if x<3L/4, then y=4x-2L; else y=L
 * ��ɫ�任��if x<L/4, then y=4x; else if x<3L/4, then y=L; else y=-4x+4L
 * ��ɫ�任��if x<L/4, then y=L; else if x<L/2, then y=-2x+3L/2; else y=0
 */
void pseudoColorTransform(const Mat& grayImg, Mat& colorImg)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return;
	}

	float totL = 255.0f, quadL = totL/4.0f, halfL = totL/2.0f, quad3L = 3.0f*totL/4.0f;

	colorImg = cv::Mat::zeros(grayImg.size(), CV_8UC3);

	const uchar* pGrayData = NULL;
	Vec3b* pColorData = NULL;
	for(int r = 0; r < grayImg.rows; r++)
	{
		pGrayData = grayImg.ptr<uchar>(r);
		pColorData = colorImg.ptr<Vec3b>(r);

		for(int c = 0; c < grayImg.cols; c++)
		{
			if(pGrayData[c] < quadL)
			{
				pColorData[c][0] = totL;					// Blue
				pColorData[c][1] = 4.0f * pGrayData[c];		// Green
				pColorData[c][2] = 0;						// Red
			}
			else if(pGrayData[c] < halfL)
			{
				pColorData[c][0] = -2.0f * pGrayData[c] + totL + halfL;	// Blue
				pColorData[c][1] = totL;								// Green
				pColorData[c][2] = 0;									// Red
			}
			else if(pGrayData[c] < quad3L)
			{
				pColorData[c][0] = 0;				// Blue
				pColorData[c][1] = totL;		// Green
				pColorData[c][2] = 4.0f * pGrayData[c] - 2.0f * totL;	// Red
			}
			else
			{
				pColorData[c][0] = 0;								// Blue
				pColorData[c][1] = 4.0f * (totL - pGrayData[c]);	// Green
				pColorData[c][2] = totL;							// Red
			}
		}
	}
}

int main(int argc, char** argv)
{	
	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	cv::Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// ת��Ϊ�Ҷ�ͼ��
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// ��ʾԭʼͼ��
	imshow("Original Image", srcImg);

	/// α��ɫ�任
	cv::Mat dstImg;
	pseudoColorTransform(srcImg, dstImg);

	/// ��ʾ�任ͼ��
	imshow("Pseudo-Color Image", dstImg);

	waitKey();

	return 0;
}

