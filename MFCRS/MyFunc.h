
#ifndef MYFUNC_H
#define MYFUNC_H
#include"stdafx.h"
#include"MyObject.h"
#include"tiff-debug\include\tiffio.h"

//---------------------------
//点相关
extern int PointNum;
extern Point *HeadPoint;
extern Point *EndPoint;
extern CObjPointClass objpoint;
//线相关
extern int CurveNum;
extern Curve *HeadCurve;
extern Curve *EndCurve;
extern CObjCurveClass objcurve;
extern float TablCoorAdr[];
extern int TablPointNum;
//点线相关参数
extern long MouseDraw;
extern long shpfileflag;
extern CString mshpfilename;
//遥感数据相关
extern BOOL SetGrayBol;
extern RSImg *mrsimg;
COLORREF CustColors[];
extern CObjRSImgClass objrsimg;
//读取文件相关
extern char Ipdri[],Ipdir[],Ipname[],Ipext[];
extern char LoadSaveFileName[],OnlyFileName[],PathFileName[];
extern char RecoCode[],ErrStr[],ReadStr[];
extern char *SS_Dat[];
//---------------------------
//---------------------------------------------------------
//函数申明
int SsToStr60(char* str,char* s_dat[]);
void fnmerge(char * pathfilename,char *dri,char *dir,char *name,char *ext);
BOOL ReadOneTextColorSpectrmFunc(const char* filename);
void InsertionSort(ElementType A[],int N);
void Swap(ElementType* A,ElementType* B);
void Quick_Sort(ElementType A[],int N);
void QuickSort(ElementType A[],int Left,int Right);
int Median3(ElementType A[],int Left,int Right);
double GenerateGaussianNoise(double mu,double sigma);

#endif