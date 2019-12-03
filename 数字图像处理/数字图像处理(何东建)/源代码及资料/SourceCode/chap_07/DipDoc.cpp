// DipDoc.cpp : implementation of the CDipDoc class
//

#include "stdafx.h"
#include "Dip.h"

#include "DipDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipView.h"

//ͼ����ͷ�ļ�
#include "PointPro.h"
#include "AreaPro.h"
#include "GeoTrans.h"
#include "FreqPro.h"

//�Ի���ͷ�ļ�
#include "DlgLinerPara.h"
#include "DlgPointStre.h"
#include "DlgThreshold.h"
#include "DlgSmooth.h"
#include "EdgeDetDlg.h"
#include "LapDlg.h"
#include "DlgColor.h"
#include "DlgMedianFilter.h"
#include "DlgSeedFill.h"
#include "DlgProject.h"
#include "DlgGeoTran.h"
#include "DlgGeoZoom.h"
#include "DlgGeoRota.h"
#include "DlgFouriorFilte.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char szFilter[];

/////////////////////////////////////////////////////////////////////////////
// CDipDoc

IMPLEMENT_DYNCREATE(CDipDoc, CDocument)

BEGIN_MESSAGE_MAP(CDipDoc, CDocument)
	//{{AFX_MSG_MAP(CDipDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_POINT_LINER, OnPointLiner)
	ON_COMMAND(ID_POINT_STRE, OnPointStre)
	ON_COMMAND(ID_POINT_CONTAUTO, OnPointContauto)
	ON_COMMAND(ID_POINT_THRE, OnPointThre)
	ON_COMMAND(ID_ENHA_SMOOTH, OnEnhaSmooth)
	ON_COMMAND(ID_POINT_PSEUDOCOLOR, OnPointPseudocolor)
	ON_COMMAND(ID_ENHA_GRADSHARP, OnEnhaGradsharp)
	ON_COMMAND(ID_ENHA_TEMPSHARP, OnEnhaTempsharp)
	ON_COMMAND(ID_ENHA_LAPSHARP, OnEnhaLapsharp)
	ON_COMMAND(ID_POINT_INVERT, OnPointInvert)
	ON_COMMAND(ID_CHANNEL_SPLIT, OnChannelSplit)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_SPLIT, OnUpdateChannelSplit)
	ON_COMMAND(ID_CHANNEL_BLUE, OnChannelBlue)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_BLUE, OnUpdateChannelBlue)
	ON_COMMAND(ID_CHANNEL_GREEN, OnChannelGreen)
	ON_COMMAND(ID_CHANNEL_MIXED, OnChannelMixed)
	ON_COMMAND(ID_CHANNEL_RED, OnChannelRed)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_GREEN, OnUpdateChannelGreen)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_MIXED, OnUpdateChannelMixed)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_RED, OnUpdateChannelRed)
	ON_UPDATE_COMMAND_UI(ID_POINT_THRE, OnUpdatePointThre)
	ON_UPDATE_COMMAND_UI(ID_POINT_PSEUDOCOLOR, OnUpdatePointPseudocolor)
	ON_COMMAND(ID_ENHA_MidianF, OnEnhaMidianF)
	ON_COMMAND(ID_DETECT_PROJECT, OnDetectProject)
	ON_UPDATE_COMMAND_UI(ID_DETECT_PROJECT, OnUpdateDetectProject)
	ON_COMMAND(ID_EDGE_FILL, OnEdgeFill)
	ON_UPDATE_COMMAND_UI(ID_EDGE_FILL, OnUpdateEdgeFill)
	ON_COMMAND(ID_DETECT_TEMPLATE, OnDetectTemplate)
	ON_UPDATE_COMMAND_UI(ID_DETECT_TEMPLATE, OnUpdateDetectTemplate)
	ON_COMMAND(ID_EDGE_TRACE, OnEdgeTrace)
	ON_UPDATE_COMMAND_UI(ID_EDGE_TRACE, OnUpdateEdgeTrace)
	ON_COMMAND(ID_EDGE_CONTOUR, OnEdgeContour)
	ON_UPDATE_COMMAND_UI(ID_EDGE_CONTOUR, OnUpdateEdgeContour)
	ON_COMMAND(ID_DETECT_MINUS, OnDetectMinus)
	ON_UPDATE_COMMAND_UI(ID_DETECT_MINUS, OnUpdateDetectMinus)
	ON_COMMAND(ID_GEOM_MIRH, OnGeomMirh)
	ON_COMMAND(ID_GEOM_MIRV, OnGeomMirv)
	ON_COMMAND(ID_GEOM_ROTA, OnGeomRota)
	ON_COMMAND(ID_GEOM_TRAN, OnGeomTran)
	ON_COMMAND(ID_GEOM_TRPO, OnGeomTrpo)
	ON_COMMAND(ID_GEOM_ZOOM, OnGeomZoom)
	ON_UPDATE_COMMAND_UI(ID_GEOM_MIRH, OnUpdateGeomMirh)
	ON_UPDATE_COMMAND_UI(ID_GEOM_MIRV, OnUpdateGeomMirv)
	ON_UPDATE_COMMAND_UI(ID_GEOM_ROTA, OnUpdateGeomRota)
	ON_UPDATE_COMMAND_UI(ID_GEOM_TRAN, OnUpdateGeomTran)
	ON_UPDATE_COMMAND_UI(ID_GEOM_TRPO, OnUpdateGeomTrpo)
	ON_UPDATE_COMMAND_UI(ID_GEOM_ZOOM, OnUpdateGeomZoom)
	ON_COMMAND(ID_FREQ_DCT, OnFreqDct)
	ON_COMMAND(ID_FREQ_FFT, OnFreqFft)
	ON_COMMAND(ID_FREQ_FFT_FILTE, OnFreqFftFilte)
	ON_COMMAND(ID_FREQ_NFFT, OnFreqNfft)
	ON_COMMAND(ID_FREQ_WALH, OnFreqWalh)
	ON_UPDATE_COMMAND_UI(ID_FREQ_DCT, OnUpdateFreqDct)
	ON_UPDATE_COMMAND_UI(ID_FREQ_FFT_FILTE, OnUpdateFreqFftFilte)
	ON_UPDATE_COMMAND_UI(ID_FREQ_NFFT, OnUpdateFreqNfft)
	ON_UPDATE_COMMAND_UI(ID_FREQ_WALH, OnUpdateFreqWalh)
	ON_UPDATE_COMMAND_UI(ID_FREQ_FFT, OnUpdateFreqFft)
	ON_COMMAND(ID_FREQ_NDCT, OnFreqNdct)
	ON_UPDATE_COMMAND_UI(ID_FREQ_NDCT, OnUpdateFreqNdct)
	ON_COMMAND(ID_FREQ_NWALH, OnFreqNwalh)
	ON_UPDATE_COMMAND_UI(ID_FREQ_NWALH, OnUpdateFreqNwalh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipDoc construction/destruction

CDipDoc::CDipDoc()
{
	// TODO: add one-time construction code here
	m_pDibObject = NULL;
	m_bImageLoaded = FALSE;
	m_nColorIndex = 0;
	m_lHeight = 0;
	m_lWidth = 0;

	m_bFft = FALSE;
	m_bDct = FALSE;
	m_bWht = FALSE;

	//��ʼ��Matrix<LIB>���
	initM(MATCOM_VERSION);
}

CDipDoc::~CDipDoc()
{
	if(m_pDibObject != NULL)
	{
		delete m_pDibObject;
		m_pDibObject = NULL;
	}

	//����Matrix<LIB>������
	exitM();	

}

BOOL CDipDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDipDoc serialization

void CDipDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDipDoc diagnostics

#ifdef _DEBUG
void CDipDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDipDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDipDoc commands

void CDipDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

void CDipDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		AfxGetApp()->OpenDocumentFile( strPathName );
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;
		if( !LoadImageToDocument() )
		{
			AfxMessageBox("�޷�����ͼ���ļ���");
			return;
		}
	}
}

BOOL CDipDoc::LoadImageToDocument()
{
	CString strPathName = GetPathName();
	
	//���õȴ����
	BeginWaitCursor();
	m_pDibObject = new CDibObject( strPathName.GetBuffer(3) );
	//ȡ���ȴ����
	EndWaitCursor();

	//����ͼ���ļ�ʧ��
	if( m_pDibObject == NULL )
	{
		AfxMessageBox("�޷�����ͼ�������");
		//����FALSE
		return(FALSE);
	}

	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	//��ȡ�ӿ�ܴ���ָ��
	CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
	pChild->m_nWidth = m_pDibObject->GetWidth(); 
	pChild->m_nHeight = m_pDibObject->GetHeight();
	pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
		pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );
	

	//����ͼ���ļ��ɹ���������Ӧ����
	m_bImageLoaded = TRUE;

	//����TRUE
	return(TRUE);
}


void CDipDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;
	
	CFileDialog DialogSaveAs( FALSE, NULL, m_pDibObject->GetImageName(),
							OFN_HIDEREADONLY, szFilter );

	DialogSaveAs.m_ofn.nFilterIndex = (DWORD) nIndex;
	
	if( DialogSaveAs.DoModal() == IDOK )
	{
		CMainFrame *pMainFrame = ( CMainFrame * )AfxGetMainWnd();
		CChildFrame *pChildFrame = ( CChildFrame * )pMainFrame->MDIGetActive();
		CDipView *pDipView = ( CDipView * )pChildFrame->GetActiveView();
		
		nIndex = (int) DialogSaveAs.m_ofn.nFilterIndex;
		if( nIndex == 5 )
		{
			if( m_pDibObject->GetNumBits() != 24 )
			{
				AfxMessageBox("������24λ���ɫͼ����ܴ�ΪJPEG��ʽ��");
				return;
			}
		}

		if( m_pDibObject != NULL )
		{
			CString strPathName = DialogSaveAs.GetPathName();
			int nFindIndex = strPathName.Find(".");
			if( nFindIndex != -1)
				strPathName = strPathName.Left( nFindIndex );
			strPathName += CDibObject::szExtensions[ nIndex - 1 ];
			//m_pDibObject->ProcessImageHeader();
			//m_pDibObject->ProcessPalette();
			m_pDibObject->Save( strPathName );

			CString strFileName = DialogSaveAs.GetFileName();
			nFindIndex = strFileName.Find(".");
			if ( nFindIndex != -1 )
				strFileName = strFileName.Left( nFindIndex );
			strFileName += CDibObject::szExtensions[ nIndex - 1 ];
			pChildFrame->SetWindowText( strFileName );

			SetPathName( strPathName );
			if( nIndex == 5 )
			{
				m_pDibObject->Load( strPathName );
				pDipView->InvalidateRect( NULL, FALSE );
				pDipView->UpdateWindow();
			}
		}
	}
}

void CDipDoc::OnPointEqua() 
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ����LinerTrans()�����������Ա任
	PointOperation.InteEqualize();

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnPointLiner() 
{
	// �����Ի���
	CDlgLinerPara dlgPara;
	
	// ���Ա任��б��
	float fa;	
	// ���Ա任�Ľؾ�
	float fb;

	// ��ʼ������ֵ
	dlgPara.m_fA = 1.0;
	dlgPara.m_fB = 0.0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	fa = dlgPara.m_fA;
	fb = dlgPara.m_fB;

	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	PointOperation.LinerTran(fa, fb);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CDipDoc::OnPointStre() 
{
	// �����Ի���
	CDlgPointStre dlgPara;
	
	// ��1����
	BYTE	bX1;
	BYTE	bY1;
	
	// ��2����
	BYTE	bX2;
	BYTE	bY2;

	// ��ʼ������ֵ
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;
	
	// ��ʾ�Ի�����ʾ�û��趨����λ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	PointOperation.GrayStretch(bX1, bY1, bX2, bY2);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnPointContauto() 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( m_pDibObject );

	int *pHistogram = PointOperation.GetHistogram();
	
	int i = 0;
	int nGrayMax = 0, nGrayMin = 0;

	//������С�Ҷ�ֵ
	while(nGrayMin == 0 && i <= 255)
	{
		if( pHistogram[i] >= 100 )
		{
			nGrayMin = i;
		}
		i++;
	}

	//�������Ҷ�ֵ
	i = 255;
	while(nGrayMax == 0 && i >= 0)
	{
		if( pHistogram[i] >= 100 )
		{
			nGrayMax = i;
		}
		i--;
	}

	delete [] pHistogram;

	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	PointOperation.GrayStretch(nGrayMin,0,nGrayMax,255);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnPointThre() 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( m_pDibObject );

	int *pHistogram = PointOperation.GetHistogram();
	
	//����һ���Ի���CHistDlg���ʵ��
	CDlgThreshold ThreshDlg;

	//������ֱ��ͼ�����ݴ��ݸ�CHistDlg�Ի�����Ĺ��г�Ա����m_pnHistogram
	if( pHistogram != NULL )
	{
		ThreshDlg.m_pnHistogram = pHistogram;
		//���õ�ǰ����ֵΪ0��������
		//HistDlg.m_nCurrentPiexsNum = pHistogram[0];

	}
	
	// ���Ĺ����״
	BeginWaitCursor();

	ThreshDlg.m_nThresholdAutoAdjust = PointOperation.FindThresh_Discriminant();

	ThreshDlg.m_nThresholdDiscriminant = PointOperation.FindThresh_Auto();

	ThreshDlg.m_nCurrentThreshold = ThreshDlg.m_nThresholdAutoAdjust;
	
	//��ʾ�Ի���
	if ( ThreshDlg.DoModal() != IDOK)
		return;

	delete [] pHistogram;

	

	// ����Threshold()����������ֵ������
	PointOperation.Threshold(ThreshDlg.m_nCurrentThreshold, 
		ThreshDlg.m_nThresholdType, TRUE);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnPointInvert() 
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	BeginWaitCursor();
	//��ɫ��fa=-1.0��fb=255
	PointOperation.LinerTran(-1.0, 255);
	EndWaitCursor();

	//SetModifiedFlag(TRUE);

	UpdateAllViews(NULL);
}

void CDipDoc::OnEnhaSmooth() 
{
	// ģ��߶�
	int	nTempH;
	
	// ģ����
	int nTempW;
	
	// ģ��ϵ��
	float fTempC;
	
	// ģ������Ԫ��X����
	int nTempXc;
	
	// ģ������Ԫ��Y����
	int nTempYc;
	
	// ģ��Ԫ������
	float aValue[25];

	// �����Ի���
	CDlgSmooth dlgPara;
	
	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 1.0;
	aValue[4] = 1.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 1.0;
	aValue[9] = 1.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 1.0;
	aValue[14] = 1.0;
	aValue[15] = 1.0;
	aValue[16] = 1.0;
	aValue[17] = 1.0;
	aValue[18] = 1.0;
	aValue[19] = 1.0;
	aValue[20] = 1.0;
	aValue[21] = 1.0;
	aValue[22] = 1.0;
	aValue[23] = 1.0;
	aValue[24] = 1.0;
	
	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 9.0;
	dlgPara.m_fpArray = aValue;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.Smoothing(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
	
}

void CDipDoc::OnPointPseudocolor() 
{
	CDlgColor dlgPara;
	
	// ��ʼ������ֵ
	if (m_nColorIndex >= 0)
	{
		// ��ʼѡ�е�ǰ��α��ɫ
		dlgPara.m_nColor = m_nColorIndex;
	}
	else
	{
		// ��ʼѡ�лҶ�α��ɫ�����
		dlgPara.m_nColor = 0;
	}
	
	// ָ�����������ָ��
	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
	
	// α��ɫ������Ŀ
	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
	
	// �����ַ�������
	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	int nColor = dlgPara.m_nColor;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ����ReplaceColorPal()�����任��ɫ��
	PointOperation.ReplaceColorPal(m_pDibObject, (BYTE*) ColorsTable[nColor]);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();

}

void CDipDoc::OnEnhaGradsharp() 
{
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.GradSharping(10);

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}


void CDipDoc::OnEnhaMidianF() 
{
	// ���ڸ߶�
	int	nTempH;
	
	// ���ڿ��
	int nTempW;
	
	// ��������Ԫ��X����
	int nTempXc;
	
	// ��������Ԫ��Y����
	int nTempYc;

	// ����ʽ
	int nSortType;

	
	// �����Ի���
	CDlgMedianFilter dlgPara;
	
	// ��ʼ���Ի������ֵ
	dlgPara.m_nWindowSize = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_nSortType = 0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	nSortType  = dlgPara.m_nSortType;

	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.MedianFilter(nSortType, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnEnhaTempsharp() 
{
	// ģ��߶�
	int	nTempH;
	
	// ģ����
	int nTempW;
	
	// ģ��ϵ��
	float fTempC;
	
	// ģ������Ԫ��X����
	int nTempXc;
	
	// ģ������Ԫ��Y����
	int nTempYc;
	
	// ģ��Ԫ������
	float aValue[25];

	// ����Ԫ������
	int nDirection[8];

	// �����Ի���
	CEdgeDetDlg dlgPara;
	
	// ��ģ�����鸳��ֵ��ΪRobertsģ��1��
	aValue[0]  = -1.0;
	aValue[1]  =  0.0;
	aValue[2]  =  0.0;
	aValue[3]  =  0.0;
	aValue[4]  =  0.0;
	aValue[5]  =  0.0;
	aValue[6]  =  1.0;
	aValue[7]  =  0.0;
	aValue[8]  =  0.0;
	aValue[9]  =  0.0;
	aValue[10] =  0.0;
	aValue[11] =  0.0;
	aValue[12] =  0.0;
	aValue[13] =  0.0;
	aValue[14] =  0.0;
	aValue[15] =  0.0;
	aValue[16] =  0.0;
	aValue[17] =  0.0;
	aValue[18] =  0.0;
	aValue[19] =  0.0;
	aValue[20] =  0.0;
	aValue[21] =  0.0;
	aValue[22] =  0.0;
	aValue[23] =  0.0;
	aValue[24] =  0.0;
	
	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 1.0;
	dlgPara.m_fpArray = aValue;
	dlgPara.m_nDirection = nDirection;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;

	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.Sharping(aValue, nDirection);
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
	
}

void CDipDoc::OnEnhaLapsharp() 
{
	// ģ��߶�
	int	nTempH;
	
	// ģ����
	int nTempW;
	
	// ģ��ϵ��
	float fTempC;
	
	// ģ������Ԫ��X����
	int nTempXc;
	
	// ģ������Ԫ��Y����
	int nTempYc;
	
	// ģ��Ԫ������
	float aValue[25];

	// �����Ի���
	CLapDlg dlgPara;
	
	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	aValue[0]  = -1.0;
	aValue[1]  = -1.0;
	aValue[2]  = -1.0;
	aValue[3]  =  0.0;
	aValue[4]  =  0.0;
	aValue[5]  = -1.0;
	aValue[6]  =  9.0;
	aValue[7]  = -1.0;
	aValue[8]  =  0.0;
	aValue[9]  =  0.0;
	aValue[10] = -1.0;
	aValue[11] = -1.0;
	aValue[12] = -1.0;
	aValue[13] =  0.0;
	aValue[14] =  0.0;
	aValue[15] =  0.0;
	aValue[16] =  0.0;
	aValue[17] =  0.0;
	aValue[18] =  0.0;
	aValue[19] =  0.0;
	aValue[20] =  0.0;
	aValue[21] =  0.0;
	aValue[22] =  0.0;
	aValue[23] =  0.0;
	aValue[24] =  0.0;
	
	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 1.0;
	dlgPara.m_fpArray = aValue;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.LapSharping(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
	
}


void CDipDoc::OnChannelSplit() 
{
	//��ȡӦ�ó�����ָ��
	CDipApp *pApp = GetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *red = new CDibObject(m_pDibObject);
	CDibObject *green = new CDibObject(m_pDibObject);
	CDibObject *blue = new CDibObject(m_pDibObject);
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	if (PointOperation.SplitChannels(red, green, blue))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = red;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_RED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
		pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = green;
		pDocument->m_bImageLoaded = TRUE;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_GREEN_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
		pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = blue;
		pDocument->m_bImageLoaded = TRUE;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_BLUE_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// �ָ����
	EndWaitCursor();

}

void CDipDoc::OnChannelBlue() 
{
	//��ȡӦ�ó�����ָ��
	CDipApp *pApp = GetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *blue = new CDibObject(m_pDibObject);
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(blue))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = blue;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_BLUE_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnChannelGreen() 
{
	//��ȡӦ�ó�����ָ��
	CDipApp *pApp = GetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *green = new CDibObject(m_pDibObject);
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(green))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = green;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_GREEN_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnChannelMixed() 
{
	//��ȡӦ�ó�����ָ��
	CDipApp *pApp = GetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *mixed = new CDibObject(m_pDibObject);
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(mixed))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = mixed;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_MIXED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnChannelRed() 
{
	//��ȡӦ�ó�����ָ��
	CDipApp *pApp = GetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *red = new CDibObject(m_pDibObject);
	
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(red))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = red;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_RED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnUpdateChannelGreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelMixed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelRed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelBlue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelSplit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdatePointThre(CCmdUI* pCmdUI) 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( m_pDibObject );

	//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnUpdatePointPseudocolor(CCmdUI* pCmdUI) 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( m_pDibObject );

	//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnDetectProject() 
{
	//����ͶӰģʽѡ��Ի���
	CDlgProject projectmode;
	if(projectmode.DoModal() == IDOK)
	{
		//�����㴦��CPointPro�����
		CAreaPro AreaPro( m_pDibObject );
		// ���Ĺ����״
		BeginWaitCursor();
		//ѡ��ͶӰģʽ
		if(projectmode.m_iProjectMode == 0)
			AreaPro.ProjectH();
		else
			AreaPro.ProjectV();
		// �ָ����
		EndWaitCursor();
		
		// ������ͼ
		UpdateAllViews(NULL);
	}
}

void CDipDoc::OnUpdateDetectProject(CCmdUI* pCmdUI) 
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	//ֻ��8λ��ֵͼ��������
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnEdgeFill() 
{
	//����������䷽ʽѡ��Ի���
	CDlgSeedFill seedfill;
	if(seedfill.DoModal() == IDOK)
	{
		//�����㴦��CPointPro�����
		CAreaPro AreaPro( m_pDibObject );
		// ���Ĺ����״
		BeginWaitCursor();
		//ѡ��������䷽��
		if(seedfill.m_iSeedMethod == 0)
			AreaPro.Feeding1();
		else
			AreaPro.Feeding2();
		// �ָ����
		EndWaitCursor();
		
		// ������ͼ
		UpdateAllViews(NULL);
	}
}

void CDipDoc::OnUpdateEdgeFill(CCmdUI* pCmdUI) 
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );
	//ֻ��8λ��ֵͼ��������
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnDetectTemplate() 
{
	CDibObject *pTemplate;
	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;

		//���õȴ����
		BeginWaitCursor();

		pTemplate = new CDibObject( strPathName.GetBuffer(3) );

		//ȡ���ȴ����
		EndWaitCursor();

		//����ͼ���ļ�ʧ��
		if( pTemplate == NULL )
		{
			AfxMessageBox("�޷�����ƥ��ģ��ͼ�������");
			//����FALSE
			return;
		}

		//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
		CPointPro PointOperation( pTemplate );
		
		//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
		if(!PointOperation.IsGray256())
		{
			AfxMessageBox("ƥ��ģ��ͼ����256���Ҷ�ͼ���޷����в�����");
			//����FALSE
			return;
		}

		//ԭʼͼ���С
		int nOldWidth = m_pDibObject->GetWidth();
		int nOldHeight = m_pDibObject->GetHeight();
		
		//ģ��ͼ���С
		int nTempWidth = pTemplate->GetWidth();
		int nTempHeight = pTemplate->GetHeight();
		
		//����ͼ��Ĵ�СӦһ��
		if (nTempWidth > nOldWidth || nTempHeight > nOldHeight)
		{
			AfxMessageBox("ģ��ͼ��̫���޷���ɲ�����");
			return;
		}

		//�����㴦��CPointPro�����
		CAreaPro AreaPro( m_pDibObject );
		// ���Ĺ����״
		BeginWaitCursor();
		
		AreaPro.TemplateMatch(pTemplate);
		
		// �ָ����
		EndWaitCursor();
		
		// ������ͼ
		UpdateAllViews(NULL);
	}	
}

void CDipDoc::OnUpdateDetectTemplate(CCmdUI* pCmdUI) 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( m_pDibObject );

	//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnEdgeTrace() 
{
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.EdgeTracing();

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnUpdateEdgeTrace(CCmdUI* pCmdUI) 
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnEdgeContour() 
{
	//�����㴦��CPointPro�����
	CAreaPro AreaPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.ContourPickUp();

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnUpdateEdgeContour(CCmdUI* pCmdUI) 
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnDetectMinus() 
{
	//ģ��ͼ������ָ��
	unsigned char *pBitsTemp;

	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;

		//���õȴ����
		BeginWaitCursor();

		CDibObject *pTemplate = new CDibObject( strPathName.GetBuffer(3) );

		//ȡ���ȴ����
		EndWaitCursor();

		//����ͼ���ļ�ʧ��
		if( pTemplate == NULL )
		{
			AfxMessageBox("�޷�������Ӱģ��ͼ�������");
			//����FALSE
			return;
		}

		//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
		CPointPro PointOperation( pTemplate );
		
		//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
		if(!PointOperation.IsGray256())
		{
			AfxMessageBox("��Ӱģ��ͼ����256���Ҷ�ͼ���޷����в�����");
			//����FALSE
			return;
		}

		//ԭʼͼ���С
		int nOldWidth = m_pDibObject->GetWidth();
		int nOldHeight = m_pDibObject->GetHeight();
		
		//ģ��ͼ���С
		int nTempWidth = pTemplate->GetWidth();
		int nTempHeight = pTemplate->GetHeight();
		
		CRect rectOld(0, 0, nOldWidth, nOldHeight);
		CRect rectTemp(0, 0, nTempWidth, nTempHeight);
		
		//����ͼ��Ĵ�СӦһ��
		if (rectOld != rectTemp)
		{
			AfxMessageBox("ģ��ͼ���ԭͼ���С��һ�£��޷���ɲ�����");
			return;
		}

		//�����㴦��CPointPro�����
		CAreaPro AreaPro1( pTemplate );
		// ���Ĺ����״
		BeginWaitCursor();
		
		pBitsTemp = AreaPro1.GetBitsPointer();
		
		// �ָ����
		EndWaitCursor();

		//�����㴦��CPointPro�����
		CAreaPro AreaPro2( m_pDibObject );

		// ���Ĺ����״
		BeginWaitCursor();
		
		AreaPro2.Minusing(pBitsTemp);
		
		// �ָ����
		EndWaitCursor();
		
		// ������ͼ
		UpdateAllViews(NULL);
		
	}
}

void CDipDoc::OnUpdateDetectMinus(CCmdUI* pCmdUI) 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( m_pDibObject );

	//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnGeomMirh() 
{
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()�������д�ֱ����
	GeoTrans.Mirror(TRUE);

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnGeomMirv() 
{
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()��������ˮƽ����
	GeoTrans.Mirror(FALSE);

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnGeomRota() 
{
	//��ת�Ƕ�
	int nRotateAngle;
	int nIntorType;
	BOOL bIntorType;
	
	//�����Ի���
	CDlgGeoRota dlgPara;
	
	//��ʼ������ֵ
	dlgPara.m_iRotateAngle = 90;
	dlgPara.m_nIntorType = 0;
	
	//��ʾ�Ի�����ʾ�û��趨��ת�Ƕ�
	if(dlgPara.DoModal() != IDOK)
	{
		//����
		return;
	}
	
	//��ȡ�û��趨�ĽǶ�
	nRotateAngle = dlgPara.m_iRotateAngle;
	nIntorType = dlgPara.m_nIntorType;
	
	//ɾ���Ի���
	delete dlgPara;	

	if(nIntorType == 0)
	{
		bIntorType = FALSE;
	}
	else
	{
		bIntorType = TRUE;
	}

	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()�������д�ֱ����
	GeoTrans.Rotate(nRotateAngle, bIntorType);

	// ��������
	//	SetModifiedFlag(TRUE);
	
	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnGeomTran() 
{
	// �����Ի���
	CDlgGeoTran dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_XOffset = 0;
	dlgPara.m_YOffset = 0;
	dlgPara.m_nTransType = 0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	LONG lXOffset = dlgPara.m_XOffset;
	LONG lYOffset = dlgPara.m_YOffset;
	int nTransType = dlgPara.m_nTransType;
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	if(nTransType == 0)
	{
		// ����TranslationPixel()��������ƽ��
		GeoTrans.TranslationPixel(lXOffset, lYOffset);
	}
	else if(nTransType == 1)
	{
		// ����TranslationLine()��������ƽ��
		GeoTrans.TranslationLine(lXOffset, lYOffset);
	}

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnGeomTrpo() 
{
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()�������д�ֱ����
	GeoTrans.Transpose();

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDipDoc::OnGeomZoom() 
{
// ���ű���
	float fXZoomRatio;
	float fYZoomRatio;
	int nIntorType;
	BOOL bIntorType;
	
	// �����Ի���
	CDlgGeoZoom dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_XZoom = 4.0;
	dlgPara.m_YZoom = 4.0;
	dlgPara.m_nIntorType = 0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	fXZoomRatio = dlgPara.m_XZoom;
	fYZoomRatio = dlgPara.m_YZoom;
	nIntorType = dlgPara.m_nIntorType;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	if(nIntorType == 0)
	{
		bIntorType = FALSE;
	}
	else
	{
		bIntorType = TRUE;
	}
	
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();
	
	//����Mirror()�������д�ֱ����
	GeoTrans.ZoomImage(fXZoomRatio, fYZoomRatio, bIntorType);
	
	// ��������
	//	SetModifiedFlag(TRUE);
	
	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

//�����������������Ϣӳ�䣬��֤ͼ�񼸺α任ֻ��8λ���ϵ�ͼ����в�����
void CDipDoc::OnUpdateGeomMirh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomMirv(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomRota(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomTran(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomTrpo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnFreqDct() 
{
	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	m_matTransed = FreqPro.DCT2() ;

	if(!isempty(m_matTransed)) m_bDct = TRUE;
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqNdct() 
{
	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	if(FreqPro.NDCT2(m_matTransed)) m_bDct = FALSE;
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqFft() 
{
	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	m_matTransed = FreqPro.FFT2() ;

	if(!isempty(m_matTransed)) m_bFft = TRUE;
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqNfft() 
{
	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	if(FreqPro.NFFT2(m_matTransed)) m_bFft = FALSE;
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqFftFilte() 
{
	// �����Ի���
	CDlgFouriorFilte dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_nFitleParaA = 0.0;
	dlgPara.m_nFilteType = 0;
	
	// ��ʾ�Ի�����ʾ�û��趨�˲�����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨���˲�����
	double a = dlgPara.m_nFitleParaA;
	int nType = dlgPara.m_nFilteType;

	// ɾ���Ի���
	delete dlgPara;	
	
	if(nType == 0)
	{
		//��ͨ�˲�
		Mm matSize = size(m_matTransed);
		Mm matTemp = mabs(m_matTransed);
		Mm matVector = reshape(m_matTransed, matSize.r(1,1)*matSize.r(1,2) , 1);

		Mm matMax = mmax(matVector);

		matTemp = matTemp / matMax;
		Mm matL = matTemp >= (double)a;
		m_matTransed = times(m_matTransed, matL);
	}
	else if(nType == 1)
	{
		//��ͨ�˲�
		Mm matSize = size(m_matTransed);
		Mm matTemp = mabs(m_matTransed);
		Mm matVector = reshape(m_matTransed, matSize.r(1,1)*matSize.r(1,2) , 1);

		Mm matMax = mmax(matVector);

		matTemp = matTemp / matMax;
		Mm matL = matTemp <= (double)a;
		m_matTransed = times(m_matTransed, matL);
	}

	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	if(FreqPro.NFFT2(m_matTransed)) m_bFft = FALSE;
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);

}

void CDipDoc::OnFreqWalh() 
{
	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	m_matTransed = FreqPro.WHT2() ;

	if(!isempty(m_matTransed)) m_bWht = TRUE;
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqNwalh() 
{
	//�����㴦��CImagePro�����
	CFreqPro FreqPro( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	if(FreqPro.NWHT2(m_matTransed)) m_bWht = FALSE;
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

//�����߸����������Ϣӳ�䣬��֤Ƶ��任ֻ��8λͼ����в�����
void CDipDoc::OnUpdateFreqDct(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateFreqNdct(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bDct);
}

void CDipDoc::OnUpdateFreqFft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateFreqNfft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bFft);
}

void CDipDoc::OnUpdateFreqFftFilte(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bFft);
}

void CDipDoc::OnUpdateFreqWalh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateFreqNwalh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bWht);
}
