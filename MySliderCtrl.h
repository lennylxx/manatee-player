#if !defined(AFX_MYSLIDERCTRL_H__20602712_6EB2_4321_BB87_E478DAD936B2__INCLUDED_)
#define AFX_MYSLIDERCTRL_H__20602712_6EB2_4321_BB87_E478DAD936B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySliderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySliderCtrl window

class CMySliderCtrl : public CSliderCtrl
{
  DECLARE_DYNAMIC(CMySliderCtrl) 
// Construction
public:
	CMySliderCtrl();

private: 
	bool SetThumb(const CPoint& pt); 
	void PostMessageToParent(const int nTBCode) const; 
	bool m_bDragging; 
	bool m_bDragChanged;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySliderCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSLIDERCTRL_H__20602712_6EB2_4321_BB87_E478DAD936B2__INCLUDED_)
