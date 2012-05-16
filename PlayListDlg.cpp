// PlayListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manatee.h"
#include "ManateeDlg.h"
#include "PlayListDlg.h"
#include "FileID3.h"
#include "StdioFileEx.h"
#include "Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString m_strListName = L"MyList.mpl";
CString m_strFilter = 
L"Audio Files (*.mp3;*.mp2;*.mp1;*.ogg;*.flac;*.oga*.ac3;*.aac;*.wav)|*.mp3;*.mp2;*.mp1;*.ogg;*.flac;*.oga*.ac3;*.aac;*.wav|\
MPEG-1 or MPEG-2 Audio Layer-3 (*.mp3)|*.mp3|\
MPEG-1 Audio Layer-2 (*.mp2)|*.mp2|\
MPEG-1 Audio Layer-1 (*.mp1)|*.mp1|\
OGG Vorbis (*.ogg)|*.ogg|\
OGG FLAC (*.flac;*.oga)|*.flac;*.oga|\
ATSC A/52 streams (*.ac3)|*.ac3|\
AAC (*.aac)|*.aac|\
Wave Files (*.wav)|*.wav|\
All Files (*.*)|*.*||";

/////////////////////////////////////////////////////////////////////////////
// CPlayListDlg dialog

CPlayListDlg::CPlayListDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CPlayListDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPlayListDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  m_pManateeDlg = (CManateeDlg *)pParent;
  m_pFileID3Dlg = NULL;
}


void CPlayListDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPlayListDlg)
    DDX_Control(pDX, IDC_LST_PLAYLIST, m_PlayList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayListDlg, CDialog)
  //{{AFX_MSG_MAP(CPlayListDlg)
  ON_WM_DESTROY()
  ON_NOTIFY(NM_CUSTOMDRAW, IDC_LST_PLAYLIST, OnListCustomDraw)
  ON_NOTIFY(NM_DBLCLK, IDC_LST_PLAYLIST, OnDblclkLstPlaylist)
  ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
  ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
  ON_BN_CLICKED(IDC_BTN_PLAYMODE, OnBtnPlaymode)
  ON_COMMAND(IDM_ADD_FILE, OnMenuAddFile)
  ON_COMMAND(IDM_ADD_FOLDER, OnMenuAddFolder)
  ON_COMMAND(IDM_DELETE_SELECTED, OnMenuDeleteSelected)
  ON_COMMAND(IDM_DELETE_ALL, OnMenuDeleteAll)
  ON_COMMAND(IDM_NORMAL, OnMenuNormal)
  ON_COMMAND(IDM_RANDOM, OnMenuRandom)
  ON_COMMAND(IDM_REPEAT_ALL, OnMenuRepeatAll)
  ON_COMMAND(IDM_REPEAT_TRACK, OnMenuRepeatTrack)
  ON_COMMAND(IDM_SHUFFLE, OnMenuShuffle)
  ON_WM_SYSCOMMAND()
  ON_WM_MOVE()
  ON_NOTIFY(NM_RCLICK, IDC_LST_PLAYLIST, OnRclickLstPlaylist)
  ON_COMMAND(IDM_FILE_ID3, OnMenuFileAttrib)
  ON_WM_DROPFILES()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayListDlg message handlers

BOOL CPlayListDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  m_PlayList.SetExtendedStyle(LVS_EX_INFOTIP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
  m_PlayList.InsertColumn(0,L"No.",LVCFMT_LEFT,35);
  m_PlayList.InsertColumn(1,L"FileName",LVCFMT_LEFT,295);
  m_PlayList.InsertColumn(2,L"Type",LVCFMT_CENTER,40);
  
  //Initial
  m_nNowPlay = 0;
  m_nTotalMusic = 0;
  m_nShuffleIdx = 0;
  m_PlayMode = NORMAL;
  m_Attached = BOTTOM;
  m_bFileID3Show = FALSE;

  m_pStaticMode = m_pManateeDlg->GetDlgItem(IDC_STT_PLAYMODE);

  LoadPlayList();
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayListDlg::OnCancel()
{
  DestroyWindow();
}

void CPlayListDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  delete this;
}

void CPlayListDlg::OnDestroy() 
{
  CDialog::OnDestroy();
  SavePlayList();  
}

void CPlayListDlg::AttachWindow()
{
  CRect rectParent;
  CRect rect;
  m_pManateeDlg->GetWindowRect(&rectParent);
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

void CPlayListDlg::OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
  NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
  
  if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
  {
    *pResult = CDRF_NOTIFYITEMDRAW;
  }
  else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
  {
    *pResult = CDRF_NOTIFYSUBITEMDRAW;
  }
  else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
  {
    COLORREF clrNewTextColor, clrNewBkColor;
    int item = static_cast<int>( pLVCD->nmcd.dwItemSpec );
    
    POSITION pos = m_PlayList.GetFirstSelectedItemPosition();
    int index = m_PlayList.GetNextSelectedItem(pos);
    
    if (index == item)
    {
      clrNewTextColor = RGB(255,255,255);        //Set the text to white
      clrNewBkColor = RGB(51,153,255);        //Set the background color to blue
    }
    else
    {
      clrNewTextColor = RGB(0,0,0);        //set the text black
      clrNewBkColor = pLVCD->clrTextBk;    //leave the background color default(maybe white);
    }
    
    pLVCD->clrText = clrNewTextColor;
    pLVCD->clrTextBk = clrNewBkColor;

    *pResult = CDRF_DODEFAULT;
  }
}

void CPlayListDlg::OnDblclkLstPlaylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
  HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
  if(phdn->iItem != -1)
  {
    m_pManateeDlg->Stop();
    m_nNowPlay = phdn->iItem + 1;
    m_pManateeDlg->Play(m_strPlayListArray.GetAt(phdn->iItem));
  }
  *pResult = 0;
}

void CPlayListDlg::LoadPlayList()
{
  m_strPlayListArray.RemoveAll();
  m_PlayList.DeleteAllItems();
  
  CStdioFileEx ListFile;
  if(ListFile.Open(m_strListName, CFile::typeText|CFile::modeRead))
  {
    CString strTemp, strFormat;
    while(ListFile.ReadString(strTemp))
    {
      m_strPlayListArray.Add(strTemp);
      strFormat.Format(L"%d", ++m_nTotalMusic);
      m_PlayList.InsertItem(m_nTotalMusic-1, strFormat);
      m_PlayList.SetItemText(m_nTotalMusic-1, 1, GetFileTitle(strTemp));
      m_PlayList.SetItemText(m_nTotalMusic-1, 2, GetExtension(strTemp));
    }
    ShuffleSequence();
  }
  else ListFile.Open(m_strListName, CFile::typeText|CFile::modeCreate);
  
  ListFile.Close();
}

void CPlayListDlg::ShuffleSequence()
{
  m_nShuffleIdx = 0;
  m_PlaySequence.RemoveAll();
  while(1)
  {
    if(m_nTotalMusic == 0) break;
    int nNowPlay = rand()%m_nTotalMusic+1;
    BOOL bIsInserted = FALSE;
    for(int i=0; i<m_PlaySequence.GetSize(); i++) 
    {
      if(nNowPlay == m_PlaySequence.GetAt(i))
      {
        bIsInserted = TRUE;
        break;
      }
    }
    if(bIsInserted == FALSE)
      m_PlaySequence.Add(nNowPlay);
    if(m_nTotalMusic == m_PlaySequence.GetSize())
      break;
  }
}
void CPlayListDlg::SavePlayList()
{
  CStdioFileEx ListFile(m_strListName, CFile::modeCreate|CFile::modeReadWrite|CStdioFileEx::modeWriteUnicode);

  CString strTemp;
  for(int i=0; i<m_strPlayListArray.GetSize(); i++)
  {
    strTemp = m_strPlayListArray.GetAt(i);
    ListFile.WriteString(strTemp);
    ListFile.WriteString(L"\r\n");
  }
  ListFile.Close();
}

void CPlayListDlg::OnBtnOpen() 
{ 
  CFileDialog fdlg(TRUE, NULL, NULL, 
    OFN_EXPLORER|OFN_HIDEREADONLY|OFN_READONLY|OFN_EXPLORER|OFN_NOCHANGEDIR
    |OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, m_strFilter);
  fdlg.m_ofn.lStructSize = 88;
  fdlg.m_ofn.lpstrTitle = L"Open File";
  if(IDOK == fdlg.DoModal())
  {
    CString strFileName = fdlg.GetPathName();
    m_strPlayListArray.RemoveAll();
    m_PlaySequence.RemoveAll();
    m_PlaySequence.Add(1);
    m_PlayList.DeleteAllItems();
    m_PlayList.RedrawWindow();
    m_strPlayListArray.Add(strFileName);
    m_nTotalMusic = 1;
    m_nNowPlay = 1;
    m_nShuffleIdx = 0;
    CString strFormat;
    strFormat.Format(L"%d", 1);
    m_PlayList.InsertItem(0, strFormat);
    m_PlayList.SetItemText(0, 1, GetFileTitle(strFileName));
    m_PlayList.SetItemText(0, 2, GetExtension(strFileName));
    m_pManateeDlg->Stop();
    m_pManateeDlg->Play(strFileName);
  }
}

void CPlayListDlg::OnBtnAdd() 
{
  RECT rect;
  GetDlgItem(IDC_BTN_ADD)->GetWindowRect(&rect);
  CMenu menu;
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_STRING, IDM_ADD_FILE, L"Add &File(s)");
  menu.AppendMenu(MF_STRING, IDM_ADD_FOLDER, L"Add Fol&der");

  menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left, rect.bottom, this);
  menu.DestroyMenu();
}

void CPlayListDlg::DeleteFromList()
{
  int nIndex = m_PlayList.GetSelectionMark();
  if(nIndex == -1) return;
  m_PlayList.DeleteItem(nIndex);
  m_strPlayListArray.RemoveAt(nIndex);
  for(int i=0; i<m_PlayList.GetItemCount(); i++)
  {
    CString str;
    str.Format(L"%d", i+1);
    m_PlayList.SetItemText(i, 0, str);
  }
  m_nTotalMusic--;
  ShuffleSequence();
}

void CPlayListDlg::OnMenuDeleteSelected() 
{
  POSITION sSelPos = NULL;
  while(sSelPos = m_PlayList.GetFirstSelectedItemPosition())
  {
    int nSelItem = m_PlayList.GetNextSelectedItem(sSelPos);
    if(nSelItem >= 0 && nSelItem<m_PlayList.GetItemCount())
    {
      m_PlayList.DeleteItem(nSelItem);
      m_strPlayListArray.RemoveAt(nSelItem);
      m_nTotalMusic--;
    }
  }
  for(int i=0; i<m_PlayList.GetItemCount(); i++)
  {
    CString str;
    str.Format(L"%d", i+1);
    m_PlayList.SetItemText(i, 0, str);
  }
  ShuffleSequence();
}

void CPlayListDlg::OnMenuDeleteAll()
{
  if(m_nTotalMusic!=0)
  {
    m_strPlayListArray.RemoveAll();
    m_PlayList.DeleteAllItems();
    m_PlayList.RedrawWindow();
    m_nTotalMusic = 0;
    ShuffleSequence();
  }
}

void CPlayListDlg::OnBtnPlaymode() 
{
  RECT rect;
  GetDlgItem(IDC_BTN_PLAYMODE)->GetWindowRect(&rect);

  CMenu menu;
  menu.CreatePopupMenu();
  
  menu.AppendMenu(MF_STRING, IDM_NORMAL, L"Normal");
  menu.AppendMenu(MF_STRING, IDM_RANDOM, L"Random");
  menu.AppendMenu(MF_STRING, IDM_REPEAT_ALL, L"Repeat All");
  menu.AppendMenu(MF_STRING, IDM_REPEAT_TRACK, L"Repeat Track");
  menu.AppendMenu(MF_STRING, IDM_SHUFFLE, L"Shuffle");

  switch(m_PlayMode)
  {
  case NORMAL:
    menu.CheckMenuItem(IDM_NORMAL, MF_CHECKED);
    break;
  case RANDOM:
    menu.CheckMenuItem(IDM_RANDOM, MF_CHECKED);
    break;
  case REPEATALL:
    menu.CheckMenuItem(IDM_REPEAT_ALL, MF_CHECKED);
    break;
  case REPEATTRACK:
    menu.CheckMenuItem(IDM_REPEAT_TRACK, MF_CHECKED);
    break;
  case SHUFFLE:
    menu.CheckMenuItem(IDM_SHUFFLE, MF_CHECKED);
    break;
  }
  
  menu.TrackPopupMenu(TPM_LEFTALIGN,rect.left, rect.bottom, this);
  menu.DestroyMenu();
}

void CPlayListDlg::OnMenuAddFile() 
{
  CFileDialog fdlg(TRUE, NULL, NULL, 
    OFN_EXPLORER|OFN_HIDEREADONLY|OFN_READONLY|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
    |OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR, m_strFilter, this);
  fdlg.m_ofn.lStructSize = 88;
  fdlg.m_ofn.lpstrTitle = L"Add File(s)";
  if(IDOK == fdlg.DoModal())
  {
    POSITION pos_file = fdlg.GetStartPosition();
    while(pos_file != NULL)
    {
      CString strFileName = fdlg.GetNextPathName(pos_file);
      if(!IsSupportFile(strFileName)) continue;
      m_strPlayListArray.Add(strFileName);
      CString strFormat;
      strFormat.Format(L"%d", ++m_nTotalMusic);
      m_PlayList.InsertItem(m_nTotalMusic-1, strFormat);
      m_PlayList.SetItemText(m_nTotalMusic-1, 1, GetFileTitle(strFileName));
      m_PlayList.SetItemText(m_nTotalMusic-1, 2, GetExtension(strFileName));
      m_PlayList.EnsureVisible(m_nTotalMusic-1, FALSE);
    }
    ShuffleSequence();
  }
}

void CPlayListDlg::OnMenuAddFolder() 
{
  TCHAR szDir[MAX_PATH];
  BROWSEINFO   bi; 
  bi.hwndOwner      = this->m_hWnd; 
  bi.pidlRoot       = NULL; 
  bi.pszDisplayName = szDir; 
  bi.lpszTitle      = L"Select a Folder"; 
  bi.ulFlags        = BIF_RETURNONLYFSDIRS; 
  bi.lpfn           = NULL; 
  bi.lParam         = 0; 
  bi.iImage         = 0; 
  
  ZeroMemory(szDir, MAX_PATH);
  LPITEMIDLIST lpItemId = ::SHBrowseForFolder(&bi);
  if(lpItemId)
  {
    ::SHGetPathFromIDList(lpItemId, szDir);
    ::GlobalFree(lpItemId);
  }

  ListFile(szDir);
  ShuffleSequence();
}

void CPlayListDlg::OnMenuNormal() 
{
  m_PlayMode = NORMAL;
  SetModeText();
}

void CPlayListDlg::OnMenuRandom() 
{
  m_PlayMode = RANDOM;
  SetModeText();
}

void CPlayListDlg::OnMenuRepeatAll() 
{
  m_PlayMode = REPEATALL;
  SetModeText();
}

void CPlayListDlg::OnMenuRepeatTrack() 
{
  m_PlayMode = REPEATTRACK;
  SetModeText();
}

void CPlayListDlg::OnMenuShuffle() 
{
  m_PlayMode = SHUFFLE;
  SetModeText();
}

void CPlayListDlg::SetModeText()
{
  TCHAR *modeText;
  switch(m_PlayMode)
  {
  case NORMAL:
    modeText = L"Normal"; break;
  case RANDOM:
    modeText = L"Random"; break;
  case REPEATALL:
    modeText = L"Repeat All"; break;
  case REPEATTRACK:
    modeText = L"Repeat Track"; break;
  case SHUFFLE:
    modeText = L"Shuffle"; break;
  }
  m_pStaticMode->SetWindowText(modeText);
}

void CPlayListDlg::ListFile(CString strPath)
{
  if(strPath.IsEmpty()) return;
  WIN32_FIND_DATA FindFileData;
  CString strCur = strPath + L"\\*.*";
  HANDLE hFind = FindFirstFile(strCur, &FindFileData);

  if(hFind == INVALID_HANDLE_VALUE) return;
  while(FindNextFile(hFind, &FindFileData))
  {
    if(lstrcmp(FindFileData.cFileName, L".") == 0
      || lstrcmp(FindFileData.cFileName, L"..") == 0)
      continue;
    
    if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
      strCur = strPath + L"\\" + FindFileData.cFileName;
      ListFile(strCur);//Deep First Search
    }
    else
    {
      if(!IsSupportFile(FindFileData.cFileName)) continue;
      CString strFormat;
      m_strPlayListArray.Add(strPath + L"\\" + FindFileData.cFileName);
      strFormat.Format(L"%d", ++m_nTotalMusic);
      m_PlayList.InsertItem(m_nTotalMusic-1, strFormat);
      m_PlayList.SetItemText(m_nTotalMusic-1, 1, GetFileTitle(FindFileData.cFileName));
      m_PlayList.SetItemText(m_nTotalMusic-1, 2, GetExtension(FindFileData.cFileName));
    }
  }
  FindClose(hFind);
}

CString CPlayListDlg::GetNextTrack()
{
  m_PlayList.SetSelectionMark(-1);
  m_PlayList.SetItemState(m_nNowPlay-1, 0, -1);
  switch(m_PlayMode)
  {
  case NORMAL:
    if(m_bIsNext)
    { if(m_nNowPlay!=0)  m_nNowPlay++;}
    else
      m_nNowPlay--;
    if(m_nNowPlay>m_nTotalMusic || m_nNowPlay<0)
      m_nNowPlay = 0;
    break;
  case RANDOM:
    m_nNowPlay = rand()%m_nTotalMusic+1;
    break;
  case REPEATALL:
    if(m_bIsNext)
      m_nNowPlay++;
    else
      m_nNowPlay--;
    if(m_nNowPlay>m_nTotalMusic) m_nNowPlay = 1;
    if(m_nNowPlay<=0) m_nNowPlay = m_nTotalMusic;
    break; 
  case REPEATTRACK:
    //do nothing
    break;
  case SHUFFLE:
    if(m_bIsNext)
      m_nShuffleIdx++;
    else
      m_nShuffleIdx--;
    if(m_nShuffleIdx>=m_nTotalMusic) m_nShuffleIdx = 0;
    if(m_nShuffleIdx<0) m_nShuffleIdx = m_nTotalMusic-1;
    if(m_PlaySequence.GetSize() != 0)
      m_nNowPlay = m_PlaySequence.GetAt(m_nShuffleIdx);
    else m_nNowPlay = 0;
    break;
  }

  CString strFileName;
  if(m_nNowPlay!=0)
  {
    m_PlayList.SetSelectionMark(m_nNowPlay-1);
    m_PlayList.SetItemState(m_nNowPlay-1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED); 
    m_PlayList.EnsureVisible(m_nNowPlay-1, FALSE);
    strFileName = m_strPlayListArray.GetAt(m_nNowPlay-1);
  }
  else strFileName = L"";

  return strFileName;
}

void CPlayListDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
  {
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_bListShow = FALSE;
  }
  else 
    CDialog::OnSysCommand(nID, lParam);
}

void CPlayListDlg::OnMove(int x, int y) 
{
  CDialog::OnMove(x, y);
  
  if(m_pFileID3Dlg)
  {
    if(m_bFileID3Show && m_pFileID3Dlg->m_Attached)
      m_pFileID3Dlg->AttachWindow();
  }

  RECT rectParent;
  RECT rect;
  m_pManateeDlg->GetWindowRect(&rectParent);
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

void CPlayListDlg::OnRclickLstPlaylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
  POINT point;
  GetCursorPos(&point);

  CMenu menu;
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_STRING, IDM_FILE_ID3, L"File ID3 Tag");
  menu.AppendMenu(MF_SEPARATOR,0,L"");
  menu.AppendMenu(MF_STRING, IDM_DELETE_SELECTED, L"Delete Selected File(s)");
  menu.AppendMenu(MF_STRING, IDM_DELETE_ALL, L"Delete All");
  menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
  menu.DestroyMenu();

  *pResult = 0;
}

void CPlayListDlg::OnMenuFileAttrib() 
{
  int nItem = m_PlayList.GetSelectionMark();
  if(nItem == -1) return;

  if(m_pFileID3Dlg == NULL)
  {
    m_pFileID3Dlg = new CFileID3(this->m_pManateeDlg);
    m_pFileID3Dlg->Create(IDD_FILEID3);
  }

  m_pFileID3Dlg->ShowWindow(SW_SHOW);
  m_bFileID3Show = TRUE;
  m_pFileID3Dlg->AttachWindow();
  m_pFileID3Dlg->FillAttrib(nItem);
}

void CPlayListDlg::OnDropFiles(HDROP hDropInfo) 
{
  UINT nCount;
  TCHAR szFilePath[MAX_PATH];
  nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
  if(nCount)
  {
    for(UINT i=0; i<nCount; i++)
    {
      int nPathLen = DragQueryFile(hDropInfo, i, szFilePath, sizeof(szFilePath));
      if(!IsSupportFile(szFilePath)) continue;
      CString strFormat;
      m_strPlayListArray.Add(szFilePath);
      strFormat.Format(L"%d", ++m_nTotalMusic);
      m_PlayList.InsertItem(m_nTotalMusic-1, strFormat);
      m_PlayList.SetItemText(m_nTotalMusic-1, 1, GetFileTitle(szFilePath));
      m_PlayList.SetItemText(m_nTotalMusic-1, 2, GetExtension(szFilePath));
      m_PlayList.EnsureVisible(m_nTotalMusic-1, FALSE);
    }
    ShuffleSequence();
  }
  DragFinish(hDropInfo);  
  CDialog::OnDropFiles(hDropInfo);
}

BOOL CPlayListDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->wParam == VK_DELETE)//when 'delete' key is pressed
  {
    OnMenuDeleteSelected();
    return TRUE;
  }
  if(pMsg->wParam == VK_ESCAPE)
  { 
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_bListShow = FALSE;
    return TRUE;
  }
  else return CDialog::PreTranslateMessage(pMsg);
}
