/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code05-05-dwtDecRec.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��3��
  ��  ��: ʵ����ɢС���ֽ���ع�������С������������С��������
          ��ѡ��ͨ��������ʵ�֡�
  ��  ��:
  �����б�:
    1. void dwt() С���任
    2. void idwt() С����任
    3. void decomposeWavelet() ��������С���ֽ�
    4. void reconstructWavelet() ��������С���ع�
    5. void genWavelet() ����С������ͨ�͸�ͨ�˲���
    6. void sampleDown() �²���
    7. void sampleUp() �ϲ���
    8. static void onTrackbar() �������ص�����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���    2014/03/03     1.0     �������ļ�
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

#define CV_HAAR 1
#define CV_SYM2 2
#define CV_DB1 3
#define CV_DAUB 4 //Daubechies

// ����ԭ��
void dwt(Mat src, Mat &dst, int type, int level) ;
void idwt(Mat src, Mat& dst, int type, int level);
void decomposeWavelet(Mat src, Mat &dstLow, Mat &dstHigh, Mat lowFilter, Mat highFilter);
void reconstructWavelet(Mat src, Mat& dst, Mat lowFilter, Mat highFilter);
void genWavelet(int type, Mat &lowFilter, Mat &highFilter);
void sampleDown(Mat src, Mat& dst);
void sampleUp(Mat src, Mat& dst);
// �������ص�����
static void onTrackbar(int pos, void * param);

// ������λ��
int Level = 0, WaveletType = 0;

int main(int argc, char **argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, 1);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    Mat dst;

    // ת��Ϊ�Ҷ�ͼ��
    if (img.channels() >= 3)
        cvtColor(img, img, CV_RGB2GRAY);

    namedWindow("OrigImg", CV_WINDOW_AUTOSIZE);
    imshow("OrigImg",img);

    createTrackbar("Level", "OrigImg", &Level, 5, onTrackbar, &img);
    createTrackbar("Wave", "OrigImg", &WaveletType, 2, 0);
    // С���任
    dwt(img, dst, CV_DAUB, 2);
    // ��ʾ�任���
    imshow("ImgDec", Mat_<uchar>(dst));
    // ��任
    idwt(dst, img, CV_DAUB, 2);
    // ��ʾ��任���
    imshow("ImgRec", Mat_<uchar>(img));

    waitKey(0);

    return 0;
}

/*************************************************
  ��    ��: static void onTrackbar(int pos, void * param)
  ��    ��: Trackbar�ص�����
  ���ú���: drawFilledCircle()��shift()��updateMag()
            updateResult()(������OpenCV������C++����)
  ��������: Trackbar���黬���¼�
  �������: int pos-->��������ǰ����λ��            
  �������: void * param-->����ָ�룬voidָ�룬
                           ����ָ���κ����ݣ��˴�ָ��ͼ������
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��4��
*************************************************/
static void onTrackbar(int pos, void * param)
{
    Mat* p_img = (Mat*)param;
    Mat img = *p_img;
    Mat dst;
    // С���任
    dwt(img, dst, WaveletType + 1, Level + 1);
    // ��ʾ�任���
    imshow("ImgDec", Mat_<uchar>(dst));
    // ��任
    idwt(dst, img, WaveletType + 1, Level + 1);
    // ��ʾ��任���
    imshow("ImgRec", Mat_<uchar>(img));
}

/*************************************************
  ��    ��: void dwt(Mat src, Mat &dst, int type, int level)
  ��    ��: ʵ����ɢС���任
  ���ú���: decomposeWavelet()��genWavelet()(������OpenCV������C++����)
  ��������: main()
  �������: Mat src-->����ͼ�񣬱����ǵ�ͨ���Ҷ�ͼ��
            int type-->С������������ȡCV_HAAR��CV_SYM2��
                       CV_DB1��CV_DAUB
            int level-->С���ֽ����
  �������: Mat &dst-->С���ֽ������ݾ���
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void dwt(Mat src, Mat &dst, int type, int level)
{
    // ������ͨ���Ҷ�ͼ��
    if (src.empty() || src.channels() >= 3)
        return;

    // ת��Ϊ������
    src.convertTo(src, CV_32FC1);
    // �������ͼ�񣬳�ʼ��Ϊ0
    dst = Mat::zeros(src.rows, src.cols, src.type());
    // ������ͨ�͸�ͨ�˲���
    Mat lowFilter;
    Mat highFilter;
    // �����˲���
    genWavelet(type, lowFilter, highFilter);

    int t=1;
    int rows = src.rows;
    int cols = src.cols;
    Mat rowROI; // ���и���Ȥ����
    Mat colROILow, colROIHigh; // ��Ƶ�͸�Ƶ���и���Ȥ����
    Mat rowROIdstLow, rowROIdstHigh; // ��Ƶ�͸�Ƶ���и���Ȥ����
    Mat colROIdstLow, colROIdstHigh; // ��Ƶ�͸�Ƶ���и���Ȥ����

    while ( t<=level )
    {
        // �任��
        for ( int i=0; i<rows; i++ )
        {
            rowROI = src(Rect(0, i, cols, 1));
            rowROIdstLow = dst(Rect(0, i, cols / 2, 1));
            rowROIdstHigh = dst(Rect(cols / 2, i, cols / 2, 1));
            decomposeWavelet(rowROI, rowROIdstLow, rowROIdstHigh, lowFilter, highFilter);
        }
        // �任��
        src = dst;
        for (int j = 0; j < cols / 2; j++)
        {
            // ��Ƶ���ָ���
            colROILow = src(Rect(j, 0, 1, rows));
            colROIdstLow = dst(Rect(j, 0, 1, rows / 2));
            colROIdstHigh = dst(Rect(j, rows / 2, 1, rows / 2));
            Mat colROIdstLowT = colROIdstLow.t();
            Mat colROIdstHighT = colROIdstHigh.t();
            decomposeWavelet(colROILow.t(), colROIdstLowT, colROIdstHighT, lowFilter, highFilter);
            colROIdstLow = colROIdstLowT.t();
            colROIdstHigh = colROIdstHighT.t();

            // ��Ƶ���ָ���
            colROIHigh = src(Rect(j + cols / 2, 0, 1, rows));
            colROIdstLow = dst(Rect(j + cols / 2, 0, 1, rows / 2));
            colROIdstHigh = dst(Rect(j + cols / 2, rows / 2, 1, rows / 2));
            colROIdstLowT = colROIdstLow.t();
            colROIdstHighT = colROIdstHigh.t();
            decomposeWavelet(colROIHigh.t(), colROIdstLowT, colROIdstHighT, lowFilter, highFilter);
            colROIdstLow = colROIdstLowT.t();
            colROIdstHigh = colROIdstHighT.t();
        }
        // ���²���
        rows = rows / 2;
        cols = cols / 2;
        t++;
        src = dst;
    }
    //normalize(dst, dst, 0, 255, NORM_MINMAX);
    // ����Ҫ�洢�ý��Ϊͼ���ļ�����ȡ����ע��
    //imwrite("dwt.png", dst);
}

/*************************************************
  ��    ��: void idwt(Mat src, Mat &dst, int type, int level)
  ��    ��: ʵ����ɢС����任
  ���ú���: reconstructWavelet()��genWavelet()(������OpenCV������C++����)
  ��������: main()
  �������: Mat src-->�������ݣ�С���ֽ������ݾ���
            int type-->С������������ȡCV_HAAR��CV_SYM2��
                       CV_DB1��CV_DAUB
            int level-->С���ֽ����
  �������: Mat &dst-->С���ع����ͼ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void idwt(Mat src, Mat & dst, int type, int level)
{
    // ������ͨ���Ҷ�ͼ��
    if (src.empty() || src.channels() >= 3)
        return;

    // ת��Ϊ������
    src.convertTo(src, CV_32FC1);
    int rows = src.rows;
    int cols = src.cols;

    // �����˲���
    Mat lowFilter;
    Mat highFilter;
    genWavelet(type, lowFilter, highFilter);

    int t = level;

    // ��С���任
    Mat rowROI, colROI, rowROIDst, colROIDst, colROIDstT;
    Rect rowROIRect, colROIRectLeft, colROIRectRight;
    Size sizeCurrent;

    int row = rows / pow(2., level);
    int col = cols / pow(2., level);
    Mat tempDst;
    Mat srcROI;

    while (t >= 1)
    {
        sizeCurrent = Size(2 * col, 2 * row);
        tempDst = Mat::zeros(sizeCurrent, src.type());
        dst = Mat::zeros(sizeCurrent, src.type());

        // С������任
        for (int j = 0; j < col; j++)
        {
            // ȡ��һ�У���Ƶ����
            colROIRectLeft = Rect(j, 0, 1, row * 2);
            colROI = src(colROIRectLeft);
            colROIDst = tempDst(colROIRectLeft);
            colROIDstT = colROIDst.t();
            reconstructWavelet(colROI.t(), colROIDstT, lowFilter, highFilter);
            colROIDst = colROIDstT.t();

            //ȡ��һ�У���Ƶ����
            colROIRectRight = Rect(j + col, 0, 1, row * 2);
            colROI = src(colROIRectRight);
            colROIDst = tempDst(colROIRectRight);
            colROIDstT = colROIDst.t();
            reconstructWavelet(colROI.t(), colROIDstT, lowFilter, highFilter);
            colROIDst=colROIDstT.t();
        }

        //С������任
        for (int i = 0; i < 2 * row; i++)
        {
            //һ�δ���һ��
            rowROIRect = Rect(0, i, col * 2, 1);
            rowROI = tempDst(rowROIRect);
            rowROIDst = dst(rowROIRect);
            reconstructWavelet(rowROI, rowROIDst, lowFilter, highFilter);
        }
        // ��������
        srcROI = src(Rect(0, 0, 2 * col, row * 2));
        dst.copyTo(srcROI);
        row *= 2;
        col *= 2;
        t--;
    }
    normalize(dst, dst, 0, 255, NORM_MINMAX);
    // ����Ҫ�洢�ý��Ϊͼ���ļ�����ȡ����ע��
    //imwrite("idwt.png", dst);
}

/*************************************************
  ��    ��: void decomposeWavelet(Mat src,
                                  Mat &dstLow, Mat &dstHigh,
                                  Mat lowFilter, Mat highFilter)
  ��    ��: ʵ�ֵ�������С���ֽ�
  ���ú���: sampleDown()(������OpenCV������C++����)
  ��������: dwt()
  �������: Mat src-->�������ݣ�����Ҫ����С���ֽ�����ݾ���
            Mat lowFilter-->��ͨ�˲�����Ҫ�����ͨ�˲�����Ϊ����
            Mat highFilter-->��ͨ�˲�����Ҫ�����ͨ�˲�����Ϊ����
  �������: Mat &dstLow-->С���ֽ��ĵ�Ƶ����
            Mat &dstHigh-->С���ֽ��ĸ�Ƶ����
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void decomposeWavelet(Mat src, Mat &dstLow, Mat &dstHigh, Mat lowFilter, Mat highFilter)
{
    // ������Ч�Լ���
    assert(src.rows == 1 && lowFilter.rows == 1 && highFilter.rows == 1);
    assert(src.cols >= lowFilter.cols && src.cols >= highFilter.cols);

    int len = src.cols;

    // ������ת��Ϊ������
    src.convertTo(src, CV_32FC1);
    lowFilter.convertTo(lowFilter, CV_32FC1);
    highFilter.convertTo(highFilter, CV_32FC1);

    // ׼����ͨ�͸�ͨ�˲���ľ��󣬳�ʼ��Ϊ0
    Mat LowFiltered = Mat::zeros(1, len, src.type());
    Mat HighFiltered = Mat::zeros(1, len, src.type());

    // ִ���˲�����
    filter2D(src, LowFiltered, -1, lowFilter);
    filter2D(src, HighFiltered, -1, highFilter);

    // �²���
    sampleDown(LowFiltered, dstLow);
    sampleDown(HighFiltered, dstHigh);
}

/*************************************************
  ��    ��: void reconstructWavelet(Mat src, Mat &dst,
                                    Mat lowFilter, Mat highFilter)
  ��    ��: ʵ�ֵ�������С���ع�
  ���ú���: sampleUp()(������OpenCV������C++����)
  ��������: idwt()
  �������: Mat src-->�������ݣ�����Ҫ����С���ع����ݾ���
            Mat lowFilter-->��ͨ�˲�����Ҫ�����ͨ�˲�����Ϊ����
            Mat highFilter-->��ͨ�˲�����Ҫ�����ͨ�˲�����Ϊ����
  �������: Mat &dst-->С���ع�������ݾ���
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void reconstructWavelet(Mat src, Mat &dst, Mat lowFilter, Mat highFilter)
{
    // ������Ч�Լ���
    assert(src.rows == 1 && lowFilter.rows == 1 && highFilter.rows == 1);
    assert(src.cols >= lowFilter.cols && src.cols >= highFilter.cols);

    // ת��Ϊ����������
    src.convertTo(src, CV_32FC1);
    int len = src.cols;
    lowFilter.convertTo(lowFilter, CV_32FC1);
    highFilter.convertTo(highFilter, CV_32FC1);

    // ��ֵ����
    Mat UpLow = Mat::zeros(1, len, src.type());
    Mat UpHigh = Mat::zeros(1, len, src.type());
    // ���Բ�ֵ
    Mat roiLow(src, Rect(0, 0, len / 2, 1));
    Mat roiHigh(src, Rect(len / 2, 0, len / 2, 1));
    // �ϲ���
    sampleUp(roiLow, UpLow);
    sampleUp(roiHigh, UpHigh);

    // ��ͨ�͸�ͨ�˲�
    Mat dstLow=Mat::zeros(1, len, src.type());
    Mat dstHigh=Mat::zeros(1, len, src.type());

    filter2D(UpLow, dstLow, -1, lowFilter);
    filter2D(UpHigh, dstHigh, -1, highFilter);

    dst = dstLow + dstHigh;
}

/*************************************************
  ��    ��: void genWavelet(int type,
                            Mat &lowFilter, Mat &highFilter)
  ��    ��: ��������С���ֽ���ع����˲���
  ���ú���: ��(������OpenCV������C++����)
  ��������: dwt()��idwt()
  �������: int type-->С������������ȡCV_HAAR��CV_SYM2��
                        CV_DB1��CV_DAUB
  �������: Mat &lowFilter-->��ͨ�˲�����Ҫ�����ͨ�˲�����Ϊ����
            Mat &highFilter-->��ͨ�˲�����Ҫ�����ͨ�˲�����Ϊ����
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void genWavelet(int type, Mat &lowFilter, Mat &highFilter)
{
    if (type == CV_HAAR || type == CV_DB1)
    {
        int N = 2;
        lowFilter = Mat::zeros(1, N, CV_32FC1);
        highFilter = Mat::zeros(1, N, CV_32FC1);

        lowFilter.at<float>(0, 0) = 1 / sqrtf(N);
        lowFilter.at<float>(0, 1) = 1 / sqrtf(N);
        highFilter.at<float>(0, 0) = -1 / sqrtf(N);
        highFilter.at<float>(0, 1) = 1 / sqrtf(N);
    }

    if ( type==CV_SYM2 )
    {
        int N = 4;
        float h[] = {-0.483, 0.836, -0.224, -0.129};
        float l[] = {-0.129, 0.224,  0.836,  0.483};

        lowFilter = Mat::zeros( 1, N, CV_32FC1 );
        highFilter = Mat::zeros( 1, N, CV_32FC1 );

        for (int i = 0; i < N; i++ )
        {
            lowFilter.at<float>(0, i) = l[i];
            highFilter.at<float>(0, i) = h[i];
        }
    }


    if ( type==CV_DAUB )
    {
        int N = 6;
        float l[] = {.332670552950,  .806891509311,
                     .459877502118, -.135011020010,
                     -.085441273882,  .035226291882
                    };
        float h[] = {.035226291882,  .085441273882,
                     -.135011020010, -.459877502118,
                     .806891509311, -.332670552950
                    };

        lowFilter = Mat::zeros( 1, N, CV_32FC1 );
        highFilter = Mat::zeros( 1, N, CV_32FC1 );

        for (int i = 0; i < N; i++ )
        {
            lowFilter.at<float>(0, i) = l[i];
            highFilter.at<float>(0, i) = h[i];
        }
    }
}

/*************************************************
  ��    ��: void sampleDown(Mat src, Mat& dst)
  ��    ��: ʵ���²���
  ���ú���: ��(������OpenCV������C++����)
  ��������: decomposeWavelet()
  �������: Mat src-->���б��²������ݾ���
  �������: Mat& dst-->�����²�����õ��ĵ������ݾ���
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void sampleDown(Mat src, Mat& dst)
{
    /// ������1������
    if (src.empty() || src.rows != 1 || dst.rows != 1)
        return;

    /// resize����ʵ���²���
    resize(src, dst, dst.size());
}

/*************************************************
  ��    ��: void sampleUp(Mat src, Mat& dst)
  ��    ��: ʵ���ϲ���
  ���ú���: ��(������OpenCV������C++����)
  ��������: reconstructWavelet()
  �������: Mat src-->���б��ϲ������ݾ���
  �������: Mat& dst-->�����ϲ�����õ��ĵ������ݾ���
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��3��
*************************************************/
void sampleUp(Mat src, Mat& dst)
{
    if (src.empty() || src.rows != 1 || dst.rows != 1 || dst.empty() || 2 * src.cols != dst.cols)
        return;

    /// resize����ʵ���ϲ���
    /*
    interpolation method:
    INTER_NEAREST - a nearest-neighbor interpolation
    INTER_LINEAR - a bilinear interpolation (used by default)
    INTER_AREA - resampling using pixel area relation. It may be a preferred method for image decimation, as it gives moire��-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method.
    INTER_CUBIC - a bicubic interpolation over 4x4 pixel neighborhood
    INTER_LANCZOS4 - a Lanczos interpolation over 8x8 pixel neighborhood
    */
    resize(src, dst, dst.size(), 0, 0, INTER_CUBIC);
}
