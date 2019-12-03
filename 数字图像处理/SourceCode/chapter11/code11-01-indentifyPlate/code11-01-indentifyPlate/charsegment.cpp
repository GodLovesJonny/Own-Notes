/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: charsegment.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: CharSegment�ඨ�壬��¼�ַ�ͼ�����ڳ����е�λ��
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/
#include "charsegment.h"

CharSegment::CharSegment()
{
    //ctor
}
CharSegment::CharSegment(Mat matImg, Rect rectPos)
{
    charImg = matImg;
    charPos = rectPos;
}
CharSegment::~CharSegment()
{
    //dtor
}

Mat CharSegment::getCharImg()
{
    return charImg;
}
void CharSegment::setCharImg(Mat img)
{
    charImg = img;
}
Rect CharSegment::getCharPos()
{
    return charPos;
}
void CharSegment::setCharPos(Rect rectPos)
{
    charPos = rectPos;
}
