/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-12-graphCut.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��10��
  ��  ��: ����ͼ�Graph Cut����ͼ��ָ���Ȳ��ø�˹���ģ��GMM��ͼ��Ŀ���뱳���ĻҶȸ��ʷֲ����н�ģ��
          ��ͨ��EM������⣬���Ź���s-tͼ���������Min-Cut/Max-Flow�������õ��������Ż���ķָ�����
  ��  ��:
  �ο�����:
    1. "An Experimental Comparison of Min-Cut/Max-Flow Algorithms for Energy Minimization in Vision."
	    Yuri Boykov and Vladimir Kolmogorov.
	    In IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI), September 2004
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/10    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\ml\ml.hpp>
#include <iostream>

// ����Yuri Boykov and Vladimir Kolmogorov����Сͼ�����
#include "gc\GCoptimization.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	/// ����ԭʼͼ��
	string fileName;
	cout << "Enter the source image file name: ";
	cin >> fileName;
	Mat srcImg = imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << fileName << endl;
		return -1;
	}

	///==================================================================================
	//               ���ø�˹���ģ�ͣ�GMM����ͼ��Ŀ���뱳�����н�ģ
	// ͨ����˹���ģ�ͻ��Ŀ���뱳���ĻҶȸ��ʷֲ��������ڹ���s-tͼʱ����t-links��Ȩֵ
	// ע������Ҳ����������K-��ֵ����ȷ�������Ŀ���뱳���ĻҶȸ��ʷֲ���
	// ע������ͬһ��ͼ�񣬿��Խ���ģ������������������ͼ����ʹ�ã��Խ�ʡѵ��ʱ�䡣
	///----------------------------------------------------------------------------------
	// ������˹���ģ��
	int labelNum = 2;	// ������Ŀ������ֻʵ��ǰ���뱳���ķָ��ֻ��2�࣬�û��ɸ���ʵ��ͼ�������趨��
	cv::EM gmmModel(labelNum);

	// ָ��ѵ����������ͨ����ÿ����Ϊһ��������
	Mat sampleImg;
	srcImg.reshape(1, srcImg.rows * srcImg.cols).convertTo(sampleImg, CV_32FC1, 1.0/255.0);
	
	// ����ѵ������ȡѵ�����
	Mat likeImg;	// ��Ȼ���ʣ��߶ȣ�������Ŀ*1�����ͣ�CV_32FC1��
	Mat labelImg;	// ��ǽ�����߶ȣ�������Ŀ*1�����ͣ�CV_32SC1��
	Mat probImg;	// ������ʣ��߶ȣ�������Ŀ*������Ŀ�����ͣ�CV_64FC1��
	gmmModel.set("maxIters", 300);
	gmmModel.set("epsilon", 0.001);
	cout << "Starting EM training" << endl; 
	gmmModel.train(sampleImg, likeImg, labelImg, probImg); // ����Ҫ������������noArray()����
	cout << "Finished training EM" << endl;

	///==================================================================================
	//                 ����ͼ�����ݹ���s-tͼ
	// ͼ���ÿ�����ض���Ϊs-tͼ�е�һ����ͨ���㣬ÿ�����������������ĸ����ض���һ��
	// n-links�ߣ�ÿ���������ն˶���S��T����һ��t-links�ߡ�n-links�ߵ�Ȩֵ�������ضԵĻҶ�
	// ���������㣬t-links�ߵ�Ȩֵ�������صĺ�����ʣ�������GMM����õ��������㡣
	//
	// ע����n-links�ߵ�Ȩֵ��Ҫ�����ضԵĿռ�λ�ö��仯ʱ������ʹ��GCoptimizationGeneralGraph
	// ������s-tͼ��ͨ��setNeighbors��Ա�������Զ������򣬷���ʹ��GCoptimizationGridGraph���ɡ�
	///----------------------------------------------------------------------------------
	// ����s-tͼ
	//GCoptimizationGridGraph gc(srcImg.cols, srcImg.rows, labelNum);
	GCoptimizationGeneralGraph gc(srcImg.cols * srcImg.cols, labelNum);

	// ����t-links�ߵ�Ȩֵ����Ӧ���������
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			int pos = r * srcImg.cols + c;
			double *pp = probImg.ptr<double>(pos);
			for(int l = 0; l < labelNum; l++)
			{
				// �������صĺ������������ͨ������Դ��S�ͻ��T������Ȩֵ
				// ע���û����Ը�����Ҫ��Ʋ�ͬ��Ȩֵ���㷽������������ͼ���ݶȵȡ�
				gc.setDataCost(pos, l, (float)-log(pp[l]));
			}
		}
	}

	// ����n-links�ߵ�Ȩֵ����Ӧ�߽�ƽ���
	//float beta = 20;
	//for(int l1 = 0; l1 < labelNum; l1++)
	//{
	//	for(int l2 = 0; l2 < labelNum; l2++)
	//	{
	//		// ע���û����Ը�����Ҫ��Ʋ�ͬ��Ȩֵ���㷽������������ͼ���ݶȵȡ�
	//		float cost = beta * (1 - (int)(l1 == l2));
	//		gc.setSmoothCost(l1, l2, cost);
	//	}
	//}

	//===================================================================================
	// ����һ�δ���������ض�<p,q>�ĻҶȲ���������������Ȩֵ�� ����
	// cost(p,q,Lp,Lq) = exp(-(Ip-Iq)^2/(2*sigma^2)) * (1-(Lp==Lq)
	// ʽ�У�Lp��Lq�ֱ�Ϊ���������p��q�ı�Ǻţ�Ip��Iq�ֱ�Ϊ����p��q�ĻҶ�ֵ
	// ע�⣺��Ҫ��GCoptimizationGeneralGraph������s-tͼ��ͨ��setNeighbors�������������ؼ���Ȩֵ
	
	// �߽�ƽ����Ĵ���
	float beta = 20;
	for(int lp = 0; lp < labelNum; lp++)
	{
		for(int lq = 0; lq < labelNum; lq++)
		{
			float pqDelta = beta * (1 - (int)(lp == lq));
			gc.setSmoothCost(lp, lq, (float)pqDelta);
		}
	}

	// ����ˮƽ����
	float sigma = 50;
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 1; c < srcImg.cols; c++)
		{
			int q = r * srcImg.cols + c;
			int p = q - 1;

			// ����������p��q֮��Ĳ������Խ��гͷ�
			int pqDiff = srcImg.at<uchar>(r, c-1) - srcImg.at<uchar>(r, c);
			float pqCost = exp(-(pqDiff * pqDiff) / (2 * sigma * sigma));
			gc.setNeighbors(p, q, pqCost);
		}
	}
	//-----------------------------------------------------------------------------------


	// ���ô�ֱ����
	for(int r = 1; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			int q = r * srcImg.cols + c;
			int p = q - srcImg.cols;

			// ����������p��q֮��Ĳ������Խ��гͷ�
			int pqDiff = srcImg.at<uchar>(r-1, c) - srcImg.at<uchar>(r, c);
			float pqCost = exp(-(pqDiff * pqDiff) / (2 * sigma * sigma));
			gc.setNeighbors(p, q, pqCost);
		}
	}

	///==================================================================================
	//                 �����С��������
	///----------------------------------------------------------------------------------
	cout << "Before optimization energy is " << gc.compute_energy() << endl; 
	gc.expansion(1); 
	cout << "After optimization energy is " << gc.compute_energy() << endl;

	///==================================================================================
	//                 ���ͼ����
	///----------------------------------------------------------------------------------
	// ���������ɫ�����ڸ�ÿһ�������Ǹ���һ�������ɫ��
	vector<Vec3b> colorTab;
	colorTab.reserve(labelNum);
	for(int i = 0; i < labelNum; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// ����ͼ�����������ͼ��
	Mat dstImg;
	cvtColor(srcImg, dstImg, CV_GRAY2BGR);
	int i = 0;
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			dstImg.at<Vec3b>(r, c) = colorTab[gc.whatLabel(i++)];
		}
	}

	///==================================================================================
	//                 ��ʾ�ָ���
	///----------------------------------------------------------------------------------
	// ��ʾԭʼͼ��
	imshow("Original image", srcImg);

	// ��ʾ��ǽ��
	Mat colorLabel(srcImg.size(), CV_8UC3);
	int j = 0;
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			colorLabel.at<Vec3b>(r, c) = colorTab[labelImg.at<int>(j++)];
		}
	}
	imshow("Labeled image", colorLabel);

	// ��ʾ��Ȼ����
	likeImg *= -1;
	likeImg = likeImg.reshape(1, srcImg.rows);
	imshow("Minus log likelihood image", likeImg);

	// ��ʾ�ָ���
	imshow("Segmentation result", dstImg);
	waitKey(0);

	return 0;
}

