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
	
	void MoveBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);

//��ϵ��Ժ���
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

private:
	CDibObject *m_pDibObject;

};

#endif // !defined(AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_)
