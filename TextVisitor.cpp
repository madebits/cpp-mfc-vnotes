// TextVisitor.cpp: implementation of the TextVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VNotes.h"
#include "TextVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR* NL = _T("\r\n");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TextVisitor::TextVisitor(LPCTSTR fileName)
{
	m_topLevel = 0;
	m_lastNoteNr = 0;
	m_initOk = TRUE;
	m_buffer = NULL;
	m_connents = "";
	if(!m_outFile.Open(fileName, CFile::modeCreate | CFile::modeWrite)){
		m_initOk = FALSE;
		AfxMessageBox(IDS_OPEN_ERR);
	}
	if(IsInitialized()){
		for(int i = 0; i < 40; i++){
			m_separator += _T("-");
		}
		m_separator += NL;
		CString title;
		AfxGetApp()->m_pMainWnd->GetWindowText(title);
		WriteTextLn(title + _T(" generated on ") + XMLDoc::GetTimeString());
		WriteTextLn(CString(_T("")));
	}
}

TextVisitor::~TextVisitor()
{
	if(IsInitialized()){
		WriteTextLn(CString(_T("\r\n<<--- CONTENTS --->>\r\n")));
		WriteTextLn(m_connents);
		WriteText(CString(_T("EOF ") + XMLDoc::GetTimeString()), TRUE);
		m_outFile.Close();
	}
}

LPARAM TextVisitor::VisitNode(IXMLElement *pElem, LPARAM p)
{
	if(!IsInitialized()) return (LPARAM)NULL;
	if(pElem == NULL) return (LPARAM)NULL;
 	BSTR bstrString;
 	pElem->get_tagName(&bstrString);
 	_bstr_t tagName(bstrString);
	SysFreeString(bstrString);
	if(lstrcmp((TCHAR*)tagName, NoteTag::NOTE) == 0){
		if(p == 0){
			m_topLevel++;
			m_lastNoteNr = -1;
		}
		Note n;
		if(!XMLDoc::GetNoteData(pElem, &n)) return p;
		CString sTitle;
		sTitle.Format(_T("#[%d.%d](L%d)"), m_topLevel, ++m_lastNoteNr, p);
		CString note = Note::Note2Text(n);
		WriteTextLn(sTitle + NL + note + NL);
		WriteTextLn(m_separator);
		for(int i = 0; i < p; i++){
			m_connents += _T("   ");
		}
		m_connents += sTitle + _T(" ") + n.m_title + NL;

	}
	return ++p;
}

void TextVisitor::WriteTextLn(CString &text)
{
	WriteText((text + NL));
}

void TextVisitor::WriteText(CString& text)
{
	WriteText(text, FALSE);
}

void TextVisitor::WriteText(CString &text, BOOL flush)
{
	if(m_buffer == NULL) m_buffer = new CString();
	(*m_buffer) += text;
	if(flush || (m_buffer->GetLength() > 8192)){
		m_outFile.Write((LPCSTR)(*m_buffer), m_buffer->GetLength());
		m_buffer = NULL;
	}
}

BOOL TextVisitor::IsInitialized()
{
	return m_initOk;
}


