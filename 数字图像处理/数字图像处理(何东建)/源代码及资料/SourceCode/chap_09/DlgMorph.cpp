// CDlgMorph.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgMorph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMorph dialog


CDlgMorph::CDlgMorph(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMorph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMorph)
	m_nStruH  = 3;
	m_nStruW  = 3;
	m_nStruXc = 1;
	m_nStruYc = 1;
	m_bStru00 = FALSE;
	m_bStru01 = TRUE;
	m_bStru02 = FALSE;
	m_bStru03 = FALSE;
	m_bStru04 = FALSE;
	m_bStru05 = TRUE;
	m_bStru06 = TRUE;
	m_bStru07 = TRUE;
	m_bStru08 = FALSE;
	m_bStru09 = FALSE;
	m_bStru10 = FALSE;
	m_bStru11 = TRUE;
	m_bStru12 = FALSE;
	m_bStru13 = FALSE;
	m_bStru14 = FALSE;
	m_bStru15 = FALSE;
	m_bStru16 = FALSE;
	m_bStru17 = FALSE;
	m_bStru18 = FALSE;
	m_bStru19 = FALSE;
	m_bStru20 = FALSE;
	m_bStru21 = FALSE;
	m_bStru22 = FALSE;
	m_bStru23 = FALSE;
	m_bStru24 = FALSE;
	m_nMode = 0;
	//}}AFX_DATA_INIT
	m_nMaskLen = 0;
}


void CDlgMorph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMorph)
	DDX_Control(pDX, IDC_STRU00, m_ctrlStru00);
	DDX_Control(pDX, IDC_STRU01, m_ctrlStru01);
	DDX_Control(pDX, IDC_STRU02, m_ctrlStru02);
	DDX_Control(pDX, IDC_STRU03, m_ctrlStru03);
	DDX_Control(pDX, IDC_STRU04, m_ctrlStru04);
	DDX_Control(pDX, IDC_STRU05, m_ctrlStru05);
	DDX_Control(pDX, IDC_STRU06, m_ctrlStru06);
	DDX_Control(pDX, IDC_STRU07, m_ctrlStru07);
	DDX_Control(pDX, IDC_STRU08, m_ctrlStru08);
	DDX_Control(pDX, IDC_STRU09, m_ctrlStru09);
	DDX_Control(pDX, IDC_STRU10, m_ctrlStru10);
	DDX_Control(pDX, IDC_STRU11, m_ctrlStru11);
	DDX_Control(pDX, IDC_STRU12, m_ctrlStru12);
	DDX_Control(pDX, IDC_STRU13, m_ctrlStru13);
	DDX_Control(pDX, IDC_STRU14, m_ctrlStru14);
	DDX_Control(pDX, IDC_STRU15, m_ctrlStru15);
	DDX_Control(pDX, IDC_STRU16, m_ctrlStru16);
	DDX_Control(pDX, IDC_STRU17, m_ctrlStru17);
	DDX_Control(pDX, IDC_STRU18, m_ctrlStru18);
	DDX_Control(pDX, IDC_STRU19, m_ctrlStru19);
	DDX_Control(pDX, IDC_STRU20, m_ctrlStru20);
	DDX_Control(pDX, IDC_STRU21, m_ctrlStru21);
	DDX_Control(pDX, IDC_STRU22, m_ctrlStru22);
	DDX_Control(pDX, IDC_STRU23, m_ctrlStru23);
	DDX_Control(pDX, IDC_STRU24, m_ctrlStru24);
	DDX_Control(pDX, IDC_EDIT_STRU_YC_SPIN, m_ctrlStruYCSpin);
	DDX_Control(pDX, IDC_EDIT_STRU_XC_SPIN, m_ctrlStruXCSpin);
	DDX_Control(pDX, IDC_EDIT_STRU_W_SPIN, m_ctrlStruWSpin);
	DDX_Control(pDX, IDC_EDIT_STRU_H_SPIN, m_ctrlStruHSpin);
	DDX_Text(pDX, IDC_EDIT_STRU_H, m_nStruH);
	DDV_MinMaxInt(pDX, m_nStruH, 1, 5);
	DDX_Text(pDX, IDC_EDIT_STRU_W, m_nStruW);
	DDV_MinMaxInt(pDX, m_nStruW, 1, 5);
	DDX_Text(pDX, IDC_EDIT_STRU_XC, m_nStruXc);
	DDV_MinMaxInt(pDX, m_nStruXc, 0, 4);
	DDX_Text(pDX, IDC_EDIT_STRU_YC, m_nStruYc);
	DDV_MinMaxInt(pDX, m_nStruYc, 0, 4);
	DDX_Check(pDX, IDC_STRU00, m_bStru00);
	DDX_Check(pDX, IDC_STRU01, m_bStru01);
	DDX_Check(pDX, IDC_STRU02, m_bStru02);
	DDX_Check(pDX, IDC_STRU03, m_bStru03);
	DDX_Check(pDX, IDC_STRU04, m_bStru04);
	DDX_Check(pDX, IDC_STRU05, m_bStru05);
	DDX_Check(pDX, IDC_STRU06, m_bStru06);
	DDX_Check(pDX, IDC_STRU07, m_bStru07);
	DDX_Check(pDX, IDC_STRU08, m_bStru08);
	DDX_Check(pDX, IDC_STRU09, m_bStru09);
	DDX_Check(pDX, IDC_STRU10, m_bStru10);
	DDX_Check(pDX, IDC_STRU11, m_bStru11);
	DDX_Check(pDX, IDC_STRU12, m_bStru12);
	DDX_Check(pDX, IDC_STRU13, m_bStru13);
	DDX_Check(pDX, IDC_STRU14, m_bStru14);
	DDX_Check(pDX, IDC_STRU15, m_bStru15);
	DDX_Check(pDX, IDC_STRU16, m_bStru16);
	DDX_Check(pDX, IDC_STRU17, m_bStru17);
	DDX_Check(pDX, IDC_STRU18, m_bStru18);
	DDX_Check(pDX, IDC_STRU19, m_bStru19);
	DDX_Check(pDX, IDC_STRU20, m_bStru20);
	DDX_Check(pDX, IDC_STRU21, m_bStru21);
	DDX_Check(pDX, IDC_STRU22, m_bStru22);
	DDX_Check(pDX, IDC_STRU23, m_bStru23);
	DDX_Check(pDX, IDC_STRU24, m_bStru24);
	DDX_Radio(pDX, IDC_DIAMOND, m_nMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMorph, CDialog)
	//{{AFX_MSG_MAP(CDlgMorph)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_VERT, OnVert)
	ON_BN_CLICKED(IDC_HORI, OnHori)
	ON_BN_CLICKED(IDC_SQUARE, OnSquare)
	ON_BN_CLICKED(IDC_RECT, OnRect)
	ON_BN_CLICKED(IDC_DIAMOND, OnDiamond)
	ON_BN_CLICKED(IDC_CIRCLE, OnCircle)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_H, OnKillfocusEditStruH)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_W, OnKillfocusEditStruW)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_XC, OnKillfocusEditStruXc)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_YC, OnKillfocusEditStruYc)
	ON_EN_CHANGE(IDC_EDIT_STRU_XC, OnChangeEditStruXc)
	ON_EN_CHANGE(IDC_EDIT_STRU_YC, OnChangeEditStruYc)
	ON_EN_CHANGE(IDC_EDIT_STRU_H, OnChangeEditStruH)
	ON_EN_CHANGE(IDC_EDIT_STRU_W, OnChangeEditStruW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMorph message handlers
void CDlgMorph::UpdateEdit()
{
	BOOL	bEnable;
	
	//ѭ������
	int		i;
	int		j;
	
	//�ж��ǲ����Զ���ṹԪ��
	if (m_nMode == 6) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	//�����ı�����΢���ؼ���״̬
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_H)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_H_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_W)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_W_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_XC)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_XC_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_YC)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_YC_SPIN)->EnableWindow(bEnable);
	
	int nMid = m_nStruYc * 5 + m_nStruXc;

	//Ϊ��������ؼ�����ͼ��
	for(i = 0; i < 25; i++)
	{
		//IDC_STRU00��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
		//��ȡ��Ӧ����ؼ���ָ��
		CButtonST *pCheckBox = (CButtonST *) GetDlgItem(IDC_STRU00 + i);

		pCheckBox->SetFlat(TRUE, TRUE);

		if(i == nMid)
		{
			pCheckBox->SetIcon(IDI_MIDON, IDI_MIDOFF);
		}
		else
		{
			pCheckBox->SetIcon(IDI_LEDON, IDI_LEDOFF);
		}
	}

	//����ģ��Ԫ���ı���Enable״̬
	for (i = IDC_STRU00; i <= IDC_STRU24; i++)
	{
		//���ü���Ϊ������״̬
		(CButtonST *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	//��ʾӦ�ÿ��õĽṹԪ�ؼ���
	for (i = 0; i < m_nStruH; i++)
	{
		for (j = 0; j < m_nStruW; j++)
		{
			//���ü���Ϊ����״̬
			(CButtonST *) GetDlgItem(IDC_STRU00 + i * 5 + j)->ShowWindow(SW_SHOW);
		}
	}
	
	//���ز�����״̬�ļ���ǰm_nStruH�еĺ��У�
	for (i = 0; i < m_nStruH; i++)
	{
		for (j = m_nStruW; j < 5; j++)
		{
			//����Ϊ������
			(CButtonST *) GetDlgItem(IDC_STRU00 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}
	
	//����Ӧ�ò��ɼ���ģ��Ԫ���ı��򣨺��У�
	for (i = m_nStruH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			//����Ϊ������
			(CButtonST *) GetDlgItem(IDC_STRU00 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}

}

BOOL CDlgMorph::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//����΢���ؼ��ķ�Χ�ͻ��༭��

	m_ctrlStruWSpin.SetRange(1, 5);
	m_ctrlStruWSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_W));
	
	m_ctrlStruHSpin.SetRange(1, 5);
	m_ctrlStruHSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_H));
	
	m_ctrlStruXCSpin.SetRange(0, 4);
	m_ctrlStruXCSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_XC));

	m_ctrlStruYCSpin.SetRange(0, 4);
	m_ctrlStruYCSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_YC));
	

	int nMid = m_nStruYc * 5 + m_nStruXc;
	//Ϊ��������ؼ�����ͼ��
	for(int i = 0; i < 25; i++)
	{
		//IDC_STRU00��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
		//��ȡ��Ӧ����ؼ���ָ��
		CButtonST *pCheckBox = (CButtonST *) GetDlgItem(IDC_STRU00 + i);

		pCheckBox->SetFlat(TRUE, TRUE);

		if(i == nMid)
		{
			pCheckBox->SetIcon(IDI_MIDON, IDI_MIDOFF);
		}
		else
		{
			pCheckBox->SetIcon(IDI_LEDON, IDI_LEDOFF);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMorph::OnDiamond() 
{
	//3��3���νṹԪ��
	m_nMode = 0;
	m_nStruW  = 3;
	m_nStruH  = 3;
	m_nStruXc = 1;
	m_nStruYc = 1;
	
	m_bStru00 = FALSE;
	m_bStru01 = TRUE;
	m_bStru02 = FALSE;
	m_bStru03 = FALSE;
	m_bStru04 = FALSE;
	m_bStru05 = TRUE;
	m_bStru06 = TRUE;
	m_bStru07 = TRUE;
	m_bStru08 = FALSE;
	m_bStru09 = FALSE;
	m_bStru10 = FALSE;
	m_bStru11 = TRUE;
	m_bStru12 = FALSE;
	m_bStru13 = FALSE;
	m_bStru14 = FALSE;
	m_bStru15 = FALSE;
	m_bStru16 = FALSE;
	m_bStru17 = FALSE;
	m_bStru18 = FALSE;
	m_bStru19 = FALSE;
	m_bStru20 = FALSE;
	m_bStru21 = FALSE;
	m_bStru22 = FALSE;
	m_bStru23 = FALSE;
	m_bStru24 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMorph::OnSquare() 
{
	//3��3���νṹԪ��
	m_nMode = 1;
	m_nStruW  = 3;
	m_nStruH  = 3;
	m_nStruXc = 1;
	m_nStruYc = 1;
	
	m_bStru00 = TRUE;
	m_bStru01 = TRUE;
	m_bStru02 = TRUE;
	m_bStru03 = FALSE;
	m_bStru04 = FALSE;
	m_bStru05 = TRUE;
	m_bStru06 = TRUE;
	m_bStru07 = TRUE;
	m_bStru08 = FALSE;
	m_bStru09 = FALSE;
	m_bStru10 = TRUE;
	m_bStru11 = TRUE;
	m_bStru12 = TRUE;
	m_bStru13 = FALSE;
	m_bStru14 = FALSE;
	m_bStru15 = FALSE;
	m_bStru16 = FALSE;
	m_bStru17 = FALSE;
	m_bStru18 = FALSE;
	m_bStru19 = FALSE;
	m_bStru20 = FALSE;
	m_bStru21 = FALSE;
	m_bStru22 = FALSE;
	m_bStru23 = FALSE;
	m_bStru24 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMorph::OnCircle() 
{
	//5��5Բ�νṹԪ��
	m_nMode = 2;
	m_nStruW  = 5;
	m_nStruH  = 5;
	m_nStruXc = 2;
	m_nStruYc = 2;
	
	m_bStru00 = FALSE;
	m_bStru01 = TRUE;
	m_bStru02 = TRUE;
	m_bStru03 = TRUE;
	m_bStru04 = FALSE;
	m_bStru05 = TRUE;
	m_bStru06 = TRUE;
	m_bStru07 = TRUE;
	m_bStru08 = TRUE;
	m_bStru09 = TRUE;
	m_bStru10 = TRUE;
	m_bStru11 = TRUE;
	m_bStru12 = TRUE;
	m_bStru13 = TRUE;
	m_bStru14 = TRUE;
	m_bStru15 = TRUE;
	m_bStru16 = TRUE;
	m_bStru17 = TRUE;
	m_bStru18 = TRUE;
	m_bStru19 = TRUE;
	m_bStru20 = FALSE;
	m_bStru21 = TRUE;
	m_bStru22 = TRUE;
	m_bStru23 = TRUE;
	m_bStru24 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMorph::OnRect() 
{
	//5��3���νṹԪ��
	m_nMode = 3;
	m_nStruW  = 5;
	m_nStruH  = 3;
	m_nStruXc = 2;
	m_nStruYc = 1;
	
	m_bStru00 = TRUE;
	m_bStru01 = TRUE;
	m_bStru02 = TRUE;
	m_bStru03 = TRUE;
	m_bStru04 = TRUE;
	m_bStru05 = TRUE;
	m_bStru06 = TRUE;
	m_bStru07 = TRUE;
	m_bStru08 = TRUE;
	m_bStru09 = TRUE;
	m_bStru10 = TRUE;
	m_bStru11 = TRUE;
	m_bStru12 = TRUE;
	m_bStru13 = TRUE;
	m_bStru14 = TRUE;
	m_bStru15 = FALSE;
	m_bStru16 = FALSE;
	m_bStru17 = FALSE;
	m_bStru18 = FALSE;
	m_bStru19 = FALSE;
	m_bStru20 = FALSE;
	m_bStru21 = FALSE;
	m_bStru22 = FALSE;
	m_bStru23 = FALSE;
	m_bStru24 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMorph::OnHori() 
{
	//3��1ˮƽ�ṹԪ��
	m_nMode = 4;
	m_nStruW  = 3;
	m_nStruH  = 1;
	m_nStruXc = 1;
	m_nStruYc = 0;
	
	m_bStru00 = TRUE;
	m_bStru01 = TRUE;
	m_bStru02 = TRUE;
	m_bStru03 = FALSE;
	m_bStru04 = FALSE;
	m_bStru05 = FALSE;
	m_bStru06 = FALSE;
	m_bStru07 = FALSE;
	m_bStru08 = FALSE;
	m_bStru09 = FALSE;
	m_bStru10 = FALSE;
	m_bStru11 = FALSE;
	m_bStru12 = FALSE;
	m_bStru13 = FALSE;
	m_bStru14 = FALSE;
	m_bStru15 = FALSE;
	m_bStru16 = FALSE;
	m_bStru17 = FALSE;
	m_bStru18 = FALSE;
	m_bStru19 = FALSE;
	m_bStru20 = FALSE;
	m_bStru21 = FALSE;
	m_bStru22 = FALSE;
	m_bStru23 = FALSE;
	m_bStru24 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMorph::OnVert() 
{
	//1��3��ֱ�ṹԪ��
	m_nMode = 5;
	m_nStruW  = 1;
	m_nStruH  = 3;
	m_nStruXc = 0;
	m_nStruYc = 1;
	
	m_bStru00 = TRUE;
	m_bStru01 = FALSE;
	m_bStru02 = FALSE;
	m_bStru03 = FALSE;
	m_bStru04 = FALSE;
	m_bStru05 = TRUE;
	m_bStru06 = FALSE;
	m_bStru07 = FALSE;
	m_bStru08 = FALSE;
	m_bStru09 = FALSE;
	m_bStru10 = TRUE;
	m_bStru11 = FALSE;
	m_bStru12 = FALSE;
	m_bStru13 = FALSE;
	m_bStru14 = FALSE;
	m_bStru15 = FALSE;
	m_bStru16 = FALSE;
	m_bStru17 = FALSE;
	m_bStru18 = FALSE;
	m_bStru19 = FALSE;
	m_bStru20 = FALSE;
	m_bStru21 = FALSE;
	m_bStru22 = FALSE;
	m_bStru23 = FALSE;
	m_bStru24 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMorph::OnCustom() 
{
	// �Զ���ģ��
	m_nMode = 6;
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMorph::OnKillfocusEditStruH() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMorph::OnKillfocusEditStruW() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMorph::OnKillfocusEditStruXc() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMorph::OnKillfocusEditStruYc() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMorph::OnOK() 
{
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	
	// �ж������Ƿ���Ч
	if ((m_nStruW < 0) || (m_nStruXc > m_nStruW - 1) ||
		(m_nStruH < 0) || (m_nStruYc > m_nStruH - 1))
	{
		// ��ʾ�û��������ô���
		MessageBox("����Ԫ�ز������ô���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	int Mask[200];

	m_nMaskLen = 0;

	int i, j, k = 0;

	//���ݶԼ����ѡ��״̬���ṹԪ�ظ�ֵ����¼����Ӧ���X��Y���꣩
	for(i = 0; i < m_nStruH; i++)
	{
		for(j = 0; j < m_nStruW; j++)
		{
			//IDC_STRU00��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
			
			//��ȡ��Ӧ����ؼ���ָ��
			CButtonST *pCheckBox = (CButtonST *) GetDlgItem(IDC_STRU00 + i * 5 + j);
			//�����״̬
			if(pCheckBox->GetCheck() == 1)
			{
				//�ṹԪ�ؼ������Ե���Ϊ������λ��
				m_nMaskLen++;
				Mask[k] = j - m_nStruXc;
				k++;
				Mask[k] = m_nStruYc - i;
				k++;
			}
		}
	}

	//������¼�ṹԪ�ص�����
	m_npMask = new int [2 * m_nMaskLen];

	//��ͳ�Ƴ��ĽṹԪ�ؿ�����m_npMaskָ���������
	memcpy(m_npMask, Mask, 2 * m_nMaskLen * sizeof(int));

	CDialog::OnOK();
}

void CDlgMorph::OnChangeEditStruXc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
	
}

void CDlgMorph::OnChangeEditStruYc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
	
}

void CDlgMorph::OnChangeEditStruH() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMorph::OnChangeEditStruW() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

