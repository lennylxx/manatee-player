#if !defined(AFX_LYRICDLG_H__2F2D8F05_3E74_479A_B328_460EB839FAC0__INCLUDED_)
#define AFX_LYRICDLG_H__2F2D8F05_3E74_479A_B328_460EB839FAC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LyricDlg.h : header file
//

#include "Lrc.h"
/////////////////////////////////////////////////////////////////////////////
// CLyricDlg dialog
class CManateeDlg;
class CLrc;
class CLyricDlg : public CDialog
{
// Construction
public:
  CLyricDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CLyricDlg)
  enum { IDD = IDD_LYRIC };
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CLyricDlg)
  public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL
public:
  enum {NONE=0, LEFT, TOP, RIGHT, BOTTOM} m_Attached;
  CLrc m_Lrc;
  CFont m_Font;
  int m_nPaintPos;
public:
  void AttachWindow();
  void InvalidateCtrl();
  BOOL LoadLyric();
  void CountPos();
// Implementation
protected:
  CManateeDlg* m_pManateeDlg;
  // Generated message map functions
  //{{AFX_MSG(CLyricDlg)
  virtual BOOL OnInitDialog();
  virtual void OnCancel();
  afx_msg void OnDestroy();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnMove(int x, int y);
  afx_msg void OnPaint();
  afx_msg void OnTimer(UINT nIDEvent);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LYRICDLG_H__2F2D8F05_3E74_479A_B328_460EB839FAC0__INCLUDED_)
