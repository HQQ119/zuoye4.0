
// hanqingqiongView.cpp: ChanqingqiongView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "hanqingqiong.h"
#endif

#include "hanqingqiongDoc.h"
#include "hanqingqiongView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChanqingqiongView

IMPLEMENT_DYNCREATE(ChanqingqiongView, CView)

BEGIN_MESSAGE_MAP(ChanqingqiongView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(ID_QUATERNION, &ChanqingqiongView::OnQuaternion)
	ON_COMMAND(ID_ANGLE, &ChanqingqiongView::OnAngle)
END_MESSAGE_MAP()

// ChanqingqiongView 构造/析构

ChanqingqiongView::ChanqingqiongView() noexcept
{
	// TODO: 在此处添加构造代码
	this->m_GLPixelIndex = 0;
	this->m_hGLContext = NULL;

	m_xrotation = 0.0f;
	m_yrotation = 90.0f;
	m_zrotation = 0.0f;

	m_xrotation_begin = 0.0f;
	m_yrotation_begin = 90.0f;
	m_zrotation_begin = 0.0f;

	m_xrotation_end = 90.0f;
	m_yrotation_end = 45.0f;
	m_zrotation_end = 90.0f;

	t = 0.0f;

	interpolation_mode = 1;

}

ChanqingqiongView::~ChanqingqiongView()
{
}

BOOL ChanqingqiongView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	return CView::PreCreateWindow(cs);
}

// ChanqingqiongView 绘图

void ChanqingqiongView::OnDraw(CDC* pDC)
{
	ChanqingqiongDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(m_zrotation, 0.0, 0.0, 1.0);
	glRotatef(m_yrotation, 0.0, 1.0, 0.0);
	glRotatef(m_xrotation, 1.0, 0.0, 0.0);

	//////////////////////////////////////////////
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	float mat_ambient[] = { 0.1f,0.1f,0.1f,1.0f };
	float mat_diffuse[] = { 0.5f,0.3f,0.1f,1.0f };
	float mat_specular[] = { 0.9f,0.9f,0.9f,1.0f };
	float mat_emission[] = { 0.5f,0,0,1.0f };
	float mat_shiness[] = { 30.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

	glColor3f(1.0, 0.0, 0.0);
	auxSolidTeapot(0.5);


	SwapBuffers(pDC->m_hDC);


}


// ChanqingqiongView 打印

BOOL ChanqingqiongView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void ChanqingqiongView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void ChanqingqiongView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// ChanqingqiongView 诊断

#ifdef _DEBUG
void ChanqingqiongView::AssertValid() const
{
	CView::AssertValid();
}

void ChanqingqiongView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

ChanqingqiongDoc* ChanqingqiongView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ChanqingqiongDoc)));
	return (ChanqingqiongDoc*)m_pDocument;
}
#endif //_DEBUG


// ChanqingqiongView 消息处理程序
BOOL ChanqingqiongView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR  pixelDesc = { sizeof(PIXELFORMATDESCRIPTOR), 1,
																			   PFD_DRAW_TO_WINDOW |
																			   PFD_SUPPORT_OPENGL |
																			   PFD_DOUBLEBUFFER,
																			   PFD_TYPE_RGBA,   24,  0,0,0,0,0,0,  0,  0,  0,
																			   0,0,0,0,  32,   0,  0, 0,  0,  0,0,0 };
	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);

	if (m_GLPixelIndex == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL ChanqingqiongView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL) //创建失败
	{
		return FALSE;
	}

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
	{//选为当前RC失败
		return FALSE;
	}
	return TRUE;
}

int ChanqingqiongView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	HWND hWnd = this->GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if (this->SetWindowPixelFormat(hDC) == FALSE)
	{
		return 0;
	}

	if (this->CreateViewGLContext(hDC) == FALSE)
	{
		return 0;
	}

	glewInit();
	glDrawBuffer(GL_BACK);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	///////////////////
	////////////////////////////////////////
	//如果光照定义放在这里，光源位置相对视点固定
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float light_ambient[] = { 0.1f,0.1f,0.1f,1.0f };
	float light_diffuse[] = { 1,1,1,1 };
	float light_specular[] = { 1,1,1,1 };
	float light_position[] = { 1,1,1,0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	////////////////////////////////////////

	SetTimer(0, 1, NULL);

	return 0;
}

void ChanqingqiongView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	if (wglGetCurrentContext() != NULL)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (this->m_hGLContext != NULL)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}
}

void ChanqingqiongView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	glViewport(0, 0, cx, cy);
	GLdouble aspect;
	if (cy == 0)
	{
		aspect = (GLdouble)cx;
	}
	else
	{
		aspect = (GLdouble)cx / (GLdouble)cy;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0 * aspect, 2.0 * aspect, -2.0, 2.0, -13.0, 13.0);
	Invalidate();

}

BOOL ChanqingqiongView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);
	return true;
}

void ChanqingqiongView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (interpolation_mode == 1)
	{
		if (m_xrotation <= m_xrotation_end && m_yrotation >= m_yrotation_end && m_zrotation <= m_zrotation_end)
		{
			m_xrotation += 0.005f * (m_xrotation_end - m_xrotation_begin);
			m_yrotation += 0.005f * (m_yrotation_end - m_yrotation_begin);
			m_zrotation += 0.005f * (m_zrotation_end - m_zrotation_begin);
		}
	}

	else if (interpolation_mode == 2)
	{
		///////////////////////////////////////////////////////////////////////四元数插值
		Quaternion_begin.SetEulerAngle(m_xrotation_begin, m_yrotation_begin, m_zrotation_begin);
		Quaternion_end.SetEulerAngle(m_xrotation_end, m_yrotation_end, m_zrotation_end);

		float temp = Quaternion_begin.qw * Quaternion_end.qw + Quaternion_begin.qx * Quaternion_end.qx + Quaternion_begin.qy * Quaternion_end.qy + Quaternion_begin.qz * Quaternion_end.qz;
		if (temp < 0)
		{
			Quaternion_end.qw *= -1;
			Quaternion_end.qx *= -1;
			Quaternion_end.qy *= -1;
			Quaternion_end.qz *= -1;
		}

		if (t < 1.0f)
			t += 0.005f;

		Quaternion_middle.qw = (1 - t) * Quaternion_begin.qw + t * Quaternion_end.qw;
		Quaternion_middle.qx = (1 - t) * Quaternion_begin.qx + t * Quaternion_end.qx;
		Quaternion_middle.qy = (1 - t) * Quaternion_begin.qy + t * Quaternion_end.qy;
		Quaternion_middle.qz = (1 - t) * Quaternion_begin.qz + t * Quaternion_end.qz;

		// 归一化：模长必须先算好再除，不能边除边重算（qw 除过之后模长就变了）
		float norm = (float)sqrt(Quaternion_middle.qw * Quaternion_middle.qw + Quaternion_middle.qx * Quaternion_middle.qx + Quaternion_middle.qy * Quaternion_middle.qy + Quaternion_middle.qz * Quaternion_middle.qz);
		Quaternion_middle.qw /= norm;
		Quaternion_middle.qx /= norm;
		Quaternion_middle.qy /= norm;
		Quaternion_middle.qz /= norm;

		Quaternion_middle.GetEulerAngle(m_xrotation, m_yrotation, m_zrotation);
		/////////////////////////////////////////////////////////////////////////////////
	}

	/////////////////////////////////////////////////////////////////////////////////

	Invalidate(TRUE);

	CView::OnTimer(nIDEvent);
}

void ChanqingqiongView::OnQuaternion()
{
	// TODO: 在此添加命令处理程序代码
	t = 0;

	interpolation_mode = 2;

}

void ChanqingqiongView::OnAngle()
{
	// TODO: 在此添加命令处理程序代码
	m_xrotation = m_xrotation_begin;
	m_yrotation = m_yrotation_begin;
	m_zrotation = m_zrotation_begin;

	interpolation_mode = 1;

}
