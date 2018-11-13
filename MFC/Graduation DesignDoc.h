// Graduation DesignDoc.h : interface of the CGraduationDesignDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADUATIONDESIGNDOC_H__48380D2B_A959_4BBE_96F0_7AC665C993C1__INCLUDED_)
#define AFX_GRADUATIONDESIGNDOC_H__48380D2B_A959_4BBE_96F0_7AC665C993C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ImageCenterDib.h"

class CGraduationDesignDoc : public CDocument
{
protected: // create from serialization only
	CGraduationDesignDoc();
	DECLARE_DYNCREATE(CGraduationDesignDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraduationDesignDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraduationDesignDoc();
		//返回m_dib的指针
		ImgCenterDib*GetPDib()
	{
		return &m_dib;
	}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:

	//ImgCenterDib类的对象
	ImgCenterDib m_dib;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraduationDesignDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADUATIONDESIGNDOC_H__48380D2B_A959_4BBE_96F0_7AC665C993C1__INCLUDED_)
