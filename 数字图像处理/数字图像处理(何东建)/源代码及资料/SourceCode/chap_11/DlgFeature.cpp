// DlgFeature.cpp : implementation file

#include "stdafx.h"
#include "dip.h"
#include "DlgFeature.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipDoc.h"
#include "DibObject.h"
#include "ImgAnalyse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFeature dialog


CDlgFeature::CDlgFeature(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFeature::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFeature)
	m_fXScale = 1.0f;
	m_fXYScale = 1.414f;
	m_fYScale = 1.0f;
	m_fPermeter = 0.0f;
	m_fCompact = 0.0f;
	m_fArea = 0.0f;
	m_IsUseCurValue = TRUE;
	m_SelOpt = -1;
	m_strCxAndCy = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFeature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFeature)
	DDX_Text(pDX, IDC_EDIT_XSCALE, m_fXScale);
	DDX_Text(pDX, IDC_EDIT_XYSCALE, m_fXYScale);
	DDX_Text(pDX, IDC_EDIT_YSCALE, m_fYScale);
	DDX_Text(pDX, IDC_EDIT_PERIMETER, m_fPermeter);
	DDX_Text(pDX, IDC_EDIT_COMPACT, m_fCompact);
	DDX_Text(pDX, IDC_EDIT_AREA, m_fArea);
	DDX_Check(pDX, IDC_CHECK_RETAIN, m_IsUseCurValue);
	DDX_Radio(pDX, IDC_RADIO_RECALIB, m_SelOpt);
	DDX_Text(pDX, IDC_EDIT_CENTROID, m_strCxAndCy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgFeature, CDialog)
	//{{AFX_MSG_MAP(CDlgFeature)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_CHECK_RETAIN, OnCheckRetain)
	ON_BN_CLICKED(IDC_RADIO_RECALIB, OnRadioRecalib)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFeature message handlers

BOOL CDlgFeature::OnInitDialog() 
{
	CDialog::OnInitDialog();


	CalPara(&m_fXScale,&m_fYScale,&m_fXYScale);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFeature::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//��ȡ�Ի��򴰿ڴ�С,�����������������Ļ�ͻ����ġ�
	RECT rect1;
	GetWindowRect(&rect1);

	//��ȡGroup���ָ��
	CWnd *pWnd;
	pWnd=GetDlgItem(IDC_STATIC_FOURIER);

	//��û���ֱ��ͼ����Ͽ�ĳߴ磬��������Ҳ���������Ļ�ͻ�����
	RECT rect2;
	pWnd->GetWindowRect(&rect2);

	//���廭ˢ�ͻ���
	COLORREF color=GetSysColor(COLOR_BTNFACE);
	CBrush brush(color);
	CPen pen( PS_SOLID, 1, RGB(0, 0, 0));
	CBrush *pOldBrush;
	CPen *pOldPen;

	//����ѡ���豸����
	pOldBrush = (CBrush *) dc.SelectObject( &brush );
	pOldPen = (CPen *) dc.SelectObject( &pen );

	//����������ߵ�������꣬��Ϊ�ڶԻ����ϻ�ͼʱ����������ԶԻ����
	int x1 = rect2.left-rect1.left;
	int y1 = rect2.top-rect1.top-5;
	int x2 = rect2.right - rect2.left;
	int y2 = rect2.bottom - rect1.top-50;

	//��������ϵ
	dc.MoveTo(x1,y2);
	dc.LineTo(x1,y1);
	dc.MoveTo(x1,y2);
	dc.LineTo(x2,y2);
	dc.MoveTo(x1+32,y2);
	dc.LineTo(x1+32,y2+3);
	dc.MoveTo(x1+64,y2);
	dc.LineTo(x1+64,y2+3);
	dc.MoveTo(x1+96,y2);
	dc.LineTo(x1+96,y2+3);
	dc.MoveTo(x1+128,y2);
	dc.LineTo(x1+128,y2+3);
	dc.MoveTo(x1+160,y2);
	dc.LineTo(x1+160,y2+3);
	dc.MoveTo(x1+192,y2);
	dc.LineTo(x1+192,y2+3);
	dc.MoveTo(x1+224,y2);
	dc.LineTo(x1+224,y2+3);
	dc.MoveTo(x1+256,y2);
	dc.LineTo(x1+256,y2+3);
	//�����ı����ʱ�ı���ɫ
	dc.SetBkColor( color );
	dc.TextOut(x1-2,y1-14,"��S(��)��");
	dc.TextOut(x2-5,y2+3,"N");
	dc.TextOut(x1,y2+3,"0");
	dc.TextOut(x1+28,y2+3,"32");
	dc.TextOut(x1+60,y2+3,"64");
	dc.TextOut(x1+92,y2+3,"96");
	dc.TextOut(x1+120,y2+3,"128");
	dc.TextOut(x1+152,y2+3,"160");
	dc.TextOut(x1+184,y2+3,"192");
	dc.TextOut(x1+216,y2+3,"224");
	dc.TextOut(x1+248,y2+3,"256");

	GetFourier();

	//������Ƶ�������
	int nMaxNumP = 400;
	if(m_iNumPoints <= nMaxNumP) nMaxNumP = m_iNumPoints;

	//���Ƹ���ҶƵ�׷���
	for(int i=0; i<nMaxNumP; i++ )
	{
		dc.MoveTo(x1+i+1, y2);
		
		//��ֹ����ʱ�����߿�
		if(m_Fourier[i] <= y2-y1-20)
			dc.LineTo( x1+i+1,y2-(int)(m_Fourier[i]));
		else
			dc.LineTo( x1+i+1,y1+10);
	}

	//��ԭ��ˢ�ͻ���ѡ���豸����
	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );

	//Do not call CDialog::OnPaint() for painting messages
}

//���������
void CDlgFeature::CalPara(float *X_Scale, float *YScale, float *XY_Scale)
{
	//�������ݵ��ؼ���Ա����
	UpdateData();

	CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame=(CChildFrame *)pMainFrame->MDIGetActive();
	CDipDoc *pDoc = (CDipDoc *)pChildFrame->GetActiveDocument();
	CImgAnalyse ImgAnalyse(pDoc->m_pDibObject);
	//ImgAnalyse.Calibrate(X_Scale,YScale,XY_Scale,26.0f);
	//����������ܳ�
	if(!ImgAnalyse.PeriByChainCode(*X_Scale,*YScale,*XY_Scale,&m_fPermeter))
		return;
	//�ɱ߽�����������
	if(!ImgAnalyse.AreaByGreen(*X_Scale,*YScale,&m_fArea))
		return;
	
	//���������������õ�����
	POINT m_CxCy = ImgAnalyse.GetCentroid();
	m_strCxAndCy.Format("%d , %d",m_CxCy.x,m_CxCy.y);
	
	//�������ܶ�,�Ѿ��淶��
	m_fCompact = m_fPermeter*m_fPermeter/(4*3.1415926f*m_fArea);

	//��ʾ�����д洢������
	UpdateData(false);
}

void CDlgFeature::OnCheckRetain() 
{
	if(m_IsUseCurValue == TRUE)
	{
		//ʹ��ѡ�����
		GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow();
		GetDlgItem(IDC_RADIO_RECALIB)->EnableWindow();
		m_IsUseCurValue = FALSE;
		
		//��ȡ����ѡ��ѡ��ʱ�������ǰ��ѡ���Զ�����ʹ�ı������	
		if(m_SelOpt == 1)
		{
			GetDlgItem(IDC_EDIT_XSCALE)->EnableWindow();
			GetDlgItem(IDC_EDIT_YSCALE)->EnableWindow();
			GetDlgItem(IDC_EDIT_XYSCALE)->EnableWindow();
		}
	}
	else
	{
		//ʹ��ѡ�򲻿���
		GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RECALIB)->EnableWindow(FALSE);
		m_IsUseCurValue = TRUE;
		
		//���¼��㲢��ʾ����
		CalPara(&m_fXScale,&m_fYScale,&m_fXYScale);
	
		//ʹ�ı������,��ʱ���Զ���궨ϵ��	
		GetDlgItem(IDC_EDIT_XSCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_YSCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_XYSCALE)->EnableWindow(FALSE);
	}
}
//�Զ���궨ϵ��
void CDlgFeature::OnRadioCustom() 
{
	//ʹ�ı������	
	GetDlgItem(IDC_EDIT_XSCALE)->EnableWindow();
	GetDlgItem(IDC_EDIT_YSCALE)->EnableWindow();
	GetDlgItem(IDC_EDIT_XYSCALE)->EnableWindow();
}

BOOL CDlgFeature::GetFourier()
{
	CImgAnalyse ImgAnalyse;
	//��ȡ����Ҷ����������ָ��
	//m_iNumPointsΪ�߽������������Ҷ�任��ϵ������
	m_Fourier = ImgAnalyse.FourierFactor(&m_iNumPoints);
	if(m_Fourier == NULL) return false;
	else
		return true;
}

void CDlgFeature::OnRadioRecalib() 
{
	char szBdFilter[] = "λͼ�ļ�(*.BMP)|*.BMP||";

	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szBdFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		CString PathName = FileDlg.GetPathName();
		PathName.MakeUpper();
		AfxGetApp()->OpenDocumentFile( PathName );

		CDibObject pDibObject(PathName);

		CImgAnalyse ImgAnalyse(&pDibObject);
		ImgAnalyse.Calibrate(&m_fXScale,&m_fYScale,&m_fXYScale,26.0f);
		UpdateData(false);
	}	
}
