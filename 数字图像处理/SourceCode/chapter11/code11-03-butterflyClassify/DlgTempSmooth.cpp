// DlgTempSmooth.cpp : implementation file
//

#include "stdafx.h"
#include "Insects.h"
#include "DlgTempSmooth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTempSmooth dialog


CDlgTempSmooth::CDlgTempSmooth(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTempSmooth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTempSmooth)
	m_intType = -1;
	m_iTempH = 0;
	m_iTempW = 0;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_fTempC = 0.0f;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTempSmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTempSmooth)
	DDX_Radio(pDX, IDC_TEMP1, m_intType);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_iTempH);
	DDV_MinMaxInt(pDX, m_iTempH, 2, 5);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_iTempW);
	DDV_MinMaxInt(pDX, m_iTempW, 2, 5);
	DDX_Text(pDX, IDC_EDIT_MX, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_iTempMY);
	DDX_Text(pDX, IDC_EDIT_TEMPC, m_fTempC);
	DDX_Text(pDX, IDC_EDIT_SELF1, m_fpArray[0]);
	DDX_Text(pDX, IDC_EDIT_SELF2, m_fpArray[1]);
	DDX_Text(pDX, IDC_EDIT_SELF3, m_fpArray[2]);
	DDX_Text(pDX, IDC_EDIT_SELF4, m_fpArray[3]);
	DDX_Text(pDX, IDC_EDIT_SELF5, m_fpArray[4]);
	DDX_Text(pDX, IDC_EDIT_SELF6, m_fpArray[5]);
	DDX_Text(pDX, IDC_EDIT_SELF7, m_fpArray[6]);
	DDX_Text(pDX, IDC_EDIT_SELF8, m_fpArray[7]);
	DDX_Text(pDX, IDC_EDIT_SELF9, m_fpArray[8]);
	DDX_Text(pDX, IDC_EDIT_SELF10, m_fpArray[9]);
	DDX_Text(pDX, IDC_EDIT_SELF11, m_fpArray[10]);
	DDX_Text(pDX, IDC_EDIT_SELF12, m_fpArray[11]);
	DDX_Text(pDX, IDC_EDIT_SELF13, m_fpArray[12]);
	DDX_Text(pDX, IDC_EDIT_SELF14, m_fpArray[13]);
	DDX_Text(pDX, IDC_EDIT_SELF15, m_fpArray[14]);
	DDX_Text(pDX, IDC_EDIT_SELF16, m_fpArray[15]);
	DDX_Text(pDX, IDC_EDIT_SELF17, m_fpArray[16]);
	DDX_Text(pDX, IDC_EDIT_SELF18, m_fpArray[17]);
	DDX_Text(pDX, IDC_EDIT_SELF19, m_fpArray[18]);
	DDX_Text(pDX, IDC_EDIT_SELF20, m_fpArray[19]);
	DDX_Text(pDX, IDC_EDIT_SELF21, m_fpArray[20]);
	DDX_Text(pDX, IDC_EDIT_SELF22, m_fpArray[21]);
	DDX_Text(pDX, IDC_EDIT_SELF23, m_fpArray[22]);
	DDX_Text(pDX, IDC_EDIT_SELF24, m_fpArray[23]);
	DDX_Text(pDX, IDC_EDIT_SELF25, m_fpArray[24]);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTempSmooth, CDialog)
	//{{AFX_MSG_MAP(CDlgTempSmooth)
	ON_BN_CLICKED(IDC_TEMP1, OnTemp1)
	ON_BN_CLICKED(IDC_TEMP2, OnTemp2)
	ON_BN_CLICKED(IDC_TEMP3, OnTemp3)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPH, OnKillfocusEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPW, OnKillfocusEditTempw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTempSmooth message handlers

void CDlgTempSmooth::OnTemp1() 
{
	// 3��3ƽ��ģ��
	m_intType = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 9.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 1.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 1.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 1.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 1.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
	
}

void CDlgTempSmooth::OnTemp2() 
{
	// 3��3��˹ģ��
	m_intType = 1;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 16.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 2.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 2.0;
	m_fpArray[6] = 4.0;
	m_fpArray[7] = 2.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 2.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
	
}

void CDlgTempSmooth::OnTemp3() 
{
	// �Զ���ģ��
	m_intType = 2;
	
	// �����ı���״̬
	UpdateEdit();
	
}

void CDlgTempSmooth::OnOK() 
{
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	
	// �ж������Ƿ���Ч
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		// ��ʾ�û��������ô���
		MessageBox("����Ԫ�ز������ô���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ����ģ��Ԫ�����飨����ЧԪ�ط����������ǰ�棩
	for (int i = 0; i < m_iTempH; i++)
	{
		for (int j = 0; j < m_iTempW; j++)
		{
			m_fpArray[i * m_iTempW + j] = m_fpArray[i * 5 + j];
		}
	}
	
	// ����
	UpdateData(FALSE);
	
	// �˳�
	CDialog::OnOK();
}

void CDlgTempSmooth::UpdateEdit()
{
	BOOL	bEnable;
	
	// ѭ������
	int		i;
	int		j;
	
	// �ж��ǲ����Զ���ģ��
	if (m_intType == 2) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// �����ı������״̬
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPC)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);
	
	// IDC_EDIT_V0��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
	
	// ����ģ��Ԫ���ı���Enable״̬
	for (i = IDC_EDIT_SELF1; i <= IDC_EDIT_SELF25; i++)
	{
		// �����ı��򲻿ɱ༭
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	// ��ʾӦ�ÿɼ���ģ��Ԫ���ı���
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = 0; j < m_iTempW; j++)
		{
			// �����ı���ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT_SELF1 + i*5 + j)->ShowWindow(SW_SHOW);
		}
	}
	
	// ����Ӧ�ò��ɼ���ģ��Ԫ���ı���ǰm_iTempH�еĺ��У�
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = m_iTempW; j < 5; j++)
		{
			// ���ò��ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT_SELF1 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}
	
	// ����Ӧ�ò��ɼ���ģ��Ԫ���ı��򣨺��У�
	for (i = m_iTempH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			// ���ò��ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT_SELF1 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}

}

void CDlgTempSmooth::OnKillfocusEditTemph() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgTempSmooth::OnKillfocusEditTempw() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}
