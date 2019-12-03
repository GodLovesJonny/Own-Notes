/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-06-edgeDetect.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��5��
  ��  ��: ʵ�ֱ�Ե��⣺�ݶ����ӡ���˹��������˹���ӡ�Canny���ӡ�
  ��  ��:
  �����б�:
    1. void edgeRober() Robert��Ե���
    2. void edgePrewitt() Prewitt��Ե���
    3. void edgeCanny() Canny��Ե���
    4. void edgeLog() LOG��Ե���
	5. void edgeSobel() Sobel��Ե���
	6. void edgeKrisch() Krisch��Ե���
	7. void onThresholdTrackbar() ��ֵ����Ļص�����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/05    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int g_threshold = 50;

/**
 * Robert��Ե���
 * @param src ����ͼ��
 * @param dst ���ͼ��Rorbert��Ե����
 */
void edgeRobert(Mat src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);

	/// ˮƽ�����ݶ�
	Mat gradX, absGradX;
	Mat kernelX = (Mat_<int>(2, 2) << 0, -1, 1, 0);
	filter2D(src, gradX, CV_16S, kernelX, Point(0, 0));
	convertScaleAbs(gradX, absGradX);

	/// ��ֱ�����ݶ�
	Mat gradY, absGradY;
	Mat kernelY = (Mat_<int>(2, 2) << -1, 0, 0, 1);	
	filter2D(src, gradY, CV_16S, kernelY, Point(0, 0));
	convertScaleAbs(gradY, absGradY);

	/// ���ݶ�
	//cv::sqrt(gradX * gradX + gradY * gradY, grad);
	//cv::convertScaleAbs(grad, grad);

	/// �����ݶ�
	Mat grad;
	addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	dst = grad;
}

/* Prewitt��Ե���
 * @param src ����ͼ��
 * @param dst ���ͼ��Prewitt��Ե����
 */
void edgePrewitt(Mat src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);

	/// ˮƽ�����ݶ�
	Mat gradX, absGradX;
	Mat kernelX = (Mat_<int>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
	filter2D(src, gradX, CV_16S, kernelX);
	convertScaleAbs(gradX, absGradX);

	/// ��ֱ�����ݶ�
	Mat gradY, absGradY;
	Mat kernelY = (Mat_<int>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
	filter2D(src, gradY, CV_16S, kernelY);
	convertScaleAbs(gradY, absGradY);

	/// ���ݶ�
	//cv::sqrt(gradX * gradX + gradY * gradY, grad);
	//cv::convertScaleAbs(grad, grad);

	/// �����ݶ�
	Mat grad;
	addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	dst = grad;	
}

/* Canny��Ե���
 * @param src ����ͼ��
 * @param dst ���ͼ��canny��Ե�����
 * @param lowThresh ����ֵ
 * @param ratio ����ֵ�����ֵ֮�ȣ�����ȡ2��3��
 */
void edgeCanny(Mat src, Mat& dst, double lowThresh, double ratio = 3)
{
	CV_Assert(src.depth() == CV_8U);

	/// ͼ��ƽ��
	Mat edge;
	cv::blur(src, dst, Size(3, 3));

	/// Ӧ��Canny����
	cv::Canny(dst, dst, lowThresh, lowThresh * ratio, 3);
}

/*
 * LOG��Ե���
 * @param src ����ͼ��
 * @param dst ���ͼ��LOG��Ե�����
 * @param kernelSize ƽ���˳ߴ磨Ĭ��Ϊ3��
 * @param sigma ��˹ƽ���˵ľ����Ĭ��Ϊ0��
 * @note ���õ�5*5LOG���ӣ�
		-2, -4, -4, -4, -2,
		-4,  0,  8,  0, -4,
		-4,  8, 24,  8, -4,
		-4,  0,  8,  0, -4,
		-2, -4, -4, -4, -2
 */
void edgeLog(Mat src, Mat& dst, int kernelSize = 5, double sigma = 0)
{
	CV_Assert(src.depth() == CV_8U);

	/// ���ø�˹�˲�����
	Mat smooth;
	GaussianBlur(src, smooth, Size(kernelSize, kernelSize), sigma, sigma);

	/// Ӧ��������˹���ӣ�������ͼ��ΪCV_8U���˴�ʹ��CV_16S�������磩
	Laplacian(smooth, dst, CV_16S, kernelSize);

	/// �����ͼ��ת��ΪCV_8U
	convertScaleAbs(dst, dst);
}

/*
 * Sobel��Ե���
 * @param src ����ͼ��
 * @param dst ���ͼ��Sobel��Ե����
 */
void edgeSobel(Mat src, Mat& dst)
{
	CV_Assert(src.depth() == CV_8U);

	/// ˮƽ�����ݶ�
	Mat gradX, absGradX;
	Sobel(src, gradX, CV_16S, 1, 0);
	cv::convertScaleAbs(gradX, absGradX);

	/// ��ֱ�����ݶ�
	Mat gradY, absGradY;
	cv::Sobel(src, gradY, CV_16S, 0, 1);
	cv::convertScaleAbs(gradY, absGradY);

	/// ���ݶ�
	//cv::sqrt(gradX * gradX + gradY * gradY, grad);
	//cv::convertScaleAbs(grad, grad);

	/// �����ݶ�
	Mat grad;
	addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	dst = grad;
}

/**
 * Krisch��Ե��⣺����8��Krisch��Ե����������ζ�ͼ�����ģ������
 * ȡ���ֵ��Ϊ��Ե����ͼ��������
 * @param src ����ͼ��
 * @param dst ���ͼ��Krisch��Ե����
 */
void edgeKrisch(Mat src, Mat& dst)
{
	CV_Assert(src.depth() == CV_8U);

	Mat grad, gradMax, kernel;

	/// ����1�ľ��
	kernel = (Mat_<int>(3, 3) << 5, 5, 5, -3, 0, -3, -3, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = grad.clone();

	/// ����2�ľ��
	kernel = (Mat_<int>(3, 3) << -3, 5, 5, -3, 0, 5, -3, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// ����3�ľ��
	kernel = (Mat_<int>(3, 3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// ����4�ľ��
	kernel = (Mat_<int>(3, 3) << -3, -3, -3, -3, 0, 5, -3, 5, 5);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// ����5�ľ��
	kernel = (Mat_<int>(3, 3) << -3, -3, -3, -3, 0, -3, 5, 5, 5);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// ����6�ľ��
	kernel = (Mat_<int>(3, 3) << -3, -3, -3, 5, 0, -3, 5, 5, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// ����7�ľ��
	kernel = (Mat_<int>(3, 3) << 5, -3, -3, 5, 0, -3, 5, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// ����8�ľ��
	kernel = (Mat_<int>(3, 3) << 5, 5, -3, 5, 0, -3, -3, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// �����ͼ��ת��ΪCV_8U
	convertScaleAbs(gradMax, dst);
}

/// �Զ�����û���������
struct UserData
{
	string dstWinName; // ��Ե��������
	int keyVal;	// �ַ���ֵ
	Mat srcImg;	// ԭʼͼ��
	Mat dstImg;	// ��Եͼ��
};

/**
 * ��ֵ����Ļص������������û�����ָ���ķ�������ֵ���б�Ե���
 * @param th ��ֵ����λ��ֵ������ֵ
 * @param userdata UserData�͵��û�����ָ�룬�����а�����Ϣ���������ͼ��
 */
static void onThresholdTrackbar(int th, void* userdata)
{
	UserData* pUserData = (UserData*)userdata;
	if(!pUserData)
	{
		return;
	}

	switch (pUserData->keyVal)
	{
	case 'r':
		cout << "Robert edge detector" << endl;
		edgeRobert(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, th, 255, THRESH_TOZERO);
		break;
	case 'p':
		cout << "Prewitt edge detector" << endl;
		edgePrewitt(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 's':
		cout << "Sobel edge detector" << endl;
		edgeSobel(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 'k':
		cout << "Krisch edge detector" << endl;
		edgeKrisch(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 'l':
		cout << "LOG edge detector" << endl;
		edgeLog(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 'c':
		cout << "Canny edge detector" << endl;
		edgeCanny(pUserData->srcImg, pUserData->dstImg, th);
		break;
	default:
		break;
	}

	// ��ʾ��Ե
	imshow(pUserData->dstWinName, pUserData->dstImg);
}

int main(int argc, char** argv)
{
	UserData userData;

	/// ���������Ϣ
	cout << "r - Robert edge detector\n"
		 << "s - Sobel edge detector\n"
		 << "p - Prewitt edge detector\n"
		 << "k - Krisch edge detector\n"
		 << "l - LOG edge detector\n"
		 << "c - Canny edge detector\n"
		 << "Esc - Quit\n";

	/// ����ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	userData.srcImg = imread("rice.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if(userData.srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// ������ʾ����
	const string srcWinName = "Source image";
	userData.dstWinName = "Edge map";
	namedWindow(srcWinName);
	namedWindow(userData.dstWinName, CV_WINDOW_AUTOSIZE);
	
	/// ������ֵ������
	createTrackbar("Threshold:", userData.dstWinName, &g_threshold, 100, onThresholdTrackbar, &userData);

	/// ��ʾԭʼͼ��
	imshow(srcWinName, userData.srcImg);

	/// Ӧ�ñ�Ե�������
	userData.keyVal = 'c';
	while(userData.keyVal != 27)
	{
		// ��Ե���
		onThresholdTrackbar(g_threshold, &userData);

		// ��ȡ��ֵ
		userData.keyVal = waitKey(0);
	}

	waitKey(0);

	return 0;
}

