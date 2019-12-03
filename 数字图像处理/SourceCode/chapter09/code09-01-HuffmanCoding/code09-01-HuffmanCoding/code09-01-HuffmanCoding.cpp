/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code09-01-HuffmanCoding.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��5��3��
  ��  ��: ʵ�ֹ���������ͽ��롣
  ��  ��: 
  �����б�:
    1. void computeSymbolFreq();���㵥ͨ��ͼ��ĻҶȳ��ָ���
    2. void selectTwoMinNodes();�ӷ��ŵĵ�ǰ�ĸ��ʼ�����ѡ��������С�ĸ���
    3. void initHuffmanTree();��ʼ����������
    4. void createHuffmanTree();������������
    5. void makeHuffmanTable();���ɹ��������
    6. void HuffmanEncode();����������
	7. void HuffmanDecode();����������
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/05/03    1.0     �����ļ�
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <bitset>

using namespace cv;
using namespace std;

// ������ֵ�����Ÿ���
typedef std::pair<int, float> SymbolFreqPair;

// �����������
struct HuffmanNode
{
	float weight;	// Ȩֵ�����ų��ָ���
	int parent;		// ����㣺�ڽ�������е�����
	int lchild;		// ���ӽ�㣺�ڽ�������е�����
	int rchild;		// ���ӽ�㣺�ڽ�������е�����
};

// ������������Ϣ������������Ϣ��
struct HuffmanCodeInfo
{
	int rows;		// ͼ������
	int cols;		// ͼ������
	int type;		// ��������
	float cr;		// ѹ����
	std::map<int, std::string> symbolCodeMap;	// ���������������ֵ --> ��������
};


// ���㵥ͨ��ͼ��ĻҶȳ��ָ���
void computeSymbolFreq(const cv::Mat &srcMat, std::vector<SymbolFreqPair> &symbolFreqVec);

// �ӷ��ŵĵ�ǰ�ĸ��ʼ�����ѡ��������С�ĸ���
void selectTwoMinNodes(std::vector<HuffmanNode> &huffmanTree, int nextParentIdx, int &minIdx1, int &minIdx2);

// ��ʼ����������
void initHuffmanTree(std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec);

// ������������
void createHuffmanTree(std::vector<HuffmanNode> &nodeVec, int leafNodeNum);

// ���ɹ��������
void makeHuffmanTable(const std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec, std::map<int, std::string> &symbolCodeMap);

// ����������
void HuffmanEncode(cv::Mat &srcMat, HuffmanCodeInfo &codeInfo, std::vector<unsigned char> &codeStream);

// ����������
void HuffmanDecode(cv::Mat &dstMat, HuffmanCodeInfo &codeInfo, const std::vector<unsigned char> &codeStream);

int main(int argc, char** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if (srcImg.empty() || srcImg.data==NULL)
	{
		cout << "Failed to load image: " << srcFileName << endl;
		return 1;
	}

	/// ��ȡͼ���ָ��ͨ��
	cv::Mat srcMat;
	cv::extractChannel(srcImg, srcMat, 0);

	/// ����������
	HuffmanCodeInfo codeInfo;
	std::vector<unsigned char> codeStream;
	HuffmanEncode(srcMat, codeInfo, codeStream);


	/// ����������
	cv::Mat dstMat;
	HuffmanDecode(dstMat, codeInfo, codeStream);

	/// �洢����ͼ��
	cv::imwrite("decodedImage.bmp", dstMat);
	
	return 0;
}

/*************************************************
  ��    ��: computeSymbolFreq(const cv::Mat &srcMat, std::vector<SymbolFreqPair> &symbolFreqVec)
  ��    ��: ���㵥ͨ��ͼ��ĻҶȳ��ָ���
  ���ú���: 
  ��������: 
  �������: cv::Mat &srcMat-->������ĵ�ͨ��ͼ��
  �������: std::vector<SymbolFreqPair> &symbolFreqVec-->�Ҷȵĳ��ָ��ʣ�����Ϊ0�ĻҶȳ��⣩
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void computeSymbolFreq(const cv::Mat &srcMat, std::vector<SymbolFreqPair> &symbolFreqVec)
{
	/// ͳ�����п��ܻҶȵĳ��ִ���
	std::vector<float> freqVec((0x01 << (srcMat.elemSize1()*8)), 0);
	const uchar *p = NULL;
	for(int r = 0; r < srcMat.rows; r++)
	{
		p = srcMat.ptr<uchar>(r);
		for (int c = 0; c < srcMat.cols; c++)
		{
			freqVec[p[c]]++;
		}
	}

	/// ����Ҷȵĳ��ָ��ʣ�����Ϊ0�ĻҶȳ��⣩
	symbolFreqVec.reserve(freqVec.size());
	unsigned int pixels = srcMat.rows * srcMat.cols;
	for(unsigned int i = 0; i < freqVec.size(); i++)
	{
		if(freqVec[i] != 0)
		{
			symbolFreqVec.push_back(SymbolFreqPair(i, freqVec[i]/pixels));
		}
	}
	symbolFreqVec.shrink_to_fit();
}

/*************************************************
  ��    ��: selectTwoMinNodes(std::vector<HuffmanNode> &huffmanTree, int upIdx, int &minIdx1, int &minIdx2)
  ��    ��: �ӷ��ŵĵ�ǰ�ĸ��ʼ�����ѡ��������С�ĸ���
  ���ú���: 
  ��������: 
  �������: std::vector<HuffmanNode> &huffmanTree-->���������������
            int nextParentIdx-->������С���ʽ�㽫Ҫ�ϲ��γɵ��½���ڽ�������е�����
  �������: int &minIdx1-->��С���ʽ���ڽ�������е�����
            int &minIdx2-->����С���ʽ���ڽ�������е�����
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void selectTwoMinNodes(std::vector<HuffmanNode> &huffmanTree, int nextParentIdx, int &minIdx1, int &minIdx2)
{
	// ��С����������
	float minFreq1 = 1.0f, minFreq2 = 1.0f;

	/// �ӵ�ǰ���ʼ��ϣ������ڴ���ķ��ӽ�㼯�ϣ����ҳ���С����������
	minIdx1 = minIdx2 = 0;
	for(int j = 0; j < nextParentIdx; j++)
	{
		if(huffmanTree[j].parent == -1)
		{
			if(huffmanTree[j].weight < minFreq1)
			{
				minFreq2 = minFreq1;
				minIdx2 = minIdx1;
				minFreq1 = huffmanTree[j].weight;
				minIdx1 = j;
			}
			else if(huffmanTree[j].weight < minFreq2)
			{
				minFreq2 = huffmanTree[j].weight;
				minIdx2 = j;
			}
		}
	}
}

/*************************************************
  ��    ��: initHuffmanTree(std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec)
  ��    ��: ��ʼ����������
  ���ú���: 
  ��������: 
  �������: const std::vector<SymbolFreqPair> &symbolFreqVec-->���ŵĳ��ָ���
  �������: std::vector<HuffmanNode> &nodeVec-->���������������
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void initHuffmanTree(std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec)
{
	/// Ҷ�ӽ����Ϊn�Ĺ�����������2n-1�����
	int leafNum = symbolFreqVec.size();
	int nodeNum = 2 * leafNum - 1;
	nodeVec.resize(nodeNum);

	/// ��ʼ��Ҷ�ӽ��
	for(int i = 0; i < leafNum; i++)
	{
		HuffmanNode &curNode = nodeVec[i];
		curNode.weight = symbolFreqVec[i].second;
		curNode.parent = -1;
		curNode.lchild = -1;
		curNode.rchild = -1;
	}

	/// ��ʼ����Ҷ�ӽ��
	for(int i = leafNum; i < nodeNum; i++)
	{
		HuffmanNode &curNode = nodeVec[i];
		curNode.weight = 0;
		curNode.parent = -1;
		curNode.lchild = -1;
		curNode.rchild = -1;
	}
}

/*************************************************
  ��    ��: createHuffmanTree(std::vector<HuffmanNode> &nodeVec, int leafNum)
  ��    ��: ������������
  ���ú���: selectTwoMinNodes()
  ��������: 
  �������: std::vector<HuffmanNode> &nodeVec-->���������������
            int leafNodeNum-->Ҷ�ӽ����Ŀ���������������Ŀ��
  �������: std::vector<HuffmanNode> &nodeVec-->���������������
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void createHuffmanTree(std::vector<HuffmanNode> &nodeVec, int leafNodeNum)
{
	int minIdx1, minIdx2;
	for(unsigned int i = leafNodeNum; i < nodeVec.size(); i++)
	{
		/// ѡ�������С���ʵ��������
		selectTwoMinNodes(nodeVec, i, minIdx1, minIdx2);

		/// �ϲ�������С���ʵ��������
		nodeVec[minIdx1].parent = i;
		nodeVec[minIdx2].parent = i;
		nodeVec[i].weight = nodeVec[minIdx1].weight + nodeVec[minIdx2].weight;
		nodeVec[i].lchild = minIdx2;
		nodeVec[i].rchild = minIdx1;
	}
}

/*************************************************
  ��    ��:  makeHuffmanTable(const std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec, std::map<int, std::string> &symbolCodeMap)
  ��    ��: ���ɹ��������
  ���ú���: 
  ��������: 
  �������: const std::vector<HuffmanNode> &nodeVec-->���������������
            const std::vector<SymbolFreqPair> &symbolFreqVec-->����--�������飩
  �������: std::map<int, std::string> &symbolCodeMap-->�������������--����
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void makeHuffmanTable(const std::vector<HuffmanNode> &nodeVec, const std::vector<SymbolFreqPair> &symbolFreqVec, std::map<int, std::string> &symbolCodeMap)
{
	for(unsigned int i = 0; i < symbolFreqVec.size(); i++)
	{
		// ��Ҷ�ӽ�����ݵ�����㣬���ӽ������ָ�λ���0�����ӽ������ָ�λ���1
		int p = nodeVec[i].parent, c = i;
		std::string szCode;
		while(p != -1)
		{
			if(nodeVec[p].lchild == c)
				szCode = "0" + szCode;
			else
				szCode = "1" + szCode;
			c = p;
			p = nodeVec[c].parent;
		}
		symbolCodeMap[symbolFreqVec[i].first] = szCode;
	}
}

/*************************************************
  ��    ��: HuffmanEncode(cv::Mat &srcMat, std::vector<unsigned char> &codeStream)
  ��    ��: �Ե�ͨ��ͼ����й���������
  ���ú���: computeSymbolFreq()��initHuffmanTree()��createHuffmanTree()
            makeHuffmanTable()
  ��������: 
  �������: cv::Mat &srcMat-->������ĵ�ͨ��ͼ��
            HuffmanCodeInfo &codeInfo-->������������Ϣ�����ڹ��������룩
  �������: std::vector<unsigned char> &codeStream-->������ѹ��λ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void HuffmanEncode(cv::Mat &srcMat, HuffmanCodeInfo &codeInfo, std::vector<unsigned char> &codeStream)
{
	/// ������Ÿ���
	std::vector<SymbolFreqPair> symbolFreqVec;
	computeSymbolFreq(srcMat, symbolFreqVec);

	/// ��ʼ����������
	std::vector<HuffmanNode> huffmanTree;
	initHuffmanTree(huffmanTree, symbolFreqVec);

	/// ������������
	createHuffmanTree(huffmanTree, symbolFreqVec.size());

	/// ���ɹ��������
	makeHuffmanTable(huffmanTree, symbolFreqVec, codeInfo.symbolCodeMap);

	/// ����ͼ�����볤
	unsigned int totCodeLen = 0;
	unsigned int totPixels = srcMat.rows * srcMat.cols;
	for(unsigned int i = 0; i < symbolFreqVec.size(); i++)
	{
		totCodeLen = totCodeLen + (unsigned int)(totPixels * symbolFreqVec[i].second) * codeInfo.symbolCodeMap[symbolFreqVec[i].first].length();
	}

	/// ���������ַ���
	std::string szCodeStream;
	szCodeStream.reserve(totCodeLen);
	const uchar *p = 0;
	for(int r = 0; r < srcMat.rows; r++)
	{
		p = srcMat.ptr<uchar>(r);
		for(int c = 0; c < srcMat.cols; c++)
		{
			std::string szTemp = codeInfo.symbolCodeMap[p[c]];
			szCodeStream.append(szTemp);
		}
	}

	/// ת��Ϊ����λ��
	unsigned int codeBytes = (totCodeLen + 7) / 8;
	codeStream.reserve(codeBytes);
	for(unsigned int i = 0; i < codeBytes; i++)
	{
		codeStream.push_back((unsigned char)(std::strtoul(szCodeStream.substr(i*8, 8).c_str(), NULL, 2)));
	}

	/// ���������Ϣ
	codeInfo.rows = srcMat.rows;
	codeInfo.cols = srcMat.cols;
	codeInfo.type = srcMat.type();
	codeInfo.cr = srcMat.rows * srcMat.cols * srcMat.elemSize1() * 8 / (float)totCodeLen;

	/// ���������Ϣ
	cout << "ѹ���ʣ�" << codeInfo.cr << endl;
	cout << "���������\n������ֵ\t���ű���" << endl;
	std::map<int, std::string>::iterator codeIter = codeInfo.symbolCodeMap.begin();
	for(;codeIter != codeInfo.symbolCodeMap.end(); ++codeIter)
	{
		cout << codeIter->first << "\t" << codeIter->second << endl;
	}
}

/*************************************************
  ��    ��: HuffmanDecode(cv::Mat &dstMat, HuffmanCodeInfo &codeInfo, const std::vector<unsigned char> &codeStream)
  ��    ��: ��λ�����й���������
  ���ú���: 
  ��������: 
  �������: std::vector<unsigned char> &codeStream-->������ѹ��λ��
            HuffmanCodeInfo &codeInfo-->������������Ϣ�����ڹ��������룩
  �������: cv::Mat &dstMat-->������ĵ�ͨ��ͼ��
  ��    ��: ��
  ��    ��: ��
  ��    �ߣ�������
  ��    �ڣ�2014��5��3��
*************************************************/
void HuffmanDecode(cv::Mat &dstMat, HuffmanCodeInfo &codeInfo, const std::vector<unsigned char> &codeStream)
{
	/// ��������������������볤
	std::map<std::string, int> codeMap;	// ���ű��� -- ������ֵ
	unsigned int maxCodeLen = 0; // ����볤
	std::map<int, std::string>::iterator iter = codeInfo.symbolCodeMap.begin();
	for(; iter != codeInfo.symbolCodeMap.end(); ++iter)
	{
		if(maxCodeLen < iter->second.length())
		{
			maxCodeLen = iter->second.length();
		}

		codeMap[iter->second] = iter->first;
	}

	/// ���ݱ�����Ϣ�������ͼ��
	dstMat.create(codeInfo.rows, codeInfo.cols, codeInfo.type);

	/// ���ڹ�����������������
	std::string szCode;	// ����
	unsigned int decodedSymbols = 0; // �ѽ�������Ŀ
	unsigned int totPixels = codeInfo.rows * codeInfo.cols; // ͼ��������Ŀ
	std::map<std::string, int>::const_iterator codeIter;
	for(unsigned int i = 0; i < codeStream.size(); i++)
	{
		/// ��������ȡ��һ�ֽڣ�ת��Ϊ�������ַ���
		std::string szByte = std::bitset<8>(codeStream[i]).to_string();

		/// ÿ�δ��ַ�����ȡ��1λ������֣����ݹ������������
		for(int j = 0; j < 8 && decodedSymbols < totPixels; j++)
		{
			/// ���ַ�����ȡ��1λ�������
			szCode.push_back(szByte[j]);

			/// ���ڹ�����������ҵ���Ӧ���֣��������Ӧ����ֵ���Ҷȣ���ͼ��
			codeIter = codeMap.find(szCode);
			if(codeIter != codeMap.end())
			{
				dstMat.at<uchar>(decodedSymbols / codeInfo.cols, decodedSymbols % codeInfo.cols) = codeIter->second;
				decodedSymbols++;
				szCode.clear();
			}
			else if(szCode.length() > maxCodeLen)
			{
				cerr << "Incorrect code stream!" << endl;
				exit(1);
			}
		}
	}
}
