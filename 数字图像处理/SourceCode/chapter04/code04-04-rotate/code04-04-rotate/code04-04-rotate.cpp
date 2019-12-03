/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code04-04-rotate.cpp
  ��  ��: �λ���
  ��  ��: 1.0
  ��  ��: 2014-08-11
  ��  ��: ʵ��ͼ�����ת������ͼ����ʾ
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      �λ���   2014-08-11     1.0     �������ļ�
***********************************************************/

#include <iostream>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	/// ����ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat src = imread(srcFileName, 1);
	if(src.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// �����任����
	Point2f center(src.cols / 2, src.rows / 2);
	Mat rotmat = getRotationMatrix2D(center, 30, 1);
	Mat dst(src.cols, src.rows, src.type());

	/// ����任
	warpAffine(src, dst, rotmat, dst.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

	/// ��ʾԭͼ��
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	imshow("src", src);

	/// ��ʾ��ת���ͼ��
	namedWindow("dst", CV_WINDOW_AUTOSIZE);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}
