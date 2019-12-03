/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code05-02-dctTransform.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��4��
  ��  ��: ʵ����ɢ���ұ任������ʾ��Ƶ�ס�
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

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    // ����ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    // ͼ���͸߱�����2��������������������е���
    Mat padded;
    int w = img.cols;
    int h = img.rows;
    int w2,h2;
    if (w % 2 == 0)
        w2 = w;
    else
        w2 = w + 1;
    if (h % 2 == 0)
        h2 = h;
    else
        h2 = h + 1;

    // ��չͼ�񣬲�����չ��������0���
    copyMakeBorder(img, padded, 0, h2 - h, 0, w2 - w, BORDER_CONSTANT, Scalar::all(0));

    // dct()����Ҫ�������Ǹ�����
    Mat imgFloat = Mat(padded.rows, padded.cols, CV_64FC1);
    padded.convertTo(imgFloat, CV_64FC1);

    // ִ��DCT�任
    Mat imgFreq;
    dct(imgFloat, imgFreq);

    // ת��Ϊ8λ�Ҷ�
    Mat imgdisp = Mat(imgFloat.rows, imgFloat.cols, CV_8UC1);
    imgFloat.convertTo(imgdisp, CV_8UC1);

    Mat freqDisp = Mat(imgFreq.rows, imgFreq.cols, CV_8UC1);
    imgFreq.convertTo(freqDisp, CV_8UC1);

    // ��ʾ���
    imshow("Input Image", imgdisp);
    imshow("spectrum magnitude", freqDisp);

    waitKey(0);

    return 0;
}
