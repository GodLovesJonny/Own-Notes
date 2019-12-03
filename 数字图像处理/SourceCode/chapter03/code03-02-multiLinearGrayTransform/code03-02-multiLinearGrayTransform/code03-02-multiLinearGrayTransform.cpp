/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-02-multiLinearGrayTransform.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��6��28��
  ��  ��: ͨ��ָ�����������������Ҷ�ʵ�ֶַλҶ����Ա任���Ҷ����죩
  ��  ��:
  �����б�:
    1. bool multiLinearGrayTransform() �ֶλҶ����Ա任���Ҷ����죩
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/06/28    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


/*************************************************
  ��    ��: bool multiLinearGrayTransform(const Mat& srcImg, Mat& dstImg, 
                 const Point2d& ptFirst, const Point2d& ptSecond)
  ��    ��: ʵ�ֶַλҶ����Ա任���Ҷ����죩
  ���ú���: ��(������OpenCV������C++����)
  ��������: main()
  �������: const Mat& srcImg-->����ͼ��CV_8U��
            const Point2d& ptFirst-->�任ǰ�ĻҶȷ�Χ[a,c]
            const Point2d& ptSecond-->�任��ĻҶȷ�Χ[b,d]
  �������: Mat& dstImg-->���ͼ��CV_8U��
  ��    ��: �ɹ��򷵻�true�����򷵻�false
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��6��28��
*************************************************/
bool multiLinearGrayTransform(const Mat& srcImg, Mat& dstImg, const Point2d& ptFirst, const Point2d& ptSecond)
{
	/// Ϊ�˼򻯴��룬��ʱֻ�����޷����ַ���ͼ��
	if(srcImg.empty() || srcImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return false;
	}
	
	/// �����������������Ҷȼ���Ҷȱ任��ӳ���
	uchar grayMap[256];	// ����Ҷ�-->����Ҷȵ�ӳ���
	int i;
	
	// ��Ե�һ�ν��лҶ�ӳ��
	for(i = 0; i <= ptFirst.x; i++)
	{
		if(ptFirst.x > 0)
		{
			grayMap[i] = uchar(ptFirst.y * i / ptFirst.x);
		}
		else
		{
			grayMap[i] = uchar(ptFirst.y);
		}
	}
	
	// ��Եڶ��ν��лҶ�ӳ��
	for(; i <= ptSecond.x; i++)
	{
		if(ptSecond.x > ptFirst.x)
		{
			grayMap[i] = uchar((ptSecond.y - ptFirst.y) * (i - ptFirst.x) / (ptSecond.x - ptFirst.x) + ptFirst.y);
		}
		else
		{
			grayMap[i] = uchar(ptSecond.y);
		}
	}

	// ��Ե����ν��лҶ�ӳ��
	for(; i < 256; i++)
	{
		if(ptSecond.x != 255)
		{
			grayMap[i] = uchar((255 - ptSecond.y) * (i - ptSecond.x) / (255 - ptSecond.x) + ptSecond.y);
		}
		else
		{
			grayMap[i] = 255;
		}
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

	/// ʵʩ�ֶλҶ����Ա任
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = grayMap[p[c]];
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

	/// ��������ĻҶ�����(a,c)��(b,d)
	Point2i ptFirst, ptSecond;
	cout << "Enter gray coordinate of the first point[a c]: ";
	cin >> ptFirst.x >> ptFirst.y;
	cout << "Enter gray coordinate of the second point[b d]: ";
	cin >> ptSecond.x >> ptSecond.y;

	/// ��ʾԭʼͼ��
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// �Ҷ����Ա任
	multiLinearGrayTransform(srcImg, dstImg, ptFirst, ptSecond);

	/// ��ʾ�任ͼ��
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	return 0;
}

