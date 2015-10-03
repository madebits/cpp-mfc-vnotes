// Note.h: interface for the Note class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTE_H__49CBD63F_4019_4D8A_BFF0_9D8ECF9B8FB6__INCLUDED_)
#define AFX_NOTE_H__49CBD63F_4019_4D8A_BFF0_9D8ECF9B8FB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Note  
{
public:
	static CString Note2Text(Note &n);
	CString m_body;
	CString m_modified;
	CString m_created;
	CString m_comment;
	CString m_title;
	Note();
	virtual ~Note();

};

struct NoteTag{
	static TCHAR* NOTES;
	static TCHAR* NOTE;
	static TCHAR* TITLE;
	static TCHAR* COMMENT;
	static TCHAR* CREATED;
	static TCHAR* MODIFIED;
	static TCHAR* BODY;
	static TCHAR* NEW_NOTE_NAME;
};

#endif // !defined(AFX_NOTE_H__49CBD63F_4019_4D8A_BFF0_9D8ECF9B8FB6__INCLUDED_)
