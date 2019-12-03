/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code03-13-homoFilter.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��23��
  ��  ��: ����̬ͬ�˲���ǿͼ��
  ��  ��:
  �����б�:
    1. Mat computeDFT() ����Ҷ�任
    2. void shiftDFT() �������ݵ�˫���Բ�ֵ
    3. Mat createSpectrumMagnitudeForDisplay()  ����������ʾ��Ƶ��ͼ��
    4. Mat createHomoFilter() ����̬ͬ�˲���
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/23    1.0     �������ļ�
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// ����Ҷ�任
Mat computeDFT(const Mat &grayImg);

/// Ƶ��ƽ��
void shiftDFT(Mat &dftImg);

/// ����������ʾ��Ƶ��ͼ��
Mat createSpectrumMagnitudeForDisplay(const Mat &complexImg, bool bShift = true);

/// ����̬ͬ�˲���
Mat createHomoFilter(Size filterSize, double d0, double c, double rL, double rH);

int main(int argc, char** argv)
{	
	/// ����ԭʼͼ�񣨻�ֵ����
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// ��ʾԭͼ��
	imshow("Original Image", srcImg);

	/// ���㸵��ҶƵ��
	Mat complexImg = computeDFT(srcImg);

	/// ��ʾ�˲�ǰ��Ƶ�׷���
	Mat magImg = createSpectrumMagnitudeForDisplay(complexImg);
	imshow("Fourier Spectrum", magImg);

	/// Ƶ��ƽ��
	// ע������˲�ǰ��Ҫƽ��Ƶ�ף���Ӧ�ڴ����˲���ǰ���ã��Ա�֤ͼ�����˲����Ĵ�Сһ�£�
	// ����mulSpectrums���ܳ�����shiftDFT���ܻ�ı�ͼ��ߴ硣
	shiftDFT(complexImg);

	/// ����̬ͬ�˲���
	Mat homoFilter = createHomoFilter(complexImg.size(), 10, 4, 0.5, 2);	

	/// Ƶ���˲�
	mulSpectrums(complexImg, homoFilter, complexImg, 0);

	/// Ƶ��ƽ��
	shiftDFT(complexImg);

	/// ����Ҷ���任
	idft(complexImg, complexImg);

	/// ָ���任


	/// ��ʾ�˲����ͼ��
	Mat planes[2];
	split(complexImg, planes);
	Mat dstImg;
	normalize(planes[0], dstImg, 0, 1, CV_MINMAX);
	imshow("Filtered Image", dstImg);

	waitKey(0);

	return 0;
}

/**
 * ���㸵��ҶƵ��
 * @param grayImg ��ͨ���Ҷ�ͼ��
 * @return ˫ͨ�������͵ĸ���ҶƵ�����ݣ�ʵ��+�鲿��
 */
Mat computeDFT(const Mat &grayImg)
{
	// �Ż�ͼ���С
	int m = getOptimalDFTSize(grayImg.rows);
	int n = getOptimalDFTSize(grayImg.cols);

	// ��չͼ����չ������0���
	Mat paddedImg;
	copyMakeBorder(grayImg, paddedImg, 0, m - grayImg.rows, 0, n - grayImg.cols, BORDER_CONSTANT, Scalar(0));

	// ����Ҷ�任��Ҫ������ͼ��Ϊ�����ͣ�
	Mat complexImg;
	dft(Mat_<float>(paddedImg), complexImg, DFT_COMPLEX_OUTPUT);

	// ���ظ���Ƶ������
	return complexImg;
}


/**
 * Ƶ��ƽ��
 * @param dftImg DFTƵ�����ݻ�Ƶ�׷���
 * @return ��
 */
void shiftDFT(Mat &dftImg)
{
	// ������������Ϊ��������ü���ż������������
	dftImg = dftImg(Rect(0, 0, dftImg.cols & -2, dftImg.rows & -2));

	// Ƶ��ƽ�Ʋ������������Խ����ޣ�
    // 0 | 1      3 | 2
    // -----  ==> -----
    // 2 | 3      1 | 0
	int cx = dftImg.cols / 2;
	int cy = dftImg.rows / 2;
	Mat tmpMat;
	Mat topLeft = dftImg(Rect(0, 0, cx, cy));
	Mat downLeft = dftImg(Rect(0, cy, cx, cy));
	Mat topRight = dftImg(Rect(cx, 0, cx, cy));
	Mat downRight = dftImg(Rect(cx, cy, cx, cy));
	topLeft.copyTo(tmpMat); downRight.copyTo(topLeft); tmpMat.copyTo(downRight);
	topRight.copyTo(tmpMat); downLeft.copyTo(topRight); tmpMat.copyTo(downLeft);
}

/**
 * ������ʾ�õĸ���ҶƵ�׷���ͼ�񣨾��������任�͹�һ����
 * @param complexImg ����ҶƵ�����ݣ�˫ͨ���������ͣ�ʵ��+�鲿��
 * @param bShift Ƶ��ƽ�Ʊ�־����Ϊtrue����Ƶ��ƽ����ͼ�����ģ�
 * @return ��ʾ�õ�Ƶ�׷���ͼ�񣨾��������任�͹�һ����
 */
Mat createSpectrumMagnitudeForDisplay(const Mat &complexImg, bool bShift)
{
	// ͨ������
	Mat planes[2];
	split(complexImg, planes);

	// ������ȣ�sqrt(Re(DFT(img))^2 + Im(DFT(img))^2)
	Mat magImg;
	magnitude(planes[0], planes[1], magImg);

	// �����任��log(1 + mag)
	magImg += Scalar::all(1);
	log(magImg, magImg);

	// Ƶ��ƽ����ͼ������
	if(bShift)
	{
		shiftDFT(magImg);
	}

	// Ƶ�׹�һ����[0, 1]
	normalize(magImg, magImg, 0, 1, CV_MINMAX);

	// ����Ƶ�׷���ͼ��
	return magImg;
}

/**
 * ����̬ͬ�˲���
 * @param filterSize �˲��뾶
 * @param d0 ��ֹ�뾶
 * @param c ���ڿ����˲���б����񻯳̶�
 * @param rL ���ڿ����˲�ǿ�ȣ��½磩
 * @param rH ���ڿ����˲�ǿ�ȣ��Ͻ磩
 * @return ˫ͨ�������͵�̬ͬ�˲���
 */
Mat createHomoFilter(Size filterSize, double d0, double c, double rL, double rH)
{
	Mat tmpFilter = Mat::zeros(filterSize, CV_32F);
	int cx = filterSize.width / 2;
	int cy = filterSize.height / 2;

	double d = 0;
	for(int i = 0; i < filterSize.height; i++)
	{
		for(int j = 0; j < filterSize.width; j++)
		{
			d = sqrt((j - cx) * (j - cx) + (i - cy) * (i - cy));
			tmpFilter.at<float>(i, j) = (float)((rH - rL) * (1 - exp(-c * pow(d / d0, 2))) + rL);
		}
	}

	Mat planes[2] = {tmpFilter, tmpFilter};
	Mat filterImg;
	merge(planes, 2, filterImg);

	return filterImg;
}