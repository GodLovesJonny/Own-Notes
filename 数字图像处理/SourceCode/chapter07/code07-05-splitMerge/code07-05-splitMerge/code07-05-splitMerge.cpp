/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-05-splitMerge.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��1��
  ��  ��: ���������ϲ�,���Զ��������ϲ���һ����׼��
  ��  ��:
	1. struct QuadTreeNode �Ĳ����ڵ�ṹ
	2. class QuadTree �Ĳ�����
  �����б�:
    1. bool predicate() ͼ��ϲ�/���ѵ�һ����׼��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/01    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


// �Ĳ����ڵ�ṹ
struct QuadTreeNode
{
	// �ӽڵ��б�
	QuadTreeNode* childs[4];

	// �ڵ������
	Rect rect;

	// �ڵ���Ч�ԣ����ڱ�ǽڵ��Ƿ��ѱ��ϲ���
	bool active;

	// �ڵ�����ֵ����ҶȾ�ֵ����׼�
	Scalar prop;

	// ���캯��
	QuadTreeNode(const Rect& roi)
	{
		rect = roi;
		active = true;
		prop = Scalar::all(0);

		for(int i = 0; i < 4; i++)
		{
			childs[i] = NULL;
		}
	}


	// �Ƿ�ΪҶ�ӽڵ�
	bool isLeafNode()
	{
		return (!childs[0] && !childs[1] && !childs[2] && !childs[3]);
	}
};

// �Ĳ�����
class QuadTree
{
public:
	// ���캯��
	QuadTree(const Mat& srcImg);

	// ��������
	~QuadTree();

	// �������
	void split(QuadTreeNode* &node, bool (*pred)(const Mat&));

	// ����ϲ�������һ����׼��ϲ�ָ������µ������ӽڵ㣩
	bool merge(QuadTreeNode* childA, QuadTreeNode* childB, bool (*pred)(const Mat&));

	// ����ϲ�������һ����׼��ݹ�ϲ�ָ������µ��ӽڵ㣩
	void merge(QuadTreeNode* node, bool (*pred)(const Mat&));

	// �������
	void draw(Mat& dstImg, QuadTreeNode* node);

	// ���ٽڵ�
	void destroy(QuadTreeNode* &node);

	// �Ĳ������ڵ�
	QuadTreeNode *root;

	// ͼ������
	Mat img;
};

/**
 * ���캯��
 * @param srcImg ���ڹ����Ĳ�����ͼ��ע��Ҫ��ͼ��ߴ�Ϊ2���������ݣ�
 */
QuadTree::QuadTree(const Mat& srcImg): img(srcImg)
{
	// �����Ĳ����ĸ��ڵ㣬��Ӧ������Ϊ����ͼ��
	root = new QuadTreeNode(Rect(0, 0, img.cols, img.rows));
}

/**
 * ��������
 */
QuadTree::~QuadTree()
{
	// �����Ĳ���
	if(root != NULL)
	{
		destroy(root);
	}
}

/**
 * ����ָ����һ����׼���ָ����ͼ����������Ĳ����ָ�
 * @param node ��Ҫ�ָ��ͼ�������Ӧ���Ĳ����ڵ㣨Ҫ��ýڵ㲻Ϊ�գ�
 * @param pred �����ж�ͼ�������Ƿ���Ҫ���ѵ�һ����׼�򣨺���ָ�룩
 */
void QuadTree::split(QuadTreeNode* &node, bool (*pred)(const Mat&))
{
	if(node == NULL)
	{
		cerr << "Error: Current node is NULL!" << endl;
		return;
	}
	
	// �����ǰ����Ӧ����������һ����Ҫ���������Ϊ4��������
	if(pred(img(node->rect)) != true && (node->rect.width > 1 && node->rect.height > 1))
	{
		int x = node->rect.x;
		int y = node->rect.y;
		int w = node->rect.width / 2;
		int h = node->rect.height / 2;

		node->childs[0] = new QuadTreeNode(Rect(x, y, w, h));
		node->childs[1] = new QuadTreeNode(Rect(x+w, y, w, h));
		node->childs[2] = new QuadTreeNode(Rect(x, y+h, w, h));
		node->childs[3] = new QuadTreeNode(Rect(x+w, y+h, w, h));

		///////////////////////////
		// ������ѱ�������
		//   0   ��   1
		// --------------
		//   2   ��   3
		///////////////////////////
		split(node->childs[0], pred);
		split(node->childs[1], pred);
		split(node->childs[2], pred);
		split(node->childs[3], pred);
	}
}

/**
 * ����ָ����һ����׼��ϲ������ڵ�
 * @param childA ��Ҫ�ϲ����Ĳ����ڵ�
 * @param childB ��Ҫ�ϲ����Ĳ����ڵ�
 * @param pred �����ж��Ƿ���Ҫ�ϲ���һ����׼�򣨺���ָ�룩
 */
bool QuadTree::merge(QuadTreeNode* childA, QuadTreeNode* childB, bool (*pred)(const Mat&))
{
	if(childA->isLeafNode() && childB->isLeafNode())
	{
		Rect rectAB = childA->rect | childB->rect;

		if(pred(img(rectAB)))
		{
			childA->rect = rectAB;
			childB->active = false;

			return true;
		}
	}

	return false;
}

/**
 * ����ָ����һ����׼��ϲ��Ĳ���/����
 * @param node ��Ҫ�ϲ����Ĳ���/�����ĸ��ڵ�
 * @param pred �����ж�ͼ�������Ƿ���Ҫ�ϲ���һ����׼�򣨺���ָ�룩
 */
void QuadTree::merge(QuadTreeNode* node, bool (*pred)(const Mat&))
{
	if(node->isLeafNode() != true)
	{
		bool bRow1, bRow2, bCol1, bCol2;

		// �ȳ��Ժϲ��з�������ڽڵ�����
		bRow1 = merge(node->childs[0], node->childs[1], pred);
		bRow2 = merge(node->childs[2], node->childs[3], pred);

		// �ٳ��Ժϲ��з�������ڽڵ�����
		if(!(bRow1 || bRow2))
		{
			bCol1 = merge(node->childs[0], node->childs[2], pred);
			bCol2 = merge(node->childs[1], node->childs[3], pred);
		}

		// �ݹ�ϲ�����ڵ�
		for(int i = 0; i < 4; i++)
		{
			if(node->childs[i]->isLeafNode() != true)
			{
				merge(node->childs[i], pred);
			}
		}
	}
}

/**
 * ��������ʽ��ָ��ͼ���ϻ���ָ�����Ĳ�����
 * @param dstImg ���ڻ����Ĳ��������ͼ������ڵ��ڴ����Ĳ�����Դͼ��
 * @param node ��Ҫ���Ƶ������ĸ����
 */
void QuadTree::draw(Mat& dstImg, QuadTreeNode* node)
{
	if(node != NULL)
	{
		if(node->active && node->isLeafNode())
		{
			rectangle(dstImg, node->rect, 50, 1);
		}
		else
		{
			draw(dstImg, node->childs[0]);
			draw(dstImg, node->childs[1]);
			draw(dstImg, node->childs[2]);
			draw(dstImg, node->childs[3]);
		}
	}
}

/**
 * ����ָ���ڵ㼰���ӽڵ㣨������
 * @param node ��Ҫ���ٵ��������ڵ�
 */
void QuadTree::destroy(QuadTreeNode* &node)
{
	if(node->isLeafNode())
	{
		delete node;
		node = NULL;
	}
	else
	{
		destroy(node->childs[0]);
		destroy(node->childs[1]);
		destroy(node->childs[2]);
		destroy(node->childs[3]);
	}
}

/**
 * ͼ��ϲ�/���ѵ�һ����׼��
 * @param src ͼ������
 * @return ������һ����Ҫ���򷵻�true�����򷵻�false
 */
bool predicate(const Mat& src)
{
	Scalar mean, sigma;
	meanStdDev(src, mean, sigma);

	return sigma[0] <= 5;
}


int main(int argc, char** argv)
{
	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the source file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// ��ͼ������߶����µ�����2����������
	int imgRows = (int)pow(2.0, (int)(log(srcImg.rows) / log(2)));
	int imgCols = (int)pow(2.0, (int)(log(srcImg.cols) / log(2)));
	Mat workImg = srcImg(Rect(0, 0, imgCols, imgRows));

	/// �����Ĳ���
	QuadTree qTree(workImg);

	/// �������
	qTree.split(qTree.root, predicate);

	/// ���Ʒ��ѽ��
	cv::Mat splitImg = workImg.clone();
	qTree.draw(splitImg, qTree.root);
	imshow("Split Result", splitImg);

	/// ����ϲ�
	qTree.merge(qTree.root, predicate);

	/// ���ƺϲ����
	cv::Mat mergeImg = workImg.clone();
	qTree.draw(mergeImg, qTree.root);
	imshow("Merge Image", mergeImg);	
	waitKey(0);

	return 0;
}

