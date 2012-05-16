// Manatee.h : main header file for the MANATEE application
//

#if !defined(AFX_MANATEE_H__39CEB3A1_A913_41C9_82FC_1181D87E4277__INCLUDED_)
#define AFX_MANATEE_H__39CEB3A1_A913_41C9_82FC_1181D87E4277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CManateeApp:
// See Manatee.cpp for the implementation of this class
//

class CManateeApp : public CWinApp
{
public:
	CManateeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManateeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CManateeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANATEE_H__39CEB3A1_A913_41C9_82FC_1181D87E4277__INCLUDED_)
