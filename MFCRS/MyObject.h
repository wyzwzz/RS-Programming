#ifndef MYOBJECT_H
#define MYOBJECT_H
#include"stdafx.h"
//---------------------------------------------------
//�궨��
//---------------------------------------------------
#define UCHAR unsigned char
#define Float_Type float
#define ElementType UCHAR
//---------------------------------------------------
#define MOUSE_EDIT_POINT 0x00000001L//edit point
#define MOUSE_EDIT_CURVE 0x00000002L//edit curve
#define MOUSE_INSL_BEGIN 0x00000020L//ins line begin
#define MOUSE_DRAW_BEGIN 0x00000010L//�϶�һ���λ�һֱ�ߣ�����꣩������
#define FILE_POINT_FLAG 0x00000001L
#define FILE_CURVE_FLAG 0x00000002L
#define FILE_PANDC_FLAG 0x00000004L
#define BMP_MOVE_FLAG 0x00000008L
#define ForLoop(p,b) for(p=b;p!=NULL;p=p->Next)
#define MAXIPOINTNUM 1000
#define MAXPATH 200
#define MAXDRIVE 100
#define MAXDIR 100
#define MAXFILE 100
#define MAXEXT 100
#define MAX_DAT 100
#define EPSILON 0.000001

//---------------------------------------------------
//define point list struct
//---------------------------------------------------
typedef struct PointStruct Point;
struct PointStruct{
	short CharNum;//��ID
	float X0,Y0;//������
	char *TextAdr;//��ע��
	Point *Next;
};
//-------------------------------------------
//define point class
//-------------------------------------------
class CObjPointClass
{
public:
	CObjPointClass(int x=1){};
	~CObjPointClass(){};
public:
//List operation
	Point* AddOnePoint(float X0,float Y0,CString text);
	Point* AddOnePoint(float X0,float Y0,const char *text);
	void DeletePointLst();
//input and output
	BOOL LoadPointFunc(CString filename);
	void SavePointFile(CString filename);
	int GetAllPointSize();
//screen printf
	void PaintMyPointFunc(CDC *dc);
	void DrawOnePoint(CDC *dc,int x0,int y0,int len,char*text);
};
//----------------------------------------------
//define curve struct
//----------------------------------------------
typedef struct CurveStruct Curve;
struct CurveStruct
{
	short FoColoNo;//�ߵ�ID
	short NodeNum;//�ߵĵ���
	float *CoorAdr;//�����������
	Curve *Next;
};
//-----------------------------------------------
//define curve class
//-----------------------------------------------
class CObjCurveClass
{
public:
	CObjCurveClass(int x){};
	~CObjCurveClass(){};
public:
	Curve* AddOneCurve(int np,float* cooradr);
	void DeleteCurveFunc();
	BOOL LoadCurveFunc(CString filename);
	BOOL SaveCurveFunc(CString filename);
	int GetAllCurveSize();
	void PaintMyCurveFunc(CDC* dc);
	void DrawOneCurve(CDC* dc,int n,float* pcoor);
};
//--------------------------------------------------
//define remote sensing image data struct
//--------------------------------------------------
typedef struct RSImgStruct RSImg;
struct RSImgStruct{
	int Img_W0,Img_H0,Img_B0;//Ӱ����   B0:����ֵ
	UCHAR **Rimg;            //Ӱ������
	UCHAR **Gimg;
	UCHAR **Bimg;
	UCHAR **Mimg;
	UCHAR InRmin,InRmax;//����Ӱ����С���ֵ�����������ͼ������ͳ�ƺ�õ�
	UCHAR InGmin,InGmax;
	UCHAR InBmin,InBmax;
	UCHAR InMmin,InMmax;
	int   EnRmin,EnRmax;//��ʾӰ�����С���ֵ��һ��Ϊ0-255��Ϊͼ����ʾλ�����
	int   EnGmin,EnGmax;
	int   EnBmin,EnBmax;
	int   EnMmin,EnMmax;
	char  R_name[MAXPATH];//Ӱ���ļ���
	char  G_name[MAXPATH];
	char  B_name[MAXPATH];
	char  M_name[MAXPATH];
	BOOL  InMBol,ModifyBol;
	long  factR[256],factG[256],factB[256],factM[256];//Ӱ��Ƶ�ʷֲ�
	int   ImgXoff,ImgYoff;
	long  FastLen;
	int   FastWidth,FastHeight;//������ʾ��
	char *FastData;//��ɫ�ã���ң��Ӱ��XOR
	BITMAPINFO FastInfo;
};
//-------------------------------------------------
//���� ң��Ӱ�� �����
//-------------------------------------------------
class CObjRSImgClass
{
public:
	BOOL istostart;
	CObjRSImgClass(int x=1);
	~CObjRSImgClass(){};
public:
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
public:
	//�ڴ�ռ䴦��
	BOOL    Gene_Char_RSImg_MemorySpace(int Height,int Width,RSImg *rsimg);
	UCHAR** Gene_Char_MatrixMemorySpace(int Height,int Width);
	UCHAR** Gene_Char_MatrixMemorySpace(int Height,int Width,BOOL *pGeneBol);
	void    Delete_Char_MatrixMemorySpace(int Height,UCHAR **pData);
	void    Delete_Char_RSImg_MemorySpace(RSImg *rsimg);
	//---------------------------------------------------
	long** Gene_Long_MatrixMemorySpace(int Height,int Width);
	long** Gene_Long_MatrixMemorySpace(int Height,int Width,BOOL *pGeneBol);
	void   Delete_Long_MatrixMemorySpace(int Height,long **pData);
	//---------------------------------------------------
	//����Ӱ��
	BOOL Read000RSImgFile_Func(const char * filename,int *pImgW,int *pImgH,char *Rname,char *Gname,char *Bname,char *Mname,int *Rmin,int *Rmax,int *Gmin,int *Gmax,int *Bmin,int *Bmax,int *Mmin,int *Mmax);
	BOOL ReadRSFileData_Func(RSImg *rsimg,const char *B000name,int Width,int Height,char *Rname,char *Gname,char *Bname,char *Mname,BOOL DownBol);
	BOOL ReadRS_Data_UCHAR_Func(const char* filename,int Height,int Width,UCHAR **pData,BOOL DownBol);
	BOOL ReadRS_Data_UCHAR_Func(FILE *fio,int Height,int Width,UCHAR **pData,BOOL DownBol);
	//Ӱ������
	BOOL GencRemoteImgFunc(RSImg *rsimg,int bmpw,int bmph);//�ز���
	BOOL GencRemoColorImgFunc(RSImg *rsimg,int bmpw,int bmph);
	BOOL GencRemoGray_ImgFunc(RSImg *rsimg,int bmpw,int bmph);
	void AllocFastData(RSImg *rsimg,int nHeight,int nWidth);//������ʾ�ڴ�
	char* AllocFastData(RSImg *rsimg,int nHeight,int nWidth,int nBits);
	void DeleteFastData(RSImg *rsimg);//ɾ���ڴ�ռ�
	void Init_BitMap_Info(int nWidth,int nHeight,int colorbit,BITMAPINFO* pinfo);//��ʼ��λͼͷ�ļ���Ϣ
	//Ӱ����ʾ
	void PaintRemoteImgFunc(CDC *dc,RSImg *rsimg);//����strechDIBits��ͼ
	void Plot_Etm_Bitmap_By_Fast_File(CDC* dc,int xL,int yT,int bW0,int bH0,char *pdata,int CopyMode);//��������������ʾʧ��ʱ��
	BOOL WriteToBmpFile(const char* filename,int nWidth,int nHeight,char *lpBits);
	void Init_BitMap_FileHeader(int nWidth,int nHeight,int colorbit,BITMAPFILEHEADER* pbmfh,BITMAPINFOHEADER* pbmih);
    //ֱ��ͼͳ��
	long Get_Data_UChar_Fact_Func(int ImgH0,int ImgW0,int ImgB0,UCHAR** pData,long* factData,UCHAR* fmin,UCHAR* fmax);
	void Get_RSNew_Band_Fact_Func(RSImg* mrsimg,int RGB_Min,int RGB_Max,float Min_Perc,float Max_Perc);//�Զ�����
	BOOL Get_RSNew_Band_Fact_Func(long* pfact,int RGB_Min,int RGB_Max,float Min_Perc,float Max_Perc,int* psn1,int* psn2);
    //��άɢ��ͼ
	void PlotScatDiagramFunc(CDC* dc,int ZoomW,int ZoomH,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax);
	void FillScatDiagramFunc(CDC* dc,int ZoomW,int ZoomH,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax,BOOL SmooBol,int nr0);
	void CalcFreqDistZoomFunc(int ZoomW,int ZoomH,long** XYimg,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax,BOOL SmooBol,int nr0);//���㴰�ڵ�ķֲ�Ƶ��
	long Get_Long_Min_Max_Avg_S0_Func(int ImgH0,int ImgW0,int ImgB0,long** pData,long* pfmin,long* pfmax,double* pAvg,double* pS0);
	void Draw_One_Cross(CDC* dc,int x0,int y0,int len);
	//��������
	void SaltPepperNoiseGrayFunc(RSImg* mrsimg,int SaNum,int PeNum);
	void SaltPepperNoiseColorFunc(RSImg* mrsig,int SaNum,int PeNum);
	void SaltandPepperNoiseFunc(int ImgW0,int ImgH0,UCHAR** Hdata,int SaNum,int PeNum);
	void SaltandPepperNoiseFunc(int ImgW0,int ImgH0,UCHAR** HdataR,UCHAR** HdataG,UCHAR** HdataB,int SaNum,int PeNum);//ȫɫ����Ҫ��RGBһ�������
	//��˹����
	void AddGussianNoiseGray(RSImg* mrsimg,double mu,double sigma,int k);
	void AddGussianNoiseColor(RSImg* mrsimg,double mu,double sigma,int k);
	void GussianNoise(int ImgW0,int ImgH0,UCHAR** Mdata,double mu,double sigma,int k);
	void GussianNoise(int ImgW0,int ImgH0,UCHAR** Rdata,UCHAR** Gdata,UCHAR** Bdata,double mu,double sigma,int k);
	//С���任
	void MyHaar_8_BitTransform(UCHAR** Rimg,int ImgW0,int ImgH0,int level,BOOL Invbol);//levelΪ�任����
	void HaarTransform(Float_Type *Hdata,int ImgW0,int ImgH0,int level);//���仯
	void HaarRowDecompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//�зֽ�
	void HaarColDecompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//�зֽ�
	void HaarIvsTransform(Float_Type *Hdata,int ImgW0,int ImgH0,int level);//��任
	void HaarRowCompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//�кϳ�
	void HaarColCompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//�кϳ�
	//ͼ����ת
	void RSImgRotete(RSImg* mrsimg,BOOL ClockRotate);
	void LRotate90(int *pImgW,int *pImgH,int nBits,UCHAR* pNBits,UCHAR* pOBits);
	void RRotate90(int *pImgW,int *pImgH,int nBits,UCHAR* pNBits,UCHAR* pOBits);
	//ͼ�񼸺α任
	void Bitmap_X_Miror_RGBFunc(RSImg* mrsimg);
	void Bitmap_X_MirorGrayFunc(RSImg* mrsimg);
	void Bitmap_X_MirorFunc(int ImgW0,int ImgH0,UCHAR** pBits);
	void Bitmap_Y_Miror_RGBFunc(RSImg* mrsimg);
	void Bitmap_Y_MirorGrayFunc(RSImg* mrsimg);
	void Bitmap_Y_MirorFunc(int ImgW0,int ImgH0,UCHAR** pBits);
	void Bitmap_Rota90DFunc(RSImg* mrsimg);
	void Bitmap_Rota90DFunc(int nImgW,int nImgH,UCHAR** pNBits,UCHAR** pOBits);
	void Bitmap_Rota270Func(RSImg* mrsimg);
	void Bitmap_Rota270Func(int nImgW,int nImgH,UCHAR** pNBits,UCHAR** pOBits);
	//ƽ���˲� smooBol=true �����˲� smooBol=false
	void Image_SmoothConv2GrayFunc(RSImg* mrsimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol);
	void Image_SmoothConv2_RGBFunc(RSImg* mrsimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol);
	void Image_SmoothConv2BandFunc(RSImg* mrsimg,UCHAR **Mimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol);
	void Image_SmoothConvMediumGrayFunc(RSImg* mrsimg);
	void Image_SmoothConvMedium_RGBFunc(RSImg* mrsimg);
	void Image_SmoothConvMediumBandFunc(RSImg* mrsimg,UCHAR** Mimg);
	//�г̱���
	BOOL RunLengthCode256BmpFunc(FILE* fio,FILE* fou,FILE* fcd);
	//ת������ 
	//Bmp->Img
	BOOL BmpToImgFileFunc(const char* filename);
	BOOL ReadBmpFileToImgDataFunc(const char* filename,FILE *fio);
	BOOL ReadBmpToImgBy_RGB_FileFunc(FILE* fio,FILE* rfio,FILE* gfio,FILE* bfio,int nHeight,int nWidth,long offBits);
	//Img->Bmp
	BOOL ImgToBmpFileFunc(const char* B000name,int Width,int Height,char* Rname,char* Gname,char* Bname,int nWidht,int nHeight);
	BOOL WriteImgFileTo24BmpFunc(FILE* fio,FILE *rfio,FILE* gfio,FILE* bfio,int nWidth,int nHeight);
	//-----------------------------------------------------------------------------------
	BOOL SwapBmpToJpg_FileFunc(const char* infilename,const char* outfilename,int JpgQual);//bmp->jpg
	BOOL SwapJpgToBmp_FileFunc(const char* infilename,const char* outfilename,long* pw,long *pH);//jpg->bmp
	BOOL ReadBmpFileToModifyDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,int jpgH0,int Joff);//read bmp file to certen place
	BOOL ReadBmpFileToModifyDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,long poff);//һ��ͼ�ü�������ͼ��ʱ����
	BOOL ReadBmpFileToMemDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData);
	BOOL DoTopBotBmp1JoinBmp2Func(char* file1name, char* file2name,int Ty);//��ͼ����ƴ��
	BOOL DoTwoTouBmp1Add_Bmp2Func(char *file1name,char* file2name,float cp0);//��ͼ��ӣ���ͼ1Ϊ�ף�ͼ2�Զ����У����ӣ�Ȩcp0��
	BOOL OnePictureToTwoFunc(char* filename,int Ty);//ͼ��ָ�����ţ���TyΪ����
	BOOL Add_BmpFileToMemDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,float cp0);//��bmp�ļ��ӵ�ָ���ڴ�
	//��Ļ���
	BOOL WriteToBmpFile(const char* filename,int nWidth,int nHeight);//д�հ��ļ�
	BOOL DoWriteBmpFileFunc(const char* filename,int nWidth,int nHeight,CDC* dc,CRect* imgrect);//��Ļ���
	//�����Ż�
	BOOL BmpToEtmFileFunc(RSImg* mrsimg,const char* filename,BOOL DownBmpBol);//����bmp�ļ� bmp=>Etm
	BOOL ReadBmpFileToEtmDataFunc(RSImg* mrsimg,const char* filename,FILE* fio,BOOL DownBmpBol);
	BOOL ReadBmpToEtmBy_RGB_FileFunc(RSImg* mrsimg,FILE* fio,int nHeight,int nWidth,long offBits,BOOL DownBmpBol);
	//K-T�任
	void MssTasseledCapKT4x4_Func(RSImg* mrsimg,UCHAR** pB4data,UCHAR** pB5data,UCHAR** pB6data,UCHAR** pB7data,FILE *wfo1,FILE* wfo2,FILE* wfo3,FILE* wfo4);//ӧñ�任
	//��Ԫ�ز���
	UCHAR NearestNeighbor(float x0,float y0,int width,int height,UCHAR** sData,UCHAR B0);//���ڽ���Ԫ��
	UCHAR BilinearInterpolation(float x0,float y0,int widht,int height,UCHAR **sData);//˫���Բ巨�����������������
	UCHAR BilinearInterpolation(float x0,float y0,int widht,int height,UCHAR **sData,UCHAR B0);//˫���Բ巨���������������
	UCHAR CubicConvolutionInUCHARFile(float x0,float y0,int ImgW0,int ImgH0,UCHAR **sData);//˫���ξ���������������������
	UCHAR CubicConvolutionInUCHARFile(float x0,float y0,int ImgW0,int ImgH0,UCHAR **sData,int B0);//˫���ξ�������������������
	//ң�����ݱ���
	BOOL SaveRSDataToImg_Func(RSImg* mrsimg);
	BOOL SaveRSDataToBmp_Func(RSImg* mrsimg);
	BOOL SaveRSDataToJpg_Func(RSImg* mrsimg);
};
#endif

