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

// WavefrontReaderView.h : interface of the CWavefrontReaderView class
//

#pragma once
#include "ObjectReader.h"

class CWavefrontReaderView : public CView
{
protected: // create from serialization only
	CWavefrontReaderView();
	DECLARE_DYNCREATE(CWavefrontReaderView)

// Attributes
public:
	CWavefrontReaderDoc* GetDocument() const;
	float aspect;
private:
	/* Camera variables */
	float camera_angle;
	float camera_height;
	float camera_dis;
	float camera_X, camera_Y, camera_Z;
	float lookAt_X, lookAt_Y, lookAt_Z;
// Operations
public:
	HGLRC m_hRC;
	Model_OBJ* model;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPaint();
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// setup OpenGL
	virtual void SetupOpenGL(void);

// Implementation
public:
	virtual ~CWavefrontReaderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // debug version in WavefrontReaderView.cpp
inline CWavefrontReaderDoc* CWavefrontReaderView::GetDocument() const
   { return reinterpret_cast<CWavefrontReaderDoc*>(m_pDocument); }
#endif

