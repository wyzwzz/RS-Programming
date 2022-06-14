#include"stdafx.h"
#include"MyFunc.h"
#include"Ins_Curve.h"

#pragma once

//-----------------------------------------------
//input and output
//-----------------------------------------------
BOOL CObjCurveClass::LoadCurveFunc(CString filename)
{
	CFile File;
	CFileException e;
	if(!File.Open(filename,CFile::modeRead,&e))
	{
		CString strErr;
		strErr.Format(_T("File could not be opened %d\n"),e.m_cause);
		return FALSE;
	}
	short n;
	float x0,y0;
	int offset = 0;
	if(shpfileflag == FILE_CURVE_FLAG)
		File.Seek(sizeof(long)+sizeof(int),File.begin);
	else if(shpfileflag == FILE_PANDC_FLAG)
	{
		File.Seek(sizeof(long),File.begin);
		File.Read(&offset,sizeof(int));
		File.Seek(sizeof(long)+offset+sizeof(int),File.begin);
	}
	while(File.GetPosition() != File.GetLength())
	{
		File.Read(&n,sizeof(short));
		if(n <= 0) continue;
		File.Read(TablCoorAdr,2*n*sizeof(float));
		AddOneCurve(n,TablCoorAdr);
	}
	File.Close();
	return TRUE;
}
//---------------------
BOOL CObjCurveClass::SaveCurveFunc(CString filename)
{
	CFile File;
	CFileException e;
	if(!File.Open(filename,CFile::modeWrite,&e))
	{
		if(!File.Open(filename,CFile::modeCreate|CFile::modeWrite,&e))
		{
			CString strErr;
			strErr.Format(_T("File could not be opened %d\n"),e.m_cause);
			return FALSE;
		}
	}
	short n;
	float x0,y0;
	Curve *curve;
	int l = File.GetLength();
	int curvesize = GetAllCurveSize()+sizeof(int);
	File.SeekToBegin();
	File.Write(&shpfileflag,sizeof(long));
	File.SeekToEnd();
	File.Write(&curvesize,sizeof(int));
	ForLoop(curve,HeadCurve)
	{
		n = curve->NodeNum; if(n <= 0) continue;
		File.Write(&n,sizeof(short));
		File.Write(curve->CoorAdr,sizeof(float) * 2 * n);
	}
	File.Close();
	return TRUE;
}
//--------------------------
int CObjCurveClass::GetAllCurveSize()
{
	int CPsize = sizeof(short);
	int fsize = sizeof(float);
	int allsize = 0;
	Curve * curve;
	ForLoop(curve,HeadCurve)
	{
		allsize += CPsize + (curve->NodeNum) * 2 * fsize;
	}
	return allsize;
}
//---------------------------------------------------------
//list operation
//---------------------------------------------------------
Curve* CObjCurveClass::AddOneCurve(int np,float* cooradr)
{
	if(np < 2) return NULL;//one point situation to consider
	float *pcoor = new float[np*2]; if(!pcoor) return NULL;
	memset(pcoor,0,np*2*sizeof(float));
	Curve *curve = new Curve[1]; if(!curve){ delete pcoor; return NULL; }
	memset(curve,0,sizeof(Curve));
	if(!HeadCurve) HeadCurve = curve;
	else           EndCurve->Next = curve;
	EndCurve = curve;
	curve->NodeNum = np;
	curve->CoorAdr = pcoor; memcpy_s(pcoor,np * 2 * sizeof(float),cooradr,sizeof(float) * np * 2);
	CurveNum++;
	return curve;
}
//--------------------------------
void CObjCurveClass::DeleteCurveFunc()
{
	Curve *curve;
	Curve *dcurve = NULL;
	ForLoop(curve,HeadCurve)
	{
		if(curve->CoorAdr) delete curve->CoorAdr;curve->CoorAdr = NULL;
		if(dcurve)delete[] dcurve;dcurve = curve;
	}
	if(dcurve) delete[] dcurve;dcurve = NULL;
	CurveNum = 0;
	HeadCurve = NULL;
	EndCurve  = NULL;
}
//----------------------------------------------------------------------
//screen printf
//----------------------------------------------------------------------
void CObjCurveClass::DrawOneCurve(CDC* dc,int nn,float *pcoor)
{
	if(nn < 2) return;
	float x0 = *pcoor;
	float y0 = *(pcoor + 1);
	dc->MoveTo(x0,y0);
	for( int i = 1; i < nn; i++ )
	{
		x0 = *(pcoor + 2 * i);
		y0 = *(pcoor + 2 * i + 1);
		dc->LineTo(x0,y0);
	}
}
//---------------------------------------------------
void CObjCurveClass::PaintMyCurveFunc(CDC* dc)
{
	Curve *curve;
	ForLoop(curve,HeadCurve)
		DrawOneCurve(dc,curve->NodeNum,curve->CoorAdr);
}


