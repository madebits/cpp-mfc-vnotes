// XMLDoc.h: interface for the XMLDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLDOC_H__552240D6_98C2_499B_833A_01039A57D408__INCLUDED_)
#define AFX_XMLDOC_H__552240D6_98C2_499B_833A_01039A57D408__INCLUDED_

#include <io.h>
#include <hlink.h>
#include <dispex.h>
#include "mshtml.h"
#include "msxml.h"
#include "afxpriv.h" // for CArchiveStream

#include <comdef.h>
#include "XMLVisitor.h"
#include "Note.h"

#define SAFERELEASE(p) if (p) {(p)->Release(); p = NULL;} else ;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XMLDoc  
{
public:
	void SetBodyText(IXMLElement *pBody, CString& text);
	void SetNoteData(IXMLElement* pElem, Note* n, BOOL updateBody);
	static BOOL GetNoteData(IXMLElement* pElem, Note* n);
	static IXMLElement* GetBodyTagForNote(IXMLElement *pElem);
	BOOL IsFormatSupported();
	static CString DecodeData(CString data);
	static CString EncodeData(CString data);
	BOOL DeleteElement(IXMLElement* pElem);
	static CString GetTimeString();
	IXMLElement* AddNewElement(IXMLElement* pParentElem);
	BOOL InitNewDocument();
	BOOL SaveToURL(CArchive& ar);
	void VisitNodes(IXMLElement *pElem, XMLVisitor &visitor, LPARAM p);
	void VisitNodes(XMLVisitor &visitor);
	IXMLDocument * m_pDoc;
	BOOL InitFromURL(CArchive& ar);
	XMLDoc();
	virtual ~XMLDoc();

private:
	BOOL m_xmlUnknownFormat;
	static TCHAR DecodeChar(CString c);
	static CString EncodeChar(TCHAR c);
	void ShowXMLError();
	BOOL CreateDocument(IXMLDocument** ppDoc);
	BOOL m_ComOK;
};

#endif // !defined(AFX_XMLDOC_H__552240D6_98C2_499B_833A_01039A57D408__INCLUDED_)
