// Identify.h: interface for the CIdentify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDENTIFY_H__1B8B902B_239B_45B5_A13A_F42BD0324522__INCLUDED_)
#define AFX_IDENTIFY_H__1B8B902B_239B_45B5_A13A_F42BD0324522__INCLUDED_

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

class CIdentify : public CDibObject  
{
public:

	//����������ز���
	float m_antennaPara;
	float m_bellyPara;
	int   m_swingPara;
	float m_antennaFactor;
	float m_bellyFactor;

	//�߽����
	double GetPeriByChainCode();
	BOOL EdgeTrace();
	//������ȡ����
	BOOL ContourTrace();

	//��ȡͼ�����ĺͶ������ľ�
	float GetSquare();
	POINT GetCentroid();

	//�õ�����ĸ��ֲ�������
	int GetSwingPara();      
	float GetBellyPara();
	float GetAntennaPara();
	float GetArea();
	float GetEccentricity();

	//�õ��������̬���Ӻ���
	float GetAntennaFactor();
	float GetBellyFactor();

	//����ָ��
	BOOL SwingSep();
	BOOL BellySep();
	BOOL AntennaSep();

	//�������ָ�뺯��
	CIdentify * m_pIndentify;

	//���캯��
	CIdentify();
	CIdentify(CDibObject *);
	//��������
	virtual ~CIdentify();

};

#endif // !defined(AFX_IDENTIFY_H__1B8B902B_239B_45B5_A13A_F42BD0324522__INCLUDED_)
