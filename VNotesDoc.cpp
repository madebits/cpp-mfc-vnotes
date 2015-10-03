// VNotesDoc.cpp : implementation of the CVNotesDoc class
//

#include "stdafx.h"
#include "VNotes.h"

#include "VNotesDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVNotesDoc

IMPLEMENT_DYNCREATE(CVNotesDoc, CDocument)

BEGIN_MESSAGE_MAP(CVNotesDoc, CDocument)
	//{{AFX_MSG_MAP(CVNotesDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVNotesDoc construction/destruction

CVNotesDoc::CVNotesDoc()
{
	m_xmlDoc = new XMLDoc();
}

CVNotesDoc::~CVNotesDoc()
{
	if(m_xmlDoc != NULL){
		delete m_xmlDoc;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVNotesDoc diagnostics

#ifdef _DEBUG
void CVNotesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVNotesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVNotesDoc commands

BOOL CVNotesDoc::OnNewDocument()
{
 	if (!CDocument::OnNewDocument())
 		return FALSE;
	return CreateXMLDocument();
}

BOOL CVNotesDoc::CreateXMLDocument()
{
	if(m_xmlDoc != NULL){
		delete m_xmlDoc;
		m_xmlDoc = NULL;
	}
	m_xmlDoc = new XMLDoc();
	if(!m_xmlDoc->InitNewDocument()){
		AfxMessageBox(IDS_DOC_INIT_ERR);
		return FALSE;
	}
	return TRUE;
}

void CVNotesDoc::DeleteContents() 
{
	// called from CDocument::OnNewDocument()
	// and from CDocument::OnOpenDocument()
	CMainFrame* mf = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(mf != NULL && m_xmlDoc != NULL){
		BrowseTree* btv = mf->GetLeftTreeView();
		if(btv != NULL) btv->ClearTree();
		CVNotesView* cDetails = mf->GetDetailsView();
		if(cDetails != NULL){
			cDetails->ClearData();
			cDetails->EnableDetails(FALSE);
		}
	}
}

void CVNotesDoc::Serialize(CArchive& ar) 
{
	if(m_xmlDoc == NULL) m_xmlDoc = new XMLDoc();
	if (ar.IsStoring())
	{	// storing code
		m_xmlDoc->SaveToURL(ar);
	}
	else
	{	// loading code
		m_xmlDoc->InitFromURL(ar);	
	}
}

BOOL CVNotesDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CMainFrame* mf = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(mf != NULL){
		BrowseTree* btv = mf->GetLeftTreeView();
		if(btv != NULL) btv->SaveIfModified();
	}	
	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CVNotesDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	// add support for zero length files
	if(!ValidateVXMLFile(lpszPathName)) return FALSE;
	return TRUE;
}

BOOL CVNotesDoc::ValidateVXMLFile(LPCTSTR file)
{
	// note: Serialize is not called for zero length files
	CFile cfile(file, CFile::modeReadWrite);
	int len = (int)cfile.GetLength();
	cfile.Close();
	return (len == 0) ? CreateXMLDocument() : TRUE;
}