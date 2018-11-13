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

	//获取DIB的尺寸
	CSize sizeFileDib = pDib->GetDimensions();

	//显示DIB
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
		MessageBox("只能处理8位灰度图像!");
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
	
       MessageBox("只处理灰度图像");
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
	
       MessageBox("只处理彩色图像");
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
	
       MessageBox("只处理彩色图像");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("请打开另一幅图片");

    dibr.DIBR();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutleft);//用左视图替换
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
	
       MessageBox("只处理彩色图像");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("请打开另一幅图片");

    dibr.DIBR();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutright);//用右视图替换
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
	
       MessageBox("只处理彩色图像");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("请打开另一幅图片");

    dibr.Holefilling();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CGraduationDesignView* pView=(CGraduationDesignView*)pFrame->MDIGetActive()->GetActiveView();
	CGraduationDesignDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(dibr.GetDimensions(),dibr.m_nBitCountOut,dibr.m_lpColorTableOut, dibr.m_pImgDataOutleft);//用左视图替换
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
	
       MessageBox("只处理彩色图像");
		return ;
	}
	
	Make3D dibr(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    //MessageBox("请打开另一幅图片");

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
		//获取文档类中m_dib的指针，访问当前DIB数据
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.Binary(128);
	
	//建立一个新视图，显示分割结果
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
	//获取文档类中m_dib的指针，访问当前DIB数据
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	Make3D segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//极半径检测分辨率和角度检测分辨率
	float radiusResolution=1;
	float angleResolution=1;

	//检测最长线
	segment.longestLineDetectByHough(radiusResolution,angleResolution);
	
	//新建视图，显示处理结果
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
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.Sobel();
	
	//建立一个新视图，显示分割结果
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
		//获取文档类中m_dib的指针，访问当前DIB数据
	CGraduationDesignDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//异常判断
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"请输入二值图像",MB_OK,0);   //判断是否为灰度图
		return ;
	}
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	Make3D graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.Template();
	
	//建立一个新视图，显示分割结果
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
