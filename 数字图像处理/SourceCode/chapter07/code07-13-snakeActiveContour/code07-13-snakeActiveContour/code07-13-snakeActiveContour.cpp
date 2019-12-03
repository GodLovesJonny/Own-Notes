/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-13-snakeActiveContour.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��14��
  ��  ��: ʵ�ֻ����ģ�ͣ�Snakes-Active Contour Model����
          ��ͨ�������Ƴ�ʼ�������ٵ����ݻ�������Ŀ��������
  ��  ��:
  �ο�����:
    1. Kass et al. Snakes: Active Contour Models
	2. Ritwik Kumar, Harvard University 2010 http://seas.harvard.edu/~rkkumar
	3. gujinjin http://blog.csdn.net/gujinjinseu/article/details/28894023
  �����б�:
    1. Mat cycleShift() ѭ��ƽ�ƾ������������
    2. Mat_<T> bilinearInterp() �������ݵ�˫���Բ�ֵ
    3. void onMouseEvent() ����¼��ص�����
    4. void snakeImage() ��ͼ�����Snake��������ģ�ͷָ�
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/14     1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

/// ����������ݶ���
// ����ͼ��
Mat srcImg;
// ���ͼ��
Mat dstImg;
// ͼ���ɰ壨���ڻ�ȡ��ʼ������
Mat maskImg;
// ����ͼ�����ڼ���ͼ��������
Mat workImg;
// ��ʼ���������������ƣ�
vector<Point> initPoints;
// Ŀ�����������������ݻ���
vector<Vec2f> snakePoints;
// ָʾ��ʼ�����Ƿ��趨��ϵı�־
bool bInitialFinished = false;
// ����ͼ�񴰿�����
const string workWinName = "Contour evolution";

/// Snakesģ�Ͳ���
struct SnakeParam
{
	SnakeParam(float _alpha, float _beta, float _gamma, float _kappa, float _wl, float _we, float _wt)
	{
		alpha = _alpha;
		beta = _beta;
		gamma = _gamma;
		kappa = _kappa;
		wl = _wl;
		we = _we;
		wt = _wt;
	}

	float alpha;		// ����ϵ��
	float beta;			// �ն�ϵ��
	float gamma;		// ����
	float kappa;		// ͼ������ϵ��
	float wl;			// ֱ��������Ȩ��
	float we;			// ��Ե������Ȩ��
	float wt;			// �˵�������Ȩ��
};

/*************************************************
  ��    ��: Mat cycleShift(Mat m, int xoff, int yoff)
  ��    ��: ʵ�־�������ѭ��ƽ��ָ��������������
  ���ú���: ��(������OpenCV������C++����)
  ��������: snakeImage()
  �������: Mat m-->�������
            int xoff-->ˮƽ�����ƶ���������������ʾ���ƣ�������ʾ���ƣ�
            int yoff-->��ֱ�����ƶ���������������ʾ���ƣ�������ʾ���ƣ�
  �������: ��
  ��    ��: ƽ�ƺ�ľ���
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��8��14��
*************************************************/
Mat cycleShift(Mat m, int xoff, int yoff)
{
	/// ȷ������������������֮������ж�Ӧ��ϵ
	int pos;
	vector<int> xIdx, yIdx;
	for(int x = 0; x < m.cols; x++)
	{
		pos = (x + xoff) % m.cols;
		pos = pos >= 0 ? pos : pos + m.cols;
		xIdx.push_back(pos);
	}
	for(int y = 0; y < m.rows; y++)
	{
		pos = (y + yoff) % m.rows;
		pos = pos >= 0 ? pos : pos + m.rows;
		yIdx.push_back(pos);
	}

	/// ���ն�Ӧ��ϵ������������
	Mat n(m.size(), m.type());
	for(int y = 0; y < m.rows; y++)
	{
		for(int x = 0; x < m.cols; x++)
		{
			uchar *p = m.ptr<uchar>(y) + x * m.elemSize();
			uchar *q = n.ptr<uchar>(yIdx[y]) + xIdx[x] * m.elemSize();
			for(int k = 0; k < m.elemSize(); k++)
			{
				q[k] = p[k];
			}
		}
	}

	return n;
}

/*************************************************
  ��    ��: template<typename T>
            Mat_<T> bilinearInterp(const Mat_<T>& m, const Mat2f& xy)
  ��    ��: ���һ��ָ���㣬��ͼ�������н���˫���Բ�ֵ
  ���ú���: ��(������OpenCV������C++����)
  ��������: snakeImage()
  �������: const Mat_<T> m-->�������
            const Mat2f& xy-->����ֵ�ĵ����꣨��������ÿ��Ԫ�ص�����ΪVec2f��x��y��
  �������: ��
  ��    ��: ���ֵ�����Ӧ�Ĳ�ֵ�������������Ԫ���������������Ԫ����ͬ��
  ��    ��: T --> ���Ͳ�����ָ���������ݵ����ͣ�Ҫ��֧��+��-��*���������
  ��    �ߣ�������
  ��    �ڣ�2014��8��14��
*************************************************/
template<typename T> Mat_<T> bilinearInterp(const Mat_<T>& m, const Mat2f& xy)
{
	Mat_<T> z(xy.size());

	float u, v;
	int r0, r1, c0, c1;
	for(int i = 0; i < xy.rows; i++)
	{
		c0 = cvFloor(xy(i)[0]);
		r0 = cvFloor(xy(i)[1]);
		u = xy(i)[0] - c0;
		v = xy(i)[1] - r0;

		/// ��ֹ�кš��к�Խ��
		c0 = abs(c0 % m.cols);
		r0 = abs(r0 % m.rows);
		c1 = (c0 + 1) % m.cols;
		r1 = (r0 + 1) % m.rows;

		/// ˫���Բ�ֵ
		z(i) = (1 - v) *((1 - u) * m(r0, c0) + u * m(r0, c1)) + v * ((1 - u) * m(r1, c0) + u * m(r1, c1));
	}

	return z;
}

/*************************************************
  ��    ��: void onMouseEvent(int event, int x, int y, int flags, void *param)
  ��    ��: ����¼��Ļص�������ʵ�������Ƴ�ʼ����������ƶ����ƣ��Ҽ��պ�������
  ���ú���: ��(������OpenCV������C++����)
  ��������: main()
  �������: int event-->����¼�����
            int x-->���ˮƽλ��
            int y-->��괹ֱλ��
            int flags-->����¼���־
            void *param-->��������
  �������: ��
  ��    ��: ��
  ��    ��: 
  ��    �ߣ�������
  ��    �ڣ�2014��8��14��
*************************************************/
void onMouseEvent(int event, int x, int y, int flags, void *param)
{
	if(maskImg.empty() || bInitialFinished)
		return;

	if(x < 0 || x >= maskImg.cols || y < 0 || y >= maskImg.rows)
		return;

	if(event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
	{
		/// ����ƶ�ʱ���洢����������
		 Point pt(x, y);
		if(initPoints.size() == 0)
		{
			initPoints.push_back(pt);
		}
		else
		{
			line(maskImg, initPoints.back(), pt, Scalar(255, 0, 0));
			line(dstImg, initPoints.back(), pt, Scalar(255));
			initPoints.push_back(pt);
			imshow(workWinName, dstImg);
		}
	}
	else if(event == CV_EVENT_RBUTTONDOWN)
	{
		/// �Ҽ��պϳ�ʼ����
		bInitialFinished = true;

		line(maskImg, initPoints.back(), initPoints.front(), Scalar(255, 0, 0));
		line(dstImg, initPoints.back(), initPoints.front(), Scalar(255));
		imshow(workWinName, dstImg);

		/// ��ͼ���ɰ��л�ȡ�����Ƶĳ�ʼ����
		vector<vector<Point> > contours;
		vector<Vec4i> hierachy;
		cv::findContours(maskImg, contours, hierachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		
		/// �洢��ȡ�ľ�ϸ����
		snakePoints.clear();
		int ptNum = contours[0].size();
		snakePoints.reserve(ptNum);
		for(int i = 0; i < ptNum; i++)
		{
			Point &pt = contours[0][i];
			snakePoints.push_back(Vec2f(pt.x, pt.y));
		}
	}	
}

/*************************************************
  ��    ��: void snakeImage(const Mat& image, Mat2f& points, SnakeParam params, int iters)
  ��    ��: ���ڳ�ʼ������ͼ���ϵ������Kass�ȵ�Snakes�����ģ��
  ���ú���: cycleShift()��bilinearInterp()(������OpenCV������C++����)
  ��������: main()
  �������: const Mat& image-->����ͼ��CV_32F��
            Mat2f& points-->��ʼ������Ԫ������ΪVec2f��
            SnakeParam params-->Snakesģ�Ͳ���
            int iters-->����������
  �������: Mat2f& points-->����������Ԫ������ΪVec2f��
  ��    ��: ��
  ��    ��: 
  ��    �ߣ�������
  ��    �ڣ�2014��8��14��
*************************************************/
void snakeImage(const Mat& image, Mat2f& points, SnakeParam params, int iters)
{
	if(image.depth() != CV_32F || image.empty())
	{
		cerr << "Invalid image!" << endl;
		return;
	}

	///=========================================================================
	///        ����ͼ��������eImg = wl*eLine + we*eEdge + wt*eTerm
	///-------------------------------------------------------------------------
	/// ����ֱ������
	Mat eLine = image.clone();

	/// ����˵�����
	Mat cX, cY, cXX, cYY, cXY;
	Mat cxKernel = (Mat_<int>(1, 2) << -1, 1);
	Mat cyKernel = cxKernel.t();
	Mat cxxKernel = (Mat_<int>(1, 3) << 1, -2, 1);
	Mat cyyKernel = cxxKernel.t();
	Mat cxyKernel = (Mat_<int>(2, 2) << 1, -1, -1, 1);
	cv::filter2D(image, cX, CV_32F, cxKernel, Point(0, 0), 0, BORDER_CONSTANT);
	cv::filter2D(image, cY, CV_32F, cyKernel, Point(0, 0), 0, BORDER_CONSTANT);
	cv::filter2D(image, cXX, CV_32F, cxxKernel, Point(-1, -1), 0, BORDER_CONSTANT);
	cv::filter2D(image, cYY, CV_32F, cyyKernel, Point(-1, -1), 0, BORDER_CONSTANT);
	cv::filter2D(image, cXY, CV_32F, cxyKernel, Point(0, 0), 0, BORDER_CONSTANT);
	Mat eTerm(image.size(), CV_32F);
	float Ix, Iy, Ixx, Iyy, Ixy;
	for(int r = 0; r < image.rows; r++)
	{
		for(int c = 0; c < image.cols; c++)
		{
			Ix = cX.at<float>(r, c);
			Iy = cY.at<float>(r, c);
			Ixx = cXX.at<float>(r, c);
			Iyy = cYY.at<float>(r, c);
			Ixy = cXY.at<float>(r, c);

			eTerm.at<float>(r, c) = (Iyy * Ix * Ix - 2.0f * Ixy * Ix * Iy + Ixx * Iy * Iy) / pow(1.0f + Ix * Ix + Iy * Iy, 1.5f);
		}
	}
	
	/// �����Ե����
	Mat eEdge;
	magnitude(cX, cY, eEdge);
	eEdge *= -1;

	/// ����ͼ���������ⲿ���� = ͼ������ + Լ���������˴�δ����Լ��������
	Mat eExt = params.wl * eLine + params.we * eEdge + params.wt * eTerm;

	/// ����ͼ���������ݶȣ�ͼ������
	Mat fX, fY;
	cv::Sobel(eExt, fX, CV_32F, 1, 0);
	cv::Sobel(eExt, fY, CV_32F, 0, 1);

	///=========================================================================
	///              ������Խ�״����
	///-------------------------------------------------------------------------
	float b[5];
	b[0] = b[4] = params.beta;
	b[1] = b[3] = -(params.alpha + 4 * params.beta);
	b[2] = 2 * params.alpha + 6 * params.beta;

	Mat A = Mat::zeros(points.rows, points.rows, CV_32F);
	Mat I = Mat::eye(points.rows, points.rows, CV_32F);
	A = b[0] * cycleShift(I, 0, 2);
	A = A + b[1] * cycleShift(I, 0, 1);
	A = A + b[2] * I;
	A = A + b[3] * cycleShift(I, 0, -1);
	A = A + b[4] * cycleShift(I, 0, -2);

	/// ����������
	Mat Ainv;
	cv::invert(A + params.gamma * I, Ainv);

	///=========================================================================
	///              �������㲢��������
	///-------------------------------------------------------------------------
	/// ����x�����y���꣨��Ϊ��������
	vector<Mat1f> xy;
	cv::split(points, xy);

	/// ��������
	Mat1f ssx, ssy;
	for(int i = 0; i < iters; i++)
	{
		ssx = params.gamma * xy[0] - params.kappa * bilinearInterp<float>(fX, points);
		ssy = params.gamma * xy[1] - params.kappa * bilinearInterp<float>(fY, points);

		/// ����snake����λ��
		xy[0] = Ainv * ssx;
		xy[1] = Ainv * ssy;
		cv::merge(xy, points);
		
		/// ��ʾsnake����λ��
		Mat2i contour = points;
		cvtColor(srcImg, dstImg, CV_GRAY2BGR);
		cv::polylines(dstImg, contour, true, Scalar(255,0,0));
		imshow(workWinName, dstImg);
		waitKey(10);
	}
}

int main(int argc, char **argv)
{
	///=========================================================================
	///              ��ӡ������Ϣ
	///------------------------------------------------------------------------- 
	cout << "�ָ�ǰ����ͼ��������깴������ʼ������\n"
		"\tESC - �˳�Ӧ�ó���\n"
		"\tr - ����������ָ�ԭͼ\n"
		"\tw or SPACE - ��������\n";

	///=========================================================================
	///              ����ͼ��׼������
	///-------------------------------------------------------------------------
	/// ����ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// ���ͼ��ת��Ϊ��ɫ������ͼ��ת��Ϊ�����ͣ�ͼ���ɰ��ʼ��Ϊ0
	cvtColor(srcImg, dstImg, CV_GRAY2BGR);
	srcImg.convertTo(workImg, CV_32F);
	maskImg = Mat::zeros(srcImg.size(), CV_8U);

	///=========================================================================
	///              ��˹�˲�������������ͼ��ָ��Ӱ�죩
	///-------------------------------------------------------------------------
	GaussianBlur(workImg, workImg, Size(3, 3), 1, 1);

	///=========================================================================
	///                        ���ó�ʼ������
	/// ��������ӿ��Ƶ㣬����Ҽ�ֹͣ��ӵ㣬���Ƴ��պ�����
	///-------------------------------------------------------------------------
	namedWindow(workWinName);
	setMouseCallback(workWinName, onMouseEvent);
	imshow(workWinName, dstImg);

	///=========================================================================
	///              ����Snakesģ�Ͳ���
	///-------------------------------------------------------------------------
	SnakeParam snakeParam(0.3f, 0.2f, 1.0f, 0.1f, 0.2f, 0.4f, 0.1f);
	int maxIter = 1000;		// ����������

	///=========================================================================
	///                        ��������¼�
	///-------------------------------------------------------------------------
	bool bLoop = true;
	while(bLoop)
	{
		int c = waitKey(0);
		switch (c)
		{
		case 27:	// �˳�Ӧ�ó���
			bLoop = false;
			break;
		case 'r':	// ����������ָ�ԭͼ
			initPoints.clear();
			snakePoints.clear();
			bInitialFinished = false;
			maskImg = Scalar::all(0);
			cvtColor(srcImg, dstImg, CV_GRAY2BGR);
			imshow(workWinName, dstImg);
			break;
		case 'w':
		case ' ':	// �������
			{
				Mat2f points(snakePoints);	// ���ڵ����鹹�����󣨲��������ݣ�
				snakeImage(workImg, points, snakeParam, maxIter);	
			}
			break;
		default:
			break;
		}
	}

	return 0;
}
