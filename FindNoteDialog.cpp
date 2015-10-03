// FindNoteDialog.cpp : implementation file
//

#include "stdafx.h"
#include "VNotes.h"
#include "FindNoteDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindNoteDialog dialog


CFindNoteDialog::CFindNoteDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindNoteDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindNoteDialog)
	m_FindText = _T("");
	m_caseSensitive = FALSE;
	//}}AFX_DATA_INIT
}


void CFindNoteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindNoteDialog)
	DDX_Control(pDX, IDC_FIND_NEXT, m_findNextButton);
	DDX_Text(pDX, IDC_EDIT_FIND, m_FindText);
	DDX_Check(pDX, IDC_CHECK_CASE, m_caseSensitive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindNoteDialog, CDialog)
	//{{AFX_MSG_MAP(CFindNoteDialog)
	ON_BN_CLICKED(IDOK, OnFind)
	ON_BN_CLICKED(IDC_FIND_NEXT, OnFindNext)
	ON_BN_CLICKED(IDC_CHECK_CASE, OnCheckCase)
	ON_EN_CHANGE(IDC_EDIT_FIND, OnChangeEditFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindNoteDialog message handlers

BOOL CFindNoteDialog::Find(BOOL next)
{
	UpdateData(TRUE);
	CMainFrame* mf = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	BrowseTree* btv = mf->GetLeftTreeView();
	CString title = m_FindText;
	title.TrimLeft();
	title.TrimRight();
	if(!m_caseSensitive) title.MakeUpper();
	return btv->FindNote(title, next, m_caseSensitive);
}

void CFindNoteDialog::OnFind() 
{
	m_findNextButton.EnableWindow(Find(FALSE));
}

void CFindNoteDialog::OnFindNext() 
{
	m_findNextButton.EnableWindow(Find(TRUE));
}

void CFindNoteDialog::OnCancel() 
{
	CDialog::OnCancel();
}

void CFindNoteDialog::OnCheckCase() 
{
	m_findNextButton.EnableWindow(FALSE);
}

void CFindNoteDialog::OnChangeEditFind() 
{
	m_findNextButton.EnableWindow(FALSE);
}
