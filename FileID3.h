#if !defined(AFX_FILEID3_H__858B1041_A391_489D_874D_9F9D7AC801A1__INCLUDED_)
#define AFX_FILEID3_H__858B1041_A391_489D_874D_9F9D7AC801A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileID3.h : header file
//

#include "ManateeDlg.h"
#include "PlayListDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFileID3 dialog

class CFileID3 : public CDialog
{
// Construction
public:
  CFileID3(CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CFileID3)
	enum { IDD = IDD_FILEID3 };
	CListCtrl	m_ListAttrib;
	//}}AFX_DATA

public:
  enum {NONE=0, LEFT, TOP, RIGHT, BOTTOM} m_Attached;
protected:
  CManateeDlg* m_pManateeDlg;
public:
  void AttachWindow();
  void FillAttrib(int nItem);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileID3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileID3)
	virtual BOOL OnInitDialog();
  virtual void OnCancel();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEID3_H__858B1041_A391_489D_874D_9F9D7AC801A1__INCLUDED_)
