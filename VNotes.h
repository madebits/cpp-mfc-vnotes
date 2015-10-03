// VNotes.h : main header file for the VNotes application
//

#if !defined(AFX_VNotes_H__BAD65AB7_D065_493D_AF14_914591F80363__INCLUDED_)
#define AFX_VNotes_H__BAD65AB7_D065_493D_AF14_914591F80363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVNotesApp:
// See VNotes.cpp for the implementation of this class
//

class CVNotesApp : public CWinApp
{
public:
	CVNotesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVNotesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVNotesApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VNotes_H__BAD65AB7_D065_493D_AF14_914591F80363__INCLUDED_)
