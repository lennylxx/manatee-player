// ManateeDlg.h : header file
//

#if !defined(AFX_MANATEEDLG_H__93C1ECAE_A37F_4FA0_A593_AD5CE6613E8F__INCLUDED_)
#define AFX_MANATEEDLG_H__93C1ECAE_A37F_4FA0_A593_AD5CE6613E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySliderCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CManateeDlg dialog
class CPlayListDlg;
class CLyricDlg;
class CEqualizerDlg;
class CManateeDlg : public CDialog
{
// Construction
public:
	CManateeDlg(CWnd* pParent = NULL);	// standard constructor
  
// Dialog Data
	//{{AFX_DATA(CManateeDlg)
	enum { IDD = IDD_MANATEE };
	CMySliderCtrl	m_SliderVolume;
	CMySliderCtrl	m_SliderTime;
	CButton	m_ButtonPlay;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManateeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
  
public:
  UINT m_nCurTime;//in millisecond
  UINT m_nTotalTime;//in millisecond
  BOOL m_bPlay;//play or pause
  BOOL m_bStop;//play or stop
  ZPlay *m_Player;
  CPlayListDlg* m_pPlayListDlg;
  CLyricDlg* m_pLyricDlg;
  CEqualizerDlg* m_pEqualizerDlg;
protected:
  HICON  m_hIcon;
  HACCEL m_hAccel;
  NOTIFYICONDATA m_NotifyIcon;
  CToolTipCtrl m_ToolTip;
public:
  void Play(CString strFileName);
  void Stop();
  BOOL m_bListShow;
  BOOL m_bLyricShow;
  BOOL m_bEqualizerShow;
  BYTE m_nVolume;
  //config struct
#pragma pack(1)
  typedef struct{
    BYTE ListShow;
    BYTE LyricShow;
    BYTE EqualizerShow;
    BYTE Volume;
    BYTE PlayMode;
    int  NowPlay;
    int  PosX;
    int  PosY;
  }CONFIG;
protected:
  void CountTime();
  void CreateNotifyIcon();
  void ModifyNotifyIcon(CString strTip);
  void DeleteNotifyIcon();
  void LoadConfigFile();
  void SaveConfigFile();
protected:
	// Generated message map functions
	//{{AFX_MSG(CManateeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
  afx_msg void OnBtnHelp();
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnNext();
	afx_msg void OnBtnPrev();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnShowPlayList();
  afx_msg void OnBtnShowLyric();
  afx_msg void OnBtnShowEq();
	afx_msg void OnMenuAbout();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnNotifyIcon(WPARAM wParam,LPARAM lParam);
  afx_msg void OnMenuShowWindow();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANATEEDLG_H__93C1ECAE_A37F_4FA0_A593_AD5CE6613E8F__INCLUDED_)
