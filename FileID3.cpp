// FileID3.cpp : implementation file
//

#include "stdafx.h"
#include "Manatee.h"
#include "FileID3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileID3 dialog

CFileID3::CFileID3(CWnd* pParent /*=NULL*/)
	: CDialog(CFileID3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileID3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_pManateeDlg = (CManateeDlg *)pParent;
}


void CFileID3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileID3)
	DDX_Control(pDX, IDC_LST_ATTRIB, m_ListAttrib);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileID3, CDialog)
	//{{AFX_MSG_MAP(CFileID3)
  ON_WM_SYSCOMMAND()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileID3 message handlers

BOOL CFileID3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  m_ListAttrib.SetExtendedStyle(LVS_EX_INFOTIP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
  m_ListAttrib.InsertColumn(0,L"Attribute",LVCFMT_LEFT,70);
  m_ListAttrib.InsertColumn(1,L"Value",LVCFMT_LEFT,200);

  //Initial
  m_Attached = RIGHT;

  m_ListAttrib.DeleteAllItems();
  m_ListAttrib.InsertItem(0, L"Title");
  m_ListAttrib.InsertItem(1, L"Artist");
  m_ListAttrib.InsertItem(2, L"Album");
  m_ListAttrib.InsertItem(3, L"Year");
  m_ListAttrib.InsertItem(4, L"Comment");
  m_ListAttrib.InsertItem(5, L"Genre");
  m_ListAttrib.InsertItem(6, L"Track");
  m_ListAttrib.InsertItem(7, L"Album Artist");
  m_ListAttrib.InsertItem(8, L"Composer");
  m_ListAttrib.InsertItem(9, L"Original");
  m_ListAttrib.InsertItem(10, L"Copyright");
  m_ListAttrib.InsertItem(11, L"URL");
  m_ListAttrib.InsertItem(12, L"Encoder");
  m_ListAttrib.InsertItem(13, L"Publisher");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileID3::OnCancel()
{
  DestroyWindow();
}

void CFileID3::PostNcDestroy()
{
  delete this;
  CDialog::PostNcDestroy();
}

void CFileID3::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
  {
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_pPlayListDlg->m_bFileID3Show = FALSE;
  }
  else 
    CDialog::OnSysCommand(nID, lParam);
}

void CFileID3::AttachWindow()
{
  CRect rectParent;
  CRect rect;
  m_pManateeDlg->m_pPlayListDlg->GetWindowRect(&rectParent);
  GetWindowRect(&rect);

  switch(m_Attached)
  {
  case BOTTOM:
    MoveWindow(rectParent.left, rectParent.bottom+5, 
      rect.Width(), rect.Height());
    break;
  case RIGHT:
    MoveWindow(rectParent.right+5, rectParent.top, 
      rect.Width(), rect.Height());
    break;
  case LEFT:
    MoveWindow(rectParent.left-rect.Width()-5, rectParent.top, 
      rect.Width(), rect.Height());
    break;
  case TOP:
    MoveWindow(rectParent.left, rectParent.top-rect.Height()-5,
      rect.Width(), rect.Height());
    break;
  default:
     break;
  }
}

void CFileID3::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
  RECT rectParent;
  RECT rect;
  m_pManateeDlg->m_pPlayListDlg->GetWindowRect(&rectParent);
  GetWindowRect(&rect);
  if(rect.top>=rectParent.bottom-5 && rect.top<=rectParent.bottom+15
    &&rect.left>=rectParent.left-10 && rect.left<=rectParent.left+10)
  {
    m_Attached = BOTTOM;
    AttachWindow();
  }
  else if(rect.left>=rectParent.right-5 && rect.left<=rectParent.right+15
    &&rect.top>=rectParent.top-10 && rect.top<=rectParent.top+10)
  {
    m_Attached = RIGHT;
    AttachWindow();
  }
  else if(rect.right>=rectParent.left-15 && rect.right<=rectParent.left+5
    &&rect.top>=rectParent.top-10 && rect.top<=rectParent.top+10)
  {
    m_Attached = LEFT;
    AttachWindow();
  }
  else if(rect.bottom>=rectParent.top-15 && rect.bottom<=rectParent.top+5
    &&rect.left>=rectParent.left-10 && rect.left<=rectParent.left+10)
  {
    m_Attached = TOP;
    AttachWindow();
  }
  else m_Attached = NONE;
}	

void CFileID3::FillAttrib(int nItem)
{
  // get extended ID3 info
  TID3InfoExW id3_info;
  if(m_pManateeDlg->m_Player->
    LoadFileID3ExW(m_pManateeDlg->m_pPlayListDlg->m_strPlayListArray.GetAt(nItem),sfAutodetect, &id3_info, 0))
  {
    m_ListAttrib.SetItemText(0, 1, id3_info.Title);
    m_ListAttrib.SetItemText(1, 1, id3_info.Artist);
    m_ListAttrib.SetItemText(2, 1, id3_info.Album);
    m_ListAttrib.SetItemText(3, 1, id3_info.Year);
    m_ListAttrib.SetItemText(4, 1, id3_info.Comment);
    m_ListAttrib.SetItemText(5, 1, id3_info.Genre);
    m_ListAttrib.SetItemText(6, 1, id3_info.TrackNum);
    m_ListAttrib.SetItemText(7, 1, id3_info.AlbumArtist);
    m_ListAttrib.SetItemText(8, 1, id3_info.Composer);
    m_ListAttrib.SetItemText(9, 1, id3_info.OriginalArtist);
    m_ListAttrib.SetItemText(10, 1, id3_info.Copyright);
    m_ListAttrib.SetItemText(11, 1, id3_info.URL);
    m_ListAttrib.SetItemText(12, 1, id3_info.Encoder);
    m_ListAttrib.SetItemText(13, 1, id3_info.Publisher);
  }
}
