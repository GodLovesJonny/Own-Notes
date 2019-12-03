// GeoTrans.h: interface for the CGeoTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOTRANS_H__526243D9_08A2_47D0_87CA_6AB2414B073D__INCLUDED_)
#define AFX_GEOTRANS_H__526243D9_08A2_47D0_87CA_6AB2414B073D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

// ������
#define PI 3.1415926535

//�Ƕȵ�����ת���ĺ�
#define RADIAN(angle) ((angle)*PI/180.0) 

class CGeoTrans : public CObject  
{
	DECLARE_DYNCREATE(CGeoTrans)
public:
	//���캯��
	CGeoTrans();
	CGeoTrans( CDibObject * );
	//��������
	virtual ~CGeoTrans();
	//����DibObject�����
	void SetDibObjectClass( CDibObject * );

public:
	//ͼ����ת
	BOOL Rotate(int nRotateAngle = 0,  CDibObject *pDibObject = NULL);
	//ͼ������
	BOOL ZoomImage(float fXZoomRatio = 1.0, float fYZoomRatio = 1.0, CDibObject *pDibObject = NULL);
	//ͼ��ת��
	BOOL Transpose(CDibObject *pDibObject = NULL);
	//ͼ����
	BOOL Mirror(BOOL bDirection = TRUE, CDibObject *pDibObject=NULL);
	//ͼ��ƽ��
	BOOL TranslationPixel(long lXOffset = 0, long lYOffset =0, CDibObject *pDibObject = NULL);
	
//��ϵ��Ժ���
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:
	CDibObject * m_pDibObject;
};

#endif // !defined(AFX_GEOTRANS_H__526243D9_08A2_47D0_87CA_6AB2414B073D__INCLUDED_)
