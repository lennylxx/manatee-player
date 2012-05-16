/*
  Functions.cpp 2012/4/12
  by 7k
  Info:
*/

#include "stdafx.h"
#include "Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif //_DEBUG

BOOL IsFileExist(CString strFileName)
{
  CFileFind fFind;
  return fFind.FindFile(strFileName);
}

BOOL IsDirectoryExist(CString strPath)
{
  if(strPath.IsEmpty())
    return FALSE;
  HANDLE hFind;
  WIN32_FIND_DATA wfd;
  hFind = FindFirstFile(strPath, &wfd);
  if((INVALID_HANDLE_VALUE != hFind)
    && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    return TRUE;
  FindClose(hFind);
  return FALSE;
}

CString GetFileTitle(CString strFileName)
{
  int nPos = -1;
  nPos = strFileName.ReverseFind('\\');
  strFileName = strFileName.Right(strFileName.GetLength()-nPos-1);
  nPos = strFileName.ReverseFind('.');
  strFileName = strFileName.Left(nPos);
  return strFileName;
}

CString GetExtension(CString strFileName)
{
  int nPos = -1;
  nPos = strFileName.ReverseFind('.');
  strFileName = strFileName.Right(strFileName.GetLength()-nPos-1);
  strFileName.MakeUpper();
  return strFileName;
}

BOOL IsSupportFile(CString strFileName)
{
  CString str = GetExtension(strFileName);
  if(str == L"MP3" ||
    str == L"MP2" || 
    str == L"MP1" || 
    str == L"OGG" || 
    str == L"FLAC" || 
    str == L"OGA" || 
    str == L"AC3" || 
    str == L"AAC" || 
    str == L"WAV")
  return TRUE;
  else return FALSE;
}

void MyErr(TCHAR *errmsg, TCHAR *errcode)
{
  CString str;
  str.Format(errmsg, errcode);
  AfxMessageBox(str);
}
