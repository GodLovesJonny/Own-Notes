// DlgMorth.cpp : implementation file
//

#include "stdafx.h"
#include "Insects.h"
#include "DlgMorth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMorth dialog


CDlgMorth::CDlgMorth(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMorth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMorth)
	m_nMode = 0;
	m_nStructH = 1;
	m_nStructW = 3;
	m_nStruct_X = 1;
	m_nStruct_Y = 0;
	m_bStruct1 = TRUE;
	m_bStruct2 = TRUE;
	m_bStruct12 = FALSE;
	m_bStruct13 = FALSE;
	m_bStruct14 = FALSE;
	m_bStruct15 = FALSE;
	m_bStruct16 = FALSE;
	m_bStruct17 = FALSE;
	m_bStruct18 = FALSE;
	m_bStruct19 = FALSE;
	m_bStruct20 = FALSE;
	m_bStruct21 = FALSE;
	m_bStruct22 = FALSE;
	m_bStruct23 = FALSE;
	m_bStruct24 = FALSE;
	m_bStruct25 = FALSE;
	m_bStruct3 = TRUE;
	m_bStruct4 = FALSE;
	m_bStruct5 = FALSE;
	m_bStruct6 = FALSE;
	m_bStruct7 = FALSE;
	m_bStruct8 = FALSE;
	m_bStruct9 = FALSE;
	m_bStruct10 = FALSE;
	m_bStruct11 = FALSE;
	//}}AFX_DATA_INIT
	m_nMaskLen = 0;
}


void CDlgMorth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMorth)
	DDX_Control(pDX, IDC_CHECK9, m_ctrlStruct9);
	DDX_Control(pDX, IDC_CHECK8, m_ctrlStruct8);
	DDX_Control(pDX, IDC_CHECK7, m_ctrlStruct7);
	DDX_Control(pDX, IDC_CHECK6, m_ctrlStruct6);
	DDX_Control(pDX, IDC_CHECK5, m_ctrlStruct5);
	DDX_Control(pDX, IDC_CHECK4, m_ctrlStruct4);
	DDX_Control(pDX, IDC_CHECK3, m_ctrlStruct3);
	DDX_Control(pDX, IDC_CHECK25, m_ctrlStruct25);
	DDX_Control(pDX, IDC_CHECK24, m_ctrlStruct24);
	DDX_Control(pDX, IDC_CHECK23, m_ctrlStruct23);
	DDX_Control(pDX, IDC_CHECK22, m_ctrlStruct22);
	DDX_Control(pDX, IDC_CHECK21, m_ctrlStruct21);
	DDX_Control(pDX, IDC_CHECK20, m_ctrlStruct20);
	DDX_Control(pDX, IDC_CHECK2, m_ctrlStruct2);
	DDX_Control(pDX, IDC_CHECK19, m_ctrlStruct19);
	DDX_Control(pDX, IDC_CHECK18, m_ctrlStruct18);
	DDX_Control(pDX, IDC_CHECK17, m_ctrlStruct17);
	DDX_Control(pDX, IDC_CHECK16, m_ctrlStruct16);
	DDX_Control(pDX, IDC_CHECK15, m_ctrlStruct15);
	DDX_Control(pDX, IDC_CHECK14, m_ctrlStruct14);
	DDX_Control(pDX, IDC_CHECK13, m_ctrlStruct13);
	DDX_Control(pDX, IDC_CHECK12, m_ctrlStruct12);
	DDX_Control(pDX, IDC_CHECK11, m_ctrlStruct11);
	DDX_Control(pDX, IDC_CHECK10, m_ctrlStruct10);
	DDX_Control(pDX, IDC_CHECK1, m_ctrlStruct1);
	DDX_Control(pDX, IDC_Y_SPIN, m_ctrlStruct_Y);
	DDX_Control(pDX, IDC_X_SPIN, m_ctrlStruct_X);
	DDX_Control(pDX, IDC_WIDTH_SPIN, m_ctrlStructW);
	DDX_Control(pDX, IDC_HEIGHT_SPIN, m_ctrlStructH);
	DDX_Radio(pDX, IDC_HORIZON, m_nMode);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nStructH);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nStructW);
	DDX_Text(pDX, IDC_EDIT_X, m_nStruct_X);
	DDX_Text(pDX, IDC_EDIT_Y, m_nStruct_Y);
	DDX_Check(pDX, IDC_CHECK1, m_bStruct1);
	DDX_Check(pDX, IDC_CHECK2, m_bStruct2);
	DDX_Check(pDX, IDC_CHECK12, m_bStruct12);
	DDX_Check(pDX, IDC_CHECK13, m_bStruct13);
	DDX_Check(pDX, IDC_CHECK14, m_bStruct14);
	DDX_Check(pDX, IDC_CHECK15, m_bStruct15);
	DDX_Check(pDX, IDC_CHECK16, m_bStruct16);
	DDX_Check(pDX, IDC_CHECK17, m_bStruct17);
	DDX_Check(pDX, IDC_CHECK18, m_bStruct18);
	DDX_Check(pDX, IDC_CHECK19, m_bStruct19);
	DDX_Check(pDX, IDC_CHECK20, m_bStruct20);
	DDX_Check(pDX, IDC_CHECK21, m_bStruct21);
	DDX_Check(pDX, IDC_CHECK22, m_bStruct22);
	DDX_Check(pDX, IDC_CHECK23, m_bStruct23);
	DDX_Check(pDX, IDC_CHECK24, m_bStruct24);
	DDX_Check(pDX, IDC_CHECK25, m_bStruct25);
	DDX_Check(pDX, IDC_CHECK3, m_bStruct3);
	DDX_Check(pDX, IDC_CHECK4, m_bStruct4);
	DDX_Check(pDX, IDC_CHECK5, m_bStruct5);
	DDX_Check(pDX, IDC_CHECK6, m_bStruct6);
	DDX_Check(pDX, IDC_CHECK7, m_bStruct7);
	DDX_Check(pDX, IDC_CHECK8, m_bStruct8);
	DDX_Check(pDX, IDC_CHECK9, m_bStruct9);
	DDX_Check(pDX, IDC_CHECK10, m_bStruct10);
	DDX_Check(pDX, IDC_CHECK11, m_bStruct11);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMorth, CDialog)
	//{{AFX_MSG_MAP(CDlgMorth)
	ON_BN_CLICKED(IDC_DIAMOND, OnDiamond)
	ON_BN_CLICKED(IDC_HORIZON, OnHorizon)
	ON_BN_CLICKED(IDC_VERTICAL, OnVertical)
	ON_BN_CLICKED(IDC_CIRCLE, OnCircle)
	ON_BN_CLICKED(IDC_DEFINE_SELF, OnDefineSelf)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeEditWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_X, OnKillfocusEditX)
	ON_EN_KILLFOCUS(IDC_EDIT_Y, OnKillfocusEditY)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, OnKillfocusEditHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, OnKillfocusEditWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMorth message handlers

void CDlgMorth::OnDiamond() 
{
    //3��3���νṹԪ��
	m_nMode = 2;
	m_nStructW  = 3;
	m_nStructH  = 3;
	m_nStruct_X = 1;
	m_nStruct_Y = 1;
	
	m_bStruct1 = FALSE;
	m_bStruct2 = TRUE;
	m_bStruct3 = FALSE;
	m_bStruct4 = FALSE;
	m_bStruct5 = FALSE;
	m_bStruct6 = TRUE;
	m_bStruct7 = TRUE;
	m_bStruct8 = TRUE;
	m_bStruct9 = FALSE;
	m_bStruct10 = FALSE;
	m_bStruct11 = FALSE;
	m_bStruct12 = TRUE;
	m_bStruct13 = FALSE;
	m_bStruct14 = FALSE;
	m_bStruct15 = FALSE;
	m_bStruct16 = FALSE;
	m_bStruct17 = FALSE;
	m_bStruct18 = FALSE;
	m_bStruct19 = FALSE;
	m_bStruct20 = FALSE;
	m_bStruct21 = FALSE;
	m_bStruct22 = FALSE;
	m_bStruct23 = FALSE;
	m_bStruct24 = FALSE;
	m_bStruct25 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);	
}

void CDlgMorth::OnHorizon() 
{
	//3��1ˮƽ�ṹԪ��
	m_nMode = 0;
	m_nStructW  = 3;
	m_nStructH  = 1;
	m_nStruct_X = 1;
	m_nStruct_Y = 0;
	
	m_bStruct1 = TRUE;
	m_bStruct2 = TRUE;
	m_bStruct3 = TRUE;
	m_bStruct4 = FALSE;
	m_bStruct5 = FALSE;
	m_bStruct6 = FALSE;
	m_bStruct7 = FALSE;
	m_bStruct8 = FALSE;
	m_bStruct9 = FALSE;
	m_bStruct10 = FALSE;
	m_bStruct11 = FALSE;
	m_bStruct12 = FALSE;
	m_bStruct13 = FALSE;
	m_bStruct14 = FALSE;
	m_bStruct15 = FALSE;
	m_bStruct16 = FALSE;
	m_bStruct17 = FALSE;
	m_bStruct18 = FALSE;
	m_bStruct19 = FALSE;
	m_bStruct20 = FALSE;
	m_bStruct21 = FALSE;
	m_bStruct22 = FALSE;
	m_bStruct23 = FALSE;
	m_bStruct24 = FALSE;
	m_bStruct25 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);	
}

void CDlgMorth::OnVertical() 
{
	//1*3��ֱ�ṹԪ��
	m_nMode = 1;
	m_nStructW  = 1;
	m_nStructH  = 3;
	m_nStruct_X = 0;
	m_nStruct_Y = 1;
	
	m_bStruct1 = TRUE;
	m_bStruct2 = FALSE;
	m_bStruct3 = FALSE;
	m_bStruct4 = FALSE;
	m_bStruct5 = FALSE;
	m_bStruct6 = TRUE;
	m_bStruct7 = FALSE;
	m_bStruct8 = FALSE;
	m_bStruct9 = FALSE;
	m_bStruct10 = FALSE;
	m_bStruct11 = TRUE;
	m_bStruct12 = FALSE;
	m_bStruct13 = FALSE;
	m_bStruct14 = FALSE;
	m_bStruct15 = FALSE;
	m_bStruct16 = FALSE;
	m_bStruct17 = FALSE;
	m_bStruct18 = FALSE;
	m_bStruct19 = FALSE;
	m_bStruct20 = FALSE;
	m_bStruct21 = FALSE;
	m_bStruct22 = FALSE;
	m_bStruct23 = FALSE;
	m_bStruct24 = FALSE;
	m_bStruct25 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);	
}

void CDlgMorth::OnCircle() 
{
	//5��5Բ�νṹԪ��
	m_nMode = 3;
	m_nStructW  = 5;
	m_nStructH  = 5;
	m_nStruct_X = 2;
	m_nStruct_Y = 2;
	
	m_bStruct1 = FALSE;
	m_bStruct2 = TRUE;
	m_bStruct3 = TRUE;
	m_bStruct4 = TRUE;
	m_bStruct5 = FALSE;
	m_bStruct6 = TRUE;
	m_bStruct7 = TRUE;
	m_bStruct8 = TRUE;
	m_bStruct9 = TRUE;
	m_bStruct10 = TRUE;
	m_bStruct11 = TRUE;
	m_bStruct12 = TRUE;
	m_bStruct13 = TRUE;
	m_bStruct14 = TRUE;
	m_bStruct15 = TRUE;
	m_bStruct16 = TRUE;
	m_bStruct17 = TRUE;
	m_bStruct18 = TRUE;
	m_bStruct19 = TRUE;
	m_bStruct20 = TRUE;
	m_bStruct21 = FALSE;
	m_bStruct22 = TRUE;
	m_bStruct23 = TRUE;
	m_bStruct24 = TRUE;
	m_bStruct25 = FALSE;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);	
}

void CDlgMorth::UpdateEdit()
{
	BOOL	bEnable;
	
	//ѭ������
	int		i;
	int		j;
	
	//�ж��ǲ����Զ���ṹԪ��
	if (m_nMode == 4) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	//�����ı�����΢���ؼ���״̬
	(CEdit *) GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_HEIGHT_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_WIDTH_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_X)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_X_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_Y)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_Y_SPIN)->EnableWindow(bEnable);

	//Ϊ��������ؼ�����ͼ��
	for(i = 0; i < 25; i++)
	{
		//IDC_STRU00��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
		//��ȡ��Ӧ����ؼ���ָ��
		CButtonST *pCheckBox = (CButtonST *) GetDlgItem(IDC_CHECK1 + i);
		pCheckBox->SetFlat(TRUE, TRUE);
	}

	//����ģ��Ԫ���ı���Enable״̬
	for (i = IDC_CHECK1; i <= IDC_CHECK25; i++)
	{
		//���ü���Ϊ������״̬
		(CButtonST *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	//��ʾӦ�ÿ��õĽṹԪ�ؼ���
	for (i = 0; i < m_nStructH; i++)
	{
		for (j = 0; j < m_nStructW; j++)
		{
			//���ü���Ϊ����״̬
			(CButtonST *) GetDlgItem(IDC_CHECK1 + i * 5 + j)->ShowWindow(SW_SHOW);
		}
	}
	
	//���ز�����״̬�ļ���ǰm_nStruH�еĺ��У�
	for (i = 0; i < m_nStructH; i++)
	{
		for (j = m_nStructW; j < 5; j++)
		{
			//����Ϊ������
			(CButtonST *) GetDlgItem(IDC_CHECK1 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}
	
	//����Ӧ�ò��ɼ���ģ��Ԫ���ı��򣨺��У�
	for (i = m_nStructH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			//����Ϊ������
			(CButtonST *) GetDlgItem(IDC_CHECK1 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}
}

void CDlgMorth::OnDefineSelf() 
{
	// �Զ���ģ��
	m_nMode = 4;
	
	// �����ı���״̬
	UpdateEdit();	
}

BOOL CDlgMorth::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	//����΢���ؼ��ķ�Χ�ͻ��༭��

	m_ctrlStructW.SetRange(1, 5);
	m_ctrlStructW.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_WIDTH));
	
	m_ctrlStructH.SetRange(1, 5);
	m_ctrlStructH.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_HEIGHT));
	
	m_ctrlStruct_X.SetRange(0, 4);
	m_ctrlStruct_X.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_X));

	m_ctrlStruct_Y.SetRange(0, 4);
	m_ctrlStruct_Y.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_Y));
	

	int nMid = m_nStruct_Y * 5 + m_nStruct_X;
	//Ϊ��������ؼ�����ͼ��
	for(int i = 0; i < 25; i++)
	{
		//IDC_STRU00��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
		//��ȡ��Ӧ����ؼ���ָ��
		CButtonST *pCheckBox = (CButtonST *) GetDlgItem(IDC_CHECK1 + i);

		pCheckBox->SetFlat(TRUE, TRUE);

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMorth::OnChangeEditX() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();

	
}

void CDlgMorth::OnChangeEditY() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CDlgMorth::OnChangeEditHeight() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CDlgMorth::OnChangeEditWidth() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CDlgMorth::OnKillfocusEditX() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();	
}

void CDlgMorth::OnKillfocusEditY() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();	
}

void CDlgMorth::OnKillfocusEditHeight() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();	
}

void CDlgMorth::OnKillfocusEditWidth() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();	
}

void CDlgMorth::OnOK() 
{
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	
	// �ж������Ƿ���Ч
	if ((m_nStructW < 0) || (m_nStruct_X > m_nStructW - 1) ||
		(m_nStructH < 0) || (m_nStruct_Y > m_nStructH - 1))
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
	for(i = 0; i < m_nStructH; i++)
	{
		for(j = 0; j < m_nStructW; j++)
		{
			
			//��ȡ��Ӧ����ؼ���ָ��
			CButtonST *pCheckBox = (CButtonST *) GetDlgItem(IDC_CHECK1 + i * 5 + j);
			//�����״̬
			if(pCheckBox->GetCheck() == 1)
			{
				//�ṹԪ�ؼ������Ե���Ϊ������λ��
				m_nMaskLen++;
				Mask[k] = j - m_nStruct_X;
				k++;
				Mask[k] = m_nStruct_Y - i;
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

void CDlgMorth::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
