// TextVisitor.h: interface for the TextVisitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTVISITOR_H__7C756009_96F6_4677_97C0_18769F18C673__INCLUDED_)
#define AFX_TEXTVISITOR_H__7C756009_96F6_4677_97C0_18769F18C673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLDoc.h"
#include "XMLVisitor.h"

class TextVisitor : public XMLVisitor  
{
public:
	BOOL IsInitialized();
	virtual LPARAM VisitNode(IXMLElement* pElem, LPARAM p);
	TextVisitor(LPCTSTR fileName);
	virtual ~TextVisitor();

private:
	CString m_separator;
	CString m_connents;
	int m_lastNoteNr;
	int m_topLevel;
	void WriteTextLn(CString& text);
	BOOL m_initOk;
	void WriteText(CString& text, BOOL flush);
	CString* m_buffer;
	CFile m_outFile;
	void WriteText(CString& text);
};

#endif // !defined(AFX_TEXTVISITOR_H__7C756009_96F6_4677_97C0_18769F18C673__INCLUDED_)
