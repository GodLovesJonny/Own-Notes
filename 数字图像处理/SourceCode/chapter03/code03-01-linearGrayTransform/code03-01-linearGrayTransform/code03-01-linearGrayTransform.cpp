/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-01-linearGrayTransform.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��6��27��
  ��  ��: ͨ��ָ������������ĻҶȷ�Χʵ�ֻҶ����Ա任��
  ��  ��:
  �����б�:
    1. bool linearGrayTransform() �Ҷ����Ա任
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/06/27    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*************************************************
  ��    ��: bool linearGrayTransform(const Mat& srcImg, Mat& dstImg,
                   const Vec2i& srcGrayExt, const Vec2i& dstGrayExt)
  ��    ��: ʵ�ֻҶ����Ա任
  ���ú���: ��(������OpenCV������C++����)
  ��������: main()
  �������: const Mat& srcImg-->����ͼ��CV_8U��
            const Vec2i& srcGrayExt-->�任ǰ�ĻҶȷ�Χ[a,b]
            const Vec2i& dstGrayExt-->�任��ĻҶȷ�Χ[c,d]
  �������: Mat& dstImg-->���ͼ��
  ��    ��: �ɹ��򷵻�true�����򷵻�false
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��6��27��
*************************************************/
bool linearGrayTransform(const Mat& srcImg, Mat& dstImg, const Vec2i& srcGrayExt, const Vec2i& dstGrayExt)
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

	/// ���������ԭʼ��Ŀ��Ҷȷ�Χ����Ҷ����Ա任��б�ʺͽؾ�
	double alpha, beta;
	alpha = double((dstGrayExt[1] - dstGrayExt[0])) / double((srcGrayExt[1] - srcGrayExt[0]));
	beta = dstGrayExt[0] - alpha * srcGrayExt[0];

	/* �������srcImg.convertTo(dstImg, -1, alpha, beta);���������ϵ����䣬ʵ�ֻҶ����Ա任 */

	/// ��ʼ��Ŀ��ͼ��
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());

	/// ʵʩ�Ҷ����Ա任
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = saturate_cast<uchar>(alpha * p[c] + beta);
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

	/// ����任ǰ��ĻҶȷ�Χ
	Vec2i inGrayExt, outGrayExt;
	cout << "Enter the source gray extent[a b]: ";
	cin >> inGrayExt[0] >> inGrayExt[1];
	cout << "Enter the target gray extent[c d]: ";
	cin >> outGrayExt[0] >> outGrayExt[1];

	/// ��ʾԭʼͼ��
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// �Ҷ����Ա任
	linearGrayTransform(srcImg, dstImg, inGrayExt, outGrayExt);

	/// ��ʾ�任ͼ��
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	return 0;
}

