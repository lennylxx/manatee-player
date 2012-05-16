// EqualizerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manatee.h"
#include "ManateeDlg.h"
#include "EqualizerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEqualizerDlg dialog


CEqualizerDlg::CEqualizerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEqualizerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEqualizerDlg)
	m_Slider0 = 50;
  m_Slider1 = 50;
  m_Slider2 = 50;
  m_Slider3 = 50;
  m_Slider4 = 50;
  m_Slider5 = 50;
  m_Slider6 = 50;
  m_Slider7 = 50;
  m_Slider8 = 50;
  m_Slider9 = 50;
  m_Slider10 = 50;
	//}}AFX_DATA_INIT
  m_pManateeDlg = (CManateeDlg *)pParent;
}


void CEqualizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEqualizerDlg)
	DDX_Slider(pDX, IDC_SLIDER0, m_Slider0);
  DDX_Slider(pDX, IDC_SLIDER1, m_Slider1);
  DDX_Slider(pDX, IDC_SLIDER2, m_Slider2);
  DDX_Slider(pDX, IDC_SLIDER3, m_Slider3);
  DDX_Slider(pDX, IDC_SLIDER4, m_Slider4);
  DDX_Slider(pDX, IDC_SLIDER5, m_Slider5);
  DDX_Slider(pDX, IDC_SLIDER6, m_Slider6);
  DDX_Slider(pDX, IDC_SLIDER7, m_Slider7);
  DDX_Slider(pDX, IDC_SLIDER8, m_Slider8);
  DDX_Slider(pDX, IDC_SLIDER9, m_Slider9);
  DDX_Slider(pDX, IDC_SLIDER10, m_Slider10);
	//}}AFX_DATA_MAP

  int eq_values[10];
  eq_values[0] = (50 - m_Slider1)*240;
  eq_values[1] = (50 - m_Slider2)*240;
  eq_values[2] = (50 - m_Slider3)*240;
  eq_values[3] = (50 - m_Slider4)*240;
  eq_values[4] = (50 - m_Slider5)*240;
  eq_values[5] = (50 - m_Slider6)*240;
  eq_values[6] = (50 - m_Slider7)*240;
  eq_values[7] = (50 - m_Slider8)*240;
  eq_values[8] = (50 - m_Slider9)*240;
  eq_values[9] = (50 - m_Slider10)*240;
  m_pManateeDlg->m_Player->SetEqualizerParam((50 - m_Slider0)*240, eq_values, 10);
}


BEGIN_MESSAGE_MAP(CEqualizerDlg, CDialog)
	//{{AFX_MSG_MAP(CEqualizerDlg)
  ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_CHK_ENABLE, OnChkEnable)
	ON_WM_VSCROLL()
  ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqualizerDlg message handlers

BOOL CEqualizerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Initial
	m_Attached = LEFT;
  m_bEnable = FALSE;

  //band0:0-120       60
  //band1:120-220     170
  //band2:220-400     310
  //band3:400-800     600
  //band4:800-1200    1K
  //band5:1200-4800   3K
  //band6:4800-7200   6K
  //band7:7200-12800  10K
  //band8:12800-15200 14K
  //band9:15200- samplerate/2  18K

  int FreqPoint[9] = {120, 220, 400, 800, 1200, 4800, 7200, 12800, 15200};

  m_pManateeDlg->m_Player->SetEqualizerPoints(FreqPoint, 9);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEqualizerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CEqualizerDlg::OnCancel() 
{
	DestroyWindow();
}

void CEqualizerDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
  delete this;
}

void CEqualizerDlg::AttachWindow()
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

void CEqualizerDlg::OnMove(int x, int y) 
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

void CEqualizerDlg::OnBtnReset() 
{
	m_Slider0 = 50;
  m_Slider1 = 50;
  m_Slider2 = 50;
  m_Slider3 = 50;
  m_Slider4 = 50;
  m_Slider5 = 50;
  m_Slider6 = 50;
  m_Slider7 = 50;
  m_Slider8 = 50;
  m_Slider9 = 50;
  m_Slider10 = 50;
  UpdateData(FALSE);
}

void CEqualizerDlg::OnChkEnable() 
{
  m_bEnable = !m_bEnable;
  if(m_bEnable)
  {
    UpdateData(TRUE);
    m_pManateeDlg->m_Player->EnableEqualizer(1);
  }
  else
  {
    m_pManateeDlg->m_Player->EnableEqualizer(0);
  } 
}

void CEqualizerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEqualizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
  {
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_bEqualizerShow = FALSE;
  }
  else 
    CDialog::OnSysCommand(nID, lParam);
}

BOOL CEqualizerDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->wParam == VK_ESCAPE)
  { 
    ShowWindow(SW_HIDE);
    m_pManateeDlg->m_bEqualizerShow = FALSE;
    return TRUE;
  }	
	else return CDialog::PreTranslateMessage(pMsg);
}
