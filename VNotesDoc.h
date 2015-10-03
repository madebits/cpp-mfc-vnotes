// VNotesDoc.h : interface of the CVNotesDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VNOTESDOC_H__655A5E60_F3DA_481B_9EB5_243C3F206DA7__INCLUDED_)
#define AFX_VNOTESDOC_H__655A5E60_F3DA_481B_9EB5_243C3F206DA7__INCLUDED_

#include "XMLDoc.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVNotesDoc : public CDocument
{
protected: // create from serialization only
	CVNotesDoc();
	DECLARE_DYNCREATE(CVNotesDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVNotesDoc)
	public:
	virtual void DeleteContents();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	XMLDoc *m_xmlDoc;
	virtual ~CVNotesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVNotesDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CreateXMLDocument();
	BOOL ValidateVXMLFile(LPCTSTR file);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VNOTESDOC_H__655A5E60_F3DA_481B_9EB5_243C3F206DA7__INCLUDED_)
