#include"stdafx.h"
#include"MyFunc.h"
#include"Read_Bmp.h"
#include"MFCRSDlg.h"
#include"targetver.h"
#include<math.h>
#include<cstdlib>
#pragma once

//---------------------------------------------------------------------------------------
//-----------------------------�ڴ�ռ䴦��----------------------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::Gene_Char_RSImg_MemorySpace(int Height,int Width,RSImg *rsimg)//�����ڴ�
{
	if(!rsimg) return FALSE;
	rsimg->Img_H0 = Height;//�˴���ֵ�ǵ�һ�θ�ֵ��mrsimg
	rsimg->Img_W0 = Width;
	rsimg->Rimg = Gene_Char_MatrixMemorySpace(Height,Width); if(!rsimg->Rimg) return FALSE;
	rsimg->Gimg = Gene_Char_MatrixMemorySpace(Height,Width); if(!rsimg->Gimg) return FALSE;
	rsimg->Bimg = Gene_Char_MatrixMemorySpace(Height,Width); if(!rsimg->Bimg) return FALSE;
	rsimg->Mimg = Gene_Char_MatrixMemorySpace(Height,Width); if(!rsimg->Mimg) return FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------------------
UCHAR** CObjRSImgClass::Gene_Char_MatrixMemorySpace(int Height,int Width,BOOL *pGeneBol)//�����ڴ�
{
	UCHAR **pData = new UCHAR*[Height]; if(!pData){ *pGeneBol = FALSE; return NULL; }//�ȷ���ÿ�е���ͷ(height)������һ�У��ٷ���ÿ�е�����(width)
	memset(pData,0,Height*sizeof(UCHAR*));
	UCHAR *pdata;
	for(int i = 0; i < Height; i++)
	{
		pdata = new UCHAR[Width];if(!pdata){ *pGeneBol = FALSE; }
		memset(pdata,0,Width*sizeof(UCHAR));
		*(pData+i) = pdata;
	}
	return pData;//���ڴ����ĳɹ����������pGenBol�У�����������ʧ�ܣ�û�л���delete֮ǰ����ɹ��Ŀռ�
}
//---------------------------------------------------------------------------------------
UCHAR** CObjRSImgClass::Gene_Char_MatrixMemorySpace(int Height,int Width)//����ռ�
{
	BOOL GeneBol = TRUE;
	UCHAR** pData = Gene_Char_MatrixMemorySpace(Height,Width,&GeneBol);
	if(!GeneBol)
	{
		Delete_Char_MatrixMemorySpace(Height,pData);
		return NULL;
	}
	return pData;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Delete_Char_MatrixMemorySpace(int Height,UCHAR** pData)//�ͷſռ�
{
	if(!pData) return;
	UCHAR *pdata;
	for(int i = 0; i < Height; i++)
	{
		pdata = pData[i];
		if(pdata)
			delete[] pdata;
		pData[i] = NULL;
	}
	delete[] pData;
	pData = NULL;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Delete_Char_RSImg_MemorySpace(RSImg *rsimg)//�ͷſռ�
{
	if(!rsimg) return;
	if(rsimg->Rimg) Delete_Char_MatrixMemorySpace(rsimg->Img_H0,rsimg->Rimg); rsimg->Rimg=NULL;
	if(rsimg->Gimg) Delete_Char_MatrixMemorySpace(rsimg->Img_H0,rsimg->Gimg); rsimg->Gimg=NULL;
	if(rsimg->Bimg) Delete_Char_MatrixMemorySpace(rsimg->Img_H0,rsimg->Bimg); rsimg->Bimg=NULL;
	if(rsimg->Mimg) Delete_Char_MatrixMemorySpace(rsimg->Img_H0,rsimg->Mimg); rsimg->Mimg=NULL;
	rsimg->Img_H0 = 0;
	rsimg->Img_W0 = 0;
}
//---------------------------------------------------------------------------------------
long** CObjRSImgClass::Gene_Long_MatrixMemorySpace(int Height,int Width,BOOL *pGeneBol)
{
	long **pData = new long*[Height];if(!pData){ pGeneBol = FALSE; return NULL; }
	memset(pData,0,Height*sizeof(long*));
	long* pdata0;
	for(int jj = 0; jj < Height; jj++)
	{
		pdata0 = new long[Width]; if(!pdata0){ *pGeneBol = FALSE; break; }
		memset(pdata0,0,Width*sizeof(long));
		*(pData+jj) = pdata0;
	}
	return pData;
}
//---------------------------------------------------------------------------------------
long** CObjRSImgClass::Gene_Long_MatrixMemorySpace(int Height,int Width)
{
	BOOL GeneBol = TRUE;
	long** pData = Gene_Long_MatrixMemorySpace(Height,Width,&GeneBol);
	if(!GeneBol)
	{
		Delete_Long_MatrixMemorySpace(Height,pData);
		return NULL;
	}
	return pData;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Delete_Long_MatrixMemorySpace(int Height,long** pData)
{
	if(!pData) return;
	long* pdata0;
	for(int jj = 0; jj < Height; jj++)
	{
		pdata0 = pData[jj];
		if(pdata0) delete[] pdata0;
		pData[jj] = NULL;
	}
	delete[] pData; pData = NULL;
	return;
}
//---------------------------------------------------------------------------------------
//----------------------------------------����Ӱ��---------------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::Read000RSImgFile_Func(const char * filename,int *pImgW,int *pImgH,char *Rname,char *Gname,char *Bname,char *Mname,int *Rmin,int *Rmax,int *Gmin,int *Gmax,int *Bmin,int *Bmax,int *Mmin,int *Mmax)
{
	if(!PathFileExists(filename)) return FALSE;
	FILE *fio; fopen_s(&fio,filename,"r");
	if(!fio) return FALSE;
	strcpy_s(Rname,5*sizeof(char),"null");//��ʼ��Ϊnull��һ����M����û��
	strcpy_s(Gname,5*sizeof(char),"null");
	strcpy_s(Bname,5*sizeof(char),"null");
	strcpy_s(Mname,5*sizeof(char),"null");
	int n;
	BOOL LoadWHBol = FALSE;
	int index = 0;
	while(TRUE)
	{
		if(feof(fio)) break; //������000�ļ���ĩβʱ�˳�ѭ��
		fgets(ReadStr,500,fio); if(ReadStr[0] == ';' || ReadStr[0] == '//') continue; //ע��000�ļ��п�����'//'��';'����������ע�ͣ�����
		n = SsToStr60(ReadStr,SS_Dat); //�������һ�е��ַ�����ϣ���һЩ�ַ�������ϵ��ַ�����MyFunc��SsToStr60��������ʵ�֣�n����Ч�ַ����ĸ���
		if( n < 2 ) continue; //n����Ҫ���ڵ���3
		strcpy_s(RecoCode,20,SS_Dat[0]); _strupr_s(RecoCode,20*sizeof(char));//ÿ�еĵ�һ���ַ���ȫ��ת��Ϊ��д������ÿ�еĵ�һ���ַ����Ǳ�־��������д洢�����ݲ�������
		if(strstr(RecoCode,"IMAGEWH"))
		{
			if( n < 3 ) continue;
			*pImgW = atoi(SS_Dat[1]);
			*pImgH = atoi(SS_Dat[2]);
			LoadWHBol = TRUE; //�ɹ�����000�ļ���width��height
		}
		else if(strstr(RecoCode,"ETM_BAND"))
		{
			if( n < 2 ) continue;
			if(index == 0){ strcpy_s(Rname,50,SS_Dat[5]); *Rmin = atoi(SS_Dat[3]); *Rmax = atoi(SS_Dat[4]); }
			else if(index == 1){ strcpy_s(Gname,50,SS_Dat[5]); *Gmin = atoi(SS_Dat[3]); *Gmax = atoi(SS_Dat[4]); }
			else if(index == 2){ strcpy_s(Bname,50,SS_Dat[5]); *Bmin = atoi(SS_Dat[3]); *Bmax = atoi(SS_Dat[4]); }
			else if(index == 3){ strcpy_s(Mname,50,SS_Dat[5]); *Mmin = atoi(SS_Dat[3]); *Mmax = atoi(SS_Dat[4]); }
			index++;
		}
	}
	fclose(fio);
	if(!LoadWHBol || index<3) return FALSE;//������width��height��RGB����TRUE
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadRSFileData_Func(RSImg *rsimg,const char *B000name,int Width,int Height,char *Rname,char *Gname,char *Bname,char *Mname,BOOL DownBol)
{
	if(!rsimg) return FALSE;
	Delete_Char_RSImg_MemorySpace(rsimg);                                               //���ڶ��δ�000�ļ�Я����img�ļ�����ɾ��ԭ����Ӱ��ռ�
	BOOL wbol = Gene_Char_RSImg_MemorySpace(Height,Width,rsimg); if(!wbol) return FALSE;//�����ڴ�����Ӱ��ռ䣬����width��height��ֵ��mrsimg�е�Img_X��Img_Y
	rsimg->InMBol = FALSE;
	//����Ӱ�񵽷�����ڴ���
	_splitpath(B000name,Ipdri,Ipdir,Ipname,Ipext);//���·����������000�ļ���·���ҵ�RGBM��·��
	fnmerge(PathFileName,Ipdri,Ipdir,Rname,"");//�˺���VSû���ṩ���Լ���MyFunc��ʵ��
	if(!PathFileExistsA(PathFileName)) return FALSE;
	wbol = ReadRS_Data_UCHAR_Func(PathFileName,rsimg->Img_H0,rsimg->Img_W0,rsimg->Rimg,DownBol); if(!wbol) return FALSE;

	_splitpath(B000name,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Gname,"");
	if(!PathFileExistsA(PathFileName)) return FALSE;
	wbol = ReadRS_Data_UCHAR_Func(PathFileName,rsimg->Img_H0,rsimg->Img_W0,rsimg->Gimg,DownBol); if(!wbol) return FALSE;

	_splitpath(B000name,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Bname,"");
	if(!PathFileExistsA(PathFileName)) return FALSE;
	wbol = ReadRS_Data_UCHAR_Func(PathFileName,rsimg->Img_H0,rsimg->Img_W0,rsimg->Bimg,DownBol); if(!wbol) return FALSE;

	_splitpath(B000name,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Mname,"");
	if(!PathFileExistsA(PathFileName)) 
	{ wbol = ReadRS_Data_UCHAR_Func(PathFileName,rsimg->Img_H0,rsimg->Img_W0,rsimg->Mimg,DownBol); if(wbol) rsimg->InMBol=TRUE; }
	//��һ����ʧ�ܺ�û�п���ɾ������Ŀռ䣬�ڴ˴�
	//ͳ��Ӱ������ֱ��ͼ��Ӱ�����ɹ����ͳ��
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Rimg,mrsimg->factR,&mrsimg->InRmin,&mrsimg->InRmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Gimg,mrsimg->factG,&mrsimg->InGmin,&mrsimg->InGmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Bimg,mrsimg->factB,&mrsimg->InBmin,&mrsimg->InBmax);
	if(mrsimg->InMBol)
		Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Mimg,mrsimg->factM,&mrsimg->InMmin,&mrsimg->InMmax);
	mrsimg->ModifyBol = TRUE;
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadRS_Data_UCHAR_Func(const char* filename,int Height,int Width,UCHAR **pData,BOOL DownBol)
{
	FILE *fio; fopen_s(&fio,filename,"rb"); if(!fio) return FALSE;
	BOOL wbol = ReadRS_Data_UCHAR_Func(fio,Height,Width,pData,DownBol);
	fclose(fio);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadRS_Data_UCHAR_Func(FILE *fio,int Height,int Width,UCHAR **pData,BOOL DownBol)
{
	UCHAR *pdata;
	long lnk;
	for(int j = 0; j < Height; j++)
	{
		lnk = j*Width;
		fseek(fio,lnk,SEEK_SET); if(feof(fio)) return FALSE;
		if(DownBol) pdata=pData[j];
		else pdata = pData[Height-1-j];//imgͼ��Ĭ�ϴ������ϴ洢 ����ͼ����ʾ�ĺ�����Ҫ������
		fread_s(pdata,sizeof(UCHAR)*Width,sizeof(UCHAR),Width,fio);
	}
	return TRUE;
}
//---------------------------------------------------------------------------------------
//-------------------------------������ʾ�ڴ�ͼ�������----------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::GencRemoteImgFunc(RSImg *rsimg,int bmpw,int bmph)
{
	if(!rsimg) return FALSE;
	if( bmpw < 10 || bmph < 10 ) return FALSE; //̫С�� ����������
	BOOL wbol;
	if(SetGrayBol) wbol = GencRemoGray_ImgFunc(rsimg,bmpw,bmph);
	else           wbol = GencRemoColorImgFunc(rsimg,bmpw,bmph);
	return wbol;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::GencRemoGray_ImgFunc(RSImg *rsimg,int bmpw,int bmph)
{
	if(!rsimg) return FALSE;
	if( bmpw < 10 || bmph < 10 ) return FALSE;
	if( rsimg->ModifyBol == FALSE ) return TRUE;//ModifyBol�����Ƿ���Ҫ������ʾ�ڴ�Ŀ��ٺ����ݴ��
	UCHAR **Mimg = rsimg->Rimg; if(rsimg->InMBol) Mimg = rsimg->Mimg; if(!Mimg) return FALSE;
	int ImgH0 = rsimg->Img_H0;//ͼ�����ݵ�ʵ��H W
	int ImgW0 = rsimg->Img_W0;
	double tx = ImgW0 / (float)bmpw;
	double ty = ImgH0 / (float)bmph;
	double Ra;
	if(tx>ty) { Ra = tx; bmph = ImgH0 / tx;}//���������ִ�к���ʾ�ĳ������ԭʼͼ���һ�������������ʾ��͸߻ᱻ�����������������������Ч��
	else      { Ra = ty; bmpw = ImgW0 / ty; }
	int nWidth = bmpw; nWidth = (nWidth+3) / 4 * 4;//��ʾ��ȵ���Ϊ4�ı�������ȡ�죬����˵��ʾ����Ҫ��
	int nHeight = bmph;
	//nWidth��nHeight�ǵ�����ɱ����ĳ��Ϳ�
	AllocFastData(rsimg,nHeight,nWidth); if(!rsimg->FastData) return FALSE; //������ʾ�ڴ�
	
	int ii,jj,jk,ik;
	int Roff;
	float Rsal;
	if(rsimg->InMBol){ if(rsimg->EnMmin > rsimg->EnMmax){ ik = rsimg->EnMmax; rsimg->EnMmax = rsimg->EnMmin; rsimg->EnMmin = ik; } Roff = rsimg->EnMmin; Rsal = rsimg->EnMmax - rsimg->EnMmin ;}
	else             { if(rsimg->EnRmin > rsimg->EnRmax){ ik = rsimg->EnRmax; rsimg->EnRmax = rsimg->EnRmin; rsimg->EnRmin = ik; } Roff = rsimg->EnRmin; Rsal = rsimg->EnRmax - rsimg->EnRmin ;}
	if( Rsal > EPSILON ) Rsal = 255.0 / Rsal; else Rsal = 1;//�˴�����EnMmin��EnMmax�ĸ����С����Ϊֱ��ͼͳ��ʱ��������Ĳ��淶������˵ֱ��ͼ����û�м�������

	UCHAR *sMimg;
	int sr;
	for(jj = 0; jj < rsimg->FastHeight; jj++)
	{
		printf("%d ",jj);
		jk = jj * Ra; if( jk < 0 || jk >= ImgH0 ) continue; jk = ImgH0 - jk - 1;//ע������һ�п�ʼ��ȡ����
		sMimg = Mimg[jk];
		for(ii = 0; ii < rsimg->FastWidth; ii++)
		{
			ik = ii * Ra; if( ik < 0 || ik >= ImgW0 ) continue;
			sr = sMimg[ik];
			//sr=BilinearInterpolation(ik,jk,rsimg->Img_W0,rsimg->Img_H0,Mimg,rsimg->Img_B0);
			sr = (sr-Roff) * Rsal; if( sr < 0 ) sr = 0; else if( sr > 255 ) sr = 255;//(sr-min)*(255/(max-min)) �����������죬��ͼ�������ת�Ƶ�0-255��Χ
			long off = ( jj * rsimg->FastWidth + ii ) * 3;//RGB��������һ���ϳɻҶ�ͼ��
			*(rsimg->FastData+off+0) = sr;
			*(rsimg->FastData+off+1) = sr;
			*(rsimg->FastData+off+2) = sr;
		}
	}
	rsimg->ModifyBol = FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::GencRemoColorImgFunc(RSImg *rsimg,int bmpw,int bmph)
{
	if(!rsimg) return FALSE; if( bmpw < 10 || bmph < 10 ) return FALSE; if( rsimg->ModifyBol == FALSE ) return TRUE;
	if( !rsimg->Rimg || !rsimg->Gimg || !rsimg->Bimg ) return FALSE;
	int ImgH0 = rsimg->Img_H0;
	int ImgW0 = rsimg->Img_W0;
	double tx = ImgW0 / (float)bmpw;
	double ty = ImgH0 / (float)bmph;
	double Ra;
	if( tx > ty ){ Ra = tx; bmph = ImgH0 / tx; }
	else     { Ra = ty; bmpw = ImgW0 / ty; }
	int nWidth = bmpw; nWidth = (nWidth+3) / 4 * 4;
	int nHeight = bmph;
	//---------------------------------------------------------------------------------------------
	AllocFastData(rsimg,nHeight,nWidth); if(!rsimg->FastData) return FALSE;

	int ii,jj,jk,ik;
	if( rsimg->EnRmin > rsimg->EnRmax ){ ik = rsimg->EnRmax; rsimg->EnRmax = rsimg->EnRmin; rsimg->EnRmin = ik; }
	if( rsimg->EnGmin > rsimg->EnGmax ){ ik = rsimg->EnGmax; rsimg->EnGmax = rsimg->EnGmin; rsimg->EnGmin = ik; }
	if( rsimg->EnBmin > rsimg->EnBmax ){ ik = rsimg->EnBmax; rsimg->EnBmax = rsimg->EnBmin; rsimg->EnBmin = ik; }
	int Roff = rsimg->EnRmin;
	int Goff = rsimg->EnGmin;
	int Boff = rsimg->EnBmin;
	float Rsal = rsimg->EnRmax-rsimg->EnRmin; if(Rsal>EPSILON) Rsal = 255.0 / Rsal; else Rsal = 1;//�������ε���С�����ֵ��ͬ����������������Ϊʲô������ͼ����ȥɫ���ǶԵ�
	float Gsal = rsimg->EnRmax-rsimg->EnRmin; if(Gsal>EPSILON) Gsal = 255.0 / Gsal; else Gsal = 1;//��Ϊһ��ʼ��000�ļ��ж�ȡ��0��255
	float Bsal = rsimg->EnRmax-rsimg->EnRmin; if(Bsal>EPSILON) Bsal = 255.0 / Bsal; else Bsal = 1;
	UCHAR *sRimg;
	UCHAR *sGimg;
	UCHAR *sBimg;
	int sr,sg,sb;
	for( jj = 0; jj < rsimg->FastHeight; jj++)
	{
		jk = jj * Ra; if( jk < 0 || jk >= ImgH0 ) continue; jk = ImgH0-jk-1;
		sRimg = rsimg->Rimg[jk];
		sGimg = rsimg->Gimg[jk];
		sBimg = rsimg->Bimg[jk];
		for( ii = 0; ii < rsimg->FastWidth; ii++)
		{
			ik = ii * Ra; if( ik < 0 || ik >= ImgW0 ) continue;
			sr = sRimg[ik];
			sg = sGimg[ik];
			sb = sBimg[ik];
//			sr=BilinearInterpolation(ik,jk,rsimg->Img_W0,rsimg->Img_H0,rsimg->Rimg,rsimg->Img_B0);
//			sg=BilinearInterpolation(ik,jk,rsimg->Img_W0,rsimg->Img_H0,rsimg->Gimg,rsimg->Img_B0);
//			sb=BilinearInterpolation(ik,jk,rsimg->Img_W0,rsimg->Img_H0,rsimg->Bimg,rsimg->Img_B0);
			sr = (sr-Roff) * Rsal; if( sr < 0 ) sr = 0; else if( sr > 255 ) sr = 255;
			sg = (sg-Goff) * Gsal; if( sg < 0 ) sg = 0; else if( sg > 255 ) sg = 255;
			sb = (sb-Boff) * Bsal; if( sb < 0 ) sb = 0; else if( sb > 255 ) sb = 255;

			long off = ( jj * rsimg->FastWidth + ii ) * 3;
			*(rsimg->FastData+off+0) = sb;//ע��RGB�洢˳��
			*(rsimg->FastData+off+1) = sg;
			*(rsimg->FastData+off+2) = sr;
		}	
	}
	rsimg->ModifyBol = FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::AllocFastData(RSImg *rsimg,int nHeight,int nWidth)
{
	if(!rsimg) return; if( !nWidth || !nHeight ) return;
	if( nWidth == rsimg->FastWidth && nHeight == rsimg->FastHeight ) return;//һ��ʼmemset0 FastWidth��FastHeight����0�����nWidth��nHeight��������ȣ�˵���Ѿ��������Сһ���Ŀռ��ˣ�����Ҫ��
	DeleteFastData(rsimg);//ɾ��ԭ����FastData
	long lp8 = nHeight * nWidth * 3;//��Ϊ��ʾ�ĳ��Ϳ����ر�󣬲���ң��������ô�ܴ󣬿���ֱ�ӿ��������Ŀռ䣬���÷��п�
	rsimg->FastData = new char[lp8]; if(!rsimg->FastData) return;
	memset(rsimg->FastData,0,sizeof(char) * lp8);
	rsimg->FastLen    = lp8;//FastLen�Ƿ���ռ�Ĵ�С��FastLen=3*nWidth*nHeight
	rsimg->FastWidth  = nWidth;
	rsimg->FastHeight = nHeight;
	Init_BitMap_Info(nWidth,nHeight,3,&rsimg->FastInfo);//��ʼ��bmp��ʾ����Ҫ����Ϣ
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::DeleteFastData(RSImg* rsimg)
{
	if(!rsimg) return;
	if(rsimg->FastData) delete[] rsimg->FastData;
	rsimg->FastData   = NULL;
	rsimg->FastLen    = 0;
	rsimg->FastWidth  = 0;
	rsimg->FastHeight = 0;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Init_BitMap_Info(int nWidth,int nHeight,int colorbit,BITMAPINFO *pinfo)
{
	DWORD n4 = ((DWORD)nWidth * colorbit) / 4;
	if( ((DWORD)nWidth*colorbit) % 4 ) n4++; n4 *= 4;//�趨��Ϊ4�ı���
	ULONG lk = nHeight; lk *= n4;
	pinfo->bmiHeader.biSize          = 40;
	pinfo->bmiHeader.biWidth         = nWidth;
	pinfo->bmiHeader.biHeight        = nHeight;
	pinfo->bmiHeader.biPlanes        = 1;
	pinfo->bmiHeader.biBitCount      = colorbit * 8;
	pinfo->bmiHeader.biCompression   = BI_RGB;
	pinfo->bmiHeader.biSizeImage     = lk;
	pinfo->bmiHeader.biXPelsPerMeter = 0;
	pinfo->bmiHeader.biYPelsPerMeter = 0;
	pinfo->bmiHeader.biClrImportant  = 0;
	pinfo->bmiHeader.biClrUsed       = 0;
	return;
}
//---------------------------------------------------------------------------------------
//---------------------------------------Ӱ����ʾ----------------------------------------
//---------------------------------------------------------------------------------------
CObjRSImgClass::CObjRSImgClass(int x)
{
	istostart = TRUE;//��Ҫ������һ�������У���bmpͼ������������ʱ��ȷ����һ�ε�����ɺ��ٽ�����һ�ε���
}
void CObjRSImgClass::PaintRemoteImgFunc(CDC* dc,RSImg* rsimg)
{
	if( istostart == FALSE ) return;
	istostart = FALSE;
	if(!rsimg){ istostart = TRUE; return; }
	if(!rsimg->FastData){ istostart = TRUE; return; }
	int offX   = rsimg->ImgXoff;
	int offY   = rsimg->ImgYoff;
	int Width  = rsimg->FastWidth;
	int Height = rsimg->FastHeight;
	//---------------------------------------------------------
	::StretchDIBits(dc->GetSafeHdc(),offX,offY,Width,Height,0,0,Width,Height,(void*)rsimg->FastData,&rsimg->FastInfo,DIB_RGB_COLORS,SRCCOPY);
	//Ҫ������4��������
	istostart = TRUE;
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Plot_Etm_Bitmap_By_Fast_File(CDC* dc,int xL,int yL,int bW0,int bH0,char* pdata,int CopyMode)
{
	//�˺�������һ�޷���ʾʱ������ʾ�Ķ����ŵ��ļ����ٴ���ʾ������Ŀǰδ������������
	strcpy_s(PathFileName,sizeof(char)*12,"D:\\111.bmp");
	WriteToBmpFile(PathFileName,bW0,bH0,pdata);
	CBitmap *pbitmap = new CBitmap; if(!pbitmap) return;
	pbitmap->LoadBitmapA(PathFileName);
	dc->SetROP2(CopyMode);
	dc->BitBlt(0,0,bW0,bH0,dc,0,0,SRCCOPY);
	dc->SetROP2(0);
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::WriteToBmpFile(const char* filename,int nWidth,int nHeight,char* lpBits)
{
	if(!lpBits) return FALSE;
	FILE *fou; fopen_s(&fou,filename,"wb"); if(!fou) return FALSE;
	Init_BitMap_FileHeader(nWidth,nHeight,3,&bmfh,&bmih);
	fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fou);
	fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,fou);
	fseek(fou,bmfh.bfOffBits,SEEK_SET);
	fwrite(lpBits,sizeof(char),bmih.biSizeImage,fou);
	fclose(fou);
	return TRUE;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Init_BitMap_FileHeader(int nWidth,int nHeight,int colorbit,BITMAPFILEHEADER* pbmfh,BITMAPINFOHEADER* pbmih)
{
	DWORD n4 = ( (DWORD)nWidth * colorbit ) / 4; if( ((DWORD)nWidth * colorbit ) % 4 ) n4++; n4 *= 4;
	ULONG lk = nHeight; lk *= n4;
	int off = 14 + sizeof(BITMAPINFOHEADER);
	//-----------------------------
	pbmfh->bfType           = 0x4D42;
	pbmfh->bfSize          = 14 + lk;
	pbmfh->bfReserved1     = 0;
	pbmfh->bfReserved2     = 0;
	pbmfh->bfOffBits       = off;
	//-----------------------------
	pbmih->biSize          = 40;//sizeof(BITMAPINFOHEADER)
	pbmih->biWidth         = nWidth;
	pbmih->biHeight        = nHeight;
	pbmih->biPlanes        = 1;
	pbmih->biBitCount      = 8 * colorbit;
	pbmih->biCompression   = BI_RGB;
	pbmih->biSizeImage     = lk;
	pbmih->biXPelsPerMeter = 0;
	pbmih->biYPelsPerMeter = 0;
	pbmih->biClrImportant  = 0;
	pbmih->biClrUsed       = 0;
	return;
}
//---------------------------------------------------------------------------------------
//------------------------------------ֱ��ͼͳ��-----------------------------------------
//---------------------------------------------------------------------------------------
long CObjRSImgClass::Get_Data_UChar_Fact_Func(int ImgH0,int ImgW0,int ImgB0,UCHAR** pData,long* factData,UCHAR* fmin,UCHAR* fmax)
{
	if( !pData || !factData ) return 0;
	long ii,jj;
	for( ii = 0; ii < 256; ii++ ) 
		*(factData+ii) = 0;
	UCHAR *pdata0;
	UCHAR rg;
	int sk;
	long lnk = 0;//һ����ɫ���ܵĸ���
	for( jj = 0; jj < ImgH0; jj++ )
	{
		pdata0 = pData[jj];
		for( ii = 0; ii < ImgW0 ; ii++ )
		{
			rg = *(pdata0+ii);
			sk = rg;
			if( sk == ImgB0 ) continue;//����ֵ������ͳ��
			factData[rg]++; lnk++;
		}
	}
	*fmin = 0; *fmax = 255;
	for( ii = 0; ii < 256; ii++ ){ if(factData[ii] == 0 ) continue; *fmin = ii; break; }
	for( ii = 255; ii >= 0; ii-- ){ if(factData[ii] == 0 ) continue; *fmax = ii; break; }
	return lnk;//����lnk��������������ͨ��ѭ��factData[]�ۼӵ���ǰ���ֵ���ҳ�fmin��fmax������
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Get_RSNew_Band_Fact_Func(RSImg* mrsimg,int RGB_Min,int RGB_Max,float Min_Perc,float Max_Perc)//�Զ�����
{
	int sn1,sn2;
	if(mrsimg->Rimg)
		{ if(Get_RSNew_Band_Fact_Func(mrsimg->factR,RGB_Min,RGB_Max,Min_Perc,Max_Perc,&sn1,&sn2))
	    { mrsimg->EnRmin=sn1; mrsimg->EnRmax=sn2; } }
	if(mrsimg->Gimg)
		{ if(Get_RSNew_Band_Fact_Func(mrsimg->factG,RGB_Min,RGB_Max,Min_Perc,Max_Perc,&sn1,&sn2))
	    { mrsimg->EnGmin=sn1; mrsimg->EnGmax=sn2; } }
	if(mrsimg->Bimg)
		{ if(Get_RSNew_Band_Fact_Func(mrsimg->factB,RGB_Min,RGB_Max,Min_Perc,Max_Perc,&sn1,&sn2))
	    { mrsimg->EnBmin=sn1; mrsimg->EnBmax=sn2; } }
	if(mrsimg->Mimg && mrsimg->InMBol)
		{ if(Get_RSNew_Band_Fact_Func(mrsimg->factM,RGB_Min,RGB_Max,Min_Perc,Max_Perc,&sn1,&sn2))
	    { mrsimg->EnMmin=sn1; mrsimg->EnMmax=sn2; } }
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::Get_RSNew_Band_Fact_Func(long* pfact,int RGB_Min,int RGB_Max,float Min_Perc,float Max_Perc,int* psn1,int* psn2)
{
	int i,n1=255,n2=0;
	for( i = 0; i <= 255; i++) { if(*(pfact+i) == 0) continue; n1 = i; break; }
	for( i = 255; i >= 0; i--) { if(*(pfact+i) == 0) continue; n2 = i; break; }
	n1 += RGB_Min; n2 -= RGB_Max; //�˴���ȥ��������ֵ��Ӱ�죬һ�㱳��ֵΪ0��255��Ĭ�ϴ���RGB_Min��RGB_MaxΪ2
	if(n1 >= n2) return FALSE;
	long nk;
	double sum,dt;
	sum = 0; for( i = n1; i <= n2; i++) sum += *(pfact+i); if(sum < 1) return FALSE; dt = 100.0 / sum;//��ȥ����ֵ���������죬dt�Ǳ���ϵ����sum*dt��õ�ռ�İٷֱ�
	sum = 0; for( i = n1; i <= n2; i++) { nk = *(pfact+i); sum += nk * dt; if(sum < Min_Perc) continue; n1 = i; break; }
	sum = 0; for( i = n2; i >= n1; i--) { nk = *(pfact+i); sum += nk * dt; if(sum < Max_Perc) continue; n2 = i; break; }
	*psn1=n1;
	*psn2=n2;
	return TRUE;
} 
//---------------------------------------------------------------------------------------
//------------------------------------��άɢ��ͼ-----------------------------------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::FillScatDiagramFunc(CDC* dc,int ZoomW,int ZoomH,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax,BOOL SmooBol,int nr0)
{
	if( !Limg || !Bimg || ZoomH < 10 || ZoomW < 10 ) return;
	long fmin = 0;
	long fmax = 1;
	double Avg = 0;
	double S0 = 1;
	long** XYimg = Gene_Long_MatrixMemorySpace(ZoomH,ZoomW); if(!XYimg) return;//���ռ䣬��СΪ�ؼ۴��ڵĴ�С����long����
	CalcFreqDistZoomFunc(ZoomW,ZoomH,XYimg,ImgW0,ImgH0,ImgB0,Limg,Bimg,LefMin,LefMax,BotMin,BotMax,SmooBol,nr0);//���㴰�ڵ�ķֲ�Ƶ�ʣ�������ɫ����ɫ�����ܶ�
	long npoin = Get_Long_Min_Max_Avg_S0_Func(ZoomH,ZoomW,0,XYimg,&fmin,&fmax,&Avg,&S0);//���ص�ĸ������ƺ�֮��ûʲô�ã������ж�һ��
	UCHAR** Ldata;
	UCHAR** Bdata;
	long* xyimg;
	float icDt;
	int ii,jj,ic;
	//float Dx = (BotMax - BotMin) / ZoomW;
	//float Dy = (LefMax - LefMin) / ZoomH;
	if( npoin > 1 )
	{
		if( fmax > Avg + 2 * S0 ) fmax = Avg + 2 * S0; //�������䣿����
		//float temp=(LefMax-LefMin)>(BotMax-BotMin)?(LefMax-LefMin):(BotMax-BotMin);
		icDt = 255.0 / fmax;//255��������256��  ��һ����������ɫ�ֲ�ͳһ��0-255֮�䣬��ΪƵ�ʿ��ܴ���255��������ɫֻ��256��
		
		for( jj = 0; jj < ZoomH; jj++ )
		{
			xyimg = XYimg[jj];
			for( ii = 0; ii < ZoomW; ii++ )
			{
				ic = xyimg[ii];
				ic = ic * icDt; if( ic <= 0 ) continue; if( ic > 255 ) ic = 255;
				dc->SetPixel(ii,ZoomH-jj,CustColors[ic]);//CustColors������MyFunc�У��Ѿ����ȶ�������ɫ�ļ�
			}
		}
	}
	Delete_Long_MatrixMemorySpace(ZoomH,XYimg);//���տռ�
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::PlotScatDiagramFunc(CDC* dc,int ZoomW,int ZoomH,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax)
{
	if( !Limg || !Bimg ||ZoomH < 10 || ZoomW < 10 ) return;
	int ii,jj;
	int xc,yc;
	UCHAR Lv,Bv;
	float Dx = (float)ZoomW / (BotMax - BotMin);//float Dx = ZoomW / 256;//��picture�ؼ��ĳ���ȷ�Ϊ256������ӦUCHAR256����ɫ
	float Dy = (float)ZoomH / (LefMax - LefMin);//float Dy = ZoomH / 256;
	int IDx = ImgW0 / ZoomW; if( IDx < 1 ) IDx = 1;//����picture�ؼ��ĳ�����ң��ͼ��ÿ��һ�������ȡһ�����㣬��Ϊͼ�������ʾ�Ŀؼ���С����Ϣ�������ʾЧ����һ���ģ�����ѭ���ӿ��ٶ�
	int IDy = ImgH0 / ZoomH; if( IDy < 1 ) IDy = 1;
	UCHAR *Ldata,*Bdata;
	for( jj = 0; jj < ImgH0; jj += IDy )
	{
		Ldata = Limg[jj];
		Bdata = Bimg[jj];
		for( ii = 0; ii < ImgW0; ii += IDx )
		{
			Lv = Ldata[ii]; if( Lv == ImgB0 ) continue;
			Bv = Bdata[ii]; if( Bv == ImgB0 ) continue;
			yc = (Lv - LefMin) * Dy;
			xc = (Bv - BotMin) * Dx;
			dc->SetPixel(xc,ZoomH-yc,RGB(0,0,0));
			//Draw_One_Cross(dc,xc,ZoomH-yc,1);
		}
		//���ɫ���ܴ���ɫ�Ļ�Ҫ�¿��ռ�ͳ��ÿ������ܶȣ�������ɫ���ã��򵥺ܶ�
	}
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::CalcFreqDistZoomFunc(int ZoomW,int ZoomH,long** XYimg,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax,BOOL SmooBol,int nr0)
{
	if( !Limg || !Bimg || ZoomH < 10 || ZoomW < 10 ) return;
	int ii,jj;
	int xc,yc;
	UCHAR Lv,Bv;
	float Dx = (float)ZoomW / (BotMax - BotMin);//float Dx = ZoomW / 256.0;
	float Dy = (float)ZoomH / (LefMax - LefMin);//float Dy = ZoomH / 256.0;
	UCHAR *Ldata;
	UCHAR *Bdata;
	long *xyimg;
	for( jj = 0; jj < ImgH0; jj++ )//ע�����ｫ���еĵ����Ϣ��ͳ�ƽ���
	{
		Ldata = Limg[jj];
		Bdata = Bimg[jj];
		for( ii = 0; ii < ImgW0; ii++ )
		{
			Lv = Ldata[ii]; if( Lv == ImgB0 ) continue;
			Bv = Bdata[ii]; if( Bv == ImgB0 ) continue;
			yc = (Lv - LefMin) * Dy;//yc = Lv * Dy;
			xc = (Bv - BotMin) * Dx;//xc = Bv * Dx;
			//xyimg = XYimg[yc];//�ҵ���Ӧ���У����ҵ���Ӧ���У��ٽ���++
			//xyimg[xc]++;
			if(SmooBol)
			{
				int ny1 = yc - nr0; if(ny1 < 0) ny1 = 0;
				int ny2 = yc + nr0; if(ny2 > ZoomH-1) ny2 = ZoomH -1;
				int nx1 = xc - nr0; if(nx1 < 0) nx1 = 0;
				int nx2 = xc + nr0; if(nx2 > ZoomW-1) nx2 = ZoomW -1;
				for(int jr = ny1; jr <= ny2; jr++)
					for(int ir = nx1; ir <= nx2; ir++)
					{
						xyimg=XYimg[jr];
						xyimg[ir]++;
					}
			}
			else
			{
				if(yc < 0 || yc >= ZoomH)
					continue;
				if(xc <0 || xc >= ZoomW)
					continue;
				xyimg=XYimg[yc];
				xyimg[xc]++;
			}
		}
	}
}
//---------------------------------------------------------------------------------------
long CObjRSImgClass::Get_Long_Min_Max_Avg_S0_Func(int ImgH0,int ImgW0,int ImgB0,long** pData,long* pfmin,long* pfmax,double* pAvg,double* pS0)
{//��ȡͳ��������Сֵ�����ֵ����ֵ�ͷ���
	if(!pData) return 0;
	long **pdaHead = pData;
	long *pdata;
	long rg;
	int ii,jj;
	int fmin = 9999;
	int fmax = -fmin;
	double xv;
	double SmX = 0;
	double SmX2 = 0;
	long npoin = 0;
	for( jj = 0; jj < ImgH0; jj++)
	{
		pdata = pdaHead[jj]; if(!pdata) continue;
		for( ii = 0; ii < ImgW0; ii++)
		{
			rg = *(pdata+ii); if( rg == ImgB0 ) continue;
			if( rg < fmin ) fmin = rg;
			if( rg > fmax ) fmax = rg;
			xv = rg; SmX += xv; SmX2 += xv * xv;
			npoin++;
		}
	}
	if( npoin > 1 )
	{
		SmX = SmX / npoin; SmX2 = ( SmX2 - npoin * SmX * SmX ) / ( npoin - 1 );
		if( SmX2 < EPSILON ) SmX2 = 1;
		else SmX2 = sqrt(SmX2);
	}
	else if( npoin == 1 ) SmX2 = 1.0;
	else { fmin = 0; fmax = 0; SmX = 0; SmX2 = 1.0; }
	*pfmin = fmin; *pfmax = fmax; *pAvg = SmX; *pS0 = SmX2;
	return npoin;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Draw_One_Cross(CDC* dc,int x0,int y0,int len)//ʵ����ֱ����setpixel�������ص����ɫ
{
	dc->MoveTo(x0,y0-len); dc->LineTo(x0,y0+len);
	dc->MoveTo(x0-len,y0); dc->LineTo(x0+len,y0);
}
//---------------------------------------------------------------------------------------
//----------------------------------------��������---------------------------------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::SaltPepperNoiseGrayFunc(RSImg* mrsimg,int SaNum,int PeNum)//�Ҷ�ͼ����ӽ�������
{
	if(!mrsimg) return;
	UCHAR** Gimg = mrsimg->Rimg;
	if(mrsimg->InMBol) 
		Gimg = mrsimg->Mimg;
	if(!Gimg) return;
	SaltandPepperNoiseFunc(mrsimg->Img_W0,mrsimg->Img_H0,Gimg,SaNum,PeNum);

}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::SaltPepperNoiseColorFunc(RSImg* mrsig,int SaNum,int PeNum)//��ɫͼ����ӽ�������
{
	if(!mrsimg) return;
	UCHAR** Rimg = mrsimg->Rimg; if(!Rimg) return;
	UCHAR** Gimg = mrsimg->Gimg; if(!Gimg) return;
	UCHAR** Bimg = mrsimg->Bimg; if(!Bimg) return;
	SaltandPepperNoiseFunc(mrsimg->Img_W0,mrsimg->Img_H0,Rimg,Gimg,Bimg,SaNum,PeNum);
	if(mrsimg->InMBol)
	{
		Rimg = mrsimg->Mimg; if(!Rimg) return;
		SaltandPepperNoiseFunc(mrsimg->Img_W0,mrsimg->Img_H0,Rimg,SaNum,PeNum);
	}
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::SaltandPepperNoiseFunc(int ImgW0,int ImgH0,UCHAR** Hdata,int SaNum,int PeNum)
{
	if( ImgW0 < 10 || ImgH0 < 10 ) return;
	UCHAR* pdata;
	int ix,iy,nn;
	time_t sTime = time(NULL); srand((unsigned)time(&sTime));//�������������
	nn = SaNum;
	while( nn > 0 )
	{
		ix = rand() % ImgW0;
		iy = rand() % ImgH0;
		pdata = Hdata[iy];
		*( pdata + ix ) = 0;
		nn--;
	}
	sTime = time(NULL); srand((unsigned)time(&sTime));
	nn = PeNum;
	while( nn > 0 )//�������ɵİ׵�Ḳ�Ǻڵ�
	{
		ix = rand()%ImgW0;
		iy = rand()%ImgH0;
		pdata = Hdata[iy];
		*( pdata + ix ) = 255;
		nn--;
	}
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::SaltandPepperNoiseFunc(int ImgW0,int ImgH0,UCHAR** HdataR,UCHAR** HdataG,UCHAR** HdataB,int SaNum,int PeNum)
{
	if( ImgW0 < 10 || ImgH0 < 10 ) return;
	UCHAR* pdata;
	int ix,iy,nn;
	time_t sTime = time(NULL); srand((unsigned)time(&sTime));
	nn = SaNum;
	while( nn > 0 )
	{
		ix = rand() % ImgW0;
		iy = rand() % ImgH0;
		pdata = HdataR[iy]; *( pdata + ix ) = 0;//���ǲ�ɫͼ��ͬʱ���������������ͬλ�õ�����
		pdata = HdataG[iy]; *( pdata + ix ) = 0;
		pdata = HdataB[iy]; *( pdata + ix ) = 0;
		nn--;
	}
	sTime = time(NULL); srand((unsigned)time(&sTime));
	nn = PeNum;
	while( nn > 0 )
	{
		ix = rand() % ImgW0;
		iy = rand() % ImgH0;
		pdata = HdataR[iy];*( pdata + ix ) = 255;
		pdata = HdataG[iy];*( pdata + ix ) = 255;
		pdata = HdataB[iy];*( pdata + ix ) = 255;
		nn--;
	}
	return;
}
//---------------------------------------------------------------------------------------
//-------------------------------------��˹����------------------------------------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::AddGussianNoiseGray(RSImg* mrsimg,double mu,double sigma,int k)//muΪ��ֵ��sigmaΪ���kΪ�仯����
{
	if(!mrsimg) return;
	UCHAR** pdata = mrsimg->Rimg; if(!pdata) return;
	if(mrsimg->InMBol)
		pdata = mrsimg->Mimg; if(!pdata) return;

}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::AddGussianNoiseColor(RSImg* mrsimg,double mu,double sigma,int k)
{
	if(!mrsimg) return;
	UCHAR** rdata = mrsimg->Rimg; if(!rdata) return;
	UCHAR** gdata = mrsimg->Gimg; if(!gdata) return;
	UCHAR** bdata = mrsimg->Bimg; if(!bdata) return;
	GussianNoise(mrsimg->Img_W0,mrsimg->Img_H0,rdata,gdata,bdata,mu,sigma,k);
	if(mrsimg->InMBol)
	{	rdata = mrsimg->Mimg; if(!rdata) return;
	GussianNoise(mrsimg->Img_W0,mrsimg->Img_H0,rdata,mu,sigma,k);
	}
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::GussianNoise(int ImgW0,int ImgH0,UCHAR** Mdata,double mu,double sigma,int k)
{
	if( ImgW0 < 10 || ImgH0 < 10 ) return;
	UCHAR* pdata;
	int ix,iy;
	double temp;
	time_t sTime = time(NULL); srand((unsigned)time(&sTime));
	for( iy = 0; iy < ImgH0; iy++)
	{
		pdata = Mdata[iy];
		for( ix = 0; ix < ImgW0; ix++ )
		{
			temp= *( pdata + ix ) + k * GenerateGaussianNoise(mu,sigma);//��˹�ֲ���������ɺ���������MyFunc��
			if( temp < 0 ) temp = 0;
			else if (temp > 255 ) temp = 255;
			*( pdata + ix ) = temp;
		}
	}
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::GussianNoise(int ImgW0,int ImgH0,UCHAR** Rdata,UCHAR** Gdata,UCHAR** Bdata,double mu,double sigma,int k)
{
	if( ImgW0 < 10 || ImgH0 < 10) return;
	UCHAR* rdata,*gdata,*bdata;
	int ix,iy;
	double temp;
	time_t sTime = time(NULL);srand((unsigned)time(&sTime));
	for( iy = 0; iy < ImgH0; iy++ )
	{
		rdata = Rdata[iy];
		gdata = Gdata[iy];
		bdata = Bdata[iy];
		for( ix = 0; ix < ImgW0; ix++ )
		{
			double noise = k * GenerateGaussianNoise(mu,sigma);
			temp =*(rdata + ix) + noise; if(temp < 0) temp = 0; else if(temp > 255) temp = 255; *(rdata + ix) = temp;
			temp =*(gdata + ix) + noise; if(temp < 0) temp = 0; else if(temp > 255) temp = 255; *(gdata + ix) = temp;
			temp =*(bdata + ix) + noise; if(temp < 0) temp = 0; else if(temp > 255) temp = 255; *(bdata + ix) = temp;
		}
	}
	return;
}
//---------------------------------------------------------------------------------------
//-----------------------------------С���仯--------------------------------------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::MyHaar_8_BitTransform(UCHAR** Rimg,int ImgW0,int ImgH0,int level,BOOL Invbol)//levelΪ�任����
{
	if(!Rimg) return;if( ImgW0 <10 || level < 1 ) return;
	int iSize = ImgW0 * ImgH0;
	UCHAR **Hdata = (UCHAR**) Rimg;
	UCHAR sh;
	int ii,jj;
	int iCol = ImgW0>>level;//��������������仯�������ҵ��ֽ���ϳ�ǰ����Чͼ���͸�
	int iRow = ImgH0>>level;
	long nk = 0;
	Float_Type *fData = new Float_Type[iSize]; if(!fData) return;//��Ϊ���������С���������������̵Ľ����������Ϣ�ڷֽ��ϳɹ�������ʧ
	Float_Type f0; 
	UCHAR* pbyte;
	if(!Invbol)//����任�ı�־
	{
		for( jj = 0; jj < ImgH0; jj++ )
		{
			pbyte = Hdata[jj]; if(!pbyte) continue;
			for( ii = 0; ii < ImgW0; ii++)
			{
				*(fData + nk) = *(pbyte + ii); nk++;
			}
		}
		HaarTransform(fData,ImgW0,ImgH0,level);
		nk = 0;
		for( jj = 0; jj < ImgH0; jj++ )
		{
			pbyte = Hdata[jj]; if(!pbyte) continue;
			for( ii = 0; ii < ImgW0; ii++ )
			{
				f0 = *(fData + nk);
				if( ii <iCol && jj < iRow )
					*(pbyte + ii) = f0;//�ò��������ǵ�Ƶ�ģ�����������ƽ��ֵ�������Ǹ���
				else
				{
					if( f0 < 0 )
						f0 = 128 - f0;//����ֵΪ������Ϣ��¼��UCHAR�еĵ�һλ��ע�⸺ֵ��Ϊ����2�����䲻��С��-127������仯ʱ��Ҫ�����Ϣ����Ȼ��ԭʧ��
					*(pbyte + ii) = f0;//��floatתΪUCHAR����Ϣ��ʧ
				}
				nk++;
			}
		}
	}
	else
	{
		for( jj = 0; jj < ImgH0; jj++)
		{
			pbyte = Hdata[jj]; if(!pbyte) continue;
			for( ii = 0; ii < ImgW0; ii++ )
			{
				sh = *(pbyte + ii);
				if( ii < iCol && jj < iRow )
					*(fData + nk) = sh;
				else
				{
					if(sh > 127)
						*(fData + nk) = 128 - sh;//��Ϣ��ʧ��
					else
						*(fData + nk) = sh;
				}
				nk++;
			}
		}
		HaarIvsTransform(fData,ImgW0,ImgH0,level);
		nk = 0;
		for( jj = 0; jj < ImgH0; jj++ )
		{
			pbyte = Hdata[jj];
			if(!pbyte) continue;
			for( ii = 0; ii < ImgW0; ii++ )
			{
				*(pbyte + ii) = *(fData + nk);
				nk++;
			}
		}
	}
	delete[] fData;
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::HaarTransform(Float_Type *Hdata,int ImgW0,int ImgH0,int level)//���仯
{
	int ii;
	int iCol = ImgW0;
	int iRow = ImgH0;
	for( ii = 0; ii < level; ii++ )
	{
		HaarRowDecompose(Hdata,ImgW0,iRow,iCol);
		HaarColDecompose(Hdata,ImgW0,iRow,iCol);
		iRow >>= 1; iCol >>= 1;
	}
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::HaarRowDecompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol)//�зֽ�
{
	if( iCol < 2 ) return;
	int iHalf = iCol>>1;
	Float_Type* tmp = new Float_Type[iCol];
	Float_Type* pdata;
	int ii,jj,j1,j2;
	for( ii = 0; ii < iRow; ii++ )
	{
		pdata = Hdata + ImgW0 * ii;
		for( jj = 0; jj < iHalf; jj++ )
		{
			j1 = 2 * jj;
			j2 = j1 + 1;
			tmp[jj] = (*(pdata + j1) + *(pdata + j2)) * 0.5;//��Ƶ�������ȡ��ֵ
			tmp[jj + iHalf] = (*(pdata + j1) - *(pdata + j2)) * 0.5;//��Ƶ�������ȡ��ֵ
		}
		for( jj = 0; jj < iCol; jj++ ) *(pdata + jj) = tmp[jj];
	}
	delete[] tmp;
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::HaarColDecompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol)//�зֽ�
{
	if( iRow < 2 ) return;
	int iHalf = iRow>>1;
	Float_Type* tmp = new Float_Type[iRow];
	int ii,jj,i1,i2;
	for(jj = 0; jj < iCol; jj++ )
	{
		for( ii = 0; ii < iHalf; ii++ )
		{
			i1 = ImgW0 * ii * 2 + jj;
			i2 = i1 + ImgW0;
			tmp[ii] = (*(Hdata + i1) + *(Hdata + i2)) * 0.5;//��Ƶ����
			tmp[ii + iHalf] = (*(Hdata + i1) - *(Hdata + i2)) * 0.5;//��Ƶ����
		}
		for( ii = 0; ii < iRow; ii++ ) *(Hdata + ImgW0 * ii + jj) = tmp[ii];
	}
	delete[] tmp;
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::HaarIvsTransform(Float_Type *Hdata,int ImgW0,int ImgH0,int level)//��任
{
	if( level < 1 ) return;
	int iCol = ImgW0>>(level - 1);
	int iRow = ImgH0>>(level - 1);
	int ii;
	for( ii = 0; ii < level; ii++ )
	{
		HaarColCompose(Hdata,ImgW0,iRow,iCol);
		HaarRowCompose(Hdata,ImgW0,iRow,iCol);
		iRow <<= 1;
		iCol <<= 1;
	}
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::HaarRowCompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol)//�кϳ�
{
	if( iCol < 2 ) return;
	int iHalf = iCol>>1;
	Float_Type* tmp = new Float_Type[iCol];
	Float_Type* pdata;
	int ii,jj,j2;
	for( ii = 0; ii < iRow; ii++ )
	{
		pdata = Hdata + ImgW0 * ii;
		for( jj = 0; jj < iHalf; jj++ )
		{
			j2 = 2 * jj;
			tmp[j2] = *(pdata + jj) + *(pdata + jj + iHalf);
			tmp[j2 + 1] = *(pdata + jj) - *(pdata + jj + iHalf);
		}
		for( jj = 0; jj < iCol; jj++ )
			*(pdata + jj) = tmp[jj];
	}
	delete[] tmp;
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::HaarColCompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol)//�кϳ�
{
	if( iRow < 2 ) return;
	int iHalf = iRow>>1;
	Float_Type* tmp = new Float_Type[iRow];
	int ii,jj,i1,i2;
	for( jj = 0; jj < iCol; jj++ )
	{
		for( ii = 0; ii < iHalf; ii++ )
		{
			i1 = ii * ImgW0 + jj;
			i2 = i1 + iHalf * ImgW0;
			tmp[2 * ii] = *(Hdata + i1) + *(Hdata + i2);
			tmp[2 * ii + 1] = *(Hdata + i1) - *(Hdata + i2);
		}
		for( ii = 0; ii < iRow; ii++ ) Hdata[jj + ii * ImgW0 ] = tmp[ii];
	}
	delete[] tmp;
	return;
}
//---------------------------------------------------------------------------------------
//-------------ͼ�񼸺α任 ���ڴ��е����ݸı�����ǽ�FastData�е����ݸı�---------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_X_Miror_RGBFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	if(mrsimg->Rimg) Bitmap_X_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Rimg);
	if(mrsimg->Gimg) Bitmap_X_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Gimg);
	if(mrsimg->Bimg) Bitmap_X_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Bimg);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_X_MirorGrayFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	UCHAR** Gimg = mrsimg->Rimg; if(mrsimg->InMBol) Gimg = mrsimg->Mimg; if(!Gimg) return;
	Bitmap_X_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,Gimg);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_X_MirorFunc(int ImgW0,int ImgH0,UCHAR** pBits)
{
	if( ImgW0 < 10 || ImgH0 < 10 ) return;
	int ImgHp2 = ImgH0 / 2;
	UCHAR *pTemp,*pTemp1,*pTempn;
	pTemp = new UCHAR[ImgW0]; if(!pTemp) return;
	for( int iy = 0; iy < ImgHp2; iy++ )
	{
		pTemp1 = pBits[iy];
		pTempn = pBits[ImgH0 - iy - 1];//X���񣬿������н��н���
		memcpy_s(pTemp,ImgW0,pTempn,ImgW0);
		memcpy_s(pTempn,ImgW0,pTemp1,ImgW0);
		memcpy_s(pTemp1,ImgW0,pTemp,ImgW0);
	}
	delete[] pTemp;
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Y_Miror_RGBFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	if(mrsimg->Rimg) Bitmap_Y_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Rimg);
	if(mrsimg->Gimg) Bitmap_Y_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Gimg);
	if(mrsimg->Bimg) Bitmap_Y_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Bimg);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Y_MirorGrayFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	UCHAR** Gimg=mrsimg->Rimg; if(mrsimg->InMBol) Gimg = mrsimg->Mimg; if(!Gimg) return;
	Bitmap_Y_MirorFunc(mrsimg->Img_W0,mrsimg->Img_H0,Gimg);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Y_MirorFunc(int ImgW0,int ImgH0,UCHAR** pBits)//Y�������X��������Ҫ��ÿ���������ݽ��н���
{
	if( ImgW0 < 10 || ImgH0 < 10 ) return;
	int ImgWp2 = ImgW0 / 2;
	UCHAR* pdata;
	UCHAR ch;
	for( int iy = 0; iy < ImgH0; iy++ )
	{
		pdata = pBits[iy];
		for( int ix = 0; ix < ImgWp2; ix++ )
		{
			ch = pdata[ix];
			pdata[ix] = pdata[ImgW0 - 1 - ix];
			pdata[ImgW0 - 1 - ix] = ch;
		}
	}
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Rota90DFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	int ImgW0 = mrsimg->Img_W0;
	int ImgH0 = mrsimg->Img_H0;
	int ImgWn = ImgH0;
	int ImgHn = ImgW0;
	UCHAR** Mimg;
	if(mrsimg->Rimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota90DFunc(ImgWn,ImgHn,Mimg,mrsimg->Rimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Rimg);
			mrsimg->Rimg = Mimg;
		}
	}
	if(mrsimg->Gimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota90DFunc(ImgWn,ImgHn,Mimg,mrsimg->Gimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Gimg);
			mrsimg->Gimg = Mimg;
		}
	}
	if(mrsimg->Bimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota90DFunc(ImgWn,ImgHn,Mimg,mrsimg->Bimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Bimg);
			mrsimg->Bimg = Mimg;
		}
	}
	if(mrsimg->Mimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota90DFunc(ImgWn,ImgHn,Mimg,mrsimg->Mimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Mimg);
			mrsimg->Mimg = Mimg;
		}
	}
	mrsimg->Img_W0 = ImgWn;
	mrsimg->Img_H0 = ImgHn;
	int temp=mrsimg->FastHeight;//���ǲ�������ʾ�ĳ�������ͼ�񳤿���ȣ�������ʾ��ͼ���Խ��ԽС
	mrsimg->FastHeight=mrsimg->FastWidth;
	mrsimg->FastWidth=temp;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Rota90DFunc(int nImgW,int nImgH,UCHAR** pNBits,UCHAR** pOBits)
{
	int ox,oy;
	UCHAR* ndata,*odata;
	for( int iy = 0; iy < nImgH; iy++ )
	{
		ndata = pNBits[iy];
		for( int ix = 0; ix < nImgW; ix++ )
		{
			ox = iy;
			oy = nImgW - 1 - ix;
			odata = pOBits[oy];
			*(ndata + ix) = *(odata + ox);
		}
	}//�ص����ҵ���תǰ��Ĺ�ϵ����ͨ����ͼ������ֱ���ҳ�
	return;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Rota270Func(RSImg* mrsimg)
{
	if(!mrsimg) return;
	int ImgW0 = mrsimg->Img_W0;
	int ImgH0 = mrsimg->Img_H0;
	int ImgWn = ImgH0;
	int ImgHn = ImgW0;
	UCHAR** Mimg;
	if(mrsimg->Rimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota270Func(ImgWn,ImgHn,Mimg,mrsimg->Rimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Rimg);
			mrsimg->Rimg = Mimg;
		}
	}
	if(mrsimg->Gimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota270Func(ImgWn,ImgHn,Mimg,mrsimg->Gimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Gimg);
			mrsimg->Gimg = Mimg;
		}
	}
	if(mrsimg->Bimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota270Func(ImgWn,ImgHn,Mimg,mrsimg->Bimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Bimg);
			mrsimg->Bimg = Mimg;
		}
	}
	if(mrsimg->Mimg)
	{
		Mimg = Gene_Char_MatrixMemorySpace(ImgHn,ImgWn);
		if(Mimg)
		{
			Bitmap_Rota270Func(ImgWn,ImgHn,Mimg,mrsimg->Mimg);
			Delete_Char_MatrixMemorySpace(ImgH0,mrsimg->Mimg);
			mrsimg->Mimg = Mimg;
		}
	}
	mrsimg->Img_W0 = ImgWn;
	mrsimg->Img_H0 = ImgHn;
	int temp=mrsimg->FastHeight;
	mrsimg->FastHeight=mrsimg->FastWidth;
	mrsimg->FastWidth=temp;
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Bitmap_Rota270Func(int nImgW,int nImgH,UCHAR** pNBits,UCHAR** pOBits)
{
	int ox,oy;
	UCHAR* ndata,*odata;
	for( int iy = 0; iy < nImgH; iy++ )
	{
		ndata = pNBits[iy];
		for( int ix = 0; ix < nImgW; ix++ )
		{
			ox = nImgH - iy - 1;
			oy = ix;
			odata = pOBits[oy];
			*(ndata + ix) = *(odata + ox);
		}
	}
	return;
}
//---------------------------------------------------------------------------------------
//------------------------------------ƽ���˲��;���˲�---------------------------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Image_SmoothConv2GrayFunc(RSImg* mrsimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol)
{
	if(!mrsimg) return;
	UCHAR **Mimg = mrsimg->Rimg; if(mrsimg->InMBol) Mimg = mrsimg->Mimg; if(!Mimg) return;
	Image_SmoothConv2BandFunc( mrsimg,Mimg, Smoo11, Smoo12, Smoo13, Smoo21, Smoo22, Smoo23, Smoo31, Smoo32, Smoo33,smooBol);
	//��������ֱ��ͼ������û�б�Ҫ��
	if(mrsimg->InMBol) Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Mimg,mrsimg->factM,&mrsimg->InMmin,&mrsimg->InMmax);//�˲�������ͳ��ֱ��ͼ
	else               Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Rimg,mrsimg->factR,&mrsimg->InRmin,&mrsimg->InRmax);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Image_SmoothConv2_RGBFunc(RSImg* mrsimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol)
{
	if(!mrsimg) return;
	Image_SmoothConv2BandFunc( mrsimg,mrsimg->Rimg,Smoo11, Smoo12, Smoo13,Smoo21, Smoo22, Smoo23, Smoo31, Smoo32, Smoo33,smooBol);
	Image_SmoothConv2BandFunc( mrsimg,mrsimg->Gimg,Smoo11, Smoo12, Smoo13,Smoo21, Smoo22, Smoo23, Smoo31, Smoo32, Smoo33,smooBol);
	Image_SmoothConv2BandFunc( mrsimg,mrsimg->Bimg,Smoo11, Smoo12, Smoo13,Smoo21, Smoo22, Smoo23, Smoo31, Smoo32, Smoo33,smooBol);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Rimg,mrsimg->factR,&mrsimg->InRmin,&mrsimg->InRmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Gimg,mrsimg->factG,&mrsimg->InGmin,&mrsimg->InGmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Bimg,mrsimg->factB,&mrsimg->InBmin,&mrsimg->InBmax);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Image_SmoothConv2BandFunc(RSImg* mrsimg,UCHAR **Mimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol)
{
	if(!mrsimg || !Mimg) return;
	UCHAR* s1data;
	UCHAR* s2data;
	UCHAR* s3data;
	int ImgH0 = mrsimg->Img_H0;
	int ImgW0 = mrsimg->Img_W0;
	int ImgB0 = mrsimg->Img_B0;
	//�¿�Ӱ��ռ䣬��Ϊ�µĴ����ڴ�ռ�
	UCHAR** pData = Gene_Char_MatrixMemorySpace(ImgH0,ImgW0); if(!pData) return;
	UCHAR*  p0data;
	UCHAR rg;
	double f11,f12,f13,f21,f22,f23,f31,f32,f33,sum;
	int ii,jj,nn;
	for( jj = 1; jj < ImgH0 - 1; jj++ )//��Ϊi��3*3�����Դӵ�һ�п�ʼ�������ڶ���
	{
		s1data = Mimg[jj-1]; if(!s1data) continue;
		s2data = Mimg[jj];   if(!s2data) continue;
		s3data = Mimg[jj+1]; if(!s3data) continue;
		p0data = pData[jj];  if(!p0data) continue;
		for( ii = 1 ; ii < ImgW0 - 1; ii++ )
		{
			f11 = *(s1data + ii - 1); f12 = *(s1data + ii); f13 = *(s1data + ii + 1);
			f21 = *(s2data + ii - 1); f22 = *(s2data + ii); f23 = *(s2data + ii + 1);
			f31 = *(s3data + ii - 1); f32 = *(s3data + ii); f33 = *(s3data + ii + 1);
			sum = 0; nn = 0;
			if(smooBol)
			{
				if(fabs(f11 - ImgB0) > EPSILON) { sum += f11 * Smoo11; nn++; }
				if(fabs(f12 - ImgB0) > EPSILON) { sum += f12 * Smoo12; nn++; }
				if(fabs(f13 - ImgB0) > EPSILON) { sum += f13 * Smoo13; nn++; }
				if(fabs(f21 - ImgB0) > EPSILON) { sum += f21 * Smoo21; nn++; }
				if(fabs(f22 - ImgB0) > EPSILON) { sum += f22 * Smoo22; nn++; }
				if(fabs(f23 - ImgB0) > EPSILON) { sum += f23 * Smoo23; nn++; }
				if(fabs(f31 - ImgB0) > EPSILON) { sum += f31 * Smoo31; nn++; }
				if(fabs(f32 - ImgB0) > EPSILON) { sum += f32 * Smoo32; nn++; }
				if(fabs(f33 - ImgB0) > EPSILON) { sum += f33 * Smoo33; nn++; }
			}
			else
			{
				if(fabs(f11 - ImgB0) > EPSILON) { sum += f11 * Smoo33; nn++; }
				if(fabs(f12 - ImgB0) > EPSILON) { sum += f12 * Smoo32; nn++; }
				if(fabs(f13 - ImgB0) > EPSILON) { sum += f13 * Smoo31; nn++; }
				if(fabs(f21 - ImgB0) > EPSILON) { sum += f21 * Smoo23; nn++; }
				if(fabs(f22 - ImgB0) > EPSILON) { sum += f22 * Smoo22; nn++; }
				if(fabs(f23 - ImgB0) > EPSILON) { sum += f23 * Smoo21; nn++; }
				if(fabs(f31 - ImgB0) > EPSILON) { sum += f31 * Smoo13; nn++; }
				if(fabs(f32 - ImgB0) > EPSILON) { sum += f32 * Smoo32; nn++; }
				if(fabs(f33 - ImgB0) > EPSILON) { sum += f33 * Smoo11; nn++; }
			}
			if(!nn){ *(p0data + ii) = ImgB0; continue; }
			if(nn != 9) sum = sum * 9.0 / nn;
			if(sum > 255) rg = 255;
			else if(sum < 0) rg = 0;
			else rg =sum + 0.4999;//ʵ���������������ֱ��ȡ��
			*(p0data + ii) = rg;
		}
	}
	for( jj = 1; jj < ImgH0 - 1; jj++ )
	{
		s2data = Mimg[jj]; if(!s2data) continue;
		p0data = pData[jj]; if(!p0data) continue;
		for( ii = 1; ii < ImgW0 - 1; ii++ )
			*(s2data + ii) = *(p0data + ii);
	}
	Delete_Char_MatrixMemorySpace(ImgH0,pData);
	return;
}
//--------------------------��ֵ�˲�-----------------------------------------------------
void CObjRSImgClass::Image_SmoothConvMediumGrayFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	UCHAR **Mimg = mrsimg->Rimg; if(mrsimg->InMBol) Mimg = mrsimg->Mimg; if(!Mimg) return;
	Image_SmoothConvMediumBandFunc(mrsimg,Mimg);
	if(mrsimg->InMBol) Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Mimg,mrsimg->factM,&mrsimg->InMmin,&mrsimg->InMmax);//�˲�������ͳ��ֱ��ͼ
	else               Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Rimg,mrsimg->factR,&mrsimg->InRmin,&mrsimg->InRmax);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Image_SmoothConvMedium_RGBFunc(RSImg* mrsimg)
{
	if(!mrsimg) return;
	Image_SmoothConvMediumBandFunc(mrsimg,mrsimg->Rimg);
	Image_SmoothConvMediumBandFunc(mrsimg,mrsimg->Gimg);
	Image_SmoothConvMediumBandFunc(mrsimg,mrsimg->Bimg);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Rimg,mrsimg->factR,&mrsimg->InRmin,&mrsimg->InRmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Gimg,mrsimg->factG,&mrsimg->InGmin,&mrsimg->InGmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Bimg,mrsimg->factB,&mrsimg->InBmin,&mrsimg->InBmax);
}
//---------------------------------------------------------------------------------------
void CObjRSImgClass::Image_SmoothConvMediumBandFunc(RSImg* mrsimg,UCHAR** Mimg)
{
	if( !mrsimg || !Mimg ) return;
	int ImgH0 = mrsimg->Img_H0;
	int ImgW0 = mrsimg->Img_W0;
	int ImgB0 = mrsimg->Img_B0;
	UCHAR *odata = new UCHAR[9]; if(!odata) return;
	UCHAR** pData = Gene_Char_MatrixMemorySpace(ImgH0,ImgW0); if(!pData) return;
	UCHAR* s1data,*s2data,*s3data,*p0data;
	int i,j;
	for( j = 1; j < ImgH0 - 1; j++ )
	{

		s1data = Mimg[j-1]; if(!s1data) continue;
		s2data = Mimg[j];   if(!s2data) continue;
		s3data = Mimg[j+1]; if(!s3data) continue;
		p0data = pData[j];
		for( i = 1; i < ImgW0 - 1; i++ )
		{
			*(odata + 0) = *(s1data + i - 1); *(odata + 1) = *(s1data + i); *(odata + 2) = *(s1data + i + 1);
			*(odata + 3) = *(s2data + i - 1); *(odata + 4) = *(s2data + i); *(odata + 5) = *(s2data + i + 1);
			*(odata + 6) = *(s3data + i - 1); *(odata + 7) = *(s3data + i); *(odata + 8) = *(s3data + i + 1);
			InsertionSort(odata,9);//ʹ�ò�������ʵ����MyFunc��
			*(p0data + i) = *(odata + 4);
		}
	}
	for( j = 1; j < ImgH0 - 1; j++ )
	{
		s2data = Mimg[j]; if(!s2data) continue;
		p0data = pData[j]; if(!p0data) continue;
		for( i = 1; i < ImgW0 - 1; i++ )
		{
			*(s2data + i) = *(p0data + i);
		}
	}
	Delete_Char_MatrixMemorySpace(ImgH0,pData);
	return;
}

//---------------------------------------------------------------------------------------
//--------------------------------ת������-----------------------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::BmpToImgFileFunc(const char* filename)//Bmp->Img
{
	if(!PathFileExists(filename))
	{
		strcpy_s(ErrStr,20,"�ļ�������!"); return FALSE;
	}
	_splitpath(filename,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".bmp");
	FILE* fio;
	fopen_s(&fio,PathFileName,"rb");
	if(!fio)
	{
		strcpy_s(ErrStr,20,"�ļ��򿪴���!"); return FALSE;
	}
	BOOL wbool = ReadBmpFileToImgDataFunc(filename,fio);
	fclose(fio);
	return wbool;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpFileToImgDataFunc(const char* filename,FILE *fio)
{
	fread_s(&bmfh,sizeof(BITMAPFILEHEADER),1,sizeof(BITMAPFILEHEADER),fio);
	fread_s(&bmih,sizeof(BITMAPINFOHEADER),1,sizeof(BITMAPINFOHEADER),fio);
	if(bmfh.bfType != 0x4D42) { strcpy_s(ErrStr,20,"ͼ���ʽBM����!");return FALSE; }
	if(bmih.biCompression != BI_RGB) { strcpy_s(ErrStr,20,"ͼ��ѹ����ʽ����"); return FALSE; }
	if(bmih.biBitCount != 24) { strcpy_s(ErrStr,20,"��24bit����!"); return FALSE; }
	long offBits = bmfh.bfOffBits;
	int nWidth = bmih.biWidth;
	int nHeight = bmih.biHeight;
	fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".000");
	FILE* fou; fopen_s(&fou,PathFileName,"w");
	if(fou)
	{
		fprintf(fou,"ImageWH   %5d %5d\n",nWidth,nHeight);
		strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"R.img"); fprintf(fou,"ETM_BAND  3  0  0  255 %-s\n",ReadStr);
		strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"G.img"); fprintf(fou,"ETM_BAND  2  0  0  255 %-s\n",ReadStr);
		strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"B.img"); fprintf(fou,"ETM_BAND  1  0  0  255 %-s\n",ReadStr);
		fclose(fou);
	}
	strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"R.img"); fnmerge(PathFileName,Ipdri,Ipdir,ReadStr,"");
	FILE* rfio;fopen_s(&rfio,PathFileName,"wb"); if(!rfio){ strcpy_s(ErrStr,50,"R�򿪴���!"); return FALSE; }
	strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"G.img"); fnmerge(PathFileName,Ipdri,Ipdir,ReadStr,"");
	FILE* gfio;fopen_s(&gfio,PathFileName,"wb"); if(!gfio){ strcpy_s(ErrStr,50,"G�򿪴���!"); return FALSE; }
	strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"B.img"); fnmerge(PathFileName,Ipdri,Ipdir,ReadStr,"");
	FILE* bfio; fopen_s(&bfio,PathFileName,"wb"); if(!bfio){ strcpy_s(ErrStr,50,"B�򿪴���!"); return FALSE; }
	BOOL wbool = ReadBmpToImgBy_RGB_FileFunc(fio,rfio,gfio,bfio,nHeight,nWidth,offBits);
	fclose(rfio); fclose(gfio); fclose(bfio);
	return wbool;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpToImgBy_RGB_FileFunc(FILE* fio,FILE* rfio,FILE* gfio,FILE* bfio,int nHeight,int nWidth,long offBits)
{
	char *pdata = new char[3 * nWidth + 1];//�洢���в��ε�һ�д�С�Ŀռ�
	if(!pdata) { strcpy_s(ErrStr,50,"Idata Error!"); return FALSE; }
	memset(pdata,0,3 * nWidth);
	char *rline = new char[nWidth + 1];
	if(!rline) { strcpy_s(ErrStr,50,"Rdata Error!"); delete[] pdata; return FALSE; }
	memset(rline,0,nWidth);
	char *gline = new char[nWidth + 1];
	if(!gline) { strcpy_s(ErrStr,50,"Gdata Error!"); delete[] pdata; delete[] rline; return FALSE; }
	memset(gline,0,nWidth);
	char *bline = new char[nWidth + 1];
	if(!bline){ strcpy_s(ErrStr,50,"Bdata Error!"); delete[] pdata; delete[] rline; delete[] gline; return FALSE; }
	memset(bline,0,nWidth);

	fseek(fio,offBits,SEEK_SET);
	int i,j;
	for( j = 0; j < nHeight; j++ )
	{
		fread_s(pdata,3*nWidth,1,3*nWidth,fio);
		for( i = 0; i < nWidth; i++ )
		{
			*(bline + i) = *(pdata + i * 3);
			*(gline + i) = *(pdata + i * 3 + 1);
			*(rline + i) = *(pdata + i * 3 + 2);
		}
		fwrite(rline,1,nWidth,rfio);
		fwrite(gline,1,nWidth,gfio);
		fwrite(bline,1,nWidth,bfio);
	}
	delete[] pdata; delete[] rline; delete[] gline; delete[] bline;
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ImgToBmpFileFunc(const char* B000name,int Width,int Height,char* Rname,char* Gname,char* Bname,int nWidht,int nHeight)//Img->Bmp
{
	_splitpath(B000name,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".bmp");
	FILE* fou; fopen_s(&fou,PathFileName,"wb"); if(!fou) return FALSE;
	fnmerge(PathFileName,Ipdri,Ipdir,Rname,"");
	FILE *rfio; fopen_s(&rfio,PathFileName,"rb"); if(!rfio) { fclose(fou); return FALSE; }
	fnmerge(PathFileName,Ipdri,Ipdir,Gname,"");
	FILE *gfio; fopen_s(&gfio,PathFileName,"rb"); if(!gfio) { fclose(fou); fclose(rfio); return FALSE; }
	fnmerge(PathFileName,Ipdri,Ipdir,Bname,"");
	FILE *bfio; fopen_s(&bfio,PathFileName,"rb"); if(!bfio) { fclose(fou); fclose(rfio); fclose(gfio); return FALSE; }
	BOOL wbool = WriteImgFileTo24BmpFunc(fou,rfio,gfio,bfio,nWidht,nHeight);
	fclose(fou); fclose(rfio); fclose(gfio); fclose(bfio);
	return wbool;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::WriteImgFileTo24BmpFunc(FILE* fio,FILE *rfio,FILE* gfio,FILE* bfio,int nWidth,int nHeight)
{
	char *pdata = new char[3 * nWidth + 1];
	if(!pdata){ strcpy_s(ErrStr,50,"Idata Error!"); return FALSE; }
	memset(pdata,0,3*nWidth);
	char *rline = new char[nWidth + 1];
	if(!rline){ strcpy_s(ErrStr,50,"Rdata Error!"); delete[] pdata; return FALSE; }
	memset(rline,0,nWidth);
	char *gline = new char[nWidth + 1];
	if(!gline){ strcpy_s(ErrStr,50,"Gdata Error!"); delete[] pdata; delete[] rline; return FALSE; }
	memset(gline,0,nWidth);
	char *bline = new char[nWidth + 1];
	if(!bline){ strcpy_s(ErrStr,50,"Bdata Error!"); delete[] pdata; delete[] rline; delete[] gline; return FALSE; }
	memset(bline,0,nWidth);

	Init_BitMap_FileHeader(nWidth,nHeight,3,&bmfh,&bmih);
	fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fio);
	fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,fio);
	fseek(fio,bmfh.bfOffBits,SEEK_SET);

	int j,i;
	for( j = 0; j < nHeight; j++ )
	{
		fread_s(rline,nWidth,1,nWidth,rfio);
		fread_s(gline,nWidth,1,nWidth,gfio);
		fread_s(bline,nWidth,1,nWidth,bfio);
		for( i = 0; i < nWidth; i++) 
		{
			*(pdata + i * 3) = *(bline + i);
			*(pdata + i * 3 + 1) = *(gline + i);
			*(pdata + i * 3 + 2) = *(rline + i);
		}
		fwrite(pdata,1,3*nWidth,fio);
	}
	delete[] pdata; delete[] rline; delete[] gline; delete[] bline;
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::SwapBmpToJpg_FileFunc(const char* infilename,const char* outfilename,int JpgQual)//bmp->jpg
{
	CImage mimage;
	HRESULT ret = mimage.Load(infilename);
	mimage.Save(outfilename);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::SwapJpgToBmp_FileFunc(const char* infilename,const char* outfilename,long* pw,long *pH)//jpg->bmp
{
	CImage m_image;
	m_image.Load(infilename);
	m_image.Save(outfilename);
	BITMAPFILEHEADER mbmfh;
	BITMAPINFOHEADER mbmih;
	FILE* mfile;fopen_s(&mfile,outfilename,"rb");
	fread_s(&mbmfh,sizeof(BITMAPFILEHEADER),sizeof(BITMAPFILEHEADER),1,mfile);
	fread_s(&mbmih,sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER),1,mfile);
	*pw = mbmih.biWidth;
	*pH = mbmih.biHeight;
	fclose(mfile);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpFileToModifyDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,int jpgH0,int Joff)//read bmp file to certen place
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	WORD n1;
	DWORD n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfType      = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1, fio); bmfh.bfSize      = n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfReserved1 = n1;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfReserved2 = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1, fio); bmfh.bfOffBits   = n2;
	fread_s(&bmih, sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER),1,fio);
	if(bmfh.bfType  != 0x4D42) { strcpy_s(ErrStr,30,"ͼ���ʽBM����!"); return FALSE; }
	if(bmih.biCompression != BI_RGB) { strcpy_s(ErrStr,30,"ͼ��ѹ����ʽ����!");return FALSE; }
	DWORD offBits = bmfh.bfOffBits;
	long ImgW0 = bmih.biWidth;if(ImgW0 != nWidth){ strcpy_s(ErrStr,30,"ImgW0����!"); return FALSE; }
	long ImgH0 = bmih.biHeight;if(ImgH0 != jpgH0){ strcpy_s(ErrStr,30,"ImgH0����!"); return FALSE; }
	long nLine = ImgW0*3;
	//�����ڴ�
	int jj;
	long bij;
	char *pdata;
	fseek(fio,offBits,SEEK_SET);
	for( jj = 0; jj < ImgH0; jj++ )
	{
		bij = (Joff + jj) * nLine;
		pdata = bmpHData + bij;
		fread_s(pdata,nLine,sizeof(UCHAR),nLine,fio);
	}
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpFileToModifyDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,long poff)
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	WORD n1;
	DWORD n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfType      = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1, fio); bmfh.bfSize      = n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfReserved1 = n1;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfReserved2 = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1, fio); bmfh.bfOffBits   = n2;
	fread_s(&bmih, sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER),1,fio);
	DWORD offBits = bmfh.bfOffBits;
	fseek(fio,offBits,SEEK_SET);
	int jj;
	long nLine = nWidth * 3;
	char* pdata;
	for( jj = 0; jj < nHeight; jj++ )
	{
		pdata = bmpHData + jj * nLine;
		fread_s(pdata,nLine,sizeof(UCHAR),nLine,fio);
	}
	return TRUE;

}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpFileToMemDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData)
{
	WORD  n1;
	DWORD n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1,fio); bmfh.bfType      = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1,fio); bmfh.bfSize      = n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1,fio); bmfh.bfReserved1 = n1;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1,fio); bmfh.bfReserved2 = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1,fio); bmfh.bfOffBits   = n2;
	fread_s(&bmih, sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER), 1,fio);
	if(bmfh.bfType != 0x4D42){ strcpy_s(ErrStr,30,"ͼ���ʽBM����!"); return FALSE; }
	if(bmih.biCompression != BI_RGB){ strcpy_s(ErrStr,30,"ͼ��ѹ����ʽ����!"); return FALSE; }
	DWORD offBits = bmfh.bfOffBits;
	long ImgW0 = bmih.biWidth;
	long ImgH0 = bmih.biHeight;
	long nLine = ImgW0 * 3;
	//�����ڴ�
	//�˴�ʹ������ͼ��������Ƭ����
	int joff = (nHeight - ImgH0) / 2;
	int ioff = (nWidth - ImgW0)  / 2;
	int jj;
	long bij;
	char *pdata;
	fseek(fio,offBits,SEEK_SET);
	for( jj = 0; jj < ImgH0; jj++ )
	{
		bij = ((joff + jj) * nWidth + ioff) * 3;
		pdata = bmpHData + bij;
		fread_s(pdata,nLine*sizeof(UCHAR),sizeof(UCHAR),nLine,fio);
	}
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::Add_BmpFileToMemDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,float cp0)
{
	WORD  n1;
	DWORD n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfType      = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1, fio); bmfh.bfSize      = n2;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfReserved1 = n1;
	fread_s(&n1,   sizeof(WORD),  sizeof(WORD),  1, fio); bmfh.bfReserved2 = n1;
	fread_s(&n2,   sizeof(DWORD), sizeof(DWORD), 1, fio); bmfh.bfOffBits   = n2;
	fread_s(&bmih, sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER), 1,fio);
	if(bmfh.bfType         != 0x4D42) { strcpy_s(ErrStr,30,"ͼ���ʽBM����!");  return false;}
	if(bmih.biCompression  != BI_RGB) { strcpy_s(ErrStr,30,"ͼ��ѹ����ʽ����!");return false;}
	DWORD  offBits = bmfh.bfOffBits;
	LONG   ImgW0   = bmih.biWidth;
	LONG   ImgH0   = bmih.biHeight;
	long   nLine   = ImgW0*3;
	//�����ڴ�
	int joff = (nHeight  -ImgH0) / 2;
	int ioff = (nWidth - ImgW0)  / 2;
	int ii,jj;
	long bij;
	UCHAR* pdata;
	UCHAR rg1,rg2;
	UCHAR *tmp = new UCHAR[nLine]; if(!tmp) return FALSE;
	fseek(fio,offBits,SEEK_SET);
	for( jj = 0; jj < ImgH0; jj++ )
	{
		bij = ((jj + joff) * nWidth + ioff) * 3;
		pdata = (unsigned char*)bmpHData + bij;
		fread_s(tmp,nLine*sizeof(UCHAR),sizeof(UCHAR),nLine,fio);
		for( ii = 0; ii < nLine; ii++ )
		{
			rg1 = *(pdata + ii);
			rg2 = *(tmp + ii);
			*(pdata + ii) = rg1 * (1 - cp0) + rg2 * cp0;
		}
	}
	delete[] tmp;
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::DoTopBotBmp1JoinBmp2Func( char* file1name,char* file2name,int Ty)//������ͼƴ��
{
	char *outfilename = new char[200];
	strcpy_s(outfilename,200,file1name);
	if(file1name[0] == '\0' || file2name[0] == '\0') { strcpy_s(ErrStr,50,"��������ͼ����!"); return FALSE; }
	if(!strcmp(file1name,file2name)){ strcpy_s(ErrStr,50,"ͼ��1��ͼ��2ͬ��!");return FALSE;}
	strupr(file1name); char *ext = strrchr(file1name,'.'); if(strcmp(ext,".JPG") != 0 && strcmp(ext,".JPEG") != 0){ strcpy_s(ErrStr,50,"ͼ��1��jpg!"); return FALSE; }
	strupr(file2name);       ext = strrchr(file1name,'.'); if(strcmp(ext,".JPG") != 0 && strcmp(ext,".JPEG") != 0){ strcpy_s(ErrStr,50,"ͼ��2��jpg!"); return FALSE; }
	long jpg1W0,jpg1H0;
	long jpg2W0,jpg2H0;
	if(!PathFileExists(file1name)){strcpy_s(ErrStr,30,"ͼ��1������!");return FALSE;}
	strcpy_s(PathFileName,30,"D:\\ͼ��1.bmp");SwapJpgToBmp_FileFunc(file1name,PathFileName,&jpg1W0,&jpg1H0);//����һ��ͼjpg->bmp������������Ϊͼ��1.bmp
	if(!PathFileExists(file2name)){strcpy_s(ErrStr,30,"ͼ��2������!");return FALSE;}
	strcpy_s(PathFileName,30,"D:\\ͼ��2.bmp");SwapJpgToBmp_FileFunc(file2name,PathFileName,&jpg2W0,&jpg2H0);
	if(jpg1W0 != jpg2W0) { strcpy_s(ErrStr,50,"��ͼ�����!"); return FALSE; }
	int nWidth = jpg1W0;
	int nHeight = jpg1H0 + jpg2H0 + Ty;
	if( !nWidth || !nHeight ){ strcpy_s(ErrStr,30,"���� = 0"); return FALSE; }
	DWORD nw = nWidth * 3;
	DWORD n4 = nw / 4; if(nw % 4) n4++; n4 *= 4;
	long lp8 = nHeight; lp8 *= n4;//����DWORD��
	//��һ�ο��ռ�̫��Ļ���ô��
	char *bmpHData = new char[lp8 + 10]; if(!bmpHData){ strcpy_s(ErrStr,30,"�ڴ�������!"); return FALSE; }
	memset(bmpHData,255,lp8*sizeof(char));
	strcpy_s(PathFileName,30,"D:\\ͼ��1.bmp"); FILE* fio;fopen_s(&fio,PathFileName,"rb"); if(fio) { ReadBmpFileToModifyDataFunc(fio,nHeight,nWidth,bmpHData,jpg1H0,jpg2H0+Ty); fclose(fio); }//�˴�+4���м�����һ�����ߣ���Ϊmemsetʱ��Ϊ255
	strcpy_s(PathFileName,30,"D:\\ͼ��2.bmp");          ;fopen_s(&fio,PathFileName,"rb"); if(fio) { ReadBmpFileToModifyDataFunc(fio,nHeight,nWidth,bmpHData,jpg2H0,0);         fclose(fio); }//�˴�+4���м�����һ�����ߣ���Ϊmemsetʱ��Ϊ255
	strcpy_s(PathFileName,30,"D:\\ͼ��3.bmp"); WriteToBmpFile(PathFileName,nWidth,nHeight,bmpHData);
	delete[] bmpHData;
	//תjpg��ʽ
	_splitpath(outfilename,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(outfilename,Ipdri,Ipdir,Ipname,"_J.jpeg");
	SwapBmpToJpg_FileFunc(PathFileName,outfilename,90);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::OnePictureToTwoFunc(char* filename,int Ty)//ͼ��ָ�����ţ���TyΪ���ߣ�ע�����µߵ�
{
	char *outfilename = new char[200];
	strcpy_s(outfilename,200,filename);
	if(filename[0] == '\0'){ strcpy_s(ErrStr,50,"��������ͼ����!"); return FALSE; }
	strupr(filename); char *ext = strrchr(filename,'.'); if(strcmp(ext,".JPG") != 0 && strcmp(ext,".JPEG") != 0){ strcpy_s(ErrStr,50,"ͼ���jpg!"); return FALSE; }
	long jpgW0,jpgH0;
	if(!PathFileExists(filename)){ strcpy_s(ErrStr,30,"ͼ��1������!"); return FALSE; }
	strcpy_s(PathFileName,30,"D:\\ͼ��1.bmp"); SwapJpgToBmp_FileFunc(filename,PathFileName,&jpgW0,&jpgH0);
	if( Ty <= 0 ||Ty >= jpgH0 ) { strcpy_s(ErrStr,50,"��������ȷyֵ!"); return FALSE; }
	int nWidth  = jpgW0;
	int nHeight = jpgH0;
	DWORD nw = nWidth * 3;
	DWORD n4 = nw / 4; if(nw % 4) n4++; n4 *= 4;
	long lp8 = n4 * nHeight;
	char* bmpHData = new char[lp8 + 10]; if(!bmpHData){ strcpy_s(ErrStr,30,"�ڴ�������!"); return FALSE; }memset(bmpHData,255,lp8*sizeof(char));

	FILE* fio;fopen_s(&fio,PathFileName,"rb");
	if(fio)
	{
		ReadBmpFileToModifyDataFunc(fio,nHeight,nWidth,bmpHData,0);
		fclose(fio);
	}
	_splitpath(outfilename,Ipdri,Ipdir,Ipname,Ipext);fnmerge(outfilename,Ipdri,Ipdir,Ipname,"_top.bmp");
	WriteToBmpFile(outfilename,nWidth,Ty,bmpHData);
	bmpHData = bmpHData + Ty * nWidth * 3;
	fnmerge(outfilename,Ipdri,Ipdir,Ipname,"_bottom.bmp");
	WriteToBmpFile(outfilename,nWidth,nHeight-Ty,bmpHData);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::DoTwoTouBmp1Add_Bmp2Func(char *file1name,char* file2name,float cp0)//��ͼ���
{

	char *outfilename = new char[200];
	strcpy_s(outfilename,200,file1name);
	if(file1name[0] == '\0' || file2name[0] == '\0') { strcpy_s(ErrStr,50,"��������ͼ����!");  return FALSE; }
	if(!strcmp(file1name,file2name))                 { strcpy_s(ErrStr,50,"ͼ��1��ͼ��2ͬ��!"); return FALSE; }
	strupr(file1name); char *ext=strrchr(file1name,'.'); if(strcmp(ext,".JPG") != 0 && strcmp(ext,".JPEG") != 0){ strcpy_s(ErrStr,50,"ͼ��1��jpg!"); return FALSE; }
	strupr(file2name);       ext=strrchr(file1name,'.'); if(strcmp(ext,".JPG") != 0 && strcmp(ext,".JPEG") != 0){ strcpy_s(ErrStr,50,"ͼ��2��jpg!"); return FALSE; }
	long jpg1W0,jpg1H0;
	long jpg2W0,jpg2H0;
	if(!PathFileExists(file1name)) { strcpy_s(ErrStr,30,"ͼ��1������!"); return FALSE; }
	strcpy_s(PathFileName,30,"D:\\ͼ��1.bmp"); SwapJpgToBmp_FileFunc(file1name,PathFileName,&jpg1W0,&jpg1H0);
	if(!PathFileExists(file2name)) { strcpy_s(ErrStr,30,"ͼ��2������!"); return FALSE; }
	strcpy_s(PathFileName,30,"D:\\ͼ��2.bmp"); SwapJpgToBmp_FileFunc(file2name,PathFileName,&jpg2W0,&jpg2H0);
	int jpg0W0; if(jpg1W0 > jpg2W0) jpg0W0 = jpg1W0; else jpg0W0 = jpg2W0;
	int jpg0H0; if(jpg1H0 > jpg2H0) jpg0H0 = jpg1H0; else jpg0H0 = jpg2H0;
	int nWidth = jpg0W0;
	int nHeight = jpg0H0; if( !nWidth || !nHeight) { strcpy_s(ErrStr,30,"���� = 0"); return FALSE; }
	DWORD nw = nWidth * 3;
	DWORD n4 = nw / 4; if(nw % 4) n4++; n4 *= 4;
	long lp8 = nHeight*n4;
	char* bmpHData = new char[lp8+10]; if(!bmpHData) { strcpy_s(ErrStr,30,"�ڴ�������!"); return FALSE; }
	memset(bmpHData,255,lp8*sizeof(char));
	strcpy_s(PathFileName,50,"D:\\ͼ��1.bmp"); FILE* fio;fopen_s(&fio,PathFileName,"rb"); if(fio) { ReadBmpFileToMemDataFunc(fio,nHeight,nWidth,bmpHData); fclose(fio); }
	strcpy_s(PathFileName,50,"D:\\ͼ��2.bmp");           fopen_s(&fio,PathFileName,"rb"); if(fio) { Add_BmpFileToMemDataFunc(fio,nHeight,nWidth,bmpHData,cp0); fclose(fio); }
	strcpy_s(PathFileName,50,"D:\\ͼ��3.bmp"); WriteToBmpFile(PathFileName,nWidth,nHeight,bmpHData);
	delete[] bmpHData;

	_splitpath(outfilename,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(outfilename,Ipdri,Ipdir,Ipname,"_A.jpeg");
	SwapBmpToJpg_FileFunc(PathFileName,outfilename,90);
	return TRUE;
}
//---------------------------------------------------------------------------------------
//--------------------------------------ӧñ�任-----------------------------------------
//---------------------------------------------------------------------------------------
void CObjRSImgClass::MssTasseledCapKT4x4_Func(RSImg* mrsimg,UCHAR** pB4data,UCHAR** pB5data,UCHAR** pB6data,UCHAR** pB7data,FILE *wfo1,FILE* wfo2,FILE* wfo3,FILE* wfo4)
{
	if(!mrsimg) return;
	int ImgW0 = mrsimg->Img_W0;
	int ImgH0 = mrsimg->Img_H0; if( ImgW0 <= 0 || ImgH0 <= 0) return;
	int ImgB0 = mrsimg->Img_B0;
	float* v1Adr = new float[ImgW0]; if(!v1Adr) return;
	float* v2Adr = new float[ImgW0]; if(!v2Adr){ delete[] v1Adr; return; }
	float* v3Adr = new float[ImgW0]; if(!v3Adr){ delete[] v1Adr; delete[] v2Adr; return; }
	float* v4Adr = new float[ImgW0]; if(!v4Adr){ delete[] v1Adr; delete[] v2Adr; delete[] v3Adr; return; }
	UCHAR* p4data; UCHAR* p5data; UCHAR *p6data; UCHAR *p7data;
	float KT11 =  0.433;  float KT12 =  0.632;  float KT13 =  0.586;  float KT14 = 0.264;//����仯ϵ��
	float KT21 = -0.290;  float KT22 = -0.562;  float KT23 =  0.600;  float KT24 = 0.491;
	float KT31 = -0.824;  float KT32 =  0.533;  float KT33 = -0.050;  float KT34 = 0.185;
	float KT41 =  0.223;  float KT42 =  0.012;  float KT43 = -0.543;  float KT44 = 0.809;
	int i,j;
	for( j = 0; j < ImgH0; j++ )
	{
		p4data = pB4data[j]; if(!p4data) continue;
		p5data = pB5data[j]; if(!p5data) continue;
		p6data = pB6data[j]; if(!p6data) continue;
		p7data = pB7data[j]; if(!p7data) continue;
		for( i = 0; i < ImgW0; i++ )
		{
			v1Adr[i ] =-9999; v2Adr[i] = -9999; v3Adr[i] = -9999; v4Adr[i] = -9999;
			int d4 = *(p4data + i); if(d4 == ImgB0) continue;
			int d5 = *(p5data + i); if(d5 == ImgB0) continue;
			int d6 = *(p6data + i); if(d6 == ImgB0) continue;
			int d7 = *(p7data + i); if(d7 == ImgB0) continue;
			v1Adr[i] = KT11 * d4 + KT12 * d5 + KT13 * d6 + KT14 * d7;
			v2Adr[i] = KT21 * d4 + KT22 * d5 + KT23 * d6 + KT24 * d7;
			v3Adr[i] = KT31 * d4 + KT32 * d5 + KT33 * d6 + KT34 * d7;
			v4Adr[i] = KT41 * d4 + KT42 * d5 + KT43 * d6 + KT44 * d7;
		}
		fwrite(v1Adr,4,ImgW0,wfo1);
		fwrite(v2Adr,4,ImgW0,wfo2);
		fwrite(v3Adr,4,ImgW0,wfo3);
		fwrite(v4Adr,4,ImgW0,wfo4);	
	}
	delete[] v1Adr; delete[] v2Adr; delete[] v3Adr; delete[] v4Adr;
	return;
}

//---------------------------------------------------------------------------------------
//------------------------------------�г̱���-------------------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::RunLengthCode256BmpFunc(FILE* fio,FILE* fou,FILE* fcd)
{
	fread_s(&bmfh,sizeof(BITMAPFILEHEADER),1,sizeof(BITMAPFILEHEADER),fio);
	fread_s(&bmih,sizeof(BITMAPINFOHEADER),1,sizeof(BITMAPINFOHEADER),fio);
	if(bmfh.bfType        != 0x4D42) { strcpy_s(ErrStr,30,"ͼ���ʽBM����!");   return FALSE; }
	if(bmih.biCompression != BI_RGB) { strcpy_s(ErrStr,30,"ͼ��ѹ����ʽ����!"); return FALSE; }
	if(bmih.biBitCount    != 8     ) { strcpy_s(ErrStr,30,"��256ɫ����!");      return FALSE; }
	long nWidth = bmih.biWidth;
	long nHeight= bmih.biHeight;
	//����ɫģ��
	int PaletteSize = 256;
	long len = PaletteSize * sizeof(RGBQUAD);
	UCHAR* Pdata = new UCHAR[len+1]; if(!Pdata) { strcpy_s(ErrStr,30,"�ڴ����!"); return FALSE; }
	DWORD offBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fseek(fio,offBits,SEEK_SET);
	fread_s(Pdata,len,sizeof(char),len,fio);
	//������
	len = nWidth * nHeight;
	UCHAR *Mdata = new UCHAR[len+1]; if(!Mdata){ strcpy_s(ErrStr,30,"�ڴ����!"); delete[] Pdata; return FALSE; }
	offBits = bmfh.bfOffBits;
	fseek(fio,offBits,SEEK_SET);
	fread_s(Mdata,len,sizeof(char),len,fio);//������һ���Կ��ռ��Ƿ��е㲻��
	//lw���bmp����
	long kk,ii,jj;
	UCHAR ch,och;
	for( jj = nHeight - 1; jj >= 0; jj-- )//�����һ�п�ʼ����
	{
		for( ii = 0; ii < nWidth; ii++ )
		{
			kk = jj * nWidth + ii;
			ch = *(Mdata + kk);
			fprintf_s(fou,"%-d",ch);
		}
		fprintf_s(fou,"\n");
	}
	//����ļ�ͷ
	fprintf_s(fcd,"ImageWH  %5d  %5d  %5d\n",nWidth,nHeight,PaletteSize);
	UCHAR* sdata;
	for( ii = 0; ii < PaletteSize; ii++ )
	{
		sdata = Pdata + ii * sizeof(RGBQUAD);
		UCHAR bg = *sdata;
		UCHAR gg = *(sdata + 1);
		UCHAR rg = *(sdata + 2);
		fprintf_s(fcd,"%5d,%5d,%5d,%5d\n",ii,bg,gg,rg);
	}
	//�γ̳��ȱ���
	int np;
	long lnk = 0;
	for( jj = nHeight - 1; jj >= 0; jj-- )
	{
		for( ii = 0; ii < nWidth; ii++ )
		{
			kk = jj * nWidth + ii;
			ch = *(Mdata + kk);
			if(ii == 0)  { och = ch; np = 1; continue; }
			if(ch == och){ np++; continue; }
			fprintf_s(fcd,"%-d,%-d,",och,np); lnk += 2;
			och = ch; np = 1;
		}
		fprintf_s(fcd,"%-d,%-d %-d\n",och,np,jj); lnk += 2;
	}
	//ѹ����
	len = nHeight*nWidth;
	float t0 = (len - lnk) * 100.0 / len;
	//fprintf_s(fcd," H0*W0 = %5d     lnk = %5d  ѹ���� = %10.2f\%\n",len,lnk,t0);
	fprintf_s(fcd," H0*W0 = %5d     ",len);
	fprintf_s(fcd,"   lnk = %5d ",    lnk);
	fprintf_s(fcd,"  ѹ���� = %10.2f\n",t0);
	delete[] Mdata; delete[] Pdata;
	return TRUE;
}
//---------------------------------------------------------------------------------------
//---------------------------------��Ļ�����ƴͼ::��Ļ���------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::WriteToBmpFile(const char* filename,int nWidth,int nHeight)//д�հ��ļ�
{
	FILE* fou; fopen_s(&fou,filename,"wb"); if(!fou) return FALSE;
	Init_BitMap_FileHeader(nWidth,nHeight,3,&bmfh,&bmih);
	fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fou);
	fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,fou);
	fseek(fou,bmfh.bfOffBits,SEEK_SET);
	long lp8 = bmih.biSizeImage;
	memset(ReadStr,255,512*sizeof(char));
	while(lp8 >= 512)
	{
		fwrite(ReadStr,sizeof(char),512,fou);//���ɿհ׵�bmp�ļ�ÿ�з�512���ֽڵ�����
		lp8 -= 512;
	}
	if(lp8) fwrite(ReadStr,sizeof(char),lp8,fou);
	fclose(fou);
	return TRUE;

}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::DoWriteBmpFileFunc(const char* filename,int nWidth,int nHeight,CDC* dc,CRect* imgrect)//��Ļ���
{
	if(!dc) return FALSE;
	int size = nWidth * nHeight;
	HDC hdc = dc->GetSafeHdc();
	HDC hdcmem = ::CreateCompatibleDC(hdc);
	HBITMAP hbitmap = ::CreateCompatibleBitmap(hdc,nWidth,nHeight);
	HBITMAP holdmap = (HBITMAP)::SelectObject(hdcmem,hbitmap);
	Init_BitMap_FileHeader(nWidth,nHeight,3,&bmfh,&bmih);
	BITMAPINFO* bMapInfo = (BITMAPINFO*)::LocalAlloc(LPTR,sizeof(BITMAPINFO));
	bMapInfo->bmiHeader.biBitCount      = 24;
	bMapInfo->bmiHeader.biClrImportant  = 0;
	bMapInfo->bmiHeader.biCompression   = 0;
	bMapInfo->bmiHeader.biHeight        = nHeight;
	bMapInfo->bmiHeader.biPlanes        = 1;
	bMapInfo->bmiHeader.biSize          = sizeof(BITMAPINFO);
	bMapInfo->bmiHeader.biSizeImage     = size * 3;
	bMapInfo->bmiHeader.biWidth         = nWidth;
	bMapInfo->bmiHeader.biXPelsPerMeter = 0;
	bMapInfo->bmiHeader.biYPelsPerMeter = 0;
	BITMAPFILEHEADER bmfh;
	bmfh.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize      = sizeof(BITMAPFILEHEADER);
	bmfh.bfType      = 0x4D42;
	::BitBlt(hdcmem,0,0,nWidth,nHeight,hdc,0,0,SRCCOPY);//��hdc���ڴ渴�Ƶ�hdcmem�У�������ѡ����hdcmem���ػ�
	RGBTRIPLE* rgb = new RGBTRIPLE[size];
	ZeroMemory(rgb,sizeof(byte) * size * 3);
	::GetDIBits(hdc,hbitmap,0,nHeight,rgb,bMapInfo,DIB_RGB_COLORS);
	FILE* fou; fopen_s(&fou,filename,"wb");
	fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fou);
	fwrite(&bMapInfo->bmiHeader,sizeof(BITMAPINFOHEADER),1,fou);
	fwrite(rgb,bMapInfo->bmiHeader.biSizeImage,1,fou);
	fclose(fou);
	LocalFree(bMapInfo);
	delete[] rgb;
	SelectObject(hdcmem,holdmap);
	DeleteObject(hbitmap);
	DeleteDC(hdcmem);
	DeleteDC(hdc);
}
//---------------------------------------------------------------------------------------
//---------------------------------��Ԫ�ز���--------------------------------------------
//---------------------------------------------------------------------------------------
UCHAR CObjRSImgClass::NearestNeighbor(float x0,float y0,int width,int height,UCHAR** sData,UCHAR B0)//���ڽ���Ԫ��
{
	int ix = x0 + 0.5; if( ix < 0 || ix >= width  ) return B0;
	int iy = y0 + 0.5; if( iy < 0 || iy >= height ) return B0;
	UCHAR* pdata = sData[iy];
	UCHAR ch = *(pdata + ix);
	return ch;
}
/*
UCHAR CObjRSImgClass::BilinearInterpolation(float x0,float y0,int widht,int height,UCHAR **sData)//˫���Բ巨�����������������
{

}
*/
//---------------------------------------------------------------------------------------
UCHAR CObjRSImgClass::BilinearInterpolation(float x0,float y0,int widht,int height,UCHAR **sData,UCHAR B0)//˫���Բ巨���������������
{
	int ix = x0 + EPSILON; if( ix < 0 || ix >= widht  ) return B0;
	int iy = y0 + EPSILON; if( iy < 0 || iy >= height ) return B0;
	int i1,j1;
	UCHAR* pdata;
	UCHAR v1,v2,v3,v4;
	float Vr;
	float dx,dy,fn,fm;
	float nn=height-1-EPSILON;
	if(x0 < EPSILON)
	{
		if(y0 < EPSILON)
		{
			pdata = sData[0];
			Vr = *(pdata);
		}
		else if(y0 > height - 1 - EPSILON)
		{
			pdata = sData[height - 1];
			Vr = *pdata;
		}
		else
		{
			j1 = y0;
			dy = y0-j1;//ע��y0��float��j1��int
			pdata = sData[j1]; v1 = *pdata;
			pdata = sData[j1 + 1]; v3 = *pdata;
			if      (v1 == B0) Vr = v3;
			else if (v3 == B0) Vr = v1;
			else               Vr = (v3 - v1) * dy + v1;
		}
	}
	else if(y0 < EPSILON)
	{
		if(x0 > widht - 1 - EPSILON)
		{
			pdata = sData[0];
			Vr = *(pdata + widht - 1);
		}
		else
		{
			i1 = x0;
			dx = x0 - i1;
			pdata = sData[0];
			v1 = *(pdata + i1);
			v2 = *(pdata + i1 + 1);
			if      (v1 == B0) Vr = v2;
			else if (v2 == B0) Vr = v1;
			else               Vr = (v2 - v1) * dx + v1;
		}
	}
	else if(x0 > widht - 1 - EPSILON)
	{
		if(y0 > height - 1 - EPSILON){ pdata = sData[height - 1]; Vr = *(pdata + widht - 1); }
		else
		{
			j1 = y0;dy = y0 - j1;
			pdata = sData[j1];   v2 = *( pdata + widht - 1 );
			pdata = sData[j1+1]; v4 = *( pdata + widht -1 );
			if      (v2 == B0) Vr = v4;
			else if (v4 == B0) Vr = v2;
			else               Vr = (v4 - v2) * dy + v2;
		}
	}
	else if(y0 > height - 1 - EPSILON)
	{
		i1 = x0; dx = x0 -i1;
		pdata = sData[height - 1];
		v3 = *(pdata + i1);
		v4 = *(pdata + i1 + 1);
		if      (v3 == B0) Vr = v4;
		else if (v4 == B0) Vr = v3;
		else               Vr = (v4 - v3) * dx + v3;
	}
	else
	{
		i1 = x0; dx = x0 - i1;
		j1 = y0; dy = y0 - j1;
		pdata = sData[j1];     v1= *(pdata + i1); v2 = *(pdata + i1 + 1);
		pdata = sData[j1 + 1]; v3= *(pdata + i1); v4 = *(pdata + i1 + 1);
		if      (v3 == B0) fm = v1;
		else if (v1 == B0) fm = v3;
		else               fm = (v3 - v1) * dy + v1;
		if      (v4 == B0) fn = v2;
		else if (v2 == B0) fn = v4;
		else               fn = (v4 - v2) * dy + v2;
		v1 = fm + 0.5;
		v3 = fn + 0.5;
		if      (v3 == B0) Vr = fm;
		else if (v1 == B0) Vr = fn;
		else               Vr = (fn - fm) * dx + fm;
	}
	return Vr + 0.5;
}
/*
UCHAR CObjRSImgClass::CubicConvolutionInUCHARFile(float x0,float y0,int ImgW0,int ImgH0,UCHAR **sData)//˫���ξ���������������������
{

}
*/
//---------------------------------------------------------------------------------------
UCHAR CObjRSImgClass::CubicConvolutionInUCHARFile(float x0,float y0,int ImgW0,int ImgH0,UCHAR **sData,int B0)//˫���ξ�������������������
{
	int ix = x0 + EPSILON; if(ix < 0 || ix >= ImgW0) return B0;
	int iy = y0 + EPSILON; if(iy < 0 || iy >= ImgH0) return B0;
	int j1 = iy - 1; if(j1 < 0 )     j1 = iy;
	int j3 = iy + 1; if(j3 >= ImgH0) j3 = iy;
	int j4 = iy + 2; if(j4 >= ImgH0) j4 = iy;
	int i1 = ix - 1; if(i1 < 0)      i1 = ix;
	int i3 = ix + 1; if(i3 >= ImgW0) i3 = ix;
	int i4 = ix + 2; if(i4 >= ImgW0) i4 = ix;
	UCHAR* pdata1 = sData[j1];
	UCHAR* pdata2 = sData[iy];
	UCHAR* pdata3 = sData[j3];
	UCHAR* pdata4 = sData[j4];
	UCHAR v22 = *(pdata2 + ix); if(v22 == B0) return v22;
	UCHAR v11 = *(pdata1 + i1); if(v11 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v12 = *(pdata1 + ix); if(v12 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v13 = *(pdata1 + i3); if(v13 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v14 = *(pdata1 + i4); if(v14 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v21 = *(pdata2 + i1); if(v21 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v23 = *(pdata2 + i3); if(v23 == B0) return v22;
	UCHAR v24 = *(pdata2 + i4); if(v24 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v31 = *(pdata3 + i1); if(v31 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v32 = *(pdata3 + ix); if(v32 == B0) return v22;
	UCHAR v33 = *(pdata3 + i3); if(v33 == B0) return v22;
	UCHAR v34 = *(pdata4 + i4); if(v34 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v41 = *(pdata4 + i1); if(v41 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v42 = *(pdata4 + ix); if(v42 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v43 = *(pdata4 + i3); if(v43 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	UCHAR v44 = *(pdata4 + i4); if(v44 == B0) return BilinearInterpolation(x0,y0,ImgW0,ImgH0,sData,B0);
	float dx =  x0  - ix; float dy  = y0  - iy;
	float dx2 = dx  * dx; float dy2 = dy  * dy;
	float dx3 = dx2 * dx; float dy3 = dy2 * dy;
	float wx1 =  -dx + 2 * dx2 - dx3;
	float wx2 =    1 - 2 * dx2 + dx3;
	float wx3 =   dx + dx2 - dx3;
	float wx4 = -dx2 + dx3;
	float wy1 =  -dy + 2 * dy2 - dy3;
	float wy2 =    1 - 2 * dy2 + dy3;
	float wy3 =   dy + dy2 - dy3;
	float wy4 = -dy2 + dy3;
	float Vr  =  v11 * wx1 * wy1 + v21 * wx1 * wy2 + v31 * wx1 * wy3 + v41 * wx1 * wy4 +
		         v12 * wx2 * wy1 + v22 * wx2 * wy2 + v32 * wx2 * wy3 + v42 * wx2 * wy4 +
		         v13 * wx3 * wy1 + v23 * wx3 * wy2 + v33 * wx3 * wy3 + v43 * wx3 * wy4 +
		         v14 * wx4 * wy1 + v24 * wx4 * wy2 + v34 * wx4 * wy4 + v44 * wx4 * wy4;
	return Vr;
}
//---------------------------------------------------------------------------------------
//-----------------------------------------�����Ż�--------------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::BmpToEtmFileFunc(RSImg* mrsimg,const char* filename,BOOL DownBmpBol)//����bmp�ļ� bmp=>Etm
{
	if(!PathFileExists(filename)) { strcpy_s(ErrStr,20,"�ļ�������!"); return FALSE; }
	FILE*fio; fopen_s(&fio,filename,"rb"); if(!fio) { strcpy_s(ErrStr,20,"�ļ��򿪴���!"); return FALSE; }
	bool wbool=ReadBmpFileToEtmDataFunc(mrsimg,filename,fio,DownBmpBol);
	fclose(fio);
	return wbool;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpFileToEtmDataFunc(RSImg* mrsimg,const char* filename,FILE* fio,BOOL DownBmpBol)
{
	fread_s(&bmfh,sizeof(BITMAPFILEHEADER),sizeof(BITMAPFILEHEADER),1,fio);
	fread_s(&bmih,sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER),1,fio);
	if(bmfh.bfType        != 0x4D42) { strcpy_s(ErrStr,30,"ͼ���ʽBM����!");    return FALSE; }
	if(bmih.biCompression != BI_RGB) { strcpy_s(ErrStr,30,"ͼ��ѹ����ʽ����!");  return FALSE; }
	if(bmih.biBitCount    != 24    ) { strcpy_s(ErrStr,30,"��24bit����!");       return FALSE; }
	long offBits = bmfh.bfOffBits;
	int nWidth   = bmih.biWidth;
	int nHeight  = bmih.biHeight;
	strcpy_s(ErrStr,20,"�ڴ����!");
	Delete_Char_RSImg_MemorySpace(mrsimg);//ɾ��ԭ����Ӱ��ռ�
	bool wbool=Gene_Char_RSImg_MemorySpace(nHeight,nWidth,mrsimg);
	if(!wbool) return FALSE;

	_splitpath(filename,Ipdri,Ipdir,Ipname,Ipext);
	strcpy_s(ReadStr,50,Ipname); strcat(ReadStr,"R.img"); strcpy_s(mrsimg->R_name,50,ReadStr);
	strcpy_s(ReadStr,50,Ipname); strcat(ReadStr,"G.img"); strcpy_s(mrsimg->G_name,50,ReadStr);
	strcpy_s(ReadStr,50,Ipname); strcat(ReadStr,"B.img"); strcpy_s(mrsimg->B_name,50,ReadStr);

	bool wbol=ReadBmpToEtmBy_RGB_FileFunc(mrsimg,fio,nHeight,nWidth,offBits,DownBmpBol);
	if(!wbol) return FALSE;
	//ͳ��Ӱ������ֱ��ͼ��Ӱ�����ɹ����ͳ��
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Rimg,mrsimg->factR,&mrsimg->InRmin,&mrsimg->InRmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Gimg,mrsimg->factG,&mrsimg->InGmin,&mrsimg->InGmax);
	Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Bimg,mrsimg->factB,&mrsimg->InBmin,&mrsimg->InBmax);
	if(mrsimg->InMBol)
		Get_Data_UChar_Fact_Func(mrsimg->Img_H0,mrsimg->Img_W0,mrsimg->Img_B0,mrsimg->Mimg,mrsimg->factM,&mrsimg->InMmin,&mrsimg->InMmax);
	mrsimg->ModifyBol = TRUE;
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::ReadBmpToEtmBy_RGB_FileFunc(RSImg* mrsimg,FILE* fio,int nHeight,int nWidth,long offBits,BOOL DownBmpBol)
{
	char* pdata=new char [3*nWidth+1]; if(!pdata) { strcpy_s(ErrStr,30,"�ڴ�������!"); return FALSE; } memset(pdata,0,3*nWidth);

	fseek(fio,offBits,SEEK_SET);
	int i,j,j0;
	UCHAR* rline,*bline,*gline;
	for( j = 0; j < nHeight; j++)
	{
		fread_s(pdata,3*nWidth,1,3*nWidth,fio);//B G R B G R
		j0=j; if (!DownBmpBol) j0 = nHeight - 1 - j;
		rline=mrsimg->Rimg[j0];
		gline=mrsimg->Gimg[j0];
		bline=mrsimg->Bimg[j0];
		for( i = 0; i < nWidth; i++)
		{
			*(bline + i) = *(pdata + i*3 +0);//B B B
			*(gline + i) = *(pdata + i*3 +1);
			*(rline + i) = *(pdata + i*3 +2);
		}
	}
	return TRUE;    
}
//---------------------------------------------------------------------------------------
//-------------------------------------ң�����ݱ���--------------------------------------
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::SaveRSDataToImg_Func(RSImg* mrsimg)//����Ϊ000+img��ʽ
{
	if(!mrsimg) return FALSE;
	_splitpath(LoadSaveFileName,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".000");
	int nWidth=mrsimg->Img_W0,nHeight=mrsimg->Img_H0;
	FILE* fou; fopen_s(&fou,PathFileName,"w");
	if(fou)
	{
		fprintf(fou,"ImageWH   %5d %5d\n",nWidth,nHeight);
		strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"R.img"); fprintf(fou,"ETM_BAND  3  0  0  255 %-s\n",ReadStr);
		strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"G.img"); fprintf(fou,"ETM_BAND  2  0  0  255 %-s\n",ReadStr);
		strcpy_s(ReadStr,50,Ipname); strcat_s(ReadStr,50,"B.img"); fprintf(fou,"ETM_BAND  1  0  0  255 %-s\n",ReadStr);
		fclose(fou);
	}
	fnmerge(PathFileName,Ipdri,Ipdir,mrsimg->R_name,"");
	FILE* rfio; fopen_s(&rfio,PathFileName,"wb"); if(!rfio){ strcpy_s(ErrStr,50,"R�������!"); return FALSE; }
	fnmerge(PathFileName,Ipdri,Ipdir,mrsimg->G_name,"");
	FILE* gfio; fopen_s(&gfio,PathFileName,"wb"); if(!gfio){ strcpy_s(ErrStr,50,"G�������!"); return FALSE; }
	fnmerge(PathFileName,Ipdri,Ipdir,mrsimg->B_name,"");
	FILE* bfio; fopen_s(&bfio,PathFileName,"wb"); if(!bfio){ strcpy_s(ErrStr,50,"B�������!"); return FALSE; }


	int i,j;
	for(j=0;j<nHeight;j++)
	{
		fwrite(mrsimg->Rimg[nHeight-1-j],1,nWidth,rfio);
		fwrite(mrsimg->Gimg[nHeight-1-j],1,nWidth,gfio);
		fwrite(mrsimg->Bimg[nHeight-1-j],1,nWidth,bfio);
	}
	fclose(rfio); fclose(gfio); fclose(bfio);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::SaveRSDataToBmp_Func(RSImg* mrsimg)//����Ϊbmp��ʽ
{
	if(!mrsimg) return FALSE;
	int nHeight=mrsimg->Img_H0,nWidth=mrsimg->Img_W0;
	_splitpath(LoadSaveFileName,Ipdri,Ipdir,Ipname,Ipext);
	fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".bmp");
	Init_BitMap_FileHeader(nWidth,nHeight,3,&bmfh,&bmih);
	FILE* fio; fopen_s(&fio,PathFileName,"wb"); if(!fio) return FALSE;
	UCHAR* pdata=new UCHAR[3*nWidth+1]; if(!pdata){ strcpy_s(ErrStr,50,"�ڴ�������!"); return FALSE; }
	UCHAR* rline,*gline,*bline;
	fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fio);
	fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,fio);
	fseek(fio,bmfh.bfOffBits,SEEK_SET);

	int i,j;
	for(j=0;j<nHeight;j++)
	{
		rline=mrsimg->Rimg[nHeight-1-j];
		gline=mrsimg->Gimg[nHeight-1-j];
		bline=mrsimg->Bimg[nHeight-1-j];
		for(i=0;i<nWidth;i++)
		{
			*(pdata+i*3)=*(bline+i);
			*(pdata+i*3+1)=*(gline+i);
			*(pdata+i*3+2)=*(rline+i);
		}
		fwrite(pdata,1,3*nWidth,fio);
	}
	delete[] pdata; fclose(fio);
	return TRUE;
}
//---------------------------------------------------------------------------------------
BOOL CObjRSImgClass::SaveRSDataToJpg_Func(RSImg* mrsimg)//����Ϊjpg��ʽ
{
	BOOL wbol=SaveRSDataToBmp_Func(mrsimg);
	if(!wbol) return FALSE;
	else
	{
		_splitpath(PathFileName,Ipdri,Ipdir,Ipname,Ipext);
		char JPGSaveFileName[200];
		fnmerge(JPGSaveFileName,Ipdri,Ipdir,Ipname,".jpg");
		return SwapBmpToJpg_FileFunc(PathFileName,JPGSaveFileName,90);
	}
}



