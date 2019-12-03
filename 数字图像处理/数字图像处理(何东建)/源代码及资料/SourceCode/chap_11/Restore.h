/**************************************************************************************
*
*   ��    ����CRestore
*
*   ��    �ܣ�ʵ��ͼ��ԭ
*
*   �����б�ConvBlur()      ��ͼ����о��ģ����������һ������ԭ���˻�ͼ��
*
*             InvFilter()     ���˲���ԭͼ�񣬶��ɾ��ģ������ɵ��˻�ͼ����и�ԭ 
*             
*             ConvNoiseBlur() ��ͼ����о������ģ����������һ������ԭ���˻�ͼ��
*
*             Wiener()        ά���˲���ԭͼ�񣬶��ɾ����������ɵ��˻�ͼ����и�ԭ
*
**************************************************************************************/





#include "matlib.h"



// Restore.h: interface for the CRestore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESTORE_H__617CFF60_A93F_49C1_8D14_71AB03BA3B8A__INCLUDED_)
#define AFX_RESTORE_H__617CFF60_A93F_49C1_8D14_71AB03BA3B8A__INCLUDED_

#include "DibObject.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlib.h"

class CRestore : public CObject  
{
public:

	BOOL ConvBlur();
	BOOL Wiener();
	BOOL ConvNoiseBlur();
	BOOL InvFilter();
	Mm m_matBits;
	Mm m_matTemplate;

	CDibObject *m_pDibObject;

	DECLARE_DYNCREATE(CRestore)
	CRestore();
	CRestore(CDibObject *pDibObject);
	virtual ~CRestore();
protected:
	
};

#endif // !defined(AFX_RESTORE_H__617CFF60_A93F_49C1_8D14_71AB03BA3B8A__INCLUDED_)
