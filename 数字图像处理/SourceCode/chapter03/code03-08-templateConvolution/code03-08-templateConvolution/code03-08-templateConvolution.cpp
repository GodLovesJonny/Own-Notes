/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-08-templateConvolution.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��12��
  ��  ��: ����filter2D�����ڿռ���ʵ��ģ������
  ��  ��: 
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/11    1.0     �������ļ�
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

	/// ����ģ��ԭ�� 
	Point kernelOrigin;
	cout << "Enter the convolution template origin(x and y): ";
	cin >> kernelOrigin.x >> kernelOrigin.y;

	/// ����ģ��߶�
	Size kernelSize;
	cout << "Enter the convolution template size(w and h): ";
	cin >> kernelSize.width >> kernelSize.height;

	/// ����ģ���������ĸ��
	double kernelScale = 1.0;
	cout << "Enter the kernel scale(denominator): ";
	cin >> kernelScale;

	/// ����ģ��ϵ��
	Mat kernelCoef;
	kernelCoef = Mat::zeros(kernelSize, CV_32F);
	cout << "Enter the kernel coefficients: " << endl;
	for(int r = 0; r < kernelSize.height; r++)
	{
		for(int c = 0; c < kernelSize.width; c++)
		{
			cin >> kernelCoef.at<float>(r, c);
		}
	}

	/// ����ģ��������
	Mat dstImg;
	kernelCoef /= kernelScale;
	filter2D(srcImg, dstImg, -1, kernelCoef, kernelOrigin, 0, BORDER_DEFAULT);

	/// ��ʾ���ǰ���ͼ��
	const string srcWinName = "Source image";
	const string dstWinName = "Result image";
	namedWindow(srcWinName);
	imshow(srcWinName, srcImg);
	namedWindow(dstWinName);
	imshow(dstWinName, dstImg);

	waitKey(0);

	return 0;
}