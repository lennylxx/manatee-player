// Manatee.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Manatee.h"
#include "ManateeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManateeApp

BEGIN_MESSAGE_MAP(CManateeApp, CWinApp)
	//{{AFX_MSG_MAP(CManateeApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManateeApp construction

CManateeApp::CManateeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CManateeApp object

CManateeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CManateeApp initialization

BOOL CManateeApp::InitInstance()
{
	AfxEnableControlContainer();

  //check if there is one instance already ran
  HANDLE m_hUniqueMutex = CreateMutex(NULL, FALSE, m_pszExeName);
  if (GetLastError() == ERROR_ALREADY_EXISTS)
  {
    CloseHandle(m_hUniqueMutex);
    HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
    while (::IsWindow(hWndPrevious))
    {
      if (::GetProp(hWndPrevious, m_pszExeName))
      {
        if (::IsIconic(hWndPrevious))::ShowWindow(hWndPrevious, SW_RESTORE);
        ::SetForegroundWindow(hWndPrevious);
        ::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
        return FALSE;
      }
      hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
    }
    return FALSE;
  }

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CManateeDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
