// MySliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Manatee.h"
#include "MySliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySliderCtrl
IMPLEMENT_DYNAMIC(CMySliderCtrl, CSliderCtrl)  

CMySliderCtrl::CMySliderCtrl()
   : m_bDragging(false), m_bDragChanged(false)
{
}

CMySliderCtrl::~CMySliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
  //{{AFX_MSG_MAP(CMySliderCtrl)
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySliderCtrl message handlers

void CMySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
  m_bDragging = true;   
  m_bDragChanged = false;   
  SetCapture();   
  SetFocus();   
  if (SetThumb(point))   
  {   
    m_bDragChanged = true;   
    PostMessageToParent(TB_THUMBTRACK);   
  }   
}

void CMySliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
  if(m_bDragging)   
  {   
    m_bDragging = false;   
    ::ReleaseCapture();   
    if (SetThumb(point))   
    {   
      PostMessageToParent(TB_THUMBTRACK);   
      m_bDragChanged = true;   
    }   
    if (m_bDragChanged)   
    {   
      PostMessageToParent(TB_THUMBPOSITION);   
      m_bDragChanged = false;   
    }   
  }   
  else   
  {   
    CSliderCtrl::OnLButtonUp(nFlags, point);   
  }   
}

void CMySliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{   
  if (m_bDragging)   
  { 
    if (SetThumb(point))   
    {   
      m_bDragChanged = true;   
      PostMessageToParent(TB_THUMBTRACK);   
    }
  }   
  else   
  {   
    CSliderCtrl::OnMouseMove(nFlags, point);   
  }   
}

bool CMySliderCtrl::SetThumb(const CPoint& point)   
{   
  const int nMin = GetRangeMin();   
  const int nMax = GetRangeMax()+1;   
  CRect rc;   
  GetChannelRect(rc);   
  double dPos;   
  double dCorrectionFactor = 0.0;   
  if (GetStyle() & TBS_VERT)    
  {   
    // note: there is a bug in GetChannelRect, it gets the orientation of the rectangle mixed up   
    dPos = (double)(point.y - rc.left)/(rc.right - rc.left);   
  }   
  else   
  {   
    dPos = (double)(point.x - rc.left)/(rc.right - rc.left);   
  }   
  // This correction factor is needed when you click inbetween tick marks   
  // so that the thumb will move to the nearest one   
  dCorrectionFactor = 0.5 *(1-dPos) - 0.5 *dPos;   
  int nNewPos = (int)(nMin + (nMax-nMin)*dPos + dCorrectionFactor);   
  const bool bChanged = (nNewPos != GetPos());   
  if(bChanged)   
  {   
    SetPos(nNewPos);   
  }   
  return bChanged;   
}

void CMySliderCtrl::PostMessageToParent(const int nTBCode) const   
{   
  CWnd* pWnd = GetParent();   
  if(pWnd) pWnd->PostMessage(WM_HSCROLL, (WPARAM)((GetPos() << 16) | nTBCode), (LPARAM)GetSafeHwnd());   
}
