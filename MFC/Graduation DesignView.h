// Graduation DesignView.h : interface of the CGraduationDesignView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADUATIONDESIGNVIEW_H__8EB65DE9_9DA4_4E62_8299_285E9A0B617E__INCLUDED_)
#define AFX_GRADUATIONDESIGNVIEW_H__8EB65DE9_9DA4_4E62_8299_285E9A0B617E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraduationDesignView : public CScrollView
{
protected: // create from serialization only
	CGraduationDesignView();
	DECLARE_DYNCREATE(CGraduationDesignView)

// Attributes
public:
	CGraduationDesignDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraduationDesignView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraduationDesignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraduationDesignView)
	afx_msg void OnMgetdepth();
	afx_msg void OnMbilfilter();
	afx_msg void OnGray();
	afx_msg void OnDibr();
	afx_msg void OnRightview();
	afx_msg void OnHolefilling();
	afx_msg void OnFinal();
	afx_msg void OnBinary();
	afx_msg void OnGetloneline();
	afx_msg void OnSoble();
	afx_msg void OnMTemplate();
	afx_msg void OnMDark();
	afx_msg void OnMfinaldepth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Graduation DesignView.cpp
inline CGraduationDesignDoc* CGraduationDesignView::GetDocument()
   { return (CGraduationDesignDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADUATIONDESIGNVIEW_H__8EB65DE9_9DA4_4E62_8299_285E9A0B617E__INCLUDED_)
