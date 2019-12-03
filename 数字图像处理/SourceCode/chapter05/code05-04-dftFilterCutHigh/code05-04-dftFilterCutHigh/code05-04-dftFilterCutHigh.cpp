/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code05-04-dftFilterCutHigh.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��4��
  ��  ��: DFT�任��ͨ��HIGHGUI��Ļ����������˲����뾶��
          ��������¼���Ӧ���������ѡ���˲������ģ�����
          �Ҽ�ʵ���˲���
  ��  ��:
  �����б�:
    1. Mat computeDFT();DFT�任
    2. void updateMag();����Ƶ��
    3. void updateResult();DFT��任
    4. void shift();ƽ��Ƶ��
    5. void drawFilledCircle();����ʵ��Բ
    6. static void onMouse(); ����¼��ص�����
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

// ����ԭ��
Mat computeDFT(Mat image);
void updateMag(Mat complexI);
void updateResult(Mat complexI);
void shift(Mat magI);
void drawFilledCircle( Mat img, int radius, Point center );
static void onMouse( int event, int x, int y, int, void* param);
// ȫ�ֱ���
int kernelSize = 5;
int invertType = 0;
Mat mask;

int main( int argc, char** argv )
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

    mask = Mat(img.size(), CV_8UC1, Scalar(255));

    // ��ʾԭʼͼ��
    namedWindow("Orginal window", CV_WINDOW_AUTOSIZE);
    imshow("Orginal window", img);

    // DFT����
    Mat complexI = computeDFT(img);

    // ������ʾƵ�׵Ĵ���
    namedWindow("spectrum", CV_WINDOW_AUTOSIZE);
    // �����˲����ƻ�����
    createTrackbar("kernelsize", "spectrum", &kernelSize, 10, 0);
    // �����ߵ�ͨ�˲����ƻ�����
    createTrackbar("invert", "spectrum", &invertType, 1, 0);

    // �������ص�����
    setMouseCallback("spectrum", onMouse, &complexI);

    // ����Ƶ�ײ�ת��Ϊ�������������ʾ
    updateMag(complexI);
    // ��DFT�任
    updateResult(complexI);
    waitKey(0);

    return 0;
}

// �����Ӧ�ص�����
/*************************************************
  ��    ��: static void onMouse( int event, int x, int y, int, void* param)
  ��    ��: �����Ӧ�ص�����
  ���ú���: drawFilledCircle()��shift()��updateMag()
            updateResult()(������OpenCV������C++����)
  ��������: �����Ӧ�¼�
  �������: int event-->����¼�������������Ҽ������ȣ�
            int x-->��굱ǰλ��x����
            int y-->��굱ǰλ��y����
            int flag-->����������־���磺ctrl��alt�ȣ�
  �������: void * param-->����ָ�룬voidָ�룬
                           ����ָ���κ����ݣ��˴�ָ��ͼ������
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
static void onMouse( int event, int x, int y, int, void* param)
{
    // ���������������Ĥ
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        drawFilledCircle(mask, kernelSize, Point(x, y));
        imshow("mask image", mask);
    }

    // �����Ҽ�������ͼ��
    if( event == CV_EVENT_RBUTTONDOWN )
    {
        // ǿ��ת������*paramת��ΪMat����
        Mat* pComplex = (Mat*)param;
        Mat complexI = *pComplex;

        // ת��Ϊ32λ����������
        mask.convertTo(mask, CV_32F);
        // ��һ��
        normalize(mask, mask, 1.0, 0.0, NORM_MINMAX);

        // ��ת��Ĥ
        if(invertType)
        {
            mask = Mat::ones(mask.size(), CV_32F) - mask;
        }

        // ƽ��
        shift(mask);

        // ����������Ĥ
        Mat planes[] = {Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F)};
        Mat kernel_spec;
        planes[0] = mask; // ʵ��
        planes[1] = mask; // �鲿
        merge(planes, 2, kernel_spec);

        Mat complexII;
        // �����ؽ���Ƶ�����
        mulSpectrums(complexI, kernel_spec, complexII, DFT_ROWS);

        updateMag(complexII);		// ����Ƶ����ʾ
        updateResult(complexII);	// ���任
    }
    return;
}

/*************************************************
  ��    ��: void updateResult(Mat complexI)
  ��    ��: ����DFT�任����任��������ֱ����
            �ú����ڲ�ͨ������imshow()��ʾ
  ���ú���: (������OpenCV������C++����)
  ��������: main(),onTrackbar()����
  �������: Mat complexI-->DFT�任��ĸ������ݾ���
  �������: ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
void updateResult(Mat complexI)
{
    Mat work;
    idft(complexI, work);
    Mat planes[] = {Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F)};
    // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    split(work, planes);
    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    magnitude(planes[0], planes[1], work);
    // ��һ��
    normalize(work, work, 0, 1, NORM_MINMAX);
    imshow("result", work);
}

/*************************************************
  ��    ��: void updateMag(Mat complexI)
  ��    ��: ����DFT�任��Ƶ�ף�������ֱ����
            �ú����ڲ�ͨ������imshow()��ʾ
  ���ú���: shift()(������OpenCV������C++����)
  ��������: main(),onTrackbar()����
  �������: Mat complexI-->DFT�任��ĸ������ݾ���
  �������: ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
void updateMag(Mat complexI )
{
    Mat magI;
    Mat planes[] = {Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F)};
    // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    split(complexI, planes);
    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    magnitude(planes[0], planes[1], magI);

    // ת��Ϊ��������: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);
    // ƽ�ƣ�1��3���޶Ի���2��4���޶Ի���
    shift(magI);
    // ��һ����0-1���䣬����ʾͼ��
    normalize(magI, magI, 1, 0, NORM_INF);

    imshow("spectrum", magI);
}

/*************************************************
  ��    ��: Mat computeDFT(Mat image)
  ��    ��: ִ�м���DFT�任
  ���ú���: ��(������OpenCV������C++����)
  ��������: main()����
  �������: Mat image-->ͼ�����ݾ��󣬽�֧��
                        ��ͨ���Ҷ�ͼ��
  �������: ��
  ��    ��: Mat-->DFT�任��ĸ������ݾ���
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
Mat computeDFT(Mat image)
{
    // �ο�http://opencv.itseez.com/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html

    // ��չͼ�����Ż������ٶ�
    Mat padded;
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols );
    // ��չͼ����չ��������0���
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
    // Ϊʵ�����鲿�����ռ�
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    // ����ʵ�����鲿˫ͨ������
    merge(planes, 2, complexI);
    // ִ�и���Ҷ�任DFT
    dft(complexI, complexI, DFT_COMPLEX_OUTPUT);
    return complexI;
}

/*************************************************
  ��    ��: void shift(Mat magI)
  ��    ��: ƽ��Ƶ��
  ���ú���: ��(������OpenCV������C++����)
  ��������: updateMag(),onTrackbar()����
  �������: Mat magI-->DFT�任��Ƶ�����ݾ���
  �������: ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
void shift(Mat magI)
{
    // ������ż�����вü�
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    // ƽ�Ʋ���
    // 1 | 4      3 | 2
    // -----  ==> -----
    // 2 | 3      4 | 1
    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

/*************************************************
  ��    ��: void drawFilledCircle(Mat img, int radius, Point center)
  ��    ��: ����ʵ��Բ
  ���ú���: ��(������OpenCV������C++����)
  ��������: onMouse()����
  �������: Mat mag-->ͼ������
            int radius-->Բ�İ뾶
            Point center-->Բ��
  �������: ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
void drawFilledCircle( Mat img, int radius, Point center )
{
    int thickness = -1;
    int lineType = 8;

    circle( img,
            center,
            radius,
            Scalar(0),
            thickness,
            lineType );
}
