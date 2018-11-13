// Graduation DesignDoc.cpp : implementation of the CGraduationDesignDoc class
//

#include "stdafx.h"
#include "Graduation Design.h"

#include "Graduation DesignDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignDoc

IMPLEMENT_DYNCREATE(CGraduationDesignDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraduationDesignDoc, CDocument)
	//{{AFX_MSG_MAP(CGraduationDesignDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignDoc construction/destruction

CGraduationDesignDoc::CGraduationDesignDoc()
{
	// TODO: add one-time construction code here

}

CGraduationDesignDoc::~CGraduationDesignDoc()
{
}

BOOL CGraduationDesignDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignDoc serialization

void CGraduationDesignDoc::Serialize(CArchive& ar)
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
// CGraduationDesignDoc diagnostics

#ifdef _DEBUG
void CGraduationDesignDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraduationDesignDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignDoc commands

BOOL CGraduationDesignDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{

if (m_dib.Read(lpszPathName) == TRUE) {	
		SetModifiedFlag(FALSE);     // start off with unmodified
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CGraduationDesignDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_dib.Write(lpszPathName) == TRUE)	{
		SetModifiedFlag(FALSE);     // have saved
		return TRUE;
	}
	else 
		return FALSE;


}



