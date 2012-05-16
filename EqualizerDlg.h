#if !defined(AFX_EQUALIZERDLG_H__041932D3_0F87_4D57_8B63_2B6945EC0EBD__INCLUDED_)
#define AFX_EQUALIZERDLG_H__041932D3_0F87_4D57_8B63_2B6945EC0EBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqualizerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEqualizerDlg dialog
class CManateeDlg;
class CEqualizerDlg : public CDialog
{
// Construction
public:
	CEqualizerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEqualizerDlg)
	enum { IDD = IDD_EQUALIZER };
	int		m_Slider0;
  int		m_Slider1;
  int		m_Slider2;
  int		m_Slider3;
  int		m_Slider4;
  int		m_Slider5;
  int		m_Slider6;
  int		m_Slider7;
  int		m_Slider8;
  int		m_Slider9;
  int		m_Slider10;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqualizerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
protected:
  CManateeDlg* m_pManateeDlg;
  BOOL m_bEnable;
public:
  enum {NONE=0, LEFT, TOP, RIGHT, BOTTOM} m_Attached;
public:
  void AttachWindow();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEqualizerDlg)
	virtual BOOL OnInitDialog();
  virtual void OnCancel();
  afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBtnReset();
	afx_msg void OnChkEnable();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUALIZERDLG_H__041932D3_0F87_4D57_8B63_2B6945EC0EBD__INCLUDED_)
