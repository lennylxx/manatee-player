/*
  Lrc.cpp 2012/4/12
  by 7k
  Info:

*/

#include "stdafx.h"
#include "Lrc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* 
  **Simple LRC format**
Time Tag:[mm:ss.xx] start time
     e.g.[00:23.35](0 minute 23 second and 350 millisecond)

ID Tag:(ignore except 'offset')
[offset:+/- Overall timestamp adjustment in milliseconds, 
        + shifts time up, - shifts down]
*/

#define ch2i(ch) (ch-'0')

BOOL MyCompare(const TOKEN& a, const TOKEN& b)
{  return (a.nTime<b.nTime); }

CLrc::CLrc()
{
  m_bInit   = FALSE;
  m_nOffset = 0;
  m_nLines  = 0;
}

CLrc::~CLrc()
{
}

BOOL CLrc::Init(CString strFileName)
{
  m_bInit = FALSE;
  m_TokenArray.RemoveAll();
  m_strContentArray.RemoveAll();
  CStdioFile LrcFile;
  if(!LrcFile.Open(strFileName, CFile::modeRead))
    return m_bInit;

  CString strLine;
  while(LrcFile.ReadString(strLine))
  {
    int nMultiByteLen = strLine.GetLength();
    char *szText = new char[nMultiByteLen+1];
    for (int i=0 ; i<nMultiByteLen; i++) 
      szText[i] = strLine[i];
    szText[nMultiByteLen] = '\0';
  
    int nWideCharLen = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, 0);
    wchar_t *pwText = new wchar_t[nWideCharLen];
    MultiByteToWideChar (CP_ACP, 0, szText, -1, pwText, nWideCharLen);

    CString wstrLine;
    wstrLine.Format(L"%s", pwText);

    AnalyseLine(wstrLine);

    delete []szText;
    delete []pwText;
  }

  if(m_nLines>0)
  {
    std::sort(m_TokenArray.GetData(), 
      m_TokenArray.GetData()+m_TokenArray.GetSize(), MyCompare);
  }
  LrcFile.Close();
  m_bInit = TRUE;
  return m_bInit;
  
}

void CLrc::AnalyseLine(CString strLine)
{
  TOKEN token;
  CString strTag;
  int pos = m_strContentArray.GetSize(); 
  while(1)
  {
    int startPos, endPos;
    startPos = strLine.Find('[');
    if(startPos == -1) break;
    endPos = strLine.Find(']');
    if(endPos == -1) break;
    strTag = strLine.Mid(startPos, endPos-startPos+1);
    if(!IsTag(strTag)) break;
    strLine = strLine.Right(strLine.GetLength()-1-endPos);
    if(!IsTimeTag(strTag))
    {
      if(strTag.Mid(1, 6) == L"offset")
      { m_nOffset = GetOffset(strTag); }
    }
    else
    {
      token.nTime = GetMilliSecond(strTag) + m_nOffset;
      token.nPos  = pos;
      m_TokenArray.Add(token);
      m_nLines++;
    }
  }
  m_strContentArray.Add(strLine);//content
}

UINT CLrc::GetMilliSecond(CString strTime)
{//[mm:ss.xx]
  UINT nMinute;
  UINT nSecond;
  nMinute = ch2i(strTime[1])*10 + ch2i(strTime[2]);
  nMinute *= 60*1000;
  nSecond = 1000*(ch2i(strTime[4])*10 + ch2i(strTime[5]))
          + 10*(ch2i(strTime[7])*10+ch2i(strTime[8]));
  return (nMinute+nSecond);
}

BOOL CLrc::IsTag(CString strTag)
{//[*:*]
  if(strTag[0] != '[') return FALSE;
  if(strTag.Find(':') == -1) return FALSE;
  if(strTag.Find(']') == -1) return FALSE;
  return TRUE;
}

BOOL CLrc::IsTimeTag(CString strTag)
{//[mm:ss.xx]
  if((strTag[1]>='0'&&strTag[1]<='5')
  && (strTag[2]>='0'&&strTag[2]<='9')
  && (strTag[4]>='0'&&strTag[4]<='5')
  && (strTag[5]>='0'&&strTag[5]<='9')
  && (strTag[7]>='0'&&strTag[7]<='9')
  && (strTag[8]>='0'&&strTag[8]<='9'))
  {  return TRUE; }
  else return FALSE;
}

int CLrc::GetOffset(CString strTag)
{//[offset:+/-500]
  CString strOffset = strTag.Mid(9, strTag.GetLength()-10);
  int offset = _ttoi(strOffset);
  if(strTag[8] == '-')
  { offset = -offset; }
  return offset;
}

CString CLrc::GetContent(TOKEN token)
{
  return m_strContentArray[token.nPos];
}

void CLrc::Reset()
{
  m_bInit = FALSE;
  m_TokenArray.RemoveAll();
  m_strContentArray.RemoveAll();
  m_nOffset = 0;
  m_nLines  = 0;
}