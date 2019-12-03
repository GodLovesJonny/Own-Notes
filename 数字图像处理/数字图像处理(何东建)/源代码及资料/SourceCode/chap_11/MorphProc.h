// MorphProc.h: interface for the CMorphPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_)
#define AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

class CMorphPro : public CObject  
{
	DECLARE_DYNCREATE(CMorphPro)
public:
	//�޲ι��캯��
	CMorphPro();
	//�вι��캯��
	CMorphPro(CDibObject *pDibObject);
	//��������
	virtual ~CMorphPro();

public:
	//����DibObject�����
	void SetDibObjectClass(CDibObject *pDibObject);

	//��ʴ����
	BOOL Erosion(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//��������
	BOOL Dilation(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//������
	BOOL Opening(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//������
	BOOL Closing(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//���л����б任
	BOOL HitMissTran(int *nHmask = NULL, int nHmaskLen = 9, 
				int *nMmask = NULL, int nMmaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//�����߽�
	BOOL OutEdge(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//����ڱ߽�
	BOOL InEdge(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//�����̬ѧ�ݶ�
	BOOL MorphGrads(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//��������
	BOOL Thining(int *nHmask = NULL, int nHmaskLen = 9, 
				int *nMmask = NULL, int nMmaskLen = 9, 
				CDibObject *pDibObject = NULL,
				BOOL *bIsBlank = FALSE);
	//������
	BOOL Thicking(int *nHmask = NULL, int nHmaskLen = 9, 
				int *nMmask = NULL, int nMmaskLen = 9, 
				CDibObject *pDibObject = NULL);

	BOOL ThiningDIB(CDibObject *pDibObject=NULL);

protected:
	unsigned char *GetBitsPoint();
	//��ʴ����
	BOOL MakeErosion(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight);
	//��������
	BOOL MakeDilation(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight);
	
	void AndBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void ReverseBuffer(unsigned char *pIn, LONG lSize);
	void OrBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void XorBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void CopyBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void MoveBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void CleanBuffer(unsigned char *pIn, LONG lSize);
	BOOL IsResultBlank(unsigned char *pIn, LONG lSize);

//��ϵ��Ժ���
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

private:
	CDibObject *m_pDibObject;

};

#endif // !defined(AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_)
