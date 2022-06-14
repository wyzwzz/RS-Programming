
// MFCRSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCRS.h"
#include "MFCRSDlg.h"
#include "afxdialogex.h"
#include"Ins_Point.h"
#include"MyFunc.h"
#include"PointDlginfo.h"
#include"Ins_Curve.h"
#include"LineTextDlg.h"
#include"Read_Bmp.h"
#include"LoadRSData.h"
#include"tiff-debug\include\tiffio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCRSDlg 对话框



CMFCRSDlg::CMFCRSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCRSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	StartFlag=FALSE;
	Ins_PointFlag=FALSE;
	Ins_CurveFlag=FALSE;
	IsMouseDown=FALSE;
	mreadbmpdlg=NULL;
	strech=1;
	firstx=0;
	firsty=0;
	dc=NULL;
	mreadbmpdlg=NULL;
	 mRGBStatistic=NULL;
	 mscatterdlg=NULL;
	 mpictooldlg=NULL;
	 mtranformtooldlg=NULL;
	CMFCToolBar m_ToolBar;
	penwhite=new CPen(PS_SOLID,1,RGB(255,255,255));
	pengreen=new CPen(PS_SOLID,1,RGB(0,255,0));
	penred=new CPen(PS_SOLID,1,RGB(255,0,0));
	ShowToolBar=FALSE;
}


void CMFCRSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCRSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CMFCRSDlg::OnEditBegin)
	ON_UPDATE_COMMAND_UI(ID_32771, &CMFCRSDlg::OnUpdateEditBegin)
	ON_UPDATE_COMMAND_UI(ID_32773, &CMFCRSDlg::OnUpdateIns_Point)
	ON_COMMAND(ID_32773, &CMFCRSDlg::OnIns_Point)
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_32774, &CMFCRSDlg::OnUpdateIns_Curve)
	ON_UPDATE_COMMAND_UI(ID_32772, &CMFCRSDlg::OnUpdateEditStop)
	ON_COMMAND(ID_32772, &CMFCRSDlg::OnEditStop)
	ON_COMMAND(ID_32776, &CMFCRSDlg::OnNewFile)
	ON_COMMAND(ID_Menu, &CMFCRSDlg::OnOpenFile)
	ON_COMMAND(ID_32778, &CMFCRSDlg::OnSaveFile)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32774, &CMFCRSDlg::OnIns_Curve)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32781, &CMFCRSDlg::OnFinishLineEdit)
	ON_WM_CREATE()
	ON_COMMAND(ID_32787, &CMFCRSDlg::OnOpenBMPFile)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_32790, &CMFCRSDlg::OnRGBStatistics)
	ON_COMMAND(ID_32791, &CMFCRSDlg::OnOpenScatterDlg)
	ON_COMMAND(ID_32785, &CMFCRSDlg::OnClickBar1)
	ON_COMMAND(ID_32795, &CMFCRSDlg::OnClockRotate90)
	ON_COMMAND(ID_32796, &CMFCRSDlg::OnInvClockRotate90)
	ON_COMMAND(ID_32799, &CMFCRSDlg::OnOpenTFToolDlg)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_32786, &CMFCRSDlg::OnSetPicSuitable)
	ON_COMMAND(ID_32808, &CMFCRSDlg::OnPicToolFunc)
	ON_UPDATE_COMMAND_UI(ID_32790, &CMFCRSDlg::OnUpdateFactStatistics)
	ON_UPDATE_COMMAND_UI(ID_32791, &CMFCRSDlg::OnUpdateScatter)
	ON_UPDATE_COMMAND_UI(ID_32799, &CMFCRSDlg::OnUpdateTFTool)
	ON_UPDATE_COMMAND_UI(ID_32808, &CMFCRSDlg::OnUpdatePicTool)
	ON_UPDATE_COMMAND_UI(ID_32785, &CMFCRSDlg::OnUpdateMoveBMP)
	ON_UPDATE_COMMAND_UI(ID_32786, &CMFCRSDlg::OnUpdateSetPicCenter)
	ON_COMMAND(ID_32779, &CMFCRSDlg::OnSaveOtherPlace)
	ON_UPDATE_COMMAND_UI(ID_32778, &CMFCRSDlg::OnUpdateSaveFile)
	ON_UPDATE_COMMAND_UI(ID_32779, &CMFCRSDlg::OnUpdateSaveFileOther)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_32809, &CMFCRSDlg::OnShowToolBar)
	ON_UPDATE_COMMAND_UI(ID_32809, &CMFCRSDlg::OnUpdateToolBar)
END_MESSAGE_MAP()


// CMFCRSDlg 消息处理程序

BOOL CMFCRSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetClientRect(&rect);
	dc=GetDC();
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(dc,rect.Width(),rect.Height());
    pOldBit = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0,0,2000,1500,RGB(255,255,255));
	/*	mtoolbar.CreateEx(this);
	mtoolbar.LoadToolBar(IDR_TOOLBAR1);
	mtoolbar.ShowWindow(1);*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CMFCRSDlg::DeleteRSImg_Paint()
{
	strech=1;
	firstx=0;
	firsty=0;
	ShowToolBar=0;
	OnPaint();
}

void CMFCRSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
//  创建窗口
//  如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CMFCRSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(dc)
		{
			//重画使用了双缓冲，创建了兼容的MemDC,即看不见的画布，在其上面画完后一次性拷贝到当前的dc，可以消除闪屏的现象
			GetClientRect(&rect);
			MemDC.FillSolidRect(0,0,2000,1500,RGB(255,255,255));
			objrsimg.PaintRemoteImgFunc(&MemDC,mrsimg);
			objcurve.PaintMyCurveFunc(&MemDC);
			objpoint.PaintMyPointFunc(&MemDC);
			dc->BitBlt(0,0,2000,1500,&MemDC,0,0,SRCCOPY);//将MemDC中的内存拷贝到dc当中
		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCRSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCRSDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)

{
	ASSERT(pPopupMenu != NULL);
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);
	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}
	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.
		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}
		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}
//---------------------------------------------------------------------------------------
//-------------------------------------打开、保存点线文件--------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnNewFile()//新建
{
	objpoint.DeletePointLst();//删除原来的所有数据
	objcurve.DeleteCurveFunc();
	objrsimg.DeleteFastData(mrsimg);
	objrsimg.Delete_Char_RSImg_MemorySpace(mrsimg);
	mrsimg = NULL; mshpfilename = "";
	Invalidate();
	OnPaint();//刷屏重画
}
//---------------------------------------------------------
void CMFCRSDlg::OnOpenFile()//打开点、线文件
{
	CFileDialog openfile( 1, _T("mshp"), NULL, 0, _T("mshp文件(*.mshp)|*.mshp||"), this );//文件类型后缀名未mshp
	if( IDOK == openfile.DoModal() )
	{
		CFile File;//点和线使用了CFile类，有一定方便性
		CFileException e;
		if(!File.Open(openfile.GetPathName(),CFile::modeRead,&e))
		{
			CString strErr;
			strErr.Format(_T("File could not be opened %d\n"),e.m_cause);
			MessageBox(strErr);
		}
		mshpfilename = openfile.GetPathName();
		File.Read(&shpfileflag,sizeof(long));
		File.Close();
		if(shpfileflag == FILE_PANDC_FLAG)//设置了三种mshp文件的打开类型，分别是只有点、只有线、同时有点和线，打开时根据读入首四个字节判断
		{
			objpoint.LoadPointFunc(openfile.GetPathName());
			objcurve.LoadCurveFunc(openfile.GetPathName());
		}
		else if(shpfileflag == FILE_POINT_FLAG)
		{
			objpoint.LoadPointFunc(openfile.GetPathName());
		}
		else if(shpfileflag == FILE_CURVE_FLAG)
		{
			objcurve.LoadCurveFunc(openfile.GetPathName());
		}
		else if(!shpfileflag)
		{
			MessageBox(_T("empty file!"));
		}
	}
	OnPaint();
	return;
}
//---------------------------------------------------------
void CMFCRSDlg::OnSaveFile()//保存点、线文件，若从已有文件打开，可直接保存，若是新建的，则会打开文件保存框
{
	if( mshpfilename == "" )
	{
		CFileDialog openfile( 0, _T("mshp"), NULL, 0, _T("mshp文件(*.mshp)|*.mshp||"), this );
		if( IDOK == openfile.DoModal() )
		{
			OnSaveFile(openfile.GetPathName());
		}
	}
	else
	{
		OnSaveFile(mshpfilename);
	}
	shpfileflag = 0;
}
//---------------------------------------------------------
void CMFCRSDlg::OnSaveOtherPlace()//另存点线文件，打开文件保存框自己选择保存路径和文件名
{
	CFileDialog openfile( 0, _T("mshp"), NULL, 0, _T("mshp文件(*.mshp)|*.mshp||"), this );
	if( IDOK == openfile.DoModal() )
		OnSaveFile(openfile.GetPathName());
	shpfileflag=0;
}
//---------------------------------------------------------
void CMFCRSDlg::OnSaveFile(CString filename)
{
	if(HeadPoint && HeadCurve)//根据只有点、只有线还是点和线都有分情况调用相应的函数保存
	{
		shpfileflag = FILE_PANDC_FLAG;
		objpoint.SavePointFile(filename);
		objcurve.SaveCurveFunc(filename);
	}
	else if(HeadPoint)
	{
		shpfileflag = FILE_POINT_FLAG;
		objpoint.SavePointFile(filename);
	}
	else if(HeadCurve)
	{
		shpfileflag = FILE_CURVE_FLAG;
		objcurve.SaveCurveFunc(filename);
	}
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateSaveFile(CCmdUI *pCmdUI)//没有点或线数据时，保存选项不可选
{
	if(!HeadCurve && !HeadPoint)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateSaveFileOther(CCmdUI *pCmdUI)//没有点或线数据时，另存为选项不可选
{
	if(!HeadCurve && !HeadPoint)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}
//---------------------------------------------------------------------------------------
//-----------------------------------编辑点、线------------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnEditBegin()
{
	StartFlag = !StartFlag;//改变编辑状态
}
//---------------------------------------------------------
void CMFCRSDlg::OnEditStop()//停止编辑
{
	StartFlag = !StartFlag;//相关参数全部设为0
	Ins_PointFlag = FALSE;
	Ins_CurveFlag = FALSE;
	MouseDraw = 0;
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateEditBegin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(StartFlag);//根据编辑状态更新菜单栏中开始编辑的打勾
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateEditStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(StartFlag);//未开始编辑时，停止编辑为灰，开始编辑后，开始编辑会回，停止编辑可选
}
//---------------------------------------------------------------------------------------
//----------------------------------鼠标动作事件-----------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键按下
{
	if     (MouseDraw & MOUSE_EDIT_POINT && Ins_PointFlag) PointCrossLine(point.x,point.y);//画点
	else if(MouseDraw & MOUSE_EDIT_CURVE) Ins_Begin(dc,point.x,point.y);
	else if(MouseDraw & BMP_MOVE_FLAG)  SavePreInfo(point);
	CDialogEx::OnLButtonDown(nFlags, point);
}
//---------------------------------------------------------
void CMFCRSDlg::OnLButtonUp(UINT nFlags, CPoint point)//鼠标左键抬起
{
	if     (MouseDraw & MOUSE_EDIT_POINT && Ins_PointFlag) CPointText(point.x,point.y);//画点后弹出点注记对话框
	else if(MouseDraw & BMP_MOVE_FLAG) ShowCurBMP(point);
	CDialogEx::OnLButtonUp(nFlags, point);
}
//---------------------------------------------------------
void CMFCRSDlg::OnRButtonUp(UINT nFlags, CPoint point)//鼠标右键抬起
{
	if(MouseDraw & MOUSE_EDIT_CURVE && MouseDraw & MOUSE_INSL_BEGIN)//插入线过程中右键抬起后出现子菜单
		OnEditL_RMenu(point);
}
//---------------------------------------------------------
void CMFCRSDlg::OnMouseMove(UINT nFlags, CPoint point)//鼠标移动
{
	if      (MouseDraw & MOUSE_EDIT_POINT) return;
	else if (MouseDraw & MOUSE_EDIT_CURVE && MouseDraw & MOUSE_INSL_BEGIN)
		LineTo(point.x,point.y);//插入线时移动状态下擦旧线画新线
	else if (MouseDraw & BMP_MOVE_FLAG && IsMouseDown)
		MoveBMPFunc(point);//移动图片
	CDialogEx::OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------------
//------------------------------画点和标记注记----------------------------------------
//------------------------------------------------------------------------------------
void CMFCRSDlg::OnIns_Point()//选中插入点的状态
{
	MouseDraw =  MOUSE_EDIT_POINT;//MouseDraw用来判断鼠标事件发生后处于什么状态，有插点、插线、移动图片三种
	Ins_PointFlag = !Ins_PointFlag;//此变量用来控制菜单栏插入点选项前是否打勾
	if(Ins_CurveFlag)
		Ins_CurveFlag = !Ins_CurveFlag;
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateIns_Point(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(StartFlag);//若未开始编辑，则插入点选项不可选
	if(MouseDraw != MOUSE_EDIT_POINT) Ins_PointFlag = FALSE;
	pCmdUI->SetCheck(Ins_PointFlag);//更新打勾
}
//---------------------------------------------------------
void CMFCRSDlg::PointCrossLine(int x,int y)
{
	dc->SelectObject(penred);
	dc->MoveTo(x-5,y);dc->LineTo(x+5,y);//以垂直交叉十字表示点
	dc->MoveTo(x,y-5);dc->LineTo(x,y+5);
	//dc->Ellipse(x-5,y+5,x+5,y-5);//以圆表示点
}
//---------------------------------------------------------
void CMFCRSDlg::CPointText(int x,int y)
{
	PointDlginfo mpointinfo;
	mpointinfo.point_x = x;//将点的坐标传给显示点注记的对话框并显示点的坐标
	mpointinfo.point_y = y;
	mpointinfo.DoModal();
	dc->TextOut(x+6,y,mpointinfo.instr);//mpointinfo.instr是点的注记信息
	objpoint.AddOnePoint(x,y,mpointinfo.instr);//将点添加到链表中储存
	OnPaint();//重新画点，此时点的颜色为黑色
}
//---------------------------------------------------------------------------------------
//-----------------------------------插入线----------------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnUpdateIns_Curve(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(StartFlag);//与点类似
	if(MouseDraw != MOUSE_EDIT_CURVE) Ins_CurveFlag = FALSE;
	pCmdUI->SetCheck(Ins_CurveFlag);
}
//---------------------------------------------------------
void CMFCRSDlg::OnIns_Curve()//click curve on menu
{
	MouseDraw = MOUSE_EDIT_CURVE;
	Ins_CurveFlag = !Ins_CurveFlag;//此变量用来控制菜单栏插入线选项前是否打勾
	if(Ins_PointFlag)
		Ins_PointFlag = !Ins_PointFlag;
}
//---------------------------------------------------------
void CMFCRSDlg::Ins_Begin(CDC* dc,int x,int y)
{
	if(MouseDraw & MOUSE_INSL_BEGIN)//MOUSE_INSL_BEGIN表明是否是插线时的第一个点
		InsOnePointToCurve(dc,x,y);
	else
	{
		MouseDraw |= MOUSE_INSL_BEGIN;//第一次差点，开出新的数组，用来存储一条线每个点的数据，一条线有最大点数限制
		memset(TablCoorAdr,0,MAXIPOINTNUM*2*sizeof(float));
		odx = x; ody = y;
		InsOnePointToCurve(dc,x,y);
	}
}
//---------------------------------------------------------
void CMFCRSDlg::InsOnePointToCurve(CDC *dc,int x,int y)
{
	if(!(MouseDraw & MOUSE_INSL_BEGIN)) return;
	if(TablPointNum >= MAXIPOINTNUM) return;
	*(TablCoorAdr + 2 * TablPointNum + 0) = x;
	*(TablCoorAdr + 2 * TablPointNum + 1) = y;
	dc->MoveTo(x,y);
	PointCrossLine(x,y);
	dc->SetROP2(R2_NOTXORPEN);//用异或笔，橡皮条技术
	TablPointNum++;
	odx  = x;  ody = y;
	modx = x; mody = y;
}
//---------------------------------------------------------
void CMFCRSDlg::LineTo(int x,int y)//移动时，通过在旧线处在画一次使其消失，然后再画出新线
{
	dc->SelectObject(pengreen);
	dc->MoveTo(odx,ody);
	dc->LineTo(modx,mody);
	dc->MoveTo(odx,ody);
	dc->LineTo(x,y);
	modx = x;
	mody = y;
}
//---------------------------------------------------------------------------------------
//--------------------------插线过程中右击菜单选择结束或者继续---------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnEditL_RMenu(CPoint point)//右键点击后出现子菜单
{
	CMenu menu;       
	CMenu *pSubMenu;   
	menu.LoadMenu(IDR_MENU2); //加载菜单二作为子菜单
	pSubMenu = menu.GetSubMenu(0);      
	ClientToScreen(&point); 
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this); 
}
//---------------------------------------------------------
void CMFCRSDlg::OnFinishLineEdit()//右键弹出的菜单 结束插入线的函数
{
	// TODO: 在此添加命令处理程序代码
	MouseDraw &= ~MOUSE_INSL_BEGIN;
	dc->SelectObject(pengreen);
	dc->MoveTo(odx,ody);
	dc->LineTo(modx,mody);
	CLineTextDlg mlinetext;
	mlinetext.DoModal();
	TablPointNum = atoi(mlinetext.lpointnum);//可以自己输入线的有效个数点进行调整
	objcurve.AddOneCurve(TablPointNum,TablCoorAdr);
	TablPointNum = 0;
	OnPaint();//重画
}
//---------------------------------------------------------------------------------------
//------------------------------工具栏---------------------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnShowToolBar()//显示或关闭工具栏
{
	if(!mrsimg || !mrsimg->Rimg) return;
	ShowToolBar=!ShowToolBar;
	if(!ShowToolBar)
		mtoolbar.ShowWindow(SW_HIDE);
	else
		mtoolbar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateToolBar(CCmdUI *pCmdUI)//更新工具栏是否打开还是关闭
{
	pCmdUI->SetCheck(ShowToolBar);
}
int CMFCRSDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*创建工具栏对象*/
	mtoolbar.Create(this,WS_EX_TOOLWINDOW| WS_VISIBLE| CBRS_ALIGN_TOP|CBRS_TOOLTIPS);
	/*加载工具栏资源*/
	mtoolbar.LoadToolBar(IDR_TOOLBAR1);
	/*设置工具栏位置*/
	return 0;
}
//---------------------------------------------------------------------------------------
//------------------------------------------遥感图像显示---------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::Open000File(CString filename,BOOL DownBol)
{
	mrsimg = new RSImg; if(!mrsimg) return; memset(mrsimg,0,sizeof(RSImg));
	strcpy_s(LoadSaveFileName,200,filename);
	//--------------关于图像的一些参数，从000文件中获取-------------------
	int   ImgW0,ImgH0;
	int   Rmin,Rmax,Gmin,Gmax,Bmin,Bmax,Mmin,Mmax;
	//--------------------------------------------------------------------
	bool  wbool = objrsimg.Read000RSImgFile_Func(LoadSaveFileName,&ImgW0,&ImgH0,mrsimg->R_name,mrsimg->G_name,mrsimg->B_name,mrsimg->M_name,&Rmin,&Rmax,&Gmin,&Gmax,&Bmin,&Bmax,&Mmin,&Mmax);
	//读入000文件中的信息，将其参数记录在上面定义的变量中，但还没有传给mrsimg
	if(wbool)
	{
		mrsimg->EnRmin = Rmin;mrsimg->EnRmax=Rmax;
		mrsimg->EnGmin = Gmin;mrsimg->EnGmax=Gmax;
		mrsimg->EnBmin = Bmin;mrsimg->EnBmax=Bmax;
		if(mrsimg->InMBol)
		{
			mrsimg->EnMmin = Mmin; mrsimg->EnMmax = Mmax;
		}
		wbool = objrsimg.ReadRSFileData_Func(mrsimg,filename,ImgW0,ImgH0,mrsimg->R_name,mrsimg->G_name,mrsimg->B_name,mrsimg->M_name,DownBol);
		//完成了开辟内存并将遥感图像数据加载到内存中的两步操作
	}
	if(wbool) MessageBox(_T("加载图像数据成功！"));
	else     
		{
			MessageBox(_T("加载图像数据失败！"));
			delete mrsimg;//失败后删除mrsimg并赋为NULL
			mrsimg = NULL;
	     }
}
//---------------------------------------------------------
BOOL CMFCRSDlg::ShowRSImg()//显示图像，大小为原来的FastWidth和FastHeight，即若是已分配了显示内存则不重新分配显示内存
{
	if(!mrsimg) return FALSE;
	int width  = mrsimg->Img_W0; //if(width  < 50 ||  width  > 5000) width  = 600;//太小时的处理，可以不用
	int height = mrsimg->Img_H0; //if(height < 50 ||  height > 5000) height = 400;
	mrsimg->ModifyBol = TRUE;//ModifyBol代表是否需要分配显示内存
	if(objrsimg.GencRemoteImgFunc(mrsimg,width,height))
	{ 
		Invalidate();//若是加载显示了工具了，一定需要这个函数
		objrsimg.PaintRemoteImgFunc(dc,mrsimg);
		OnPaint();
	}
}
//---------------------------------------------------------
BOOL CMFCRSDlg::ShowRSImg(int width,int height)//根据传入的width height分配显示图像的width和height，会重新分配显示内存空间，并进行重采样
{
	if(!mrsimg) return FALSE;
	//int width  = mrsimg->Img_W0; if(width  < 50 || width  > 5000) width  = 600;
	//int height = mrsimg->Img_H0; if(height < 50 || height > 5000) height = 400;
	mrsimg->ModifyBol = TRUE;
	if(objrsimg.GencRemoteImgFunc(mrsimg,width,height))
	{	
		Invalidate();
		objrsimg.PaintRemoteImgFunc(dc,mrsimg);
		OnPaint();
	}
}
//---------------------------------------------------------------------------------------
//---------------------------------------移动显示的图像----------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnClickBar1()//拖动图像移动
{
	if(MouseDraw != BMP_MOVE_FLAG)
		MouseDraw = BMP_MOVE_FLAG;
	else
		MouseDraw = 0;
	Ins_CurveFlag=0;
	Ins_PointFlag=0;
}
//---------------------------------------------------------
void CMFCRSDlg::MoveBMPFunc(CPoint point)//图像移动的参数变化
{
	if(IsMouseDown == FALSE) return;
	if(!mrsimg) return;
	mrsimg->ImgXoff = point.x - bmpoffx + firstx; firstx = mrsimg->ImgXoff; bmpoffx = point.x;
	mrsimg->ImgYoff = point.y - bmpoffy + firsty ;firsty = mrsimg->ImgYoff; bmpoffy = point.y;
	Invalidate();
	OnPaint();
}
//---------------------------------------------------------
void CMFCRSDlg::GetBMPOffYX()//获取遥感数据的左上角偏移坐标
{
	if(!mrsimg) return;
	bmpoffx = mrsimg->ImgXoff;
	bmpoffy = mrsimg->ImgYoff;
}
//---------------------------------------------------------
void CMFCRSDlg::SavePreInfo(CPoint point)
{
	bmpoffx = point.x;//上一次鼠标的位置而不是图像开始的位置
	bmpoffy = point.y;
	IsMouseDown = TRUE;
}
//---------------------------------------------------------
void CMFCRSDlg::ShowCurBMP(CPoint point)//判断鼠标是否抬起还是一直按下状态
{
	if(!mrsimg) return;
	IsMouseDown = FALSE;
}
//---------------------------------------------------------
BOOL CMFCRSDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)//图像拖动状态下，鼠标形状改变，并只在客户区才改变形状
{
	GetCursorPos(&mousepoint);
	ScreenToClient(&mousepoint);
	if(mousepoint.y >= 0)
		if(MouseDraw == BMP_MOVE_FLAG)
		{
			HCURSOR hcur = ::LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_CURSOR1));
			::SetCursor(hcur);
			return TRUE;
		}
		else 
		{
			HCURSOR hcur = ::LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_CURSOR2));
			::SetCursor(hcur);
			return TRUE;
		}
	else
		return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
//---------------------------------------------------------------------------------------
//-----------------------------------显示图像的缩放--------------------------------------
//---------------------------------------------------------------------------------------
BOOL CMFCRSDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)//鼠标滚轮事件，图像进行缩放
{
	if(!mrsimg) return FALSE;
	int width  = mrsimg->FastWidth;
	int height = mrsimg->FastHeight;
	CPoint point = pt;
	ScreenToClient(&point);//转为相对于客户区的相对坐标
	if(mrsimg->FastData) objrsimg.DeleteFastData(mrsimg);//因为缩放必定要重新分配显示内存
	int x1 = point.x-mrsimg->ImgXoff;//下面一些计算实现了在鼠标停靠处进行图片缩放，相当于在鼠标停靠处图片的内容不变
	int y1 = point.y-mrsimg->ImgYoff;//这一功能可以实现在感兴趣的区域进行缩放
	if(zDelta<0) strech = 0.9; else strech = (10.0 / 9);//strech记录了当前缩放的比例
	mrsimg->ImgXoff -= x1 * (strech - 1); firstx = mrsimg->ImgXoff;
	mrsimg->ImgYoff -= y1 * (strech - 1); firsty = mrsimg->ImgYoff;
	width *= strech;height *= strech;
	mrsimg->ModifyBol = TRUE;
	if(objrsimg.GencRemoteImgFunc(mrsimg,width,height))
	{
		Invalidate();
		OnPaint();
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
//---------------------------------------------------------
void CMFCRSDlg::OnSetPicSuitable()//图像居中，充分显示，即调整最好的长宽显示
{
	if(!mrsimg) return;
	if(!dc) return;
	int pic_w = mrsimg->FastWidth;
	int pic_h = mrsimg->FastHeight;
	int img_x = mrsimg->ImgXoff;
	int img_y = mrsimg->ImgYoff;
	int screenW = rect.Width();
	int screenH = rect.Height();
	double dx = 1.0 * screenW / pic_w, dy = 1.0 * screenH / pic_h,temp;
	temp = dx<dy?dx:dy;
	if(mrsimg->FastData) objrsimg.DeleteFastData(mrsimg);
	pic_w = pic_w * temp - 80;//稍微留点空隙美观
	pic_h = pic_h * temp - 80;
	img_x = (screenW - pic_w) / 2;
	img_y = (screenH - pic_h) / 2;
	mrsimg->ImgXoff = img_x;
	mrsimg->ImgYoff = img_y;
	firstx = img_x;
	firsty = img_y;
	mrsimg->ModifyBol = TRUE;
	if(objrsimg.GencRemoteImgFunc(mrsimg,pic_w,pic_h))
	{
		Invalidate();
		OnPaint();
	}
	return;
}
//---------------------------------------------------------------------------------------
//------------------------------打开非模态功能子对话框-----------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnOpenBMPFile()//打开遥感文件对话框
{
	if(!mreadbmpdlg)
	{
		mreadbmpdlg = new LoadRSData;
		mreadbmpdlg->Create(IDD_DIALOG3);
		mreadbmpdlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
		mreadbmpdlg->ShowWindow(SW_SHOWNORMAL);
}
//---------------------------------------------------------
void CMFCRSDlg::OnRGBStatistics()//打开直方图统计对话框
{
	if(!mRGBStatistic)
	{
		mRGBStatistic = new CFactStatistics;
		mRGBStatistic->Create(IDD_DIALOG4);
		mRGBStatistic->ShowWindow(SW_SHOWNORMAL);
	}
	else
		mRGBStatistic->ShowWindow(SW_SHOWNORMAL);
}
//---------------------------------------------------------
void CMFCRSDlg::OnOpenScatterDlg()//打开二位散点图对话框
{
	if(!mscatterdlg)
	{
		mscatterdlg = new CScatterDlg;
		mscatterdlg->Create(IDD_DIALOG5);
		mscatterdlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
		mscatterdlg->ShowWindow(SW_SHOWNORMAL);
}
//---------------------------------------------------------
void CMFCRSDlg::OnPicToolFunc()//打开图像工具对话框
{
	if(!mpictooldlg)
	{
		mpictooldlg = new CPicTool;
		mpictooldlg->Create(IDD_DIALOG6);
		mpictooldlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
		mpictooldlg->ShowWindow(SW_SHOWNORMAL);
}
//---------------------------------------------------------
void CMFCRSDlg::OnOpenTFToolDlg()//打开转换工具对话框
{
	if(!mtranformtooldlg)
	{
		mtranformtooldlg = new CTransformTool;
		mtranformtooldlg->Create(IDD_DIALOG7);
		mtranformtooldlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
		mtranformtooldlg->ShowWindow(SW_SHOWNORMAL);
}
//---------------------------------------------------------
void CMFCRSDlg::ScreenPrintf(int nWidth,int nHeight)//屏幕输出，所见及所得，需要在此处获得dc和客户大小的rect
{
	BOOL wbol = objrsimg.DoWriteBmpFileFunc(printname,nWidth,nHeight,dc,&rect);
	if(wbol) MessageBox(_T("屏幕输出成功!"));
	else     MessageBox(_T(ErrStr));
}

//---------------------------------------------------------------------------------------
//-------------------------------------工具栏功能----------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnClockRotate90()//图像顺时针旋转90度
{
	if(!mrsimg) return;
	if(!mrsimg->FastData) return;
	objrsimg.Bitmap_Rota90DFunc(mrsimg);
	ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CMFCRSDlg::OnInvClockRotate90()//图像逆时针旋转90度
{
	if(!mrsimg) return;
	if(!mrsimg->FastData) return;
	objrsimg.Bitmap_Rota270Func(mrsimg);
	ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);

}
//---------------------------------------------------------------------------------------
//----------------------------是否加载遥感数据时菜单栏状态-------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnUpdateFactStatistics(CCmdUI *pCmdUI)//未加载时，直方图统计选项不可选
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
void CMFCRSDlg::OnUpdateScatter(CCmdUI *pCmdUI)
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateTFTool(CCmdUI *pCmdUI)//因为有子级菜单选项，不可用
{
	/*if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);*/
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdatePicTool(CCmdUI *pCmdUI)//未加载时，图片工具选项不可选
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateMoveBMP(CCmdUI *pCmdUI)//未加载时，移动图片选项不可选
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
	if(MouseDraw == BMP_MOVE_FLAG)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateSetPicCenter(CCmdUI *pCmdUI)//未加载时，图片居中选项不可选
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
//---------------------------------------------------------------------------------------
//-------------------------------------系统消息函数重写----------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------
BOOL CMFCRSDlg::OnEraseBkgnd(CDC* pDC)//双缓冲相关 直接返回true
{
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}