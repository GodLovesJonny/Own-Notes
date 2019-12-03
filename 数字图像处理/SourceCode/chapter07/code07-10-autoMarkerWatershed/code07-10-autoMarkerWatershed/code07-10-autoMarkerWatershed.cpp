/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-10-autoMarkerWatershed.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��8��
  ��  ��: �����ֵ�ָ����任����̬�������Զ���ȡ��ǣ�Marker����
          �Ӷ�ʵ�ֻ��ڱ�ǵķ�ˮ��ָ��Ҫ����ճ������ķָ��
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/8     1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main( int argc, char** argv )
{
	/// ����ԭʼͼ��ת��Ϊ�Ҷ�ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << srcFileName <<endl;
		return -1;
	}
	imshow("Original image", srcImg);

	/// �Զ���ֵ�ָOtsu��ֵ�ָ
	Mat bwImg;
	threshold(srcImg, bwImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("Threshold segmentation", bwImg);

	/// ������ȥ������������������׶�
	Mat structElem = getStructuringElement(MORPH_RECT, Size(3, 3));				// �ṹԪ��
	Mat openImg;
	cv::morphologyEx(bwImg, openImg, MORPH_OPEN, structElem, Point(-1,-1), 2);	// ��̬������
	imshow("Morpholoy open", openImg);

	/// ��̬�������㣨�ҳ�ȷ�����ڱ����������Ա����ɱ�����ǣ�
	Mat backImg;
	dilate(openImg, backImg, structElem, Point(-1,-1), 5);	// ��ɫ����Ϊȷ�еı���
	imshow("Sure background region", backImg);

	/// ����任����ֵ�ָ�ҳ�ȷ������Ŀ��������Ա�����Ŀ���ǣ�
	Mat distImg, foreImg;
	distanceTransform(openImg, distImg, CV_DIST_L2, 5);
	double maxDist;	// ������
	minMaxLoc(distImg, 0, &maxDist);
	threshold(distImg, foreImg, 0.68*maxDist, 255, CV_THRESH_BINARY);
	distImg.convertTo(distImg, CV_8U, 255 / maxDist);
	imshow("Distance image", distImg);
	foreImg.convertTo(foreImg, CV_8U);
	imshow("Sure object region", foreImg);

	/// ��ȡ������ȷ�ж�ΪĿ��򱳾���δ֪�����ڱ��ͼ������Ϊ0��
	Mat unknownImg;
	unknownImg = backImg - foreImg;

	/// �������ͼ��ͼ������Ҫ��ΪCV_32S����ֵ��ʾȷ��Ŀ��/��������ı�ǣ���ֵ��ʾδ֪����
	// ��ȡ��ǣ�marker��������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(foreImg, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	// �������ͼ��ȷ�еı���������Ϊ1��ȷ�е�Ŀ��������Ϊ2���ϵ�������δ֪������Ϊ0��
	Mat markerImg = Mat::ones(foreImg.size(), CV_32S);				
	int compCount = 1;
	for(int idx = 0; idx >= 0; idx = hierarchy[idx][0], compCount++)
	{
		drawContours(markerImg, contours, idx, Scalar::all(compCount+1), CV_FILLED, 8, hierarchy);
	}
	markerImg.setTo(Scalar(0), unknownImg == 255);


	// ���������ɫ�����ڸ�������ص���ͨ�ɷָ���һ�������ɫ��
	vector<Vec3b> colorTab;
	colorTab.reserve(compCount);
	for(int i = 0; i < compCount; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// ִ�л��ڱ�ǵķ�ˮ��ָ����㣨����ͼ��Ҫ��CV_8UC3�����ͼ��Ҫ��ΪCV_32S��
	cvtColor(srcImg, srcImg, CV_GRAY2BGR);
	watershed(srcImg, markerImg);

	// �������ͼ����ʾ�ָ���
	Mat wsImg(markerImg.size(), CV_8UC3);
	for(int i = 0; i < markerImg.rows; i++)
	{
		for(int j = 0; j < markerImg.cols; j++)
		{
			int idx = markerImg.at<int>(i,j);
			if(idx == -1)
			{
				// �����ͼ���У���ˮ�����أ����Ϊ-1����Ϊ��ɫ
				wsImg.at<Vec3b>(i,j) = Vec3b(255,255,255);
			}
			else if(idx >= 1 && idx <= compCount)
			{
				// �����ͼ���У�ÿ����ͨ�ɷ֣����Ϊ1��compCount����Ϊĳ����ɫ
				wsImg.at<Vec3b>(i,j) = colorTab[idx-1];
			}
			else
			{
				// ����δ֪������Ϊ��ɫ��Ӧ�ò���������������
				wsImg.at<Vec3b>(i,j) = Vec3b(0,0,0);
			}
		}
	}

	// ��Ϸָ�����ԭʼ�Ҷ�ͼ�񣨿�ѡ��
	wsImg = wsImg * 0.5 + srcImg * 0.5;

	// ��ʾ�ָ���
	imshow( "Watershed segmentation", wsImg);

	waitKey(0);

	return 0;
}
