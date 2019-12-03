/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-07-houghTransform.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��7��
  ��  ��: ����Hough�任���ͼ���е�ֱ�߻�Բ��
  ��  ��:
  �����б�:
    1. void stHoughLine() ��׼Hough�任���ֱ��
    2. void pbHoughLine() ����Hough�任���ֱ��
    3. void stHoughCircle() Hough�任���Բ
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/07    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * ��׼Hough�任���ֱ��
 * @param src ԭʼ�Ҷ�ͼ��(��ͨ����8λ)
 * @param dst �����ͼ��
 * @param accumThreshold �ۼ�����Ԫ����ֵ��ֻ�����ۼ������д��ڸ���ֵ��Ԫ�أ�
 */
void stHoughLine(const Mat& src, Mat& dst, int accumThreshold)
{
	CV_Assert(src.depth() == CV_8U);

	/// ��ʼ�����ͼ��
	cvtColor(src, dst, CV_GRAY2BGR);

	/// ����ָ���ļ��������Ƿֱ��ʽ��б�׼Hough�任���ֱ�ߣ��������rho, theta��������lines��
	vector<Vec2f> lines;
	HoughLines(src, lines, 1, CV_PI/180, accumThreshold, 0, 0);

	/////////////////////////////////////////////
	//   0------> x
	//   |
	//   |
	//   y

	/// �����ͼ���л��Ƽ�⵽��ֱ��
	/** ����һ **/
	//for(size_t i = 0; i < lines.size(); i++)
	//{
	//	float rho = lines[i][0];	// ֱ�߶�Ӧ�ļ���
	//	float theta = lines[i][1];	// ֱ�߶�Ӧ�ļ��ǣ�������x��ļнǣ�

	//	double a = cos(theta);
	//	double b = sin(theta);

	//	// ���õ�ǰֱ�ߵ������˵����꣨�Ա���ʾ��
	//	Point pt1, pt2;
	//	if(fabs(a) < 0.0001)
	//	{
	//		// �����ǽӽ�PI/2ʱ������Ϊˮƽ��
	//		pt1.y = pt2.y = cvRound(rho);
	//		pt1.x = 0;
	//		pt2.x = src.cols;
	//	}
	//	else if(fabs(b) < 0.0001)
	//	{
	//		// �����ǽӽ�0ʱ������Ϊ��ֱ��
	//		pt1.x = pt2.x = cvRound(rho);
	//		pt1.y = 0;
	//		pt2.y = src.rows;
	//	}
	//	else
	//	{
	//		pt1.x = cvRound(rho / a);
	//		pt1.y = 0;
	//		pt2.x = cvRound(rho / a - src.rows * b / a);
	//		pt2.y = src.rows;
	//	}

	//	// ���Ƶ�ǰֱ��
	//	line(dst, pt1, pt2, Scalar(0, 0, 255));
	//}

	/** ������ **/
	int extLineLen = src.rows + src.cols;
	for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + extLineLen*(-b));
        pt1.y = cvRound(y0 + extLineLen*(a));
        pt2.x = cvRound(x0 - extLineLen*(-b));
        pt2.y = cvRound(y0 - extLineLen*(a));
        line( dst, pt1, pt2, Scalar(0,0,255));
    }
}

/**
 * ����Hough�任���ֱ��
 * @param src ԭʼ�Ҷ�ͼ��(��ͨ����8λ)
 * @param dst �����ͼ��
 * @param accumThreshold �ۼ�����Ԫ����ֵ��ֻ�����ۼ������д��ڸ���ֵ��Ԫ�أ�
 * @param minLineLen ��Ҫ������С�߶γ���
 * @param maxLineGap ͬһ�߶��������������
 */
void pbHoughLine(const Mat& src, Mat& dst, int accumThreshold, int minLineLen = 50, int maxLineGap = 10)
{
	CV_Assert(src.depth() == CV_8U);
	cvtColor(src, dst, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(src, lines, 1, CV_PI/180, accumThreshold, minLineLen, maxLineGap);

	for(size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255));
	}
}


/**
 * Hough�任���Բ
 * @param src ԭʼ�Ҷ�ͼ��(��ͨ����8λ)
 * @param dst �����ͼ��
 * @param accumThreshold �ۼ�����Ԫ����ֵ��ֻ�����ۼ������д��ڸ���ֵ��Ԫ�أ�
 * @param minRadius ��Ҫ������СԲ�뾶
 * @param maxRadius ��Ҫ�������Բ�뾶
 */
void stHoughCircle(const Mat& src, Mat& dst, int minRadius, int maxRadius)
{
	CV_Assert(src.depth() == CV_8U);
	cvtColor(src, dst, CV_GRAY2BGR);

	vector<Vec3f> circles;
	HoughCircles(src, circles, CV_HOUGH_GRADIENT, 1, 10, 100, 30, minRadius, maxRadius);

	for(size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		circle(dst, Point(c[0], c[1]), c[2], Scalar(0, 0, 255));
	}
}

int main(int argc, char** argv)
{
	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load source image: " << srcFileName << endl;
		return -1;
	}

	/// Canny��Ե���
	Canny(srcImg, srcImg, 50, 100, 3);
	imshow("Canny edge", srcImg);

	Mat dstImg;

	// ��׼Houghֱ�߼��
	//stHoughLine(srcImg, dstImg, 100);

	// ����Houghֱ�߼��
	pbHoughLine(srcImg, dstImg, 50, 50, 10);

	// HoughԲ���
	stHoughCircle(srcImg, dstImg, 60, 80);

	/// ��ʾ�����
	imshow("Detected Result", dstImg);

	waitKey(0);

	return 0;
}