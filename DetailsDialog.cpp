// DetailsDialog.cpp : implementation file
//
#include "stdafx.h"
#include "VNotes.h"
#include "DetailsDialog.h"

#include "MainFrm.h"
#include "BrowseTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetailsDialog dialog

CDetailsDialog::CDetailsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsDialog)
	m_body = _T("");
	m_comment = _T("");
	m_created = _T("");
	m_modified = _T("");
	m_title = _T("");
	//}}AFX_DATA_INIT
}

void CDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDialog)
	DDX_Control(pDX, IDC_TITLE, m_TitleC);
	DDX_Control(pDX, IDC_COMMENT, m_CommentC);
	DDX_Control(pDX, IDC_BODY, m_BodyC);
	DDX_Text(pDX, IDC_BODY, m_body);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_CREATED_TIME, m_created);
	DDX_Text(pDX, IDC_MODIFIED_TIME, m_modified);
	DDX_Text(pDX, IDC_TITLE, m_title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDialog, CDialog)
	//{{AFX_MSG_MAP(CDetailsDialog)
	ON_EN_CHANGE(IDC_BODY, OnChangeBody)
	ON_EN_CHANGE(IDC_TITLE, OnChangeTitle)
	ON_EN_CHANGE(IDC_COMMENT, OnChangeComment)
	ON_BN_CLICKED(IDC_UPDATE_TITLE, OnUpdateTitle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsDialog message handlers

void CDetailsDialog::OnCancel() // required override
{
	OnUpdateTitle();
	CMainFrame* mf = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(mf != NULL){
		mf->GetLeftTreeView()->SetFocus();
	}
}

void CDetailsDialog::OnChangeBody() 
{
	SetModified();
}

void CDetailsDialog::OnChangeTitle() 
{
	SetModified();
}

void CDetailsDialog::OnChangeComment() 
{
	SetModified();
}

void CDetailsDialog::OnUpdateTitle() 
{
	CMainFrame* mf = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(mf != NULL){
		mf->GetLeftTreeView()->SaveIfModified();
	}
}

void CDetailsDialog::SetModified()
{
	CMainFrame* mf = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(mf != NULL){
		BrowseTree* btv = mf->GetLeftTreeView();
		if(btv != NULL)
			btv->GetDocument()->SetModifiedFlag();
	}
}

BOOL CDetailsDialog::IsTextModified(UINT ID)
{
	switch(ID){
	case IDC_TITLE:
		return m_TitleC.GetModify();
	case IDC_COMMENT:
		return m_CommentC.GetModify();
	case IDC_BODY:
		return m_BodyC.GetModify();
	}
	return FALSE;
}

void CDetailsDialog::ClearModified()
{
	m_TitleC.SetModify(FALSE);
	m_CommentC.SetModify(FALSE);
	m_BodyC.SetModify(FALSE);
}

void CDetailsDialog::SetFieldsFromNote(Note &n, BOOL all)
{
	m_title = n.m_title;
	m_comment = n.m_comment;
	m_modified = n.m_modified;
	if(all){
		m_created = n.m_created;
		m_body = n.m_body;
	}
}
