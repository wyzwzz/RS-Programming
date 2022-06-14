// NoiseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCRS.h"
#include "PicTool.h"
#include "afxdialogex.h"
#include"MyFunc.h"
#include"MFCRSDlg.h"
#include<math.h>
#include<cstdlib>
// NoiseDlg 对话框

IMPLEMENT_DYNAMIC(CPicTool, CDialogEx)

CPicTool::CPicTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPicTool::IDD, pParent)
{
	HaarInvTF=FALSE;
}

CPicTool::~CPicTool()
{
}

void CPicTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, HaarInverseTransform);
}


BEGIN_MESSAGE_MAP(CPicTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1,  &CPicTool::OnBnClickedSaltPepper)
	ON_BN_CLICKED(IDC_BUTTON3,  &CPicTool::OnBnClickedGussianNoise)
	ON_BN_CLICKED(IDC_BUTTON2,  &CPicTool::OnBnClickedButtonHaarTF)
	ON_BN_CLICKED(IDC_CHECK1,   &CPicTool::OnBnClickedCheckHaarInv)
	ON_BN_CLICKED(IDC_RADIO3,   &CPicTool::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON5,  &CPicTool::OnImgSmooBtnClick)
	ON_BN_CLICKED(IDC_BUTTON6,  &CPicTool::OnConvolutionBtnClick)
	ON_BN_CLICKED(IDC_RADIO4,   &CPicTool::OnGausValueRadClick)
	ON_BN_CLICKED(IDC_RADIO5,   &CPicTool::OnSharpeningRadClick)
	ON_BN_CLICKED(IDC_RADIO6,   &CPicTool::OnLaplacianRadClick)
	ON_BN_CLICKED(IDC_RADIO7,   &CPicTool::OnHoriPrewittBtnClick)
	ON_BN_CLICKED(IDC_RADIO8,   &CPicTool::OnVertPrewittBtnClick)
	ON_BN_CLICKED(IDC_RADIO9,   &CPicTool::OnHoriSobelBtnClick)
	ON_BN_CLICKED(IDC_RADIO10,  &CPicTool::OnVertSobelBtnClick)
	ON_BN_CLICKED(IDC_BUTTON8,  &CPicTool::OnX_MirorBtnFun)
	ON_BN_CLICKED(IDC_BUTTON9,  &CPicTool::OnY_MirorBtnFunc)
	ON_BN_CLICKED(IDC_BUTTON10, &CPicTool::OnBmp_Rotate90DFunc)
	ON_BN_CLICKED(IDC_BUTTON11, &CPicTool::OnBmpRotate270DFunc)
	ON_BN_CLICKED(IDC_BUTTON12, &CPicTool::OnBnMediumConv)
	ON_BN_CLICKED(IDC_BUTTON7,  &CPicTool::OnBnKTtransform)
	ON_BN_CLICKED(IDC_BUTTON13, &CPicTool::OnBn0_MirrorFunc)
END_MESSAGE_MAP()


// PicTool 消息处理程序
//---------------------------------------------------------------------------------------
//-------------------------------------1 椒盐噪声----------------------------------------
//---------------------------------------------------------------------------------------
void CPicTool::OnBnClickedSaltPepper()
{
	CString str1,str2;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str1);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str2);
	if(SetGrayBol)
		objrsimg.SaltPepperNoiseGrayFunc(mrsimg,atoi(str1),atoi(str2));
	else
		objrsimg.SaltPepperNoiseColorFunc(mrsimg,atoi(str1),atoi(str2));
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------------------------------------
//-------------------------------------2 高斯噪音----------------------------------------
//---------------------------------------------------------------------------------------
void CPicTool::OnBnClickedGussianNoise()
{
	CString str;
	double mu,sigma,k;
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str); mu    = atof(str);//mu为高斯噪音的均值
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(str); sigma = atof(str);//sigma为高斯噪音的方差，均值和方差与产生高斯噪音值的函数有关，两者越大噪音越强
	GetDlgItem(IDC_EDIT6)->GetWindowTextA(str); k     = atof(str);//k为高斯噪音特有的系数，k越大噪声越强
	if(SetGrayBol)
		objrsimg.AddGussianNoiseGray(mrsimg,mu,sigma,k);
	else
		objrsimg.AddGussianNoiseColor(mrsimg,mu,sigma,k);
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------------------------------------
//-------------------------------------3 图像滤波----------------------------------------
//---------------------------------------------------------------------------------------
void CPicTool::OnImgSmooBtnClick()//平滑滤波 smooBol=TRUE
{
	Smoothing_ConvolutionFilterFunc(TRUE);
}
//---------------------------------------------------------
void CPicTool::OnConvolutionBtnClick()//卷积滤波 smooBol=FALSE
{
	Smoothing_ConvolutionFilterFunc(FALSE);
}
//---------------------------------------------------------
void CPicTool::Smoothing_ConvolutionFilterFunc(BOOL smooBol)
{
	CString str;
	GetDlgItem(IDC_EDIT7 )->GetWindowTextA(str);double Smoo11=atof(str);
	GetDlgItem(IDC_EDIT8 )->GetWindowTextA(str);double Smoo12=atof(str);
	GetDlgItem(IDC_EDIT12)->GetWindowTextA(str);double Smoo13=atof(str);
	GetDlgItem(IDC_EDIT9 )->GetWindowTextA(str);double Smoo21=atof(str);
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(str);double Smoo22=atof(str);
	GetDlgItem(IDC_EDIT11)->GetWindowTextA(str);double Smoo23=atof(str);
	GetDlgItem(IDC_EDIT13)->GetWindowTextA(str);double Smoo31=atof(str);
	GetDlgItem(IDC_EDIT14)->GetWindowTextA(str);double Smoo32=atof(str);
	GetDlgItem(IDC_EDIT15)->GetWindowTextA(str);double Smoo33=atof(str);
	double sum;
	sum  = fabs(Smoo11) + fabs(Smoo12) + fabs(Smoo13);
	sum += fabs(Smoo21) + fabs(Smoo22) + fabs(Smoo23);
	sum += fabs(Smoo31) + fabs(Smoo32) + fabs(Smoo33);
	if(sum<EPSILON) {MessageBox(_T("请设置参数"));return;}
	sum   = Smoo11 + Smoo12 + Smoo13;
	sum  += Smoo21 + Smoo22 + Smoo23;
	sum  += Smoo31 + Smoo32 + Smoo33;
	if(fabs(sum - 1) > EPSILON)
		sum = 1.0 / sum;
	else sum = 1.0;
	Smoo11 *= sum; Smoo12 *= sum; Smoo13 *= sum;
	Smoo21 *= sum; Smoo22 *= sum; Smoo23 *= sum;
	Smoo31 *= sum; Smoo32 *= sum; Smoo33 *= sum;
	str.Format("%lf",Smoo11); GetDlgItem(IDC_EDIT7 )->SetWindowTextA(str);
	str.Format("%lf",Smoo12); GetDlgItem(IDC_EDIT8 )->SetWindowTextA(str);
	str.Format("%lf",Smoo13); GetDlgItem(IDC_EDIT12)->SetWindowTextA(str);
	str.Format("%lf",Smoo21); GetDlgItem(IDC_EDIT9 )->SetWindowTextA(str);
	str.Format("%lf",Smoo22); GetDlgItem(IDC_EDIT10)->SetWindowTextA(str);
	str.Format("%lf",Smoo23); GetDlgItem(IDC_EDIT11)->SetWindowTextA(str);
	str.Format("%lf",Smoo31); GetDlgItem(IDC_EDIT13)->SetWindowTextA(str);
	str.Format("%lf",Smoo32); GetDlgItem(IDC_EDIT14)->SetWindowTextA(str);
	str.Format("%lf",Smoo33); GetDlgItem(IDC_EDIT15)->SetWindowTextA(str);
	if(SetGrayBol) objrsimg.Image_SmoothConv2GrayFunc(mrsimg,Smoo11, Smoo12, Smoo13,Smoo21, Smoo22, Smoo23, Smoo31, Smoo32, Smoo33,smooBol);
	else           objrsimg.Image_SmoothConv2_RGBFunc(mrsimg,Smoo11, Smoo12, Smoo13,Smoo21, Smoo22, Smoo23, Smoo31, Smoo32, Smoo33,smooBol);
	mrsimg->ModifyBol = TRUE;//需要刷屏操作重画
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OnBnMediumConv()//中值滤波
{
	if(SetGrayBol) objrsimg.Image_SmoothConvMediumGrayFunc(mrsimg);
	else           objrsimg.Image_SmoothConvMedium_RGBFunc(mrsimg);
	mrsimg->ModifyBol = TRUE;//需要刷屏操作重画
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OnBnClickedRadio3()//均值滤波的单选按钮  可以给每个编辑框添加一个变量 是均值滤波的权重系数，下同
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA("1");
}
//---------------------------------------------------------
void CPicTool::OnGausValueRadClick()//高斯滤波
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA("1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA("1");
}
//---------------------------------------------------------
void CPicTool::OnSharpeningRadClick()//锐化
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA( "5");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA("-1");
}
//---------------------------------------------------------
void CPicTool::OnLaplacianRadClick()//梯度
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA("-8");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA( "1");
}
//---------------------------------------------------------
void CPicTool::OnHoriPrewittBtnClick()//水平
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA( "1");
}
//---------------------------------------------------------
void CPicTool::OnVertPrewittBtnClick()//垂直
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA( "1");
}
//---------------------------------------------------------
void CPicTool::OnHoriSobelBtnClick()//Hori
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA("-2");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA( "2");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA( "1");
}
//---------------------------------------------------------
void CPicTool::OnVertSobelBtnClick()//Vert
{
	GetDlgItem(IDC_EDIT7 )->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT8 )->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT12)->SetWindowTextA( "1");
	GetDlgItem(IDC_EDIT9 )->SetWindowTextA("-2");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT11)->SetWindowTextA( "2");
	GetDlgItem(IDC_EDIT13)->SetWindowTextA("-1");
	GetDlgItem(IDC_EDIT14)->SetWindowTextA( "0");
	GetDlgItem(IDC_EDIT15)->SetWindowTextA( "1");
}
//---------------------------------------------------------------------------------------
//-------------------------------------4 小波变化----------------------------------------
//---------------------------------------------------------------------------------------
void CPicTool::OnBnClickedButtonHaarTF()
{
	CString str;
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
	int HarrLevel = atoi(str); if( HarrLevel < 1 || HarrLevel > 5 ) HarrLevel = 1;
	if(!mrsimg) return;
	if(!mrsimg->Rimg || !mrsimg->Gimg || !mrsimg->Bimg) return;
	OneBandHarrWaveFunc(mrsimg,mrsimg->Rimg,HarrLevel,HaarInvTF);
	OneBandHarrWaveFunc(mrsimg,mrsimg->Gimg,HarrLevel,HaarInvTF);
	OneBandHarrWaveFunc(mrsimg,mrsimg->Bimg,HarrLevel,HaarInvTF);
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OneBandHarrWaveFunc(RSImg* mrsimg,UCHAR** Rimg,int HarrLevel,BOOL InvBol)
{
	if(!mrsimg) return;
	objrsimg.MyHaar_8_BitTransform(Rimg,mrsimg->Img_W0,mrsimg->Img_H0,HarrLevel,InvBol);
	return;
}
//---------------------------------------------------------
void CPicTool::OnBnClickedCheckHaarInv()//小波正变换还是逆变换 HaarInvTF=TRUE 为逆变换
{
	HaarInvTF = !HaarInvTF;
}
//---------------------------------------------------------------------------------------
//-------------------------------------5 MSS缨帽变换-------------------------------------
//---------------------------------------------------------------------------------------
void CPicTool::OnBnKTtransform()
{
	if(!mrsimg->Rimg || !mrsimg->Gimg || !mrsimg->Bimg || !mrsimg->Mimg)
	{
		MessageBox(_T("缨帽变换数据未输入"));
		return;
	}
	//strcpy_s(LoadSaveFileName,200,mrsimg->R_name);
	FILE* wfo1;
	FILE* wfo2;
	FILE* wfo3;
	FILE* wfo4;
	_splitpath(LoadSaveFileName,Ipdri,Ipdir,Ipname,Ipext); if(!strstr(Ipname,"ISB")) strcat(Ipname,"ISB"); fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".fimg"); fopen_s(&wfo1,PathFileName,"wb"); if(!wfo1){ MessageBox(_T("ISB打开错误")); return; }
	_splitpath(LoadSaveFileName,Ipdri,Ipdir,Ipname,Ipext); if(!strstr(Ipname,"IGV")) strcat(Ipname,"IGV"); fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".fimg"); fopen_s(&wfo2,PathFileName,"wb"); if(!wfo1){ fclose(wfo1); MessageBox(_T("IGV打开错误")); return; }
	_splitpath(LoadSaveFileName,Ipdri,Ipdir,Ipname,Ipext); if(!strstr(Ipname,"I_Y")) strcat(Ipname,"I_Y"); fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".fimg"); fopen_s(&wfo3,PathFileName,"wb"); if(!wfo1){ fclose(wfo1); fclose(wfo2); MessageBox(_T("I_Y打开错误")); return; }
	_splitpath(LoadSaveFileName,Ipdri,Ipdir,Ipname,Ipext); if(!strstr(Ipname,"I_N")) strcat(Ipname,"I_N"); fnmerge(PathFileName,Ipdri,Ipdir,Ipname,".fimg"); fopen_s(&wfo4,PathFileName,"wb"); if(!wfo1){ fclose(wfo1); fclose(wfo2); fclose(wfo3); MessageBox(_T("I_N打开错误")); return; }
	objrsimg.MssTasseledCapKT4x4_Func(mrsimg,mrsimg->Rimg,mrsimg->Gimg,mrsimg->Bimg,mrsimg->Mimg,wfo1,wfo2,wfo3,wfo4);
	fclose(wfo1); fclose(wfo2); fclose(wfo3); fclose(wfo4);
}
//---------------------------------------------------------------------------------------
//-------------------------------------6 几何变化----------------------------------------
//---------------------------------------------------------------------------------------
void CPicTool::OnBn0_MirrorFunc()//0-镜像
{
	if(!SetGrayBol) { objrsimg.Bitmap_Y_Miror_RGBFunc(mrsimg); objrsimg.Bitmap_X_Miror_RGBFunc(mrsimg); }
	else            { objrsimg.Bitmap_Y_MirorGrayFunc(mrsimg); objrsimg.Bitmap_X_MirorGrayFunc(mrsimg); }   
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OnX_MirorBtnFun()//X-镜像
{
	if(!SetGrayBol) objrsimg.Bitmap_X_Miror_RGBFunc(mrsimg);
	else            objrsimg.Bitmap_X_MirorGrayFunc(mrsimg);    
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OnY_MirorBtnFunc()//Y-镜像
{
	if(!SetGrayBol) objrsimg.Bitmap_Y_Miror_RGBFunc(mrsimg);
	else            objrsimg.Bitmap_Y_MirorGrayFunc(mrsimg);    
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OnBmp_Rotate90DFunc()//顺时针旋转90度
{
	objrsimg.Bitmap_Rota90DFunc(mrsimg);
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CPicTool::OnBmpRotate270DFunc()//逆时针旋转90度
{
	objrsimg.Bitmap_Rota270Func(mrsimg);
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}



