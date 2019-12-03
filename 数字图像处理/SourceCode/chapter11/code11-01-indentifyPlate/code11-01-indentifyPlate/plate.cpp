/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: plate.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: Plate��ʵ���ļ������ڼ�¼����ͼ������Ϣ
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/
#include "plate.h"

Plate::Plate()
{
    //ctor
}

Plate::Plate(Mat img, Rect pos)
{
    plateImg = img;
    platePos = pos;
}

Plate::~Plate()
{
    //dtor
}

Rect Plate::getPlatePos()
{
    return platePos;
}
void Plate::setPlatePos(Rect rectPos)
{
    platePos = rectPos;
}
Mat Plate::getPlateImg()
{
    return plateImg;
}
void Plate::setPlateImg(Mat img)
{
    plateImg = img;
}
vector< string > Plate::getStrChars()
{
    return strChars;
}
void Plate::pushStrChars(string strChar)
{
    strChars.push_back(strChar);
}
vector< Rect > Plate::getCharsPos()
{
    return charsPos;
}
void Plate::pushCharsPos(Rect rectPos)
{
    charsPos.push_back(rectPos);
}

/*************************************************
  ��    ��: string Plate::str()
  ��    ��: �������ҵ�˳�����ɳ����ַ���
  �������: ��
  �������: ��
  ��    ��: str-->�����ַ���
  ��    ��: ��
  ��    �ߣ����
  ��    �ڣ�2014��3��11��
*************************************************/
string Plate::str()
{
    string result = "";

    // �ַ�˳��
    vector< int > orderIndex;
    vector< int > xPositions;

    for (int i = 0; i < (int)charsPos.size(); i++)
    {
        orderIndex.push_back(i);
        xPositions.push_back(charsPos[i].x);
    }

    float min = xPositions[0];
    int minIdx = 0;

    for (int i = 0; i < (int)xPositions.size(); i++)
    {
        min = xPositions[i];
        minIdx = i;
        for (int j = i; j < (int)xPositions.size(); j++)
        {
            if (xPositions[j] < min)
            {
                min = xPositions[j];
                minIdx = j;
            }
        }

        int auxI = orderIndex[i];
        int auxMin = orderIndex[minIdx];
        orderIndex[i] = auxMin;
        orderIndex[minIdx] = auxI;

        float auxXi = xPositions[i];
        float auxXmin = xPositions[minIdx];
        xPositions[i] = auxXmin;
        xPositions[minIdx] = auxXi;
    }

    for (int i = 0; i < (int)orderIndex.size(); i++)
    {
        result = result + strChars[orderIndex[i]];
    }

    return result;
}
