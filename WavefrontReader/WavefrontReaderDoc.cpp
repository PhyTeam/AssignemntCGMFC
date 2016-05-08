// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// WavefrontReaderDoc.cpp : implementation of the CWavefrontReaderDoc class
//

#include "stdafx.h"
#include "PLYReader.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WavefrontReader.h"
#endif

#include "WavefrontReaderDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWavefrontReaderDoc

IMPLEMENT_DYNCREATE(CWavefrontReaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CWavefrontReaderDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CWavefrontReaderDoc::OnFileOpen)
END_MESSAGE_MAP()


// CWavefrontReaderDoc construction/destruction

CWavefrontReaderDoc::CWavefrontReaderDoc()
{
	// TODO: add one-time construction code here
	//Mesh* mesh = readFile();
	//this->LMesh.push_back(mesh);
}

CWavefrontReaderDoc::~CWavefrontReaderDoc()
{
}

BOOL CWavefrontReaderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWavefrontReaderDoc serialization

void CWavefrontReaderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWavefrontReaderDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWavefrontReaderDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWavefrontReaderDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWavefrontReaderDoc diagnostics

#ifdef _DEBUG
void CWavefrontReaderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWavefrontReaderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWavefrontReaderDoc commands


void CWavefrontReaderDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);
	//dlg.m_ofn.nMaxFile = 1;
	dlg.m_ofn.lpstrFilter = L"Ply Files (*.ply)\0*.ply\0";
	dlg.m_ofn.lpstrTitle = L"Open PLY File As";

	CString filename;
	
	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
		::MessageBox(0, filename, L"Hello", MB_OK);
		char fn[200];
		strcpy(fn, CStringA(filename).GetString());
		Mesh* m = readFile(fn);
		theApp.AddToRecentFileList(filename.GetString());
		this->LMesh.push_back(m);
		this->UpdateAllViews(NULL);
		
	}
}
