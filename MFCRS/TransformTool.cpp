// TransformTool.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCRS.h"
#include "TransformTool.h"
#include "afxdialogex.h"
#include"MyFunc.h"
#include"MFCRSDlg.h"
// CTransformTool 对话框

IMPLEMENT_DYNAMIC(CTransformTool, CDialogEx)

CTransformTool::CTransformTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransformTool::IDD, pParent)
{

}

CTransformTool::~CTransformTool()
{
}

void CTransformTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransformTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON6,  &CTransformTool::OnBtnPicture_1)
	ON_BN_CLICKED(IDC_BUTTON7,  &CTransformTool::OnBtnPicture_2)
	ON_BN_CLICKED(IDC_BUTTON9,  &CTransformTool::OnTwoPictureAdd)
	ON_BN_CLICKED(IDC_BUTTON8,  &CTransformTool::OnUpDownPicture)
	ON_BN_CLICKED(IDC_BUTTON1,  &CTransformTool::OnBtnBmpToImgFunc)
	ON_BN_CLICKED(IDC_BUTTON2,  &CTransformTool::OnBtnImgToBmpFunc)
	ON_BN_CLICKED(IDC_BUTTON3,  &CTransformTool::OnBtnRunLength)
	ON_BN_CLICKED(IDC_BUTTON12, &CTransformTool::OnBnScreenPrintf)
	ON_BN_CLICKED(IDC_BUTTON13, &CTransformTool::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CTransformTool::OnBnOnePicToTwoFunc)
	ON_BN_CLICKED(IDC_BUTTON5,  &CTransformTool::OnBnSaveBmp_Func)
	ON_BN_CLICKED(IDC_BUTTON4,  &CTransformTool::OnBnSaveImg_Func)
	ON_BN_CLICKED(IDC_BUTTON15, &CTransformTool::OnBnSaveAsImgOtherPlace)
	ON_BN_CLICKED(IDC_BUTTON16, &CTransformTool::OnBnSaveAsBmpOtherPlace)
	ON_BN_CLICKED(IDC_BUTTON10, &CTransformTool::OnSaveRSDataAsJpg)
	ON_BN_CLICKED(IDC_BUTTON11, &CTransformTool::OnSaveAsJpgOtherPlace)
END_MESSAGE_MAP()


// CTransformTool 消息处理程序

//---------------------------------------------------------------------------------------
//---------------------------------------1 转换工具--------------------------------------
//---------------------------------------------------------------------------------------
void CTransformTool::OnBtnBmpToImgFunc()//Bmp->Img
{
	CFileDialog file( 1, _T("bmp"), NULL, 0, _T("bmp文件(*.bmp)|*.bmp||"), this );
	if( IDOK == file.DoModal() )
	{
		filename = file.GetPathName(); if( !filename ) return;
		BOOL wbool = objrsimg.BmpToImgFileFunc(filename);
		if(wbool) MessageBox(_T("Bmp=>Img成功!"));
		else      MessageBox(_T(ErrStr));
	}
}
//---------------------------------------------------------
void CTransformTool::OnBtnImgToBmpFunc()//Img->Bmp
{
	CFileDialog file( 1, _T("000"), NULL, 0, _T("000文件(*.000)|*.000||"), this );
	if( IDOK == file.DoModal() )
	{
		filename = file.GetPathName(); if(!filename) return;
		if( !PathFileExists(filename) )
		{ MessageBox(_T("File does not exist!")); return; }
		int ImgW0,ImgH0;
		int Rmin,Rmax,Gmin, Gmax,Bmin,Bmax,Mmin,Mmax;
		RSImg* mrsimg1 = new RSImg; if( !mrsimg1 ) return ; memset(mrsimg1,0,sizeof(RSImg));//因为未加载直接转化
		BOOL wbool = objrsimg.Read000RSImgFile_Func(filename,&ImgW0,&ImgH0,mrsimg1->R_name,mrsimg1->G_name,mrsimg1->B_name,mrsimg1->M_name,&Rmin,&Rmax,&Gmin,&Gmax,&Bmin,&Bmax,&Mmin,&Mmax);
		if(wbool) wbool = objrsimg.ImgToBmpFileFunc(filename,ImgW0,ImgH0,mrsimg1->R_name,mrsimg1->G_name,mrsimg1->B_name,ImgW0,ImgH0);
		delete[] mrsimg1;
		if(wbool) MessageBox(_T("Img=>Bmp成功!"));
		else      MessageBox(_T("Error!"));
	}
}
//---------------------------------------------------------
void CTransformTool::OnBnSaveBmp_Func()//SaveBmp
{
	if( !mrsimg ) { MessageBox(_T("未打开遥感图像!")); return; }
	BOOL wbol = objrsimg.SaveRSDataToBmp_Func(mrsimg);
	if(wbol) { MessageBox(_T("保存Bmp成功!")); return; }
	else     { MessageBox(_T("保存Bmp失败!")); return; }
}
//---------------------------------------------------------
void CTransformTool::OnBnSaveImg_Func()//SaveImg
{
	if( !mrsimg ) { MessageBox(_T("未打开遥感图像!")); return; }
	BOOL wbol = objrsimg.SaveRSDataToImg_Func(mrsimg);
	if(wbol) { MessageBox(_T("保存Img成功!")); return; }
	else     { MessageBox(_T("保存Img失败!")); return; }
	
}
//---------------------------------------------------------
void CTransformTool::OnBnSaveAsImgOtherPlace()//另存为Img
{
	if( !mrsimg ) { MessageBox(_T("未打开遥感图像!")); return; }
	char filename[500]; strcpy_s( filename, 500, LoadSaveFileName ); CString str;
	CFileDialog file( FALSE, _T("000"), NULL, 0, _T("000文件(*.000)|*.000||"), this );
	if( IDOK == file.DoModal() )
	{
		str = file.GetPathName(); strcpy_s(LoadSaveFileName,500,str);
		BOOL wbol = objrsimg.SaveRSDataToImg_Func(mrsimg);
		if(wbol) { MessageBox(_T("保存Img成功!")); return; }
		else     { MessageBox(_T("保存Img失败!")); return; }
	}
	strcpy_s(LoadSaveFileName,500,filename);
}
//---------------------------------------------------------
void CTransformTool::OnBnSaveAsBmpOtherPlace()//另存为Bmp
{
	if( !mrsimg ) { MessageBox(_T("未打开遥感图像!")); return; }
	char filename[500]; strcpy_s(filename,500,LoadSaveFileName); CString str;
	CFileDialog file( FALSE, _T("bmp"), NULL, 0, _T("bmp文件(*.bmp)|*.bmp||"), this );
	if( IDOK == file.DoModal() )
	{
		str = file.GetPathName(); strcpy_s(LoadSaveFileName,500,str);
		BOOL wbol = objrsimg.SaveRSDataToBmp_Func(mrsimg);
		if(wbol) { MessageBox(_T("保存Bmp成功!")); return; }
		else     { MessageBox(_T("保存Bmp失败!")); return; }
	}
	strcpy_s(LoadSaveFileName,500,filename);
}
//---------------------------------------------------------
void CTransformTool::OnSaveRSDataAsJpg()//保存为jpg格式，有损
{
	if( !mrsimg ) { MessageBox(_T("未打开遥感图像!")); return; }
	BOOL wbol = objrsimg.SaveRSDataToJpg_Func(mrsimg);
	if(wbol) { MessageBox(_T("保存Jpg成功!")); return; }
	else     { MessageBox(_T("保存Jpg失败!")); return; }
}
//---------------------------------------------------------
void CTransformTool::OnSaveAsJpgOtherPlace()//另存为jpg
{
	if( !mrsimg ) { MessageBox(_T("未打开遥感图像!")); return; }
	char filename[500]; strcpy_s(filename,500,LoadSaveFileName); CString str;
	CFileDialog file( FALSE, _T("jpg"), NULL, 0, _T("jpg文件(*.jpg)|*.jpg|jpeg文件(*.jpeg)|*.jpeg||"), this );
	if( IDOK == file.DoModal() )
	{
		str = file.GetPathName(); strcpy_s(LoadSaveFileName,500,str);
		BOOL wbol = objrsimg.SaveRSDataToJpg_Func(mrsimg);
		if(wbol) { MessageBox(_T("保存Jpg成功!")); return; }
		else     { MessageBox(_T("保存Jpg失败!")); return; }
	}
	strcpy_s(LoadSaveFileName,500,filename);
}
//---------------------------------------------------------
void CTransformTool::OnBtnRunLength()//行程编码
{
	if( !mrsimg || !mrsimg->Rimg ) { MessageBox(_T("未加载遥感数据!")); return ;}
	CFileDialog file( FALSE, NULL, NULL, 0, _T("压缩文件(*.cod)|*.cod||"), this );
	if( IDOK == file.DoModal() )
	{
		filename = file.GetPathName(); if(!filename) return;
		FILE* fio; fopen_s(&fio,filename,"rb"); if(!fio) return;
		_splitpath(filename,Ipdri,Ipdir,Ipname,Ipext);
		fnmerge(LoadSaveFileName,Ipdri,Ipdir,Ipname,".txt"); FILE* fou; fopen_s(&fou,LoadSaveFileName,"w"); if(!fou){ fclose(fio); return; }
		fnmerge(LoadSaveFileName,Ipdri,Ipdir,Ipname,".cod"); FILE* fcd; fopen_s(&fcd,LoadSaveFileName,"w"); if(!fcd){ fclose(fio); fclose(fcd); return; }
		BOOL wbool = objrsimg.RunLengthCode256BmpFunc(fio,fou,fcd);
		if(!wbool) MessageBox(ErrStr);
		fclose(fio); fclose(fou); fclose(fcd);
	}
}
//---------------------------------------------------------------------------------------
//---------------------------------------2 图像拼接--------------------------------------
//---------------------------------------------------------------------------------------
void CTransformTool::OnBtnPicture_1()//图像拼接 图像一
{
	CFileDialog mfdlg( 1, _T("jpeg|jpg"), NULL, 0, _T("jpg文件(*.jpg)|*.jpg|jpeg文件(*.jpeg)|*.jpeg||"), this );
	if( IDOK == mfdlg.DoModal() )
	{
		filename1 = mfdlg.GetPathName();
		GetDlgItem(IDC_EDIT1)->SetWindowTextA(filename1);
	}
}
//---------------------------------------------------------
void CTransformTool::OnBtnPicture_2()//图像拼接 图像二
{
	CFileDialog mfdlg( 1, _T("jpeg|jpe"), NULL, 0, _T("jpg文件(*.jpg)|*.jpg|jpeg文件(*.jpeg)|*.jpeg||"), this );
	if( IDOK == mfdlg.DoModal() )
	{
		filename2 = mfdlg.GetPathName();
		GetDlgItem(IDC_EDIT2)->SetWindowTextA(filename2);
	}
}
//---------------------------------------------------------
void CTransformTool::OnTwoPictureAdd()//两图相加
{
	char* f1 = new char[200]; if(!f1) memset(f1,0,200*sizeof(char));
	char *f2 = new char[200]; if(!f2) memset(f2,0,200*sizeof(char));
	strcpy_s(f1,200*sizeof(char),filename1);
	strcpy_s(f2,200*sizeof(char),filename2);
	BOOL wbol=objrsimg.DoTwoTouBmp1Add_Bmp2Func(f1,f2,0.7);
	if(wbol) MessageBox(_T("两图相加成功!"));
	else      MessageBox(_T(ErrStr));
}
//---------------------------------------------------------
void CTransformTool::OnUpDownPicture()//上下拼图
{
	char* f1 = new char[200]; if(!f1) memset(f1,0,200*sizeof(char));
	char* f2 = new char[200]; if(!f2) memset(f2,0,200*sizeof(char));
	strcpy_s(f1,200*sizeof(char),filename1);
	strcpy_s(f2,200*sizeof(char),filename2);
	BOOL wbool=objrsimg.DoTopBotBmp1JoinBmp2Func(f1,f2,4);
	if(wbool) MessageBox(_T("上下拼图成功!"));
	else      MessageBox(_T(ErrStr));
}
//---------------------------------------------------------------------------------------
//-------------------------------------3 屏幕输出------------------------------------------
//---------------------------------------------------------------------------------------
void CTransformTool::OnBnScreenPrintf()
{
	CString w,h,name;
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(w); if( w == "" ) { MessageBox(_T("请输入宽!")); return ;}
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(h); if( h == "" ) { MessageBox(_T("请输入高!")); return ;}
	CFileDialog file( 1, _T("bmp"), NULL, 0, _T("bmp文件(*.bmp)|*.bmp"), this );
	if( IDOK == file.DoModal() )
	{
	name = file.GetPathName(); if(!name) return;
	CMFCRSDlg *maindlg3 = (CMFCRSDlg*)this->GetParent();
	maindlg3->printname = name;
	maindlg3->ScreenPrintf(atoi(w),atoi(h));
	}
}
//---------------------------------------------------------------------------------------
//--------------------------------------4 图像裁剪---------------------------------------
void CTransformTool::OnBnClickedButton13()//输入图像名
{
	CFileDialog file( 1, _T("jpg"), NULL, 0, _T("jpeg文件(*.jpeg)|*.jpeg| jpg文件(*.jpg)|*.jpg||"), this );
	if( IDOK == file.DoModal() )
	{
	 	filename = file.GetPathName(); if(!filename) return;
		GetDlgItem(IDC_EDIT5)->SetWindowTextA(filename);
	}
}
//---------------------------------------------------------
void CTransformTool::OnBnOnePicToTwoFunc()
{
	CString Ty;
	GetDlgItem(IDC_EDIT6)->GetWindowTextA(Ty);
	char* f = new char[200]; if(!f) memset(f,0,200*sizeof(char));
	strcpy_s(f,200*sizeof(char),filename);
	BOOL wbool = objrsimg.OnePictureToTwoFunc(f,atoi(Ty));
	if(wbool) MessageBox(_T("裁剪成功!"));
	else      MessageBox(_T("裁剪失败!"));
}
