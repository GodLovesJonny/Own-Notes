/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-10-imageSharpen.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��18��
  ��  ��: �ڿռ���ʵ��ͼ���񻯡�
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/18    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	const string srcWinName = "Source image";
	const string dstWinName = "Gradient image";

	/// ����ԭʼͼ��
	Mat srcImg, dstImg, hortGrad, vertGrad;
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
	imshow(srcWinName, srcImg);

	/// Ӧ��Sobelˮƽ�ݶ�����
	cv::Sobel(srcImg, hortGrad, CV_8U, 1, 0, 3, 0.4, 128);
	
	/// ��ʾSobelˮƽ�ݶ���ͼ��
	imshow(dstWinName, hortGrad);
	cout << "Sobel ˮƽ�ݶ�����" << endl;
	waitKey(0);

	/// Ӧ��Sobel��ֱ�ݶ�����
	cv::Sobel(srcImg, vertGrad, CV_8U, 0, 1, 3, 0.4, 128);
	
	/// ��ʾSobel��ֱ�ݶ���ͼ��
	imshow(dstWinName, vertGrad);
	cout << "Sobel ��ֱ�ݶ�����" << endl;
	waitKey(0);

	/// ����L1 norm��L=abs(Gx)+abs(Gy)
	dstImg = abs(hortGrad) + abs(vertGrad);

	/// ��ʾSobel�ݶȷ���ͼ��L1 norm
	imshow(dstWinName, dstImg);
	cout << "Sobel �ݶȷ��ȣ�L1 Norm��" << endl;
	waitKey(0);

	/// �����ݶȷ���L2 norm��L=sqrt(Gx*Gx + Gy*Gy)���ݶȷ���
	cv::Mat gradNorm, gradDir;
	cv::Sobel(srcImg, hortGrad, CV_32F, 1, 0);	// ˮƽģ��
	cv::Sobel(srcImg, vertGrad, CV_32F, 0, 1);	// ��ֱģ��
	cv::cartToPolar(hortGrad, vertGrad, gradNorm, gradDir);	// �����ݶȷ��Ⱥͷ���

	/// ��ʾSobel�ݶȷ���L2 norm
	imshow(dstWinName, gradNorm);
	cout << "Sobel �ݶȷ��ȣ�L2 Norm��" << endl;
	waitKey(0);

	/// ��ʾSobel�ݶȷ���
	imshow(dstWinName, gradDir);
	cout << "Sobel �ݶȷ���" << endl;
	waitKey(0);

	/// Ӧ��Laplacian����
	cv::Laplacian(srcImg, gradNorm, CV_16S, 3);
	cv::convertScaleAbs(gradNorm, dstImg);

	/// ��ʾLaplacian���
	imshow(dstWinName, dstImg);
	cout << "Laplacian ���" << endl;
	waitKey(0);

	/**
	 * ����ͨ��filter2D������ģ�����ķ�ʽʵ��Robert��Prewitt��Sobel��
	 * Krisch��Isotropic Sobel��Laplacian�����ӵ��񻯣���������б�д��
	*/
	return 0;
}