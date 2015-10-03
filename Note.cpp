// Note.cpp: implementation of the Note class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VNotes.h"
#include "Note.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TCHAR* NoteTag::NOTES = _T("NOTES");
TCHAR* NoteTag::NOTE = _T("NOTE");
TCHAR* NoteTag::TITLE = _T("TITLE");
TCHAR* NoteTag::COMMENT = _T("COMMENT");
TCHAR* NoteTag::CREATED = _T("CREATED");
TCHAR* NoteTag::MODIFIED = _T("MODIFIED");
TCHAR* NoteTag::BODY = _T("BODY");
TCHAR* NoteTag::NEW_NOTE_NAME = _T("<new note>");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Note::Note()
{
	m_title = "";
	m_created = "";
	m_comment = "";
	m_modified = "";
	m_body = "";

}

Note::~Note()
{

}

CString Note::Note2Text(Note &n)
{
	TCHAR NL[] = _T("\r\n");
	CString note = _T("Title: ") + n.m_title + NL
		+ _T("Comment: ") + n.m_comment + NL
		+ _T("Created: ") + n.m_created
		+ _T(" Modified: ") + n.m_modified
		+ NL + NL + n.m_body;
	return note;
}
