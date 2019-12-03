#if !defined(AFX_DLGHUFFMAN_H__A0595170_1A55_4467_8AF1_33C61E8782CF__INCLUDED_)
#define AFX_DLGHUFFMAN_H__A0595170_1A55_4467_8AF1_33C61E8782CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHuffman.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman dialog

//�������ɹ��������Ľ�㣬���Զ�̬����
typedef struct 
{
	int Weight;//���������������Ȩֵ���ڴ���ָƵ��
	int Flag;//��Flag=0ʱ����ʾ�ý��δ�������У�Ϊ1ʱ��ʾ�Ѽ�������
	int Parent;//��Ž��ĸ�����ڽ�������е����
	int LChild;//��Ž������ӽ���ڽ�������е����
	int RChild;//��Ž������ӽ���ڽ�������е����
}HuffmanNode;

class CDlgHuffman : public CDialog
{
// Construction
public:
	int m_iSizeImage;
	int * m_fFreq;//��Ҷ������Ҷȵĸ���
	int m_iLeafNum;//�������Ҷ�����Ŀ

	void Huffman();
	CDlgHuffman(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHuffman)
	enum { IDD = IDD_DLG_Huffman };
	CListCtrl	m_lstTable;
	int		m_ChannelSel;
	double	m_fCodeEfficiency;
	double	m_fEntropy;
	double	m_fAvCodeLen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHuffman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHuffman)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGreen();
	afx_msg void OnRadioBlue();
	afx_msg void OnRadioRed();
	afx_msg void OnRadioGray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHUFFMAN_H__A0595170_1A55_4467_8AF1_33C61E8782CF__INCLUDED_)
