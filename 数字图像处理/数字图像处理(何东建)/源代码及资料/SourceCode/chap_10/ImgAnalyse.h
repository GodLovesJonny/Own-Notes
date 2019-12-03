// ImgAnalyse.h: interface for the CImgAnalyse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGANALYSE_H__74A2A985_E51E_43DC_893F_E82EC4C920A2__INCLUDED_)
#define AFX_IMGANALYSE_H__74A2A985_E51E_43DC_893F_E82EC4C920A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////
//���ܣ�EdgePoint��һ���洢�߽�����б߽������ꡢ��ǰʸ������ʸ���Ľṹ��
//      TraceArrayΪ�洢�߽���ٽ����Ԫ��ΪEdgePoint�͵�ģ�����顣
//˵����(1)��ǰʸ����ǰһ���߽�㵽��ǰ�߽���ʸ������;
//      (2)��ʸ�����߽��ϵ�ǰ�㵽��һ���߽���ʸ������;
//	    (3)�����в��õ�8�������룬�䷽�������£�
//            3     2     1
//            4     P     0
//            5     6     7
//      (4)�߽����ʱ�����µ��ϣ������ң���ʱ����С�
//////////////////////////////////////////////////////////////////////////////
struct EdgePoint
{
	BYTE nCurVerct;			//��ǰʸ�����������������е�ǰһ����������
	CPoint CurPoint;		//��ǰ�������
};
static CArray<EdgePoint,EdgePoint&> TraceArray;

//�����ǽ���Ľṹ��
struct LabelResult
{
	int nLabelNo;			//�����Ǻ�
	CPoint StartPoint;		//����������ʼ��
	CPoint CenterPoint;		//������������
	DWORD LabelArea;		//�����������
};

static CArray<LabelResult,LabelResult&> LabelArray;

#include "DibObject.h"
#include "matlib.h"

class CImgAnalyse : public CObject  
{
	DECLARE_DYNCREATE(CImgAnalyse)
public:
	//��ȡ����������������
	POINT GetCentroid();
	//������
	BOOL LabelArea(CDibObject *pDibObject = NULL);
	//����Ҷ������
	double * FourierFactor(int *nDimSize);
	//�ɱ߽����������������ܳ�
	BOOL PeriByChainCode(float m_fXScale,float m_fYScale,float m_fXYScale,float *fP);
	//���ø��ֹ�ʽ�ɱ߽������������������
	BOOL AreaByGreen(float m_fXScale,float m_fYScale,float *fArea);
	//�ɱ߽����������
	BOOL AreaByChain(float *fArea);
	//��ȡ�߽����
	int GetEdgePointNum();
	//�ߴ�궨
	void Calibrate(float *X_Scale, float *Y_Scale, float *XY_Scale,float realSize);
	//�߽����
	BOOL EdgeTrace(CDibObject *pDibObject = NULL);
	//���캯��
	CImgAnalyse();
	CImgAnalyse(CDibObject * );

	//��������
	virtual ~CImgAnalyse();

	//����DibObject�����
	void SetDibObjectClass( CDibObject * );

	//��ϵ��Ժ���
	#ifdef _DEBUG
		virtual void Dump( CDumpContext &dc) const;
		virtual void AssertValid() const;
	#endif

protected:
	CDibObject * m_pDibObject;
	//����MatLab�������
	Mm mFourier;

};

#endif // !defined(AFX_IMGANALYSE_H__74A2A985_E51E_43DC_893F_E82EC4C920A2__INCLUDED_)
