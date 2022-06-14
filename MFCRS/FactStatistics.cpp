// OpenBmp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCRS.h"
#include "FactStatistics.h"
#include "afxdialogex.h"
#include"MyFunc.h"
#include "MFCRSDlg.h"
#include "resource.h"
// COpenBmp �Ի���

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


// COpenBmp ��Ϣ�������
//---------------------------------------------------------------------------------------
//------------------------------------- ֱ��ͼͳ��---------------------------------------
//---------------------------------------------------------------------------------------
void CFactStatistics::OnBnClickMode()//�����ֵ��ֱ��ͼ���е�����ģʽ
{                                    //ģʽһ��Ĭ�ϣ������С�����ֵ��ֵ����ѡ��״̬���ٵ����ı����
	modetwo=!modetwo;                //ģʽ�������Ĭ�ϵ��ʱ�ı���Сֵ�������Сֵ��ť�л��ɸı����ֵ����갴ť��ʾ�����ֵ��
	GetDlgItem(IDC_BUTTON4)->EnableWindow(modetwo);//���ٴε���л�Ϊ����Сֵ��
}
//---------------------------------------------------------
void CFactStatistics::OnChangeMaxOrMin()//ģʽ���¸ı��������Сֵ�������ֵ
{                                       //��δѡ��ģʽ������ť����޷����
	if(!flag)
	{
		GetDlgItem(IDC_BUTTON4)->SetWindowTextA("���ֵ");
		flag=1;
	}
	else
	{
		GetDlgItem(IDC_BUTTON4)->SetWindowTextA("��Сֵ");
		flag=0;
	}
}
//---------------------------------------------------------
void CFactStatistics::OnBnClickedButton2()//��ʼ��
{
	InitBKColor();//��ʼ������ɫ
	InitFactLine();//��ʼ��ֱ��ͼ
	InitRGBMbol();//��ʼ��ÿ���ؼ�����ѡ��״̬
	InitStrech();//��ʼ������ϵ��
	isinited=TRUE;//�ѳ�ʼ����
	CMFCRSDlg *maindlg2 = (CMFCRSDlg*)this->GetParent();//��ʾͼ��
	maindlg2->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CFactStatistics::OnBnClickedButtonChange()//�޸�
{
	InitBKColor();//��ʼ������ɫ
	SetPenModeCopy();//���û���ΪSRCCOPYģʽ
	InitRGBMbol();//��ʼ��ÿ���ؼ�����ѡ��״̬
	GetEditInfo();//��ȡ�༭�����Ϣ
	ChangeFactLine();//����ֱ��ͼ
	CMFCRSDlg *maindlg2 = (CMFCRSDlg*)this->GetParent();//ֱ��ͼ�����������ʾͼ��
	maindlg2->ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CFactStatistics::OnBnClickedButton3Strech()//��������
{
	CString str;
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(str);//up
	float max_perc = atof(str); if(max_perc < 0) max_perc = 0; else if(max_perc > 100) max_perc = 100;
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(str);//down
	float min_perc = atof(str); if(min_perc < 0) min_perc = 0; else if(min_perc > 100) min_perc = 100;
	objrsimg.Get_RSNew_Band_Fact_Func(mrsimg,2,2,min_perc,max_perc);
	ChangeEditInfo();//���±༭�����Ϣ
	OnBnClickedButtonChange();//�����޸ĺ�������ֱ��ͼ����ʾͼ��ĸ���
}
//---------------------------------------------------------
void CFactStatistics::OnStnClickedRStatic()//��R���εĴ������¼�
{
	GetCursorPos(&mpoint);//�õ��������ʾ�����������
	rpwnd->ScreenToClient(&mpoint);//����һ��������תΪR����ռ���������
	rdc->SetROP2(R2_NOTXORPEN);//���û���Ϊ���ʣ�ʵ�ֻ�����ʱ��������
	if(modetwo)
		PicActionModeTwo(rdc,rrect,mpoint.x,&(mrsimg->EnRmin),&(mrsimg->EnRmax),1);
	else
		PictureAction(rdc,rrect,&rbol,mpoint.x,&(mrsimg->EnRmin),&(mrsimg->EnRmax),1);
	CString str;
	str.Format("%d",mrsimg->EnRmin);GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
	str.Format("%d",mrsimg->EnRmax);GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);

}
//---------------------------------------------------------
void CFactStatistics::OnStnClickedGStatic()//��G���εĴ������¼�
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
void CFactStatistics::OnStnClickedBStatic()//��B���εĴ������¼�
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
void CFactStatistics::OnStnClickedMStatic()//��M���εĴ������¼�
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
void CFactStatistics::OnPaint()//��д��ϵͳ�ػ��������رոöԻ�����ٴδ򿪣���ԭ��һ�ε�״̬
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
//Ĭ��ģʽһʱ�������ʾֱ��ͼ�Ĵ���ʱ���ж��Ƿ�����ѡ�еģ������ѡ�е�����һ�����������С��
void CFactStatistics::PictureAction(CDC* dc,CRect* rec,int *bol,int x,int* min,int* max,int color)//
{
	if(*bol == 0)//ԭ��δѡ������һ����
	{
		*bol = ClickToLine(x,*min,*max);
		if(*bol == 1) LineBeSelected(*min,rec,dc,color);
		else if(*bol == 2) LineBeSelected(*max,rec,dc,color);
	}
	else if(*bol == 1)//��ѡ����Сֵ���������
	{
		*bol = ClickToLineS(x,*min,*max,bol);
		if(*bol == 3) { LineBeSelected(*min,rec,dc,color); *bol = 0; }
		else if(*bol == 4) { LineBeSelected(*min,rec,dc,color); LineBeSelected(*max,rec,dc,color); *bol = 2; }
		else { PlotLine(dc,*min,x,color); PlotLine(dc,*min-1,x-1,color); PlotLine(dc,*min+1,x+1,color); *min = x; }
	}
	else if(*bol == 2)//��ѡ�����ֵ���������
	{
		*bol = ClickToLineS(x,*min,*max,bol);
		if(*bol == 3) { LineBeSelected(*max,rec,dc,color); LineBeSelected(*min,rec,dc,color); *bol = 1; }
		else if(*bol == 4) { LineBeSelected(*max,rec,dc,color); *bol = 0; }
		else { PlotLine(dc,*max,x,color); PlotLine(dc,*max-1,x-1,color); PlotLine(dc,*max+1,x+1,color); *max = x; }
	}
}
//---------------------------------------------------------
int CFactStatistics::ClickToLine(int x,int min,int max)//û����ѡ��״̬��ʱ�����С������ߣ��������ĺ������ж�
{                                                      //�ʵ��Ŵ�Χ�����ڵ����������x��[x-1,x+1]��Χ���Ƿ�����
	if     (x >= min - 1 && x <= min + 1) return 1;//���ǵ������Сֵ���ߣ�����1
	else if(x >= max - 1 && x <= max + 1) return 2;//���ǵ�������ֵ���ߣ�����2
	else return 0;
}
//---------------------------------------------------------
//��ѡ��һ���ߵ�������ٴε�������ǵ���ڵ�ǰѡ�е��ߴ�����ȡ��ѡ��״̬�����ǵ����һ���ߣ����л���һ����Ϊѡ��״̬
//�������������������ѡ�е����ƶ����ô���ʵ��ֱ��ͼ����Ч��
int CFactStatistics::ClickToLineS(int x,int min,int max,int* bol)
{
	if(x == min) return 3;
	else if(x == max) return 4;
	else return *bol;
}
//---------------------------------------------------------
//���ڱ�ѡ�е��߽��мӴ���ʾ������color��ֵ����ͬ����ɫ��������ͬ�Ļ���ĵ���
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
//ģʽ���µ�����ػ��ߵ�ʵ�ֺ�������ģʽһ�򵥺ܶ�
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
void CFactStatistics::PlotFact(CDC* dc,long fact[],int fmin,int fmax,int color)//��ֱ��ͼ
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
	double DT = 100.0 / smax;//��Ƶ�����쵽0-100֮�䣬�Ա���ʾ����
	for( ii = 0; ii < 256; ii++ )
	{
		iy = fact[ii];if(iy == 0) continue; 
		iy = iy * DT;//Ƶ�������ͷ�ͼyֵ��Ҫ���죬��������ϵ��
		ix = ii;
		if(ix < fmin || ix > fmax) continue;
		iy = 100 - iy;
		dc->MoveTo(ix,100);//��picture�л�����ʵ��ÿ��x��ֵ��Ӧһ����
		dc->LineTo(ix,iy);
	}
}
//---------------------------------------------------------
void CFactStatistics::PlotLine(CDC* dc,int oldv,int newv,int color)//����Сֵ���ֵ����
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
	//�˴�����������Сֵ�����ֵ�����ߣ����ƶ���֮���ػ���oldv�Ǿ��ߵ����꣬newv�����ߵ����꣬
	//�������£�����ʵ�ֲ������߻����ߵĹ���
	return;
}
//---------------------------------------------------------
void CFactStatistics::SetPenModeCopy()//����ÿ�������ģʽΪSRCCOPY
{
	rdc->SetROP2(R2_NOTXORPEN);
	gdc->SetROP2(R2_NOTXORPEN);
	bdc->SetROP2(R2_NOTXORPEN);
	mdc->SetROP2(R2_NOTXORPEN);
}
//---------------------------------------------------------
void CFactStatistics::GetEditInfo()//���༭�����Ϣ�����ṹ�����mrsimg
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
void CFactStatistics::InitBKColor()//��ʼ��ÿ������ռ�ı���ɫΪ��ɫ
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
void CFactStatistics::InitRGBMbol()//��ʼ��ÿ���������ѡ��״̬
{
	rbol=gbol=bbol=mbol=0;
}
//---------------------------------------------------------
void CFactStatistics::InitFactLine()//��ʼ��ֱ��ͼ
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
void CFactStatistics::InitStrech()//��ʼ�����������Ĭ��ֵΪ2
{
	GetDlgItem(IDC_EDIT9)->SetWindowTextA("2");
	GetDlgItem(IDC_EDIT10)->SetWindowTextA("2");
}
//---------------------------------------------------------
void CFactStatistics::ChangeFactLine()//ֱ��ͼ������ػ�
{
	CString str;
	PlotFact(rdc,mrsimg->factR,mrsimg->InRmin,mrsimg->InRmax,1);PlotLine(rdc,mrsimg->EnRmin,mrsimg->EnRmax,1);str.Format("%d",mrsimg->EnRmin);GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);str.Format("%d",mrsimg->EnRmax);GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);
	PlotFact(gdc,mrsimg->factG,mrsimg->InGmin,mrsimg->InGmax,2);PlotLine(gdc,mrsimg->EnGmin,mrsimg->EnGmax,2);str.Format("%d",mrsimg->EnGmin);GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);str.Format("%d",mrsimg->EnGmax);GetDlgItem(IDC_EDIT4)->SetWindowTextA(str);
	PlotFact(bdc,mrsimg->factB,mrsimg->InBmin,mrsimg->InBmax,3);PlotLine(bdc,mrsimg->EnBmin,mrsimg->EnBmax,3);str.Format("%d",mrsimg->EnBmin);GetDlgItem(IDC_EDIT5)->SetWindowTextA(str);str.Format("%d",mrsimg->EnBmax);GetDlgItem(IDC_EDIT6)->SetWindowTextA(str);
}
//---------------------------------------------------------
void CFactStatistics::ChangeEditInfo()//��������������Ϣ�����༭����ʾ
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


