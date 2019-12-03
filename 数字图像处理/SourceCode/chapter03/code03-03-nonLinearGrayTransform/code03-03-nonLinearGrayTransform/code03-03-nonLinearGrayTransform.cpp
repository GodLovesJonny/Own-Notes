/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-03-nonLinearGrayTransform.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��6��28��
  ��  ��: ���ö���������ָ���������ݺ�����ʵ�ַ����ԻҶȱ任��
  ��  ��:
  �����б�:
    1. bool nonLinearGrayTransform() ѭ��ƽ�ƾ������������
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/06/28    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

/**
  �Ҷȱ任��������
 */
class GrayTransform
{
public:
	virtual uchar operator()(uchar gray)
	{
		return 0;
	}
	virtual ~GrayTransform()
	{
	}
};

/**
  �����任�����ࣺy = a + log(x+1)/(c*logb)
  �磺a = 0, b = 2.0, c = 8/255 = 0.03137
 */
class LogGrayTransform : public GrayTransform
{
public:
	LogGrayTransform(float fA, float fB, float fC) : a(fA), b(fB), c(fC)
	{
	}

	uchar operator()(uchar gray)
	{
		return uchar(a + logf(gray + 1.0f) / (c * logf(b)));
	}
private:
	/// �����任��������
	float a, b, c;
};

/**
  ָ���任�����ࣺy = b^[c*(x-a)] - 1
  �磺a = 0, b = 2.0, c = 8/255 = 0.03137
 */
class ExpGrayTransform : public GrayTransform
{
public:
	ExpGrayTransform(float fA, float fB, float fC) : a(fA), b(fB), c(fC)
	{
	}

	uchar operator()(uchar gray)
	{
		return uchar(powf(b, c * (gray - a)) - 1.0f);
	}
private:
	/// ָ���任��������
	float a, b, c;
};

/**
  ���ɱ任�����ࣺy = c * x^gama
  �磺gama = 0.4, c = 255^(1-gama) = 27.792
 */
class PowGrayTransform : public GrayTransform
{
public:
	PowGrayTransform(float fC, float fGama) : c(fC), gama(fGama)
	{
	}

	uchar operator()(uchar gray)
	{
		return uchar(c * powf(gray, gama));
	}
private:
	/// ���ɱ任��������
	float c, gama;
};

/*************************************************
  ��    ��: bool nonLinearGrayTransform(const Mat& srcImg, Mat& dstImg, GrayTransform& grayFunc)
  ��    ��: ����ָ���ĻҶȱ任����ʵ�ַ����ԻҶȱ任
  ���ú���: ��(������OpenCV������C++����)
  ��������: main()
  �������: const Mat& srcImg-->����ͼ��CV_8U��
            GrayTransform& grayFunc-->�����ԻҶȱ任�ĺ������󣬿ɳ䵱����uchar (*pF)(uchar gray)�ĺ���
  �������: Mat& dstImg-->���ͼ��CV_8U��
  ��    ��: �ɹ��򷵻�true�����򷵻�false
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��6��28��
*************************************************/
bool nonLinearGrayTransform(const Mat& srcImg, Mat& dstImg, GrayTransform& grayFunc)
{
	/// Ϊ�˼򻯴��룬��ʱֻ�����޷����ַ���ͼ��
	if(srcImg.empty() || srcImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return false;
	}
	
	/// ͼ��ͨ����������������
	int iChns, iRows, iCols;
	iChns = srcImg.channels();
	iRows = srcImg.rows;
	iCols = srcImg.cols * iChns;
	
	if(srcImg.isContinuous())
	{
		iCols *= iRows;
		iRows = 1;
	}

	/// ��ʼ��Ŀ��ͼ��
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());

	/// ʵʩ�����ԻҶȱ任
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = grayFunc(p[c]);
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;
	
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// ����ԭʼͼ��
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// ������Ҫ�任�ĺ������ͼ����Ӧ����
	int funcType = 1;
	float a, b, c;
	GrayTransform *pGrayFunc = NULL;
	cout << "Enter the transform function type: \n"
		<< "1-Logarithmic function;\n"
		<< "2-Exponential function;\n"
		<< "3-Power function;\n: ";
	cin >> funcType;
	switch (funcType)
	{
	case 1:
		cout << "Enter parameters a, b and c in logarithmic function y = a + log(x+1)/(c*logb): ";
		cin >> a >> b >> c;
		pGrayFunc = new LogGrayTransform(a, b, c);
		break;
	case 2:
		cout << "Enter parameters a, b and c in exponential function y = b^[c*(x-a)] - 1: ";
		cin >> a >> b >> c;
		pGrayFunc = new ExpGrayTransform(a, b, c);
		break;
	case 3:
		cout << "Enter parameters c and gama in power function y = c * x^gama: ";
		cin >> a >> b;
		pGrayFunc = new PowGrayTransform(a, b);
		break;
	default:
		pGrayFunc = new GrayTransform();
		break;
	}
	
	/// ��ʾԭʼͼ��
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// �Ҷ����Ա任
	nonLinearGrayTransform(srcImg, dstImg, *pGrayFunc);

	/// ��ʾ�任ͼ��
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	/// ���ٺ�������
	delete pGrayFunc;

	return 0;
}

