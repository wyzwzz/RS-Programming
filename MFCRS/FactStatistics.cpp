// OpenBmp.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCRS.h"
#include "FactStatistics.h"
#include "afxdialogex.h"
#include"MyFunc.h"
#include "MFCRSDlg.h"
#include "resource.h"
// COpenBmp 对话框

IMPLEMENT_DYNAMIC(CFactStatistics, CDialogEx)

CFactStatistics::CFactStatistics(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFactStatistics::IDD, pParent)
	, modetwo(false)
{
	rrect=new CRect;grect=new CRect;brect=new CRect;mrect=new CRect;
	redpen=new CPen(PS_SOLID,1,RGB(255,0,0));greenpen=new CPen(PS_SOLID,1,RGB(0,255,0));bluepen=new CPen(PS_SOLID,1,RGB(0,0,255));graypen=new CPen(PS_SOLID,1,RGB(122,122,122));
	rbol=gbol=bbol=mbol=0;
	modetwo=FALSE;
	flag=0;
	isinited=FALSE;
}

CFactStatistics::~CFactStatistics()
{
	ReleaseDC(rdc);
	ReleaseDC(gdc);
	ReleaseDC(bdc);
	ReleaseDC(mdc);
}

void CFactStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_R_STATIC, R_Picture);
	DDX_Control(pDX, IDC_G_STATIC, G_Picture);
	DDX_Control(pDX, IDC_B_STATIC, B_Picture);
	DDX_Control(pDX, IDC_M_STATIC, M_Picture);
}


BEGIN_MESSAGE_MAP(CFactStatistics, CDialogEx)
	ON_WM_SETCURSOR()
	ON_STN_CLICKED(IDC_R_STATIC, &CFactStatistics::OnStnClickedRStatic)
	ON_STN_CLICKED(IDC_G_STATIC, &CFactStatistics::OnStnClickedGStatic)
	ON_STN_CLICKED(IDC_B_STATIC, &CFactStatistics::OnStnClickedBStatic)
	ON_STN_CLICKED(IDC_M_STATIC, &CFactStatistics::OnStnClickedMStatic)
	ON_BN_CLICKED(IDC_BUTTON1, &CFactStatistics::OnBnClickedButtonChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, &CFactStatistics::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFactStatistics::OnBnClickedButton3Strech)
	ON_BN_CLICKED(IDC_CHECK1, &CFactStatistics::OnBnClickMode)
	ON_BN_CLICKED(IDC_BUTTON4, &CFactStatistics::OnChangeMaxOrMin)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// COpenBmp 消息处理程序
//---------------------------------------------------------------------------------------
//------------------------------------- 直方图统计---------------------------------------
//---------------------------------------------------------------------------------------
void CFactStatistics::OnBnClickMode()//有两种点击直方图进行调整的模式
{                                    //模式一（默认）点击最小或最大值的值进入选中状态，再点击后改变该线
	modetwo=!modetwo;                //模式二，最初默认点击时改变最小值，点击最小值按钮切换成改变最大值，鼠标按钮显示“最大值”
	GetDlgItem(IDC_BUTTON4)->EnableWindow(modetwo);//可再次点击切换为“最小值”
}
//---------------------------------------------------------
void CFactStatistics::OnChangeMaxOrMin()//模式二下改变鼠标点击最小值还是最大值
{                                       //若未选择模式二，则按钮变灰无法点击
	if(!flag)
	{
		GetDlgItem(IDC_BUTTON4)->SetWindowTextA("最大值");
		flag=1;
	}
	else
	{
		GetDlgItem(IDC_BUTTON4)->SetWindowTextA("最小值");
		flag=0;
	}
}
//---------------------------------------------------------
void CFactStatistics::OnBnClickedButton2()//初始化
{
	InitBKColor();//初始化背景色
	InitFactLine();//初始化直方图
	InitRGBMbol();//初始化每个控件的线选中状态
	InitStrech();//初始化拉伸系数
	isinited=TRUE;//已初始化过
	CMFCRSDlg *maindlg2 = (CMFCRSDlg*)this->GetParent();//显示图像
	maindlg2->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CFactStatistics::OnBnClickedButtonChange()//修改
{
	InitBKColor();//初始化背景色
	SetPenModeCopy();//设置画笔为SRCCOPY模式
	InitRGBMbol();//初始化每个控件的线选中状态
	GetEditInfo();//获取编辑框的信息
	ChangeFactLine();//更新直方图
	CMFCRSDlg *maindlg2 = (CMFCRSDlg*)this->GetParent();//直方图拉伸后重新显示图像
	maindlg2->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CFactStatistics::OnBnClickedButton3Strech()//线性拉伸
{
	CString str;
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(str);//up
	float max_perc = atof(str); if(max_perc < 0) max_perc = 0; else if(max_perc > 100) max_perc = 100;
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(str);//down
	float min_perc = atof(str); if(min_perc < 0) min_perc = 0; else if(min_perc > 100) min_perc = 100;
	objrsimg.Get_RSNew_Band_Fact_Func(mrsimg,2,2,min_perc,max_perc);
	ChangeEditInfo();//更新编辑框的信息
	OnBnClickedButtonChange();//调用修改函数进行直方图和显示图像的更新
}
//---------------------------------------------------------
void CFactStatistics::OnStnClickedRStatic()//在R波段的窗体点击事件
{
	GetCursorPos(&mpoint);//得到相对于显示屏的相对坐标
	rpwnd->ScreenToClient(&mpoint);//将上一步的坐标转为R画框空间的相对坐标
	rdc->SetROP2(R2_NOTXORPEN);//设置画笔为异或笔，实现画新线时擦除旧线
	if(modetwo)
		PicActionModeTwo(rdc,rrect,mpoint.x,&(mrsimg->EnRmin),&(mrsimg->EnRmax),1);
	else
		PictureAction(rdc,rrect,&rbol,mpoint.x,&(mrsimg->EnRmin),&(mrsimg->EnRmax),1);
	CString str;
	str.Format("%d",mrsimg->EnRmin);GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnRmax);GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);

}
//---------------------------------------------------------
void CFactStatistics::OnStnClickedGStatic()//在G波段的窗体点击事件
{
	GetCursorPos(&mpoint);
	gpwnd->ScreenToClient(&mpoint);
	gdc->SetROP2(R2_NOTXORPEN);
	if(modetwo)
		PicActionModeTwo(gdc,grect,mpoint.x,&(mrsimg->EnGmin),&(mrsimg->EnGmax),2);
	else
		PictureAction(gdc,grect,&gbol,mpoint.x,&(mrsimg->EnGmin),&(mrsimg->EnGmax),2);
	CString str;
	str.Format("%d",mrsimg->EnGmin);GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnGmax);GetDlgItem(IDC_EDIT4)->SetWindowTextA(str);
}
//---------------------------------------------------------
void CFactStatistics::OnStnClickedBStatic()//在B波段的窗体点击事件
{
	GetCursorPos(&mpoint);
	bpwnd->ScreenToClient(&mpoint);
	bdc->SetROP2(R2_NOTXORPEN);
	if(modetwo)
		PicActionModeTwo(bdc,brect,mpoint.x,&(mrsimg->EnBmin),&(mrsimg->EnBmax),3);
	else
		PictureAction(bdc,brect,&bbol,mpoint.x,&(mrsimg->EnBmin),&(mrsimg->EnBmax),3);
	CString str;
	str.Format("%d",mrsimg->EnBmin);GetDlgItem(IDC_EDIT5)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnBmax);GetDlgItem(IDC_EDIT6)->SetWindowTextA(str);
}
//---------------------------------------------------------
void CFactStatistics::OnStnClickedMStatic()//在M波段的窗体点击事件
{
	GetCursorPos(&mpoint);
	mpwnd->ScreenToClient(&mpoint);
	mdc->SetROP2(R2_NOTXORPEN);
	if(modetwo)
		PicActionModeTwo(mdc,mrect,mpoint.x,&(mrsimg->EnMmin),&(mrsimg->EnMmax),4);
	else
		PictureAction(mdc,mrect,&mbol,mpoint.x,&(mrsimg->EnMmin),&(mrsimg->EnMmax),4);
	CString str;
	str.Format("%d",mrsimg->EnMmin);GetDlgItem(IDC_EDIT7)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnMmax);GetDlgItem(IDC_EDIT8)->SetWindowTextA(str);
}
//---------------------------------------------------------
void CFactStatistics::OnPaint()//重写的系统重画函数，关闭该对话框后再次打开，复原上一次的状态
{
	CPaintDC dc(this);
	PaintMyFact();
}
//---------------------------------------------------------
void CFactStatistics::PaintMyFact()
{
	if(isinited)
	{
		InitBKColor();
		SetPenModeCopy();
		InitRGBMbol();
		ChangeFactLine();
	}
	else return;
}
//---------------------------------------------------------
//默认模式一时鼠标点击显示直方图的窗体时，判断是否已有选中的，如果有选中的是哪一条（最大还是最小）
void CFactStatistics::PictureAction(CDC* dc,CRect* rec,int *bol,int x,int* min,int* max,int color)//
{
	if(*bol == 0)//原先未选中任意一条线
	{
		*bol = ClickToLine(x,*min,*max);
		if(*bol == 1) LineBeSelected(*min,rec,dc,color);
		else if(*bol == 2) LineBeSelected(*max,rec,dc,color);
	}
	else if(*bol == 1)//已选中最小值的线情况下
	{
		*bol = ClickToLineS(x,*min,*max,bol);
		if(*bol == 3) { LineBeSelected(*min,rec,dc,color); *bol = 0; }
		else if(*bol == 4) { LineBeSelected(*min,rec,dc,color); LineBeSelected(*max,rec,dc,color); *bol = 2; }
		else { PlotLine(dc,*min,x,color); PlotLine(dc,*min-1,x-1,color); PlotLine(dc,*min+1,x+1,color); *min = x; }
	}
	else if(*bol == 2)//已选中最大值的线情况下
	{
		*bol = ClickToLineS(x,*min,*max,bol);
		if(*bol == 3) { LineBeSelected(*max,rec,dc,color); LineBeSelected(*min,rec,dc,color); *bol = 1; }
		else if(*bol == 4) { LineBeSelected(*max,rec,dc,color); *bol = 0; }
		else { PlotLine(dc,*max,x,color); PlotLine(dc,*max-1,x-1,color); PlotLine(dc,*max+1,x+1,color); *max = x; }
	}
}
//---------------------------------------------------------
int CFactStatistics::ClickToLine(int x,int min,int max)//没有已选中状态下时点击最小或最大线，根据鼠标的横坐标判断
{                                                      //适当放大范围，在在点击数横坐标x的[x-1,x+1]范围内是否有线
	if     (x >= min - 1 && x <= min + 1) return 1;//若是点击了最小值的线，返回1
	else if(x >= max - 1 && x <= max + 1) return 2;//若是点击了最大值的线，返回2
	else return 0;
}
//---------------------------------------------------------
//在选中一条线的情况下再次点击，若是点击在当前选中的线处，则取消选中状态，若是点击另一条线，则切换另一条线为选中状态
//若不是这两种情况，则将选中的线移动到该处，实现直方图拉伸效果
int CFactStatistics::ClickToLineS(int x,int min,int max,int* bol)
{
	if(x == min) return 3;
	else if(x == max) return 4;
	else return *bol;
}
//---------------------------------------------------------
//对于被选中的线进行加粗显示，根据color的值画不同的颜色，即代表不同的画框的调用
void CFactStatistics::LineBeSelected(int x,CRect *rect,CDC* dc,int color)
{
	switch(color)
	{
	case 1:dc->SelectObject(redpen); break;
	case 2:dc->SelectObject(greenpen); break;
	case 3:dc->SelectObject(bluepen); break;
	case 4:dc->SelectObject(graypen); break;
	default:
		break;
	}
	dc->MoveTo(x-1,0);
	dc->LineTo(x-1,rect->bottom);
	dc->MoveTo(x+1,0);
	dc->LineTo(x+1,rect->bottom);
}
//---------------------------------------------------------
//模式二下点击后重画线的实现函数，比模式一简单很多
void CFactStatistics::PicActionModeTwo(CDC* dc,CRect* rec,int x,int *min,int* max,int color)
{
	if(!flag)
	{
		PlotLine(dc,*min,x,color);
		*min = x;
	}
	else
	{
		PlotLine(dc,*max,x,color);
		*max = x;
	}
}
//---------------------------------------------------------
void CFactStatistics::PlotFact(CDC* dc,long fact[],int fmin,int fmax,int color)//画直方图
{
	switch(color)
	{
	case 1:dc->SelectObject(redpen);break;
	case 2:dc->SelectObject(greenpen);break;
	case 3:dc->SelectObject(bluepen);break;
	case 4:dc->SelectObject(graypen);break;
	default:
		break;
	}
	long smax = 0;
	long ii,ix,iy;
	for( ii = 0; ii < 256; ii++ )
	{
		if(smax >= fact[ii]) continue;
		smax = fact[ii];
	}
	if(smax <= 1) return;
	double DT = 100.0 / smax;//将频数拉伸到0-100之间，以便显示美观
	for( ii = 0; ii < 256; ii++ )
	{
		iy = fact[ii];if(iy == 0) continue; 
		iy = iy * DT;//频数，即释放图y值需要拉伸，乘以拉伸系数
		ix = ii;
		if(ix < fmin || ix > fmax) continue;
		iy = 100 - iy;
		dc->MoveTo(ix,100);//在picture中画，其实是每个x的值对应一条线
		dc->LineTo(ix,iy);
	}
}
//---------------------------------------------------------
void CFactStatistics::PlotLine(CDC* dc,int oldv,int newv,int color)//画最小值最大值的线
{
	switch(color)
	{
	case 1:dc->SelectObject(redpen); break;
	case 2:dc->SelectObject(greenpen); break;
	case 3:dc->SelectObject(bluepen); break;
	case 4:dc->SelectObject(graypen); break;
	default:
		break;
	}
	int ix = oldv; if( ix > 0 && ix < 280){ dc->MoveTo(ix,100); dc->LineTo(ix,0); }
	    ix = newv; if( ix > 0 && ix < 280){ dc->MoveTo(ix,100); dc->LineTo(ix,0); }
	//此处既用来画最小值和最大值两条线，在移动线之后重画，oldv是旧线的坐标，newv是新线的坐标，
	//在异或笔下，可以实现擦除旧线画新线的功能
	return;
}
//---------------------------------------------------------
void CFactStatistics::SetPenModeCopy()//设置每个画框的模式为SRCCOPY
{
	rdc->SetROP2(R2_NOTXORPEN);
	gdc->SetROP2(R2_NOTXORPEN);
	bdc->SetROP2(R2_NOTXORPEN);
	mdc->SetROP2(R2_NOTXORPEN);
}
//---------------------------------------------------------
void CFactStatistics::GetEditInfo()//将编辑框的信息传给结构体对象mrsimg
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);mrsimg->EnRmin=atoi(str);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);mrsimg->EnRmax=atoi(str);
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);mrsimg->EnGmin=atoi(str);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str);mrsimg->EnGmax=atoi(str);
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(str);mrsimg->EnBmin=atoi(str);
	GetDlgItem(IDC_EDIT6)->GetWindowTextA(str);mrsimg->EnBmax=atoi(str);
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(str);mrsimg->EnMmin=atoi(str);
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(str);mrsimg->EnMmax=atoi(str);

}
//---------------------------------------------------------
void CFactStatistics::InitBKColor()//初始化每个画框空间的背景色为白色
{
	rdc=R_Picture.GetDC();rpwnd = GetDlgItem(IDC_R_STATIC);rpwnd->GetClientRect(rrect);
	rdc->FillSolidRect(rrect,RGB(255,255,255));
	gdc=G_Picture.GetDC();gpwnd = GetDlgItem(IDC_G_STATIC);gpwnd->GetClientRect(grect);
	gdc->FillSolidRect(grect,RGB(255,255,255));
	bdc=B_Picture.GetDC();bpwnd = GetDlgItem(IDC_B_STATIC);bpwnd->GetClientRect(brect);
	bdc->FillSolidRect(brect,RGB(255,255,255));
	mdc=M_Picture.GetDC();mpwnd = GetDlgItem(IDC_M_STATIC);mpwnd->GetClientRect(mrect);
	mdc->FillSolidRect(mrect,RGB(255,255,255));
	
}
//---------------------------------------------------------
void CFactStatistics::InitRGBMbol()//初始化每个画框的线选中状态
{
	rbol=gbol=bbol=mbol=0;
}
//---------------------------------------------------------
void CFactStatistics::InitFactLine()//初始化直方图
{
	CString str;
	PlotFact(rdc,mrsimg->factR,mrsimg->InRmin,mrsimg->InRmax,1);PlotLine(rdc,mrsimg->InRmin,mrsimg->InRmax,1);str.Format("%d",mrsimg->InRmin);GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);str.Format("%d",mrsimg->InRmax);GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);
	PlotFact(gdc,mrsimg->factG,mrsimg->InGmin,mrsimg->InGmax,2);PlotLine(gdc,mrsimg->InGmin,mrsimg->InGmax,2);str.Format("%d",mrsimg->InGmin);GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);str.Format("%d",mrsimg->InGmax);GetDlgItem(IDC_EDIT4)->SetWindowTextA(str);
	PlotFact(bdc,mrsimg->factB,mrsimg->InBmin,mrsimg->InBmax,3);PlotLine(bdc,mrsimg->InBmin,mrsimg->InBmax,3);str.Format("%d",mrsimg->InBmin);GetDlgItem(IDC_EDIT5)->SetWindowTextA(str);str.Format("%d",mrsimg->InBmax);GetDlgItem(IDC_EDIT6)->SetWindowTextA(str);
	mrsimg->EnRmin = mrsimg->InRmin;mrsimg->EnRmax = mrsimg->InRmax;
	mrsimg->EnGmin = mrsimg->InGmin;mrsimg->EnGmax = mrsimg->InGmax;
	mrsimg->EnBmin = mrsimg->InBmin;mrsimg->EnBmax = mrsimg->InBmax;
}
//---------------------------------------------------------
void CFactStatistics::InitStrech()//初始化拉伸比例，默认值为2
{
	GetDlgItem(IDC_EDIT9)->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA("2");
}
//---------------------------------------------------------
void CFactStatistics::ChangeFactLine()//直方图拉伸后重画
{
	CString str;
	PlotFact(rdc,mrsimg->factR,mrsimg->InRmin,mrsimg->InRmax,1);PlotLine(rdc,mrsimg->EnRmin,mrsimg->EnRmax,1);str.Format("%d",mrsimg->EnRmin);GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);str.Format("%d",mrsimg->EnRmax);GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);
	PlotFact(gdc,mrsimg->factG,mrsimg->InGmin,mrsimg->InGmax,2);PlotLine(gdc,mrsimg->EnGmin,mrsimg->EnGmax,2);str.Format("%d",mrsimg->EnGmin);GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);str.Format("%d",mrsimg->EnGmax);GetDlgItem(IDC_EDIT4)->SetWindowTextA(str);
	PlotFact(bdc,mrsimg->factB,mrsimg->InBmin,mrsimg->InBmax,3);PlotLine(bdc,mrsimg->EnBmin,mrsimg->EnBmax,3);str.Format("%d",mrsimg->EnBmin);GetDlgItem(IDC_EDIT5)->SetWindowTextA(str);str.Format("%d",mrsimg->EnBmax);GetDlgItem(IDC_EDIT6)->SetWindowTextA(str);
}
//---------------------------------------------------------
void CFactStatistics::ChangeEditInfo()//将线性拉伸后的信息传给编辑框并显示
{
	CString str;
	str.Format("%d",mrsimg->EnRmin);GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnRmax);GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnGmin);GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnGmax);GetDlgItem(IDC_EDIT4)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnBmin);GetDlgItem(IDC_EDIT5)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnBmax);GetDlgItem(IDC_EDIT6)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnMmin);GetDlgItem(IDC_EDIT7)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnMmax);GetDlgItem(IDC_EDIT8)->SetWindowTextA(str);
}


