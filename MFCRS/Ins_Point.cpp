
#include"stdafx.h"
#include"MyFunc.h"
#include"Ins_Point.h"

#pragma once

//--------------------------------------------
//point list operation
//--------------------------------------------
Point* CObjPointClass::AddOnePoint(float X0,float Y0,CString text)
{
	int len = text.GetLength();	  if(len<1) return NULL;
	char *pstr = new char[len+1]; if(!pstr) return NULL; 
	memset(pstr,0,((len+1)*sizeof(char)));
	strcpy_s(pstr,(len+1)*sizeof(char),text);
	Point *cpoint = new Point[1]; if(!cpoint) return NULL; memset(cpoint,0,sizeof(Point));
	if(!HeadPoint)	HeadPoint = cpoint;
	else            EndPoint->Next = cpoint;
	EndPoint = cpoint;
	cpoint->X0 = X0;
	cpoint->Y0 = Y0;
	cpoint->TextAdr = pstr;
	cpoint->CharNum = len;
	PointNum++;
	return cpoint;
}
Point* CObjPointClass::AddOnePoint(float X0,float Y0,const char *text)
{
	int len=strlen(text); if(len<1) return NULL;
	char *pstr = new char[len+1]; if(!pstr) return NULL; 
	memset(pstr,0,((len+1)*sizeof(char)));
	memcpy_s(pstr,(len+1)*sizeof(char),text,sizeof(char)*(len+1));
	Point *cpoint = new Point[1]; if(!cpoint) return NULL; memset(cpoint,0,sizeof(Point));
	if(!HeadPoint)	HeadPoint = cpoint;
	else            EndPoint->Next = cpoint;
	EndPoint = cpoint;
	cpoint->X0 = X0;
	cpoint->Y0 = Y0;
	cpoint->TextAdr = pstr;
	cpoint->CharNum = len;
	PointNum++;
	return cpoint;
}

///---------------------------------------------
void CObjPointClass::DeletePointLst()
{
	char *pstr;
	Point *cpoint;
	Point *dpoint = NULL;
	ForLoop(cpoint,HeadPoint)
	{
		pstr = cpoint->TextAdr; if(pstr) delete []pstr;
		if(dpoint) delete []dpoint;
		dpoint = cpoint;
	}
	if(dpoint) delete[] dpoint;
	PointNum = 0;
	HeadPoint = NULL;
	EndPoint  = NULL;
}
//-------------------------------------------------
//input and output
//-------------------------------------------------
BOOL CObjPointClass::LoadPointFunc(CString filename)
{
	CFile File;
	CFileException e;
	if(!File.Open(filename,CFile::modeRead,&e))
	{
		CString strErr;
		strErr.Format(_T("File could not be opened %d\n"),e.m_cause);
		return FALSE;
	}
	float x0,y0;
	short n;
	int offset = 0;
	File.Seek(sizeof(long),File.begin);
	File.Read(&offset,sizeof(int));
	while(File.GetPosition()<offset)
	{
		File.Read(&n,sizeof(short));
		File.Read(&x0,sizeof(float));
		File.Read(&y0,sizeof(float));
		File.Read(RecoCode,sizeof(char)*(n+1));
		AddOnePoint(x0,y0,RecoCode);
	}
	return TRUE;
}
//----------------------------------------
void CObjPointClass::SavePointFile(CString filename)// if use BOOL instead of void
{

	CFile File;
	CFileException e;
	if(!File.Open(filename,CFile::modeCreate|CFile::modeWrite,&e))
	{
		CString strErr;
		strErr.Format(_T("File could not be opened %d\n"),e.m_cause);//info to be used
	}
	short n;
	float x0,y0;
	Point *cpoint;
	short *nn = &n;
	float *x = &x0;
	float *y = &y0;
	int pointinfosize = GetAllPointSize()+sizeof(int);
	File.SeekToBegin();
	File.Write(&shpfileflag,sizeof(long));
	File.Write(&pointinfosize,sizeof(int));
	ForLoop(cpoint,HeadPoint)
	{
		x0 = cpoint->X0;
		y0 = cpoint->Y0;
		n = cpoint->CharNum; if(n<=0) continue;
		File.Write((char*)nn,sizeof(short));	
		File.Write((char*)x,sizeof(float));
		File.Write((char*)y,sizeof(float));	
		File.Write(cpoint->TextAdr,sizeof(char)*(n+1));
	}
	int ll = File.GetLength();
	File.Close();

}
//------------------------------------
int CObjPointClass::GetAllPointSize()
{
	int xysize = sizeof(float);
	int charsize = 0;
	int pointsize = 0;
	Point *point = NULL;
	ForLoop(point,HeadPoint)
	{
		charsize = 1 + point->CharNum;
		pointsize += charsize + 2 * xysize + sizeof(short);
	}
	return pointsize;
}
//-----------------------------------------------
//screen printf when open file not click leftmouse
//-----------------------------------------------
void CObjPointClass::PaintMyPointFunc(CDC *dc)
{
	Point *cpoint;
	CPen blackpen(PS_SOLID,1,RGB(0,0,0));
	dc->SelectObject(blackpen);
	ForLoop(cpoint,HeadPoint)
	{
		DrawOnePoint(dc,cpoint->X0,cpoint->Y0,cpoint->CharNum,cpoint->TextAdr);
	}
}
void CObjPointClass::DrawOnePoint(CDC *dc,int x0,int y0,int len,char*text)
{
	CBrush brush;
	CString str(text);
	brush.CreateSolidBrush(RGB(120,255,200));
	dc->SelectObject(&brush);
	dc->MoveTo(x0-5,y0);dc->LineTo(x0+5,y0);
	dc->MoveTo(x0,y0-5);dc->LineTo(x0,y0+5);
	dc->TextOut(x0+6,y0,str);
}



