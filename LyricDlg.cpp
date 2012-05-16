// LyricDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manatee.h"
#include "ManateeDlg.h"
#include "PlayListDlg.h"
#include "Functions.h"
#include "LyricDlg.h"
#include "Lrc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_LYRIC   1
#define ORANGE RGB(255,125,0)
#define GREEN  RGB(0,150,0)
#define BLUE   RGB(0,125,255)
/////////////////////////////////////////////////////////////////////////////
// CLyricDlg dialog


CLyricDlg::CLyricDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CLyricDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CLyricDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  m_pManateeDlg = (CManateeDlg *)pParent;
}


void CLyricDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CLyricDlg)
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLyricDlg, CDialog)
  //{{AFX_MSG_MAP(CLyricDlg)
  ON_WM_DESTROY()
  ON_WM_SYSCOMMAND()
  ON_WM_MOVE()
  ON_WM_PAINT()
  ON_WM_TIMER()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLyricDlg message handlers

BOOL CLyricDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  //Initial
  m_Attached = RIGHT;
  LOGFONT logFont={sizeof(logFont)};
  logFont.lfHeight = 13;
  lstrcpy(logFont.lfFaceName, L"Tahoma");
  m_Font.CreateFontIndirect(&logFont);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CLyricDlg::OnDestroy() 
{
  CDialog::OnDestroy();
  m_Font.DeleteObject();
}

void CLyricDlg::OnCancel()
{
  DestroyWindow();
}

void CLyricDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  delete this;
}

void CLyricDlg::AttachWindow()
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

void CLyricDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
  {
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_bLyricShow = FALSE;
  }
  else 
    CDialog::OnSysCommand(nID, lParam);
}

void CLyricDlg::OnMove(int x, int y) 
{
  CDialog::OnMove(x, y);

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

void CLyricDlg::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
  InvalidateCtrl();
}

void CLyricDlg::OnTimer(UINT nIDEvent) 
{
  if(nIDEvent == TM_LYRIC)
  {
    CountPos();
    InvalidateCtrl();
  }
  
  CDialog::OnTimer(nIDEvent);
}

void CLyricDlg::InvalidateCtrl()
{
  if(!IsWindow(m_hWnd))return;
  
  CDC *pDC = GetDC();
  CDC memDC;
  memDC.CreateCompatibleDC(pDC);
  memDC.SetBkMode(TRANSPARENT);
  
  RECT rect;
  GetClientRect(&rect);
  
  CBitmap bitmap,*pOldBitmap;
  bitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);
  pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);
  memDC.FillSolidRect(&rect,GetSysColor(COLOR_BTNFACE));
  CFont *pFont = memDC.SelectObject(&m_Font);

  //start draw text
  rect.top = m_nPaintPos;
  rect.bottom = rect.top + 13;
  for(int i=0;i<m_Lrc.m_nLines;i++)
  {
    if(rect.top == 80)
      memDC.SetTextColor(BLUE);
    else
      memDC.SetTextColor(GREEN);
    memDC.DrawText(m_Lrc.GetContent(m_Lrc.m_TokenArray[i]),&rect,DT_CENTER);
    rect.top += 18;//line space is 5
    rect.bottom = rect.top + 13;
  }
  
  pDC->BitBlt(0,0,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);
  
  memDC.SelectObject(pFont);
  memDC.SelectObject(pOldBitmap);
  memDC.DeleteDC();
  ReleaseDC(pDC);
}

BOOL CLyricDlg::LoadLyric()
{
  CString strSongName = m_pManateeDlg->m_pPlayListDlg->
     m_strPlayListArray[m_pManateeDlg->m_pPlayListDlg->m_nNowPlay-1];

  CString strLyricName = strSongName.Left(strSongName.ReverseFind('.'))+L".lrc";

  KillTimer(TM_LYRIC);
  m_nPaintPos = 80;

  m_Lrc.Reset();
  if(!m_Lrc.Init(strLyricName))
  {
    TCHAR szCurPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szCurPath);
    strLyricName = CString(szCurPath)+L"\\lyrics\\"+GetFileTitle(strSongName)+L".lrc";
    if(!m_Lrc.Init(strLyricName)) return FALSE;
  }

  InvalidateCtrl();
  SetTimer(TM_LYRIC, 5, NULL);
  return TRUE;
}

void CLyricDlg::CountPos()
{
  if(m_pManateeDlg->m_bPlay && m_Lrc.m_bInit)
  {
    TStreamTime pos;
    m_pManateeDlg->m_Player->GetPosition(&pos);
    
    for(int i=0; i<m_Lrc.m_nLines; i++)
    {
      if(pos.ms>=m_Lrc.m_TokenArray[i].nTime-100 && pos.ms<=m_Lrc.m_TokenArray[i].nTime+100)
      {
        m_nPaintPos = 80 - i*18;
        break;
      }
    }
  }
}

BOOL CLyricDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->wParam == VK_ESCAPE)
  { 
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_bLyricShow = FALSE;
    return TRUE;
  }  
  else return CDialog::PreTranslateMessage(pMsg);
}
