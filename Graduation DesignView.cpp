// Graduation DesignView.cpp : implementation of the CGraduationDesignView class
//

#include "stdafx.h"
#include "Graduation Design.h"
#include "Graduation DesignDoc.h"
#include "Graduation DesignView.h"
#include "MainFrm.h"
#include "ImageCenterDib.h"
#include "Make3D.h"
#include "BilDialog.h"
#include"BlockDialog.h"

#include"Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignView

IMPLEMENT_DYNCREATE(CGraduationDesignView, CScrollView)

BEGIN_MESSAGE_MAP(CGraduationDesignView, CScrollView)
	//{{AFX_MSG_MAP(CGraduationDesignView)
	ON_COMMAND(ID_MGETDEPTH, OnMgetdepth)
	ON_COMMAND(ID_MBILFILTER, OnMbilfilter)
	ON_COMMAND(M_Gray, OnGray)
	ON_COMMAND(M_Dibr, OnDibr)
	ON_COMMAND(M_Rightview, OnRightview)
	ON_COMMAND(M_Holefilling, OnHolefilling)
	ON_COMMAND(M_Final, OnFinal)
	ON_COMMAND(M_Binary, OnBinary)
	ON_COMMAND(M_Getloneline, OnGetloneline)
	ON_COMMAND(IDM_Soble, OnSoble)
	ON_COMMAND(ID_MTemplate, OnMTemplate)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignView construction/destruction

CGraduationDesignView::CGraduationDesignView()
{
	// TODO: add construction code here

}

CGraduationDesignView::~CGraduationDesignView()
{
}

BOOL CGraduationDesignView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignView drawing

void CGraduationDesignView::OnDraw(CDC* pDC)
{
	CGraduationDesignDoc* pDoc = GetDocument();
	// TODO: add draw code for native data here
		
	ImgCenterDib *pDib=pDoc->GetPDib();

	//��ȡDIB�ĳߴ�
	CSize sizeFileDib = pDib->GetDimensions();

	//��ʾDIB
	pDib->Draw(pDC, CPoint(0, 0), sizeFileDib);
}

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignView printing

BOOL CGraduationDesignView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGraduationDesignView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGraduationDesignView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignView diagnostics

#ifdef _DEBUG
void CGraduationDesignView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGraduationDesignView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGraduationDesignDoc* CGraduationDesignView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraduationDesignDoc)));
	return (CGraduationDesignDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignView message handlers
void CGraduationDesignView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{

	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::CalcWindowRect(lpClientRect, nAdjustType);
}

void CGraduationDesignView::OnMgetdepth() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();

	if(pDib->m_nBitCount != 8)
	{
		MessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
	Make3D segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   CBlockDialog *dlg=new CBlockDialog();
	dlg->DoModal();
	int blockw;
	int blockh;
	switch(dlg->m_nIndex1)
	{
	case 0:
		 blockw=2;
	     blockh=2;
	      break;
	case 1:
		blockw=4;
	     blockh=4;
		break;
	case 2:
		blockw=8;
	    blockh=8;
		break;
	default:
		break;
	}
    segment.MSTsegment( blockw,blockh);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTable,segment.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
}


void CGraduationDesignView::OnMbilfilter() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8)
	{
	
       MessageBox("ֻ����Ҷ�ͼ��");
		return ;
	}
	
	CBilDialog *dlg=new CBilDialog();
	dlg->DoModal();
	int modal;
	switch(dlg->m_nIndex)
	{
	case 0:
		modal=3;break;
	case 1:
		modal=5;break;
	case 2:
		modal=7;break;
	default:
		break;
	}

   Make3D imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	int iFilterH=modal;
	int iFilterW=modal;
	int iFilterCX=(modal-1)/2;
	int iFilterCY=(modal-1)/2;
    imgnoise.BilateralFilter(iFilterH, iFilterW, iFilterCX, iFilterCY);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable,imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	
	
}

void CGraduationDesignView::OnGray() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=24)
	{
	
       MessageBox("ֻ�����ɫͼ��");
		return ;
	}

    Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	graytrans.ColorToGray();
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	
}

void CGraduationDesignView::OnDibr() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
    	if(pDib->m_nBitCount!=24)
	{
	
       MessageBox("ֻ�����ɫͼ��");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("�����һ��ͼƬ");

    dibr.DIBR();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutleft);//������ͼ�滻
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
}

void CGraduationDesignView::OnRightview() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
    	if(pDib->m_nBitCount!=24)
	{
	
       MessageBox("ֻ�����ɫͼ��");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("�����һ��ͼƬ");

    dibr.DIBR();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutright);//������ͼ�滻
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
	
}

void CGraduationDesignView::OnHolefilling() 
{
	// TODO: Add your command handler code here

	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
    	if(pDib->m_nBitCount!=24)
	{
	
       MessageBox("ֻ�����ɫͼ��");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("�����һ��ͼƬ");

    dibr.Holefilling();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutleft);//������ͼ�滻
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
}

void CGraduationDesignView::OnFinal() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
    	if(pDib->m_nBitCount!=24)
	{
	
       MessageBox("ֻ�����ɫͼ��");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("�����һ��ͼƬ");

    dibr.RedCyan();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutfinal);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
	
}

void CGraduationDesignView::OnBinary() 
{
	// TODO: Add your command handler code here
		//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//����Binary()��ͼ����ж�ֵ����ȱʡ״̬����ֵΪ
	graytrans.Binary(128);
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CGraduationDesignView::OnGetloneline() 
{
	// TODO: Add your command handler code here
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	Make3D segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//���뾶���ֱ��ʺͽǶȼ��ֱ���
	float radiusResolution=1;
	float angleResolution=1;

	//������
	segment.longestLineDetectByHough(radiusResolution,angleResolution);
	
	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CGraduationDesignView::OnSoble() 
{
	// TODO: Add your command handler code here
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//����Binary()��ͼ����ж�ֵ����ȱʡ״̬����ֵΪ
	graytrans.Sobel();
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}



void CGraduationDesignView::OnMTemplate() 
{
	// TODO: Add your command handler code here
		//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"�������ֵͼ��",MB_OK,0);   //�ж��Ƿ�Ϊ�Ҷ�ͼ
		return ;
	}
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//����Binary()��ͼ����ж�ֵ����ȱʡ״̬����ֵΪ
	graytrans.Template();
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	
}


void CGraduationDesignView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT,CSize(5000,1000));
	
}
