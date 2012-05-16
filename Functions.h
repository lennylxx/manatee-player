/*
  Functions.h 2012/4/12
  by 7k
  Info:

*/

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


#ifdef _cplusplus
extern "C"{
#endif //_cplusplus

  BOOL IsFileExist(CString strFileName);
  BOOL IsDirectoryExist(CString strPath);
  CString GetFileTitle(CString strFileName);
  CString GetExtension(CString strFileName);
  BOOL IsSupportFile(CString strFileName);
  void MyErr(TCHAR *errmsg, TCHAR *errcode);

#ifdef _cplusplus
}
#endif //_cplusplus


#endif //_FUNCTIONS_H_