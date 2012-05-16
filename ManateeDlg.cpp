// ManateeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manatee.h"
#include "ManateeDlg.h"
#include "PlayListDlg.h"
#include "LyricDlg.h"
#include "EqualizerDlg.h"
#include "FileID3.h"
#include "Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_MAIN       1
#define WM_NOTIFYICON WM_USER+5
CString m_strConfigFileName = L"config";

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManateeDlg dialog

CManateeDlg::CManateeDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CManateeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CManateeDlg)
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_pPlayListDlg = NULL;
  m_pLyricDlg = NULL;
  m_pEqualizerDlg = NULL;
}

void CManateeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CManateeDlg)
  DDX_Control(pDX, IDC_SLD_VOLUME, m_SliderVolume);
  DDX_Control(pDX, IDC_SLD_TIME, m_SliderTime);
  DDX_Control(pDX, IDC_BTN_PLAY, m_ButtonPlay);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CManateeDlg, CDialog)
  //{{AFX_MSG_MAP(CManateeDlg)
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
  ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
  ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
  ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
  ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
  ON_WM_TIMER()
  ON_WM_HSCROLL()
  ON_BN_CLICKED(IDC_BTN_SHOWPLAYLIST, OnBtnShowPlayList)
  ON_BN_CLICKED(IDC_BTN_SHOWLYRIC, OnBtnShowLyric)
  ON_BN_CLICKED(IDC_BTN_SHOWEQ, OnBtnShowEq)
  ON_COMMAND(IDM_ABOUT, OnMenuAbout)
  ON_WM_SYSCOMMAND()
  ON_MESSAGE(WM_NOTIFYICON, OnNotifyIcon)
  ON_COMMAND(IDM_SHOW_WINDOW, OnMenuShowWindow)
  ON_WM_MOVE()
  ON_WM_DROPFILES()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManateeDlg message handlers

BOOL CManateeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);      // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon
  
  m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),
    MAKEINTRESOURCE(IDR_ACCELERATOR));

  m_ToolTip.Create(this, TTS_ALWAYSTIP|TTS_NOPREFIX);
  m_ToolTip.AddTool(GetDlgItem(IDC_BTN_NEXT), L"Ctrl+¡ú");
  m_ToolTip.AddTool(GetDlgItem(IDC_BTN_PREV), L"Ctrl+¡û");
  m_ToolTip.AddTool(GetDlgItem(IDC_BTN_PLAY), L"Space");

  GetDlgItem(IDC_STT_TRACKNAME)->ModifyStyle(0, SS_ENDELLIPSIS);
  m_SliderTime.EnableWindow(FALSE);
  m_SliderVolume.SetRange(0, 100);

  CreateNotifyIcon();

  if(m_pPlayListDlg == NULL)
  {
    m_pPlayListDlg = new CPlayListDlg(this);
    m_pPlayListDlg->Create(IDD_PLAYLIST);
  }
  if(m_pLyricDlg == NULL)
  {
    m_pLyricDlg = new CLyricDlg(this);
    m_pLyricDlg->Create(IDD_LYRIC);
  }
  if(m_pEqualizerDlg == NULL)
  {
    m_pEqualizerDlg = new CEqualizerDlg(this);
    m_pEqualizerDlg->Create(IDD_EQUALIZER);
  }

  //Initial
  m_bPlay = FALSE;
  m_bStop = TRUE;
  m_bListShow = TRUE;
  m_bLyricShow = FALSE;
  m_bEqualizerShow = FALSE;
  m_nVolume = 100;
  //load config
  LoadConfigFile();
  m_pPlayListDlg->SetModeText();
  m_SliderVolume.SetPos(m_nVolume);
  m_Player->SetPlayerVolume(m_nVolume, m_nVolume);

  if(m_bListShow)
  {
    m_pPlayListDlg->ShowWindow(SW_SHOW);
    m_pPlayListDlg->AttachWindow();
  }
  if(m_bLyricShow)
  {
    m_pLyricDlg->ShowWindow(SW_SHOW);
    m_pLyricDlg->AttachWindow();  
  }
  if(m_bEqualizerShow)
  {
    m_pEqualizerDlg->ShowWindow(SW_SHOW);
    m_pEqualizerDlg->AttachWindow();  
  }
  return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CManateeDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CManateeDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

int CManateeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;

  m_Player = CreateZPlay();
  if(m_Player == 0)
    return -1;
  //set find mark
  ::SetProp(m_hWnd, AfxGetApp()->m_pszExeName, (HANDLE)1);

  return 0;
}

void CManateeDlg::OnDestroy() 
{
  CDialog::OnDestroy();

  Stop();
  m_Player->Release();
  DeleteNotifyIcon();
  SaveConfigFile();
  
  // delete find mark
  ::RemoveProp(m_hWnd, AfxGetApp()->m_pszExeName);    
}

void CManateeDlg::OnBtnPlay() 
{
  if(m_bStop)
  {
    int nIndex = m_pPlayListDlg->m_PlayList.GetSelectionMark();
    if(nIndex == -1)
      Play(m_pPlayListDlg->GetNextTrack());
    else
    {
      Play(m_pPlayListDlg->m_strPlayListArray.GetAt(nIndex));
      m_pPlayListDlg->m_nNowPlay = nIndex + 1;
    }
  }
  else
  {
    CString strTitle;
    GetWindowText(strTitle);
    if(m_bPlay)
    {
      m_Player->Pause();
      m_ButtonPlay.SetWindowText(L"&Play");
      strTitle += L" [Paused]";
    }
    else
    {
      m_Player->Resume();
      m_ButtonPlay.SetWindowText(L"&Pause");
      strTitle = strTitle.Left(strTitle.ReverseFind(' '));
    }
    SetWindowText(strTitle);
    ModifyNotifyIcon(strTitle);
    m_bPlay = !m_bPlay;
  }
}
 
void CManateeDlg::OnBtnStop() 
{
  Stop();
}

void CManateeDlg::Stop()
{
  if(m_bStop == TRUE)
    return;
  
  m_Player->Stop();
  m_Player->Close();

  KillTimer(TM_MAIN);
  m_bPlay = FALSE;
  m_bStop = TRUE;
  //reset slider
  m_SliderTime.SetPos(0);
  m_ButtonPlay.SetWindowText(L"&Play");
  m_SliderTime.EnableWindow(FALSE);
  //reset time
  GetDlgItem(IDC_STT_CURTIME)->SetWindowText(L"00:00:00");
  GetDlgItem(IDC_STT_TOTALTIME)->SetWindowText(L"00:00:00");
  //reset info
  GetDlgItem(IDC_STT_TRACKNAME)->SetWindowText(L"");
  GetDlgItem(IDC_STT_INFO)->SetWindowText(L"");
  GetDlgItem(IDC_STT_BITRATE)->SetWindowText(L"");
  SetWindowText(::AfxGetAppName());
  ModifyNotifyIcon(::AfxGetAppName());
}

void CManateeDlg::Play(CString strFileName)
{
  if(strFileName.IsEmpty()) return;
  if(!IsFileExist(strFileName))
  {
    m_pPlayListDlg->DeleteFromList();
    return;
  }

  if(m_Player->OpenFileW(strFileName, sfAutodetect) == 0)
  {
    MyErr(L"Error: %s\nPress key to exit.\r\n", m_Player->GetErrorW());
    return;
  }

  TID3InfoExW ID3Info;
  // get extended ID3 info
  m_Player->LoadID3ExW(&ID3Info, 1);
  
  if(ID3Info.Picture.PicturePresent == 1)
  {
    CDC *pDC = GetDC();
    SetStretchBltMode(pDC->m_hDC,STRETCH_HALFTONE);
    m_Player->DrawBitmapToHDC(pDC->m_hDC, 10, 10, 100, 100, ID3Info.Picture.hBitmap);
  }
  else
  {
    RECT rect;
    rect.left = 10; rect.top = 10;
    rect.right = 110; rect.bottom = 110;
    InvalidateRect(&rect);
  }

  CString strInfo; 
  //set track name
  CString strArtist = ID3Info.Artist;
  CString strTitle  = ID3Info.Title;

  if(strArtist.IsEmpty()) strArtist = L"(null)";

  if(!strTitle.IsEmpty())
    strInfo.Format(L"%s - %s", strArtist, ID3Info.Title);
  else
    strInfo = GetFileTitle(strFileName);

  GetDlgItem(IDC_STT_TRACKNAME)->SetWindowText(L"Track: " + strInfo);
  m_ToolTip.AddTool(GetDlgItem(IDC_STT_TRACKNAME), strInfo);
  //now playing
  SetWindowText(strInfo);
  ModifyNotifyIcon(strInfo);
  // get and display stream info
  TStreamInfoW pInfo;
  m_Player->GetStreamInfoW(&pInfo);
  TCHAR *sVbr = L"CBR";
  if(pInfo.VBR)
  sVbr = L"VBR";
  strInfo.Format(L"Format: %s\nSamplerate: %i Hz\nChannel: %u\nEncoding: %s",
                    pInfo.Description,
                    pInfo.SamplingRate,
                    pInfo.ChannelNumber,
                    sVbr);
  GetDlgItem(IDC_STT_INFO)->SetWindowText(strInfo);
  //display bitrate zero here 
  strInfo.Format(L"Bitrate: %d kbps", m_Player->GetBitrate(0));
  GetDlgItem(IDC_STT_BITRATE)->SetWindowText(strInfo);
  //display total time
  CString strTotalTime;
  strTotalTime.Format(L"%02d:%02d.%02d", pInfo.Length.hms.hour*60+pInfo.Length.hms.minute, 
    pInfo.Length.hms.second,pInfo.Length.hms.millisecond/10);

  GetDlgItem(IDC_STT_TOTALTIME)->SetWindowText(strTotalTime);
  m_nTotalTime = (pInfo.Length.hms.hour*3600 + pInfo.Length.hms.minute*60 
    + pInfo.Length.hms.second)*1000 + pInfo.Length.hms.millisecond;
  //button
  m_ButtonPlay.SetWindowText(L"&Pause");
  //slider
  m_SliderTime.EnableWindow(TRUE);
  m_SliderTime.SetRange(0, m_nTotalTime);
  m_SliderTime.SetPos(0);
  //set volume
  int nVolume = m_SliderVolume.GetPos();
  m_Player->SetPlayerVolume(nVolume, nVolume);
  //start playing
  if(m_Player->Play() == 0)
  {
    MyErr(L"Error: %s\nPress key to exit.\r\n", m_Player->GetErrorW());
    return;
  }
  m_bPlay = TRUE;
  m_bStop = FALSE;
  SetTimer(TM_MAIN, 10, NULL);
  //lyric
  if(!m_pLyricDlg->LoadLyric())
  {
    m_pLyricDlg->Invalidate();
    m_pLyricDlg->ShowWindow(SW_HIDE);
    m_bLyricShow = FALSE;
  }
  else if(IsWindowVisible())
  {
    m_pLyricDlg->ShowWindow(SW_SHOW);
    m_pLyricDlg->AttachWindow();
    m_bLyricShow = TRUE;
  }
}

void CManateeDlg::OnBtnNext() 
{
  Stop();
  m_pPlayListDlg->m_bIsNext = TRUE;
  Play(m_pPlayListDlg->GetNextTrack());
}

void CManateeDlg::OnBtnPrev() 
{
  Stop();
  m_pPlayListDlg->m_bIsNext = FALSE;
  Play(m_pPlayListDlg->GetNextTrack());
}

void CManateeDlg::OnTimer(UINT nIDEvent) 
{
  if(m_bPlay)
  {
    if(nIDEvent == TM_MAIN)
    {
      CString str;
      str.Format(L"Bitrate: %d kbps", m_Player->GetBitrate(0));
      GetDlgItem(IDC_STT_BITRATE)->SetWindowText(str);
      CountTime();
      m_SliderTime.SetPos(m_nCurTime);//step slider
      if(m_nCurTime >= m_nTotalTime-250 && m_nCurTime <= m_nTotalTime+250)
        OnBtnNext();
    }
  }
  CDialog::OnTimer(nIDEvent);
}

void CManateeDlg::CountTime()
{
  // get current position
  TStreamTime pos;
  m_Player->GetPosition(&pos);
  CString strCurTime;
  strCurTime.Format(L"%02d:%02d.%02d", pos.hms.hour*60+pos.hms.minute,
    pos.hms.second, pos.hms.millisecond/10);
  GetDlgItem(IDC_STT_CURTIME)->SetWindowText(strCurTime);
  m_nCurTime = (pos.hms.hour*3600 + pos.hms.minute*60
    + pos.hms.second)*1000 + pos.hms.millisecond ;
}

void CManateeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  if(!m_bStop)
  {
    if(pScrollBar->m_hWnd == m_SliderTime.m_hWnd)
    {
      m_Player->Pause();
      Sleep(20);
      TStreamTime pTime;
      pTime.ms = m_SliderTime.GetPos();
      m_Player->Seek(tfMillisecond, &pTime, smFromBeginning);
      CountTime();  
      if(m_nCurTime >= m_nTotalTime)
        OnBtnNext();
    }
  }
  if(pScrollBar->m_hWnd == m_SliderVolume.m_hWnd)
  {
    m_nVolume = m_SliderVolume.GetPos();
    m_Player->SetPlayerVolume(m_nVolume, m_nVolume);
  }
  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CManateeDlg::OnBtnShowPlayList() 
{
  if(m_bListShow)
    m_pPlayListDlg->ShowWindow(SW_HIDE);
  else{
    m_pPlayListDlg->ShowWindow(SW_SHOW);
    m_pPlayListDlg->AttachWindow();
  }
  m_bListShow = !m_bListShow;
}

void CManateeDlg::OnBtnShowLyric() 
{
  if(m_bLyricShow)
    m_pLyricDlg->ShowWindow(SW_HIDE);
  else{
    m_pLyricDlg->ShowWindow(SW_SHOW);
    m_pLyricDlg->AttachWindow();
  }
  m_bLyricShow = !m_bLyricShow;
}

void CManateeDlg::OnBtnShowEq() 
{
  if(m_bEqualizerShow)
    m_pEqualizerDlg->ShowWindow(SW_HIDE);
  else{
    m_pEqualizerDlg->ShowWindow(SW_SHOW);
    m_pEqualizerDlg->AttachWindow();
  }
  m_bEqualizerShow = !m_bEqualizerShow;
}

void CManateeDlg::OnBtnHelp() 
{
  RECT rect;
  GetDlgItem(IDC_BTN_HELP)->GetWindowRect(&rect);
  CMenu menu;
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_STRING, IDM_ABOUT, L"Abou&t...");
  menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left, rect.bottom, this);
  menu.DestroyMenu();
}

BOOL CManateeDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(m_hAccel)
  {
    if(::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
    { return TRUE; }
  }

  if(m_ToolTip)
  { m_ToolTip.RelayEvent(pMsg); }

  return CDialog::PreTranslateMessage(pMsg);
}

void CManateeDlg::OnMenuAbout() 
{
  CAboutDlg dlgAbout;
  dlgAbout.DoModal();
}

void CManateeDlg::CreateNotifyIcon()
{
  m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
  m_NotifyIcon.hWnd = m_hWnd;
  m_NotifyIcon.uID = IDR_MAINFRAME;
  m_NotifyIcon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
  m_NotifyIcon.uCallbackMessage = WM_NOTIFYICON;
  m_NotifyIcon.hIcon = m_hIcon;
  lstrcpy(m_NotifyIcon.szTip, AfxGetAppName());
  
  Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);
}

void CManateeDlg::ModifyNotifyIcon(CString strTip)
{
  lstrcpy(m_NotifyIcon.szTip, strTip);
  Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIcon);
}

void CManateeDlg::DeleteNotifyIcon()
{
  Shell_NotifyIcon(NIM_DELETE,&m_NotifyIcon);
}

void CManateeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_MINIMIZE)
    OnMenuShowWindow();
  else 
    CDialog::OnSysCommand(nID, lParam);
}

void CManateeDlg::OnMenuShowWindow() 
{
  UINT MODE = IsWindowVisible()?SW_HIDE:SW_SHOW;
  ShowWindow(MODE);
  SetForegroundWindow();
  if(m_bListShow)
  {
    m_pPlayListDlg->ShowWindow(MODE);
    m_pPlayListDlg->SetForegroundWindow();
  }
  if(m_bLyricShow)
  {
    m_pLyricDlg->ShowWindow(MODE);
    m_pLyricDlg->SetForegroundWindow();
  }
  if(m_bEqualizerShow)
  {
    m_pEqualizerDlg->ShowWindow(MODE);
    m_pEqualizerDlg->SetForegroundWindow();
  }
  if(m_pPlayListDlg->m_bFileID3Show)
  {
    m_pPlayListDlg->m_pFileID3Dlg->ShowWindow(MODE);
    m_pPlayListDlg->m_pFileID3Dlg->SetForegroundWindow();
  }
}

void CManateeDlg::OnNotifyIcon(WPARAM wParam,LPARAM lParam)
{
  if(lParam == WM_LBUTTONDOWN)
  {
    OnMenuShowWindow();
  }
  else if(lParam == WM_RBUTTONDOWN)
  {
    CPoint MousePos;
    GetCursorPos(&MousePos);   
    CMenu menu;
    menu.CreatePopupMenu();
    if(IsWindowVisible())
      menu.AppendMenu(MF_STRING, IDM_SHOW_WINDOW, L"Hide Window");
    else
      menu.AppendMenu(MF_STRING, IDM_SHOW_WINDOW, L"Show Window");
    menu.AppendMenu(MF_SEPARATOR,0,L"");
    if(m_bPlay)menu.AppendMenu(MF_STRING, IDC_BTN_PLAY, L"Pause");
    else menu.AppendMenu(MF_STRING, IDC_BTN_PLAY, L"Play");
    menu.AppendMenu(MF_STRING, IDC_BTN_STOP, L"Stop");
    menu.AppendMenu(MF_STRING, IDC_BTN_PREV, L"Previous");
    menu.AppendMenu(MF_STRING, IDC_BTN_NEXT, L"Next");
    menu.AppendMenu(MF_SEPARATOR,0,L"");
    menu.AppendMenu(MF_STRING, IDM_ABOUT, L"About");
    menu.AppendMenu(MF_STRING, ID_APP_EXIT, L"Exit");
    SetForegroundWindow();
    menu.TrackPopupMenu(TPM_RIGHTBUTTON, MousePos.x, MousePos.y,this);
    menu.DestroyMenu();
  }
}

void CManateeDlg::OnMove(int x, int y) 
{
  CDialog::OnMove(x, y);
  
  if(m_pPlayListDlg)
  {
    if(m_bListShow && m_pPlayListDlg->m_Attached)
      m_pPlayListDlg->AttachWindow();
  }
  if(m_pPlayListDlg)
  {
    if(m_bLyricShow && m_pLyricDlg->m_Attached)
      m_pLyricDlg->AttachWindow();
  }
  if(m_pEqualizerDlg)
  {
    if(m_bEqualizerShow && m_pEqualizerDlg->m_Attached)
      m_pEqualizerDlg->AttachWindow();
  }
}

void CManateeDlg::OnDropFiles(HDROP hDropInfo) 
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
      m_pPlayListDlg->m_strPlayListArray.Add(szFilePath);
      strFormat.Format(L"%d", ++m_pPlayListDlg->m_nTotalMusic);
      m_pPlayListDlg->m_PlayList.InsertItem(m_pPlayListDlg->m_nTotalMusic-1, strFormat);
      m_pPlayListDlg->m_PlayList.SetItemText(m_pPlayListDlg->m_nTotalMusic-1, 1, GetFileTitle(szFilePath));
      m_pPlayListDlg->m_PlayList.SetItemText(m_pPlayListDlg->m_nTotalMusic-1, 2, GetExtension(szFilePath));
      m_pPlayListDlg->m_PlayList.EnsureVisible(m_pPlayListDlg->m_nTotalMusic-1, FALSE);
      Stop();
      Play(szFilePath);
    }
  }
  DragFinish(hDropInfo);
  CDialog::OnDropFiles(hDropInfo);
}

void CManateeDlg::LoadConfigFile()
{
  CStdioFile ConfigFile;
  if(ConfigFile.Open(m_strConfigFileName, CFile::modeRead))
  {
    char *szHeader = new char[5];
    ConfigFile.Read(szHeader, 5);
    if(strcmp(szHeader, "MPCFG") == 0){
      CONFIG config;
      ConfigFile.Read(&config, sizeof(config));
      m_bListShow = config.ListShow;
      m_bLyricShow = config.LyricShow;
      m_bEqualizerShow = config.EqualizerShow;
      m_nVolume = config.Volume;
      m_pPlayListDlg->m_PlayMode = (CPlayListDlg::PlayMode)config.PlayMode;
      m_pPlayListDlg->m_nNowPlay = config.NowPlay + 1;
      RECT rc;
      GetWindowRect(&rc);
      int Height = rc.bottom - rc.top;
      int Width = rc.right - rc.left;
      rc.left = config.PosX;
      rc.top = config.PosY;
      rc.right = Width + rc.left;
      rc.bottom = Height + rc.top;
      MoveWindow(&rc);
    }
  }
  else ConfigFile.Open(m_strConfigFileName, CFile::modeCreate);

  ConfigFile.Close();
}

void CManateeDlg::SaveConfigFile()
{
  char *szHeader = "MPCFG";
  CONFIG config;
  config.ListShow = m_bListShow;
  config.LyricShow = m_bLyricShow;
  config.EqualizerShow = m_bEqualizerShow;
  config.Volume = m_nVolume;
  config.PlayMode = m_pPlayListDlg->m_PlayMode;
  config.NowPlay = m_pPlayListDlg->m_nNowPlay;

  RECT rc;
  GetWindowRect(&rc);
  config.PosX = rc.left;
  config.PosY = rc.top;
  CStdioFile ConfigFile(m_strConfigFileName, CFile::modeWrite);
  ConfigFile.Write(szHeader, strlen(szHeader));
  ConfigFile.Write(&config, sizeof(config));
}