
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "KitchenWnd.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CKitchenWnd  mWorkshop;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

private:
    void Initialize();
	void SetScale(double scale);

public:
	afx_msg void OnScale1x();
	afx_msg void OnUpdateScale1x(CCmdUI *pCmdUI);
	afx_msg void OnScale2x();
	afx_msg void OnUpdateScale2x(CCmdUI *pCmdUI);
};


