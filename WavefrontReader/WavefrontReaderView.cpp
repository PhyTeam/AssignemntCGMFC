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

// WavefrontReaderView.cpp : implementation of the CWavefrontReaderView class
//

#include "stdafx.h"
#include <GL\glut.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WavefrontReader.h"
#endif

#include "WavefrontReaderDoc.h"
#include "WavefrontReaderView.h"
#include "ObjectReader.h"
#include "PLYReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWavefrontReaderView

IMPLEMENT_DYNCREATE(CWavefrontReaderView, CView)

BEGIN_MESSAGE_MAP(CWavefrontReaderView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWavefrontReaderView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_FILE_OPEN, &CWavefrontReaderView::OnFileOpen)
END_MESSAGE_MAP()

// CWavefrontReaderView construction/destruction

CWavefrontReaderView::CWavefrontReaderView()
{
	// TODO: add construction code here
	this->camera_dis = 10;
	this->camera_angle = 0;
	this->camera_height = 0;
}

CWavefrontReaderView::~CWavefrontReaderView()
{
}

BOOL CWavefrontReaderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//SetupOpenGL();
	return CView::PreCreateWindow(cs);
}

// CWavefrontReaderView drawing

void CWavefrontReaderView::OnDraw(CDC* pDC)
{
	CWavefrontReaderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CWavefrontReaderView printing


void CWavefrontReaderView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWavefrontReaderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWavefrontReaderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWavefrontReaderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// On Create
int CWavefrontReaderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		;// return -1;

	// TODO:  Add your specialized creation code here
	//Setup Opengl
	SetupOpenGL();
	//Create Timer
	//m_hTimer = SetTimer(1, 100, NULL);


	return 0;
}

void CWavefrontReaderView::SetupOpenGL(void)
{
	//Declare Pixel Format
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	HDC hdc = GetDC()->m_hDC;
	
	// Set Pixel format.
	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	m_hRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, m_hRC);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.1f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	//glShadeModel(GL_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glMatrixMode(GL_PROJECTION);
	//gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	//model->Load("res/cessna.obj");
}

#define DEG2RAD (3.14159f/180.0f)

void CWavefrontReaderView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float eyex = camera_dis * cos(camera_angle);
	float eyez = camera_dis * sin(camera_angle);
	float eyey = camera_height;
	gluPerspective(60, this->aspect, 1, 1000);
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	auto document = this->GetDocument();
	glTranslated(-1, -1, -1);
	glScaled(0.01, 0.01, 0.01);
	glRotated(45, 0, 1, 0);
	for (auto const& i : document->LMesh) {
		i->DrawWireframe();
	}

	SwapBuffers(dc.m_ps.hdc);
}

void CWavefrontReaderView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	GLfloat aspect = (GLfloat)cx / cy;
	
	//Setting up viewport
	if (cy == 0)
		aspect = cx;
	this->aspect = aspect;
	glViewport(0, 0, cx, cy);
}

void CWavefrontReaderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_LEFT:
		camera_angle -= 0.1f;
		this->RedrawWindow();
		break;
	case VK_RIGHT:
		camera_angle += 0.1f;
		this->RedrawWindow();
		break;
	case VK_UP:
		camera_height += 0.1f;
		this->RedrawWindow();
		break;
	case VK_DOWN:
		camera_height -= 0.1f;
		this->RedrawWindow();
		break;
	default:
		break;
	}
}

void CWavefrontReaderView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWavefrontReaderView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWavefrontReaderView diagnostics

#ifdef _DEBUG
void CWavefrontReaderView::AssertValid() const
{
	CView::AssertValid();
}

void CWavefrontReaderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWavefrontReaderDoc* CWavefrontReaderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWavefrontReaderDoc)));
	return (CWavefrontReaderDoc*)m_pDocument;
}
#endif //_DEBUG



void CWavefrontReaderView::OnFileOpen()
{

	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 1;
	dlg.m_ofn.lpstrFilter = L"Ply Files (*.ply)\0*.ply\0";
	dlg.m_ofn.lpstrTitle = L"Open PLY File As";
	
	CString filename;

	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
		::MessageBox(0, filename, L"Hello", MB_OK);

		
	}
}
