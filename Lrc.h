/*
  Lrc.h 2012/4/12
  by 7k
  Info:

*/

#ifndef _LRC_H_
#define _LRC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <algorithm>
#include <afxtempl.h>

typedef struct{
  UINT nTime;//TimeTag
  UINT nPos; //position in the content array
}TOKEN, *LPTOKEN;

class CLrc
{
public:
  CLrc();
  ~CLrc();
  
  BOOL m_bInit;
  UINT m_nLines;//number of tokens
  CArray<TOKEN,TOKEN> m_TokenArray;
  BOOL Init(CString strFileName);
  CString GetContent(TOKEN token);
  void Reset();
protected:
  int  m_nOffset;//millisecond
  CStringArray m_strContentArray;
  void AnalyseLine(CString strLine);
  UINT GetMilliSecond(CString strTime);
  BOOL IsTag(CString strTag);
  BOOL IsTimeTag(CString strTag);
  int  GetOffset(CString strTag);
};

#endif //_LRC_H_