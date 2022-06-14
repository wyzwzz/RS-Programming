
#ifndef MYFUNC_H
#define MYFUNC_H
#include"stdafx.h"
#include"MyObject.h"
#include"tiff-debug\include\tiffio.h"

//---------------------------
//�����
extern int PointNum;
extern Point *HeadPoint;
extern Point *EndPoint;
extern CObjPointClass objpoint;
//�����
extern int CurveNum;
extern Curve *HeadCurve;
extern Curve *EndCurve;
extern CObjCurveClass objcurve;
extern float TablCoorAdr[];
extern int TablPointNum;
//������ز���
extern long MouseDraw;
extern long shpfileflag;
extern CString mshpfilename;
//ң���������
extern BOOL SetGrayBol;
extern RSImg *mrsimg;
COLORREF CustColors[];
extern CObjRSImgClass objrsimg;
//��ȡ�ļ����
extern char Ipdri[],Ipdir[],Ipname[],Ipext[];
extern char LoadSaveFileName[],OnlyFileName[],PathFileName[];
extern char RecoCode[],ErrStr[],ReadStr[];
extern char *SS_Dat[];
//---------------------------
//---------------------------------------------------------
//��������
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