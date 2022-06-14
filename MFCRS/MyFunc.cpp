#include"stdafx.h"
#include"MyObject.h"
#include<math.h>
#pragma once
#define Cutoff 10

//---------------------------
//点相关
int PointNum=0;
Point *HeadPoint=NULL;
Point *EndPoint=NULL;
CObjPointClass objpoint(2);
//线相关
int CurveNum=0;
Curve *HeadCurve=NULL;
Curve *EndCurve=NULL;
CObjCurveClass objcurve(2);
float TablCoorAdr[2*MAXIPOINTNUM];
int TablPointNum=0;
//点线相关参数
long MouseDraw=0;
long shpfileflag=0;
CString mshpfilename="";
//遥感数据相关
BOOL SetGrayBol=FALSE;
RSImg *mrsimg=NULL;
COLORREF CustColors[256];
CObjRSImgClass objrsimg(2);
//读取文件相关
char Ipdri[MAXDRIVE],Ipdir[MAXDIR],Ipname[MAXFILE],Ipext[MAXEXT];
char LoadSaveFileName[MAXPATH],OnlyFileName[MAXPATH],PathFileName[MAXPATH];
char RecoCode[50];
char ErrStr[50];
char ReadStr[1024];
char *SS_Dat[MAX_DAT+1];
//---------------------------
//---------------------------------------------------------
int SsToStr60(char* str,char* s_dat[])//打断字符串
{
	int n = 0;
	char ch,*pstr;
	s_dat[0] = str;//no need???
	ch = *str; if( ch == '\0' || ch == '\n' ) return 0;
	strtok_s(str,", ;\t\r\n",&pstr);
	while(*pstr != '\0')
	{
		if(*pstr != '\0')
		{
			if(n < MAX_DAT) n++;
			s_dat[n] = pstr;
		}
	strtok_s(NULL,", ;\t\r\n",&pstr);
	}
	return n+1;
}
//---------------------------------------------------------
void fnmerge( char * pathfilename,char *dri,char *dir,char *name,char *ext)
{//通过简单的循环将四个字符串连接存到pathfilename中
	int i = 0,j = 0;
	while(dri[j] != '\0')
		pathfilename[i++] = dri[j++];
	j = 0;
	while(dir[j] != '\0')
		pathfilename[i++] = dir[j++];
	j = 0;
	while(name[j] != '\0')
	{
		if(name[j] == ' ')
		{ j++; continue; }
		pathfilename[i++] = name[j++];
	}
	j = 0;
	while(ext[j] != '\0')
		pathfilename[i++] = ext[j++];
	pathfilename[i] = '\0';
}
//---------------------------------------------------------
BOOL ReadOneTextColorSpectrmFunc(const char* filename)//读入二位散点图的颜色数据
{
	int ii;
	for( ii = 0; ii < 256; ii++ ) CustColors[ii] = RGB(255-ii,255-ii,255-ii);
	FILE* rfi; fopen_s(&rfi,filename,"r"); if(!rfi) return FALSE;
	fgets(ReadStr,450,rfi); if(!strstr(ReadStr,"ZOUCOLORLSTLIB")){ fclose(rfi); return FALSE; }
	int nn;
	int nk = 0;
	UCHAR rc,gc,bc;
	for( ii = 0; ii < 256; ii++ )
	{
		fgets(ReadStr,450,rfi); nn = SsToStr60(ReadStr,SS_Dat); if(nn < 3) continue;
		rc = atoi(SS_Dat[1]);
		gc = atoi(SS_Dat[2]);
		bc = atoi(SS_Dat[3]);
		CustColors[nk++] = RGB(rc,gc,bc);
	}
	fclose(rfi);
	return TRUE;
}
//---------------------------------------------------------
//------------------------插入排序-------------------------
//---------------------------------------------------------
void Swap(ElementType* A,ElementType* B)//交换连个变量的值
{
	ElementType temp;
	temp = *A; *A = *B; *B=temp;
	return;
}
//---------------------------------------------------------
void InsertionSort(ElementType A[],int N)//插入排序
{
	ElementType temp;
	int i,j;
	for( i = 0; i < N - 1; i++ )
	{
		temp = A[i + 1];
		for( j = i; j >= 0; j-- )
		{
			if(A[j] > temp)
				A[j + 1] = A[j];
			else
			{
				A[j + 1] = temp;
				break;
			}
		}
		if(j < 0)
			A[0] = temp;
	}
}
//---------------------------------------------------------
//---------------------------快速排序----------------------
//---------------------------------------------------------
ElementType Median3(ElementType A[],int Left,int Right)
{
	int median;
	
	median = (Left + Right) / 2;
	if(A[Left] > A[median])
		Swap(&A[Left],&A[median]);
	if(A[Left] > A[Right])
		Swap(&A[Left],&A[Right]);
	if(A[median] > A[Right])
		Swap(&A[median],&A[Right]);
	Swap(&A[median],&A[Right - 1]);
	return A[Right - 1];
}
//---------------------------------------------------------
void QuickSort(ElementType A[],int Left,int Right)
{
	ElementType Pivot;
	int i,j;
	if(Right - Left >= Cutoff)
	{
		Pivot = Median3(A,Left,Right);
		i = Left;
		j = Right - 1;
		while(TRUE)
		{
			while(A[++i] < Pivot){}
			while(A[--j] > Pivot){}
			if(i < j)
				Swap(&A[i],&A[j]);
			else
				break;
		}
		Swap(&A[i],&A[Right - 1]);
		QuickSort(A,Left,i - 1);
		QuickSort(A,i + 1,Right);
	}
	else
		InsertionSort(A + Left,Right - Left + 1);
}
//---------------------------------------------------------
void Quick_Sort(ElementType A[],int N)
{
	QuickSort(A,0,N - 1);
}
//---------------------------------------------------------
//-----------------------高斯噪音值产生--------------------
//---------------------------------------------------------
double GenerateGaussianNoise(double mu,double sigma)
{
	static double V1,V2,S;
	static int phase = 0;
	double X;
	double U1,U2;
	if(!phase)
	{
		do{
			U1 = (double)rand() / RAND_MAX;
			U2 = (double)rand() / RAND_MAX;

			V1=2 * U1 - 1;
			V2=2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		}while(S >= 1 || !S);
		X=V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	return mu + X * sigma;
}

