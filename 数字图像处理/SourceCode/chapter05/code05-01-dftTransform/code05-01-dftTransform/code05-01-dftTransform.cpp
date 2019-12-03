/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code05-01-dftTransform.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��4��
  ��  ��: ʵ����ɢ����Ҷ�任������ʾ��Ƶ�ס�
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���    2014/03/04     1.0     �����ļ�
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    // �Ż�����ͼ���С������߼����ٶ�
    Mat padded;
    int m = getOptimalDFTSize( img.rows );
    int n = getOptimalDFTSize( img.cols );
    // ����չ��������0���
    copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    // ͨ���ϲ�����ԭͼ����չΪ2��ͨ������ͨ����0���
    merge(planes, 2, complexI);
    // DFT�任
    dft(complexI, complexI);

    // �����ֵ��ת��Ϊ��������
    /// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    /// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    split(complexI, planes);
    /// sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    magnitude(planes[0], planes[1], planes[0]);
    /// planes[0] = magnitude
    Mat magI = planes[0];

    // �任Ϊ��������: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    // Ƶ��ƽ��
    /// 1 | 4      3 | 2
    /// -----  ==> -----
    /// 2 | 3      4 | 1
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    // ����ÿ������ROI����
    Mat q0(magI, Rect(0, 0, cx, cy));   // ���Ͻ�
    Mat q1(magI, Rect(cx, 0, cx, cy));  // ���Ͻ�
    Mat q2(magI, Rect(0, cy, cx, cy));  // ���½�
    Mat q3(magI, Rect(cx, cy, cx, cy)); // ���½�

    Mat tmp;
    // ���ݽ���(���Ͻ������½�)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // ���ݽ��������Ͻ������½ǣ�
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    // ������任Ϊ��׼�����ݣ�0��1��
    normalize(magI, magI, 0, 1, CV_MINMAX);

    // ��ʾ���
    imshow("Input Image", img);
    imshow("spectrum magnitude", magI);
    waitKey();

    return 0;
}
