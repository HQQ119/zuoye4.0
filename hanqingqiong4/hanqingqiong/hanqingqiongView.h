
// hanqingqiongView.h: ChanqingqiongView 类的接口
//
#include "Quaternion.h"

#pragma once


class ChanqingqiongView : public CView
{
protected: // 仅从序列化创建
	ChanqingqiongView() noexcept;
	DECLARE_DYNCREATE(ChanqingqiongView)

	int m_GLPixelIndex;
	HGLRC m_hGLContext;

// 特性
public:
	ChanqingqiongDoc* GetDocument() const;

// 操作
public:
	float m_xrotation;
	float m_yrotation;
	float m_zrotation;
	float m_xrotation_begin;
	float m_yrotation_begin;
	float m_zrotation_begin;
	float m_xrotation_end;
	float m_yrotation_end;
	float m_zrotation_end;
	Quaternion Quaternion_begin;
	Quaternion Quaternion_end;
	Quaternion Quaternion_middle;
	float t;
	int interpolation_mode;   // 1=欧拉角插值  2=四元数插值


// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);

// 实现
public:
	virtual ~ChanqingqiongView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnQuaternion();
	afx_msg void OnAngle();
};

#ifndef _DEBUG  // hanqingqiongView.cpp 中的调试版本
inline ChanqingqiongDoc* ChanqingqiongView::GetDocument() const
   { return reinterpret_cast<ChanqingqiongDoc*>(m_pDocument); }
#endif

