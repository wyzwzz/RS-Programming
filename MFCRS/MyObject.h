#ifndef MYOBJECT_H
#define MYOBJECT_H
#include"stdafx.h"
//---------------------------------------------------
//宏定义
//---------------------------------------------------
#define UCHAR unsigned char
#define Float_Type float
#define ElementType UCHAR
//---------------------------------------------------
#define MOUSE_EDIT_POINT 0x00000001L//edit point
#define MOUSE_EDIT_CURVE 0x00000002L//edit curve
#define MOUSE_INSL_BEGIN 0x00000020L//ins line begin
#define MOUSE_DRAW_BEGIN 0x00000010L//拖动一矩形或一直线（左鼠标）？？？
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
	short CharNum;//点ID
	float X0,Y0;//点坐标
	char *TextAdr;//点注记
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
	short FoColoNo;//线的ID
	short NodeNum;//线的点数
	float *CoorAdr;//点的坐标数组
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
	int Img_W0,Img_H0,Img_B0;//影像宽高   B0:背景值
	UCHAR **Rimg;            //影像数据
	UCHAR **Gimg;
	UCHAR **Bimg;
	UCHAR **Mimg;
	UCHAR InRmin,InRmax;//输入影像最小最大值，根据输入的图像数据统计后得到
	UCHAR InGmin,InGmax;
	UCHAR InBmin,InBmax;
	UCHAR InMmin,InMmax;
	int   EnRmin,EnRmax;//显示影像的最小最大值，一般为0-255，为图像显示位数相关
	int   EnGmin,EnGmax;
	int   EnBmin,EnBmax;
	int   EnMmin,EnMmax;
	char  R_name[MAXPATH];//影像文件名
	char  G_name[MAXPATH];
	char  B_name[MAXPATH];
	char  M_name[MAXPATH];
	BOOL  InMBol,ModifyBol;
	long  factR[256],factG[256],factB[256],factM[256];//影像频率分布
	int   ImgXoff,ImgYoff;
	long  FastLen;
	int   FastWidth,FastHeight;//快速显示用
	char *FastData;//填色用，与遥感影像XOR
	BITMAPINFO FastInfo;
};
//-------------------------------------------------
//定义 遥感影像 类操作
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
	//内存空间处理
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
	//输入影像
	BOOL Read000RSImgFile_Func(const char * filename,int *pImgW,int *pImgH,char *Rname,char *Gname,char *Bname,char *Mname,int *Rmin,int *Rmax,int *Gmin,int *Gmax,int *Bmin,int *Bmax,int *Mmin,int *Mmax);
	BOOL ReadRSFileData_Func(RSImg *rsimg,const char *B000name,int Width,int Height,char *Rname,char *Gname,char *Bname,char *Mname,BOOL DownBol);
	BOOL ReadRS_Data_UCHAR_Func(const char* filename,int Height,int Width,UCHAR **pData,BOOL DownBol);
	BOOL ReadRS_Data_UCHAR_Func(FILE *fio,int Height,int Width,UCHAR **pData,BOOL DownBol);
	//影像生成
	BOOL GencRemoteImgFunc(RSImg *rsimg,int bmpw,int bmph);//重采样
	BOOL GencRemoColorImgFunc(RSImg *rsimg,int bmpw,int bmph);
	BOOL GencRemoGray_ImgFunc(RSImg *rsimg,int bmpw,int bmph);
	void AllocFastData(RSImg *rsimg,int nHeight,int nWidth);//申请显示内存
	char* AllocFastData(RSImg *rsimg,int nHeight,int nWidth,int nBits);
	void DeleteFastData(RSImg *rsimg);//删除内存空间
	void Init_BitMap_Info(int nWidth,int nHeight,int colorbit,BITMAPINFO* pinfo);//初始化位图头文件信息
	//影像显示
	void PaintRemoteImgFunc(CDC *dc,RSImg *rsimg);//利用strechDIBits画图
	void Plot_Etm_Bitmap_By_Fast_File(CDC* dc,int xL,int yT,int bW0,int bH0,char *pdata,int CopyMode);//下面三个函数显示失败时用
	BOOL WriteToBmpFile(const char* filename,int nWidth,int nHeight,char *lpBits);
	void Init_BitMap_FileHeader(int nWidth,int nHeight,int colorbit,BITMAPFILEHEADER* pbmfh,BITMAPINFOHEADER* pbmih);
    //直方图统计
	long Get_Data_UChar_Fact_Func(int ImgH0,int ImgW0,int ImgB0,UCHAR** pData,long* factData,UCHAR* fmin,UCHAR* fmax);
	void Get_RSNew_Band_Fact_Func(RSImg* mrsimg,int RGB_Min,int RGB_Max,float Min_Perc,float Max_Perc);//自动拉伸
	BOOL Get_RSNew_Band_Fact_Func(long* pfact,int RGB_Min,int RGB_Max,float Min_Perc,float Max_Perc,int* psn1,int* psn2);
    //二维散点图
	void PlotScatDiagramFunc(CDC* dc,int ZoomW,int ZoomH,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax);
	void FillScatDiagramFunc(CDC* dc,int ZoomW,int ZoomH,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax,BOOL SmooBol,int nr0);
	void CalcFreqDistZoomFunc(int ZoomW,int ZoomH,long** XYimg,int ImgW0,int ImgH0,int ImgB0,UCHAR** Limg,UCHAR** Bimg,int LefMin,int LefMax,int BotMin,int BotMax,BOOL SmooBol,int nr0);//计算窗口点的分布频率
	long Get_Long_Min_Max_Avg_S0_Func(int ImgH0,int ImgW0,int ImgB0,long** pData,long* pfmin,long* pfmax,double* pAvg,double* pS0);
	void Draw_One_Cross(CDC* dc,int x0,int y0,int len);
	//椒盐噪声
	void SaltPepperNoiseGrayFunc(RSImg* mrsimg,int SaNum,int PeNum);
	void SaltPepperNoiseColorFunc(RSImg* mrsig,int SaNum,int PeNum);
	void SaltandPepperNoiseFunc(int ImgW0,int ImgH0,UCHAR** Hdata,int SaNum,int PeNum);
	void SaltandPepperNoiseFunc(int ImgW0,int ImgH0,UCHAR** HdataR,UCHAR** HdataG,UCHAR** HdataB,int SaNum,int PeNum);//全色波段要与RGB一起添加嘛
	//高斯噪声
	void AddGussianNoiseGray(RSImg* mrsimg,double mu,double sigma,int k);
	void AddGussianNoiseColor(RSImg* mrsimg,double mu,double sigma,int k);
	void GussianNoise(int ImgW0,int ImgH0,UCHAR** Mdata,double mu,double sigma,int k);
	void GussianNoise(int ImgW0,int ImgH0,UCHAR** Rdata,UCHAR** Gdata,UCHAR** Bdata,double mu,double sigma,int k);
	//小波变换
	void MyHaar_8_BitTransform(UCHAR** Rimg,int ImgW0,int ImgH0,int level,BOOL Invbol);//level为变换级数
	void HaarTransform(Float_Type *Hdata,int ImgW0,int ImgH0,int level);//正变化
	void HaarRowDecompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//行分解
	void HaarColDecompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//列分解
	void HaarIvsTransform(Float_Type *Hdata,int ImgW0,int ImgH0,int level);//逆变换
	void HaarRowCompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//行合成
	void HaarColCompose(Float_Type *Hdata,int ImgW0,int iRow,int iCol);//列合成
	//图像旋转
	void RSImgRotete(RSImg* mrsimg,BOOL ClockRotate);
	void LRotate90(int *pImgW,int *pImgH,int nBits,UCHAR* pNBits,UCHAR* pOBits);
	void RRotate90(int *pImgW,int *pImgH,int nBits,UCHAR* pNBits,UCHAR* pOBits);
	//图像几何变换
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
	//平滑滤波 smooBol=true 或卷积滤波 smooBol=false
	void Image_SmoothConv2GrayFunc(RSImg* mrsimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol);
	void Image_SmoothConv2_RGBFunc(RSImg* mrsimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol);
	void Image_SmoothConv2BandFunc(RSImg* mrsimg,UCHAR **Mimg,double Smoo11,double Smoo12,double Smoo13,double Smoo21,double Smoo22,double Smoo23,double Smoo31,double Smoo32,double Smoo33,BOOL smooBol);
	void Image_SmoothConvMediumGrayFunc(RSImg* mrsimg);
	void Image_SmoothConvMedium_RGBFunc(RSImg* mrsimg);
	void Image_SmoothConvMediumBandFunc(RSImg* mrsimg,UCHAR** Mimg);
	//行程编码
	BOOL RunLengthCode256BmpFunc(FILE* fio,FILE* fou,FILE* fcd);
	//转换工具 
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
	BOOL ReadBmpFileToModifyDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,long poff);//一幅图裁剪成两幅图的时候用
	BOOL ReadBmpFileToMemDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData);
	BOOL DoTopBotBmp1JoinBmp2Func(char* file1name, char* file2name,int Ty);//两图上下拼接
	BOOL DoTwoTouBmp1Add_Bmp2Func(char *file1name,char* file2name,float cp0);//两图相加，以图1为底，图2自动对中，叠加（权cp0）
	BOOL OnePictureToTwoFunc(char* filename,int Ty);//图像分割成两张，以Ty为横线
	BOOL Add_BmpFileToMemDataFunc(FILE* fio,int nHeight,int nWidth,char* bmpHData,float cp0);//读bmp文件加到指定内存
	//屏幕输出
	BOOL WriteToBmpFile(const char* filename,int nWidth,int nHeight);//写空白文件
	BOOL DoWriteBmpFileFunc(const char* filename,int nWidth,int nHeight,CDC* dc,CRect* imgrect);//屏幕输出
	//界面优化
	BOOL BmpToEtmFileFunc(RSImg* mrsimg,const char* filename,BOOL DownBmpBol);//读入bmp文件 bmp=>Etm
	BOOL ReadBmpFileToEtmDataFunc(RSImg* mrsimg,const char* filename,FILE* fio,BOOL DownBmpBol);
	BOOL ReadBmpToEtmBy_RGB_FileFunc(RSImg* mrsimg,FILE* fio,int nHeight,int nWidth,long offBits,BOOL DownBmpBol);
	//K-T变换
	void MssTasseledCapKT4x4_Func(RSImg* mrsimg,UCHAR** pB4data,UCHAR** pB5data,UCHAR** pB6data,UCHAR** pB7data,FILE *wfo1,FILE* wfo2,FILE* wfo3,FILE* wfo4);//缨帽变换
	//像元重采样
	UCHAR NearestNeighbor(float x0,float y0,int width,int height,UCHAR** sData,UCHAR B0);//最邻近像元法
	UCHAR BilinearInterpolation(float x0,float y0,int widht,int height,UCHAR **sData);//双线性插法（不考虑特殊情况）
	UCHAR BilinearInterpolation(float x0,float y0,int widht,int height,UCHAR **sData,UCHAR B0);//双线性插法（考虑特殊情况）
	UCHAR CubicConvolutionInUCHARFile(float x0,float y0,int ImgW0,int ImgH0,UCHAR **sData);//双三次卷积法（不考虑特殊情况）
	UCHAR CubicConvolutionInUCHARFile(float x0,float y0,int ImgW0,int ImgH0,UCHAR **sData,int B0);//双三次卷积法（考虑特殊情况）
	//遥感数据保存
	BOOL SaveRSDataToImg_Func(RSImg* mrsimg);
	BOOL SaveRSDataToBmp_Func(RSImg* mrsimg);
	BOOL SaveRSDataToJpg_Func(RSImg* mrsimg);
};
#endif

