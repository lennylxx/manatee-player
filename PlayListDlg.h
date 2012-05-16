#if !defined(AFX_PLAYLISTDLG_H__24B091A9_60B3_44BA_A81C_929CB206ABF2__INCLUDED_)
#define AFX_PLAYLISTDLG_H__24B091A9_60B3_44BA_A81C_929CB206ABF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayListDlg.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPlayListDlg dialog
class CManateeDlg;
class CFileID3;
class CPlayListDlg : public CDialog
{
// Construction
public:
  CPlayListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CPlayListDlg)
  enum { IDD = IDD_PLAYLIST };
  CListCtrl m_PlayList;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPlayListDlg)
  public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL

public:
  int  m_nNowPlay;
  int  m_nTotalMusic;
  BOOL m_bIsNext;
  int  m_nShuffleIdx;
  BOOL m_bFileID3Show;
  CArray <int, int> m_PlaySequence;
  enum {NONE=0, LEFT, TOP, RIGHT, BOTTOM} m_Attached;
  enum PlayMode{NORMAL, RANDOM, REPEATALL, REPEATTRACK, SHUFFLE} m_PlayMode;
  CStringArray m_strPlayListArray;
  CFileID3* m_pFileID3Dlg;
  CWnd* m_pStaticMode;
protected:
  CManateeDlg* m_pManateeDlg;
public:
  void AttachWindow();
  void DeleteFromList();
  CString GetNextTrack();
  void SetModeText();
protected:
  void LoadPlayList();
  void ShuffleSequence();
  void SavePlayList();
  void ListFile(CString strPath);
// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPlayListDlg)
  virtual BOOL OnInitDialog();
  virtual void OnCancel();
  afx_msg void OnDestroy();
  afx_msg void OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnDblclkLstPlaylist(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBtnOpen();
  afx_msg void OnBtnAdd();
  afx_msg void OnBtnPlaymode();
  afx_msg void OnMenuAddFile();
  afx_msg void OnMenuAddFolder();
  afx_msg void OnMenuDeleteSelected();
  afx_msg void OnMenuDeleteAll();
  afx_msg void OnMenuNormal();
  afx_msg void OnMenuRandom();
  afx_msg void OnMenuRepeatAll();
  afx_msg void OnMenuRepeatTrack();
  afx_msg void OnMenuShuffle();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnMove(int x, int y);
  afx_msg void OnRclickLstPlaylist(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnMenuFileAttrib();
  afx_msg void OnDropFiles(HDROP hDropInfo);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLISTDLG_H__24B091A9_60B3_44BA_A81C_929CB206ABF2__INCLUDED_)
