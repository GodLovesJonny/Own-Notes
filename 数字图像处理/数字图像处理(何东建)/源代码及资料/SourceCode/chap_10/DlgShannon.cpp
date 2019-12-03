// DlgShannon.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgShannon.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog


CDlgShannon::CDlgShannon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShannon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShannon)
	m_dEfficiency = 0.0;
	m_dAvgCodeLen = 0.0;
	m_dEntropy = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgShannon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShannon)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Text(pDX, IDC_EDIT_CODEEF, m_dEfficiency);
	DDX_Text(pDX, IDC_EDIT_CODELEN, m_dAvgCodeLen);
	DDX_Text(pDX, IDC_EDIT_ENTROPY, m_dEntropy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShannon, CDialog)
	//{{AFX_MSG_MAP(CDlgShannon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon message handlers

BOOL CDlgShannon::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ShannonCode();

	// ����TRUE
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShannon::ShannonCode()
{
	int i,j;
	BYTE uTempMap;
	CString str;
	//����Դ���ŵĴ��볤��
	int iCodeLen;
	
	//����Դ���Ŷ�Ӧ���ۼӸ���	
	FLOAT fTemp = 0.0f,iAccP = 0.0f;

	//�����ڴ�
	m_strCode = new CString[m_iColorNum];
	
	//����ð�ݷ��Դ��������Դ���Ű����ָ��ʴӴ�С����
	for(j=0;j<m_iColorNum-1;j++)
	{
		for(i=0;i<m_iColorNum-j-1;i++)
		{
			if( m_fFreq[i] < m_fFreq[i+1] )
			{
				fTemp = m_fFreq[i];
				m_fFreq[i] = m_fFreq[i+1];
				m_fFreq[i+1] = fTemp;

				//����ӳ���ϵ
				uTempMap = m_uMap[i];
				m_uMap[i] = m_uMap[i+1];
				m_uMap[i+1] = uTempMap;
			}
		}
	}
	
	//������ѱ�����������������Դ���ŵ�����
	for(i=0;i<m_iColorNum;i++)
	{
		//����ѱ������ۼ������i�����ŵ����ֳ���iCodeLen
		fTemp = (float)(-log(m_fFreq[i])/log(2));
		if(fTemp > (int)fTemp) iCodeLen = (int)fTemp + 1;
		else iCodeLen = (int)fTemp;
		
		//����Ӧ���ۼӸ���ת��Ϊ����ΪiCodeLen�Ķ�������
		j = 0;
		fTemp = iAccP;
		while(j < iCodeLen)
		{
			if( fTemp*2 >= 1.0 )
			{
				m_strCode[i] += "1";
				fTemp = fTemp*2 - 1;
			}
			else
			{
				m_strCode[i] += "0";
				fTemp = fTemp * 2;
			}
			j++;
		}
		
		//������һ����Դ���Ŷ�Ӧ���ۼӸ���iAccP(��һ�����ŵ��ۼӸ���Ϊ0)
		iAccP += m_fFreq[i];
	}

	// ����ƽ�����ֳ���
	for (i = 0; i < m_iColorNum; i ++)
	{
		// �ۼ�
		m_dAvgCodeLen += m_fFreq[i] * m_strCode[i].GetLength();
		//����ͼ����
		m_dEntropy -= m_fFreq[i] * log(m_fFreq[i]) / log(2.0);
	}
	
	// �������Ч��
	m_dEfficiency = m_dEntropy / m_dAvgCodeLen;

	//���¶Ի����ϵ��ı����е�����
	UpdateData(FALSE);

	// ���������
	
	// ListCtrl��ITEM
	LV_ITEM lvitem;
	
	// �м����������ListCtrl����ӵ�ITEM���
	int		iActualItem;
	
	// ����List�ؼ���ʽ
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	
	// ��List�ؼ����Header
	m_lstTable.InsertColumn(0, "�Ҷ�ֵ", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "����Ƶ��", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "��ũ��ŵ����", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "���ֳ���", LVCFMT_LEFT, 78, 2);
	
	// ������ʽΪ�ı�
	lvitem.mask = LVIF_TEXT;
	
	for (i = 0; i < m_iColorNum; i ++)
	{
		// ���һ��
		lvitem.iItem = m_lstTable.GetItemCount();
		str.Format("%u",m_uMap[i]);
		lvitem.iSubItem = 0;
		lvitem.pszText= (LPTSTR)(LPCTSTR)str;
		iActualItem = m_lstTable.InsertItem(&lvitem);
		
		// ���������
		lvitem.iItem = iActualItem;
		
		// ��ӻҶ�ֵ���ֵ�Ƶ��
		lvitem.iSubItem = 1;
		str.Format("%f",m_fFreq[i]);
		lvitem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvitem);
		
		// �����ũ��ŵ����
		lvitem.iSubItem = 2;
		lvitem.pszText = (LPTSTR)(LPCTSTR)m_strCode[i];
		m_lstTable.SetItem(&lvitem);
		
		// ������ֳ���
		lvitem.iSubItem = 3;
		str.Format("%u",m_strCode[i].GetLength());
		lvitem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvitem);
	}
	delete []m_strCode;
}