// Graduation Design.h : main header file for the GRADUATION DESIGN application
//

#if !defined(AFX_GRADUATIONDESIGN_H__8542DE08_C2A1_4225_BE44_EA6C4D5B8A5E__INCLUDED_)
#define AFX_GRADUATIONDESIGN_H__8542DE08_C2A1_4225_BE44_EA6C4D5B8A5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraduationDesignApp:
// See Graduation Design.cpp for the implementation of this class
//

class CGraduationDesignApp : public CWinApp
{
public:
	CGraduationDesignApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraduationDesignApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGraduationDesignApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADUATIONDESIGN_H__8542DE08_C2A1_4225_BE44_EA6C4D5B8A5E__INCLUDED_)
