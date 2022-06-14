
// MFCRSDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CMFCRSDlg �Ի���



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


// CMFCRSDlg ��Ϣ�������

BOOL CMFCRSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetClientRect(&rect);
	dc=GetDC();
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(dc,rect.Width(),rect.Height());
    pOldBit = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0,0,2000,1500,RGB(255,255,255));
	/*	mtoolbar.CreateEx(this);
	mtoolbar.LoadToolBar(IDR_TOOLBAR1);
	mtoolbar.ShowWindow(1);*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
//  ��������
//  �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CMFCRSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(dc)
		{
			//�ػ�ʹ����˫���壬�����˼��ݵ�MemDC,���������Ļ������������滭���һ���Կ�������ǰ��dc��������������������
			GetClientRect(&rect);
			MemDC.FillSolidRect(0,0,2000,1500,RGB(255,255,255));
			objrsimg.PaintRemoteImgFunc(&MemDC,mrsimg);
			objcurve.PaintMyCurveFunc(&MemDC);
			objpoint.PaintMyPointFunc(&MemDC);
			dc->BitBlt(0,0,2000,1500,&MemDC,0,0,SRCCOPY);//��MemDC�е��ڴ濽����dc����
		}
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
//-------------------------------------�򿪡���������ļ�--------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnNewFile()//�½�
{
	objpoint.DeletePointLst();//ɾ��ԭ������������
	objcurve.DeleteCurveFunc();
	objrsimg.DeleteFastData(mrsimg);
	objrsimg.Delete_Char_RSImg_MemorySpace(mrsimg);
	mrsimg = NULL; mshpfilename = "";
	Invalidate();
	OnPaint();//ˢ���ػ�
}
//---------------------------------------------------------
void CMFCRSDlg::OnOpenFile()//�򿪵㡢���ļ�
{
	CFileDialog openfile( 1, _T("mshp"), NULL, 0, _T("mshp�ļ�(*.mshp)|*.mshp||"), this );//�ļ����ͺ�׺��δmshp
	if( IDOK == openfile.DoModal() )
	{
		CFile File;//�����ʹ����CFile�࣬��һ��������
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
		if(shpfileflag == FILE_PANDC_FLAG)//����������mshp�ļ��Ĵ����ͣ��ֱ���ֻ�е㡢ֻ���ߡ�ͬʱ�е���ߣ���ʱ���ݶ������ĸ��ֽ��ж�
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
void CMFCRSDlg::OnSaveFile()//����㡢���ļ������������ļ��򿪣���ֱ�ӱ��棬�����½��ģ������ļ������
{
	if( mshpfilename == "" )
	{
		CFileDialog openfile( 0, _T("mshp"), NULL, 0, _T("mshp�ļ�(*.mshp)|*.mshp||"), this );
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
void CMFCRSDlg::OnSaveOtherPlace()//�������ļ������ļ�������Լ�ѡ�񱣴�·�����ļ���
{
	CFileDialog openfile( 0, _T("mshp"), NULL, 0, _T("mshp�ļ�(*.mshp)|*.mshp||"), this );
	if( IDOK == openfile.DoModal() )
		OnSaveFile(openfile.GetPathName());
	shpfileflag=0;
}
//---------------------------------------------------------
void CMFCRSDlg::OnSaveFile(CString filename)
{
	if(HeadPoint && HeadCurve)//����ֻ�е㡢ֻ���߻��ǵ���߶��з����������Ӧ�ĺ�������
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
void CMFCRSDlg::OnUpdateSaveFile(CCmdUI *pCmdUI)//û�е��������ʱ������ѡ���ѡ
{
	if(!HeadCurve && !HeadPoint)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateSaveFileOther(CCmdUI *pCmdUI)//û�е��������ʱ�����Ϊѡ���ѡ
{
	if(!HeadCurve && !HeadPoint)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}
//---------------------------------------------------------------------------------------
//-----------------------------------�༭�㡢��------------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnEditBegin()
{
	StartFlag = !StartFlag;//�ı�༭״̬
}
//---------------------------------------------------------
void CMFCRSDlg::OnEditStop()//ֹͣ�༭
{
	StartFlag = !StartFlag;//��ز���ȫ����Ϊ0
	Ins_PointFlag = FALSE;
	Ins_CurveFlag = FALSE;
	MouseDraw = 0;
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateEditBegin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(StartFlag);//���ݱ༭״̬���²˵����п�ʼ�༭�Ĵ�
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateEditStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(StartFlag);//δ��ʼ�༭ʱ��ֹͣ�༭Ϊ�ң���ʼ�༭�󣬿�ʼ�༭��أ�ֹͣ�༭��ѡ
}
//---------------------------------------------------------------------------------------
//----------------------------------��궯���¼�-----------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnLButtonDown(UINT nFlags, CPoint point)//����������
{
	if     (MouseDraw & MOUSE_EDIT_POINT && Ins_PointFlag) PointCrossLine(point.x,point.y);//����
	else if(MouseDraw & MOUSE_EDIT_CURVE) Ins_Begin(dc,point.x,point.y);
	else if(MouseDraw & BMP_MOVE_FLAG)  SavePreInfo(point);
	CDialogEx::OnLButtonDown(nFlags, point);
}
//---------------------------------------------------------
void CMFCRSDlg::OnLButtonUp(UINT nFlags, CPoint point)//������̧��
{
	if     (MouseDraw & MOUSE_EDIT_POINT && Ins_PointFlag) CPointText(point.x,point.y);//����󵯳���ע�ǶԻ���
	else if(MouseDraw & BMP_MOVE_FLAG) ShowCurBMP(point);
	CDialogEx::OnLButtonUp(nFlags, point);
}
//---------------------------------------------------------
void CMFCRSDlg::OnRButtonUp(UINT nFlags, CPoint point)//����Ҽ�̧��
{
	if(MouseDraw & MOUSE_EDIT_CURVE && MouseDraw & MOUSE_INSL_BEGIN)//�����߹������Ҽ�̧�������Ӳ˵�
		OnEditL_RMenu(point);
}
//---------------------------------------------------------
void CMFCRSDlg::OnMouseMove(UINT nFlags, CPoint point)//����ƶ�
{
	if      (MouseDraw & MOUSE_EDIT_POINT) return;
	else if (MouseDraw & MOUSE_EDIT_CURVE && MouseDraw & MOUSE_INSL_BEGIN)
		LineTo(point.x,point.y);//������ʱ�ƶ�״̬�²����߻�����
	else if (MouseDraw & BMP_MOVE_FLAG && IsMouseDown)
		MoveBMPFunc(point);//�ƶ�ͼƬ
	CDialogEx::OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------------
//------------------------------����ͱ��ע��----------------------------------------
//------------------------------------------------------------------------------------
void CMFCRSDlg::OnIns_Point()//ѡ�в�����״̬
{
	MouseDraw =  MOUSE_EDIT_POINT;//MouseDraw�����ж�����¼���������ʲô״̬���в�㡢���ߡ��ƶ�ͼƬ����
	Ins_PointFlag = !Ins_PointFlag;//�˱����������Ʋ˵��������ѡ��ǰ�Ƿ��
	if(Ins_CurveFlag)
		Ins_CurveFlag = !Ins_CurveFlag;
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateIns_Point(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(StartFlag);//��δ��ʼ�༭��������ѡ���ѡ
	if(MouseDraw != MOUSE_EDIT_POINT) Ins_PointFlag = FALSE;
	pCmdUI->SetCheck(Ins_PointFlag);//���´�
}
//---------------------------------------------------------
void CMFCRSDlg::PointCrossLine(int x,int y)
{
	dc->SelectObject(penred);
	dc->MoveTo(x-5,y);dc->LineTo(x+5,y);//�Դ�ֱ����ʮ�ֱ�ʾ��
	dc->MoveTo(x,y-5);dc->LineTo(x,y+5);
	//dc->Ellipse(x-5,y+5,x+5,y-5);//��Բ��ʾ��
}
//---------------------------------------------------------
void CMFCRSDlg::CPointText(int x,int y)
{
	PointDlginfo mpointinfo;
	mpointinfo.point_x = x;//��������괫����ʾ��ע�ǵĶԻ�����ʾ�������
	mpointinfo.point_y = y;
	mpointinfo.DoModal();
	dc->TextOut(x+6,y,mpointinfo.instr);//mpointinfo.instr�ǵ��ע����Ϣ
	objpoint.AddOnePoint(x,y,mpointinfo.instr);//������ӵ������д���
	OnPaint();//���»��㣬��ʱ�����ɫΪ��ɫ
}
//---------------------------------------------------------------------------------------
//-----------------------------------������----------------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnUpdateIns_Curve(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(StartFlag);//�������
	if(MouseDraw != MOUSE_EDIT_CURVE) Ins_CurveFlag = FALSE;
	pCmdUI->SetCheck(Ins_CurveFlag);
}
//---------------------------------------------------------
void CMFCRSDlg::OnIns_Curve()//click curve on menu
{
	MouseDraw = MOUSE_EDIT_CURVE;
	Ins_CurveFlag = !Ins_CurveFlag;//�˱����������Ʋ˵���������ѡ��ǰ�Ƿ��
	if(Ins_PointFlag)
		Ins_PointFlag = !Ins_PointFlag;
}
//---------------------------------------------------------
void CMFCRSDlg::Ins_Begin(CDC* dc,int x,int y)
{
	if(MouseDraw & MOUSE_INSL_BEGIN)//MOUSE_INSL_BEGIN�����Ƿ��ǲ���ʱ�ĵ�һ����
		InsOnePointToCurve(dc,x,y);
	else
	{
		MouseDraw |= MOUSE_INSL_BEGIN;//��һ�β�㣬�����µ����飬�����洢һ����ÿ��������ݣ�һ����������������
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
	dc->SetROP2(R2_NOTXORPEN);//�����ʣ���Ƥ������
	TablPointNum++;
	odx  = x;  ody = y;
	modx = x; mody = y;
}
//---------------------------------------------------------
void CMFCRSDlg::LineTo(int x,int y)//�ƶ�ʱ��ͨ���ھ��ߴ��ڻ�һ��ʹ����ʧ��Ȼ���ٻ�������
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
//--------------------------���߹������һ��˵�ѡ��������߼���---------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnEditL_RMenu(CPoint point)//�Ҽ����������Ӳ˵�
{
	CMenu menu;       
	CMenu *pSubMenu;   
	menu.LoadMenu(IDR_MENU2); //���ز˵�����Ϊ�Ӳ˵�
	pSubMenu = menu.GetSubMenu(0);      
	ClientToScreen(&point); 
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this); 
}
//---------------------------------------------------------
void CMFCRSDlg::OnFinishLineEdit()//�Ҽ������Ĳ˵� ���������ߵĺ���
{
	// TODO: �ڴ���������������
	MouseDraw &= ~MOUSE_INSL_BEGIN;
	dc->SelectObject(pengreen);
	dc->MoveTo(odx,ody);
	dc->LineTo(modx,mody);
	CLineTextDlg mlinetext;
	mlinetext.DoModal();
	TablPointNum = atoi(mlinetext.lpointnum);//�����Լ������ߵ���Ч��������е���
	objcurve.AddOneCurve(TablPointNum,TablCoorAdr);
	TablPointNum = 0;
	OnPaint();//�ػ�
}
//---------------------------------------------------------------------------------------
//------------------------------������---------------------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnShowToolBar()//��ʾ��رչ�����
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
void CMFCRSDlg::OnUpdateToolBar(CCmdUI *pCmdUI)//���¹������Ƿ�򿪻��ǹر�
{
	pCmdUI->SetCheck(ShowToolBar);
}
int CMFCRSDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*��������������*/
	mtoolbar.Create(this,WS_EX_TOOLWINDOW| WS_VISIBLE| CBRS_ALIGN_TOP|CBRS_TOOLTIPS);
	/*���ع�������Դ*/
	mtoolbar.LoadToolBar(IDR_TOOLBAR1);
	/*���ù�����λ��*/
	return 0;
}
//---------------------------------------------------------------------------------------
//------------------------------------------ң��ͼ����ʾ---------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::Open000File(CString filename,BOOL DownBol)
{
	mrsimg = new RSImg; if(!mrsimg) return; memset(mrsimg,0,sizeof(RSImg));
	strcpy_s(LoadSaveFileName,200,filename);
	//--------------����ͼ���һЩ��������000�ļ��л�ȡ-------------------
	int   ImgW0,ImgH0;
	int   Rmin,Rmax,Gmin,Gmax,Bmin,Bmax,Mmin,Mmax;
	//--------------------------------------------------------------------
	bool  wbool = objrsimg.Read000RSImgFile_Func(LoadSaveFileName,&ImgW0,&ImgH0,mrsimg->R_name,mrsimg->G_name,mrsimg->B_name,mrsimg->M_name,&Rmin,&Rmax,&Gmin,&Gmax,&Bmin,&Bmax,&Mmin,&Mmax);
	//����000�ļ��е���Ϣ�����������¼�����涨��ı����У�����û�д���mrsimg
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
		//����˿����ڴ沢��ң��ͼ�����ݼ��ص��ڴ��е���������
	}
	if(wbool) MessageBox(_T("����ͼ�����ݳɹ���"));
	else     
		{
			MessageBox(_T("����ͼ������ʧ�ܣ�"));
			delete mrsimg;//ʧ�ܺ�ɾ��mrsimg����ΪNULL
			mrsimg = NULL;
	     }
}
//---------------------------------------------------------
BOOL CMFCRSDlg::ShowRSImg()//��ʾͼ�񣬴�СΪԭ����FastWidth��FastHeight���������ѷ�������ʾ�ڴ������·�����ʾ�ڴ�
{
	if(!mrsimg) return FALSE;
	int width  = mrsimg->Img_W0; //if(width  < 50 ||  width  > 5000) width  = 600;//̫Сʱ�Ĵ������Բ���
	int height = mrsimg->Img_H0; //if(height < 50 ||  height > 5000) height = 400;
	mrsimg->ModifyBol = TRUE;//ModifyBol�����Ƿ���Ҫ������ʾ�ڴ�
	if(objrsimg.GencRemoteImgFunc(mrsimg,width,height))
	{ 
		Invalidate();//���Ǽ�����ʾ�˹����ˣ�һ����Ҫ�������
		objrsimg.PaintRemoteImgFunc(dc,mrsimg);
		OnPaint();
	}
}
//---------------------------------------------------------
BOOL CMFCRSDlg::ShowRSImg(int width,int height)//���ݴ����width height������ʾͼ���width��height�������·�����ʾ�ڴ�ռ䣬�������ز���
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
//---------------------------------------�ƶ���ʾ��ͼ��----------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnClickBar1()//�϶�ͼ���ƶ�
{
	if(MouseDraw != BMP_MOVE_FLAG)
		MouseDraw = BMP_MOVE_FLAG;
	else
		MouseDraw = 0;
	Ins_CurveFlag=0;
	Ins_PointFlag=0;
}
//---------------------------------------------------------
void CMFCRSDlg::MoveBMPFunc(CPoint point)//ͼ���ƶ��Ĳ����仯
{
	if(IsMouseDown == FALSE) return;
	if(!mrsimg) return;
	mrsimg->ImgXoff = point.x - bmpoffx + firstx; firstx = mrsimg->ImgXoff; bmpoffx = point.x;
	mrsimg->ImgYoff = point.y - bmpoffy + firsty ;firsty = mrsimg->ImgYoff; bmpoffy = point.y;
	Invalidate();
	OnPaint();
}
//---------------------------------------------------------
void CMFCRSDlg::GetBMPOffYX()//��ȡң�����ݵ����Ͻ�ƫ������
{
	if(!mrsimg) return;
	bmpoffx = mrsimg->ImgXoff;
	bmpoffy = mrsimg->ImgYoff;
}
//---------------------------------------------------------
void CMFCRSDlg::SavePreInfo(CPoint point)
{
	bmpoffx = point.x;//��һ������λ�ö�����ͼ��ʼ��λ��
	bmpoffy = point.y;
	IsMouseDown = TRUE;
}
//---------------------------------------------------------
void CMFCRSDlg::ShowCurBMP(CPoint point)//�ж�����Ƿ�̧����һֱ����״̬
{
	if(!mrsimg) return;
	IsMouseDown = FALSE;
}
//---------------------------------------------------------
BOOL CMFCRSDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)//ͼ���϶�״̬�£������״�ı䣬��ֻ�ڿͻ����Ÿı���״
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
//-----------------------------------��ʾͼ�������--------------------------------------
//---------------------------------------------------------------------------------------
BOOL CMFCRSDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)//�������¼���ͼ���������
{
	if(!mrsimg) return FALSE;
	int width  = mrsimg->FastWidth;
	int height = mrsimg->FastHeight;
	CPoint point = pt;
	ScreenToClient(&point);//תΪ����ڿͻ������������
	if(mrsimg->FastData) objrsimg.DeleteFastData(mrsimg);//��Ϊ���űض�Ҫ���·�����ʾ�ڴ�
	int x1 = point.x-mrsimg->ImgXoff;//����һЩ����ʵ���������ͣ��������ͼƬ���ţ��൱�������ͣ����ͼƬ�����ݲ���
	int y1 = point.y-mrsimg->ImgYoff;//��һ���ܿ���ʵ���ڸ���Ȥ�������������
	if(zDelta<0) strech = 0.9; else strech = (10.0 / 9);//strech��¼�˵�ǰ���ŵı���
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
void CMFCRSDlg::OnSetPicSuitable()//ͼ����У������ʾ����������õĳ�����ʾ
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
	pic_w = pic_w * temp - 80;//��΢�����϶����
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
//------------------------------�򿪷�ģ̬�����ӶԻ���-----------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnOpenBMPFile()//��ң���ļ��Ի���
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
void CMFCRSDlg::OnRGBStatistics()//��ֱ��ͼͳ�ƶԻ���
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
void CMFCRSDlg::OnOpenScatterDlg()//�򿪶�λɢ��ͼ�Ի���
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
void CMFCRSDlg::OnPicToolFunc()//��ͼ�񹤾߶Ի���
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
void CMFCRSDlg::OnOpenTFToolDlg()//��ת�����߶Ի���
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
void CMFCRSDlg::ScreenPrintf(int nWidth,int nHeight)//��Ļ��������������ã���Ҫ�ڴ˴����dc�Ϳͻ���С��rect
{
	BOOL wbol = objrsimg.DoWriteBmpFileFunc(printname,nWidth,nHeight,dc,&rect);
	if(wbol) MessageBox(_T("��Ļ����ɹ�!"));
	else     MessageBox(_T(ErrStr));
}

//---------------------------------------------------------------------------------------
//-------------------------------------����������----------------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnClockRotate90()//ͼ��˳ʱ����ת90��
{
	if(!mrsimg) return;
	if(!mrsimg->FastData) return;
	objrsimg.Bitmap_Rota90DFunc(mrsimg);
	ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);
}
//---------------------------------------------------------
void CMFCRSDlg::OnInvClockRotate90()//ͼ����ʱ����ת90��
{
	if(!mrsimg) return;
	if(!mrsimg->FastData) return;
	objrsimg.Bitmap_Rota270Func(mrsimg);
	ShowRSImg(mrsimg->FastWidth,mrsimg->FastHeight);

}
//---------------------------------------------------------------------------------------
//----------------------------�Ƿ����ң������ʱ�˵���״̬-------------------------------
//---------------------------------------------------------------------------------------
void CMFCRSDlg::OnUpdateFactStatistics(CCmdUI *pCmdUI)//δ����ʱ��ֱ��ͼͳ��ѡ���ѡ
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
void CMFCRSDlg::OnUpdateTFTool(CCmdUI *pCmdUI)//��Ϊ���Ӽ��˵�ѡ�������
{
	/*if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);*/
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdatePicTool(CCmdUI *pCmdUI)//δ����ʱ��ͼƬ����ѡ���ѡ
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateMoveBMP(CCmdUI *pCmdUI)//δ����ʱ���ƶ�ͼƬѡ���ѡ
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
	if(MouseDraw == BMP_MOVE_FLAG)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
//---------------------------------------------------------
void CMFCRSDlg::OnUpdateSetPicCenter(CCmdUI *pCmdUI)//δ����ʱ��ͼƬ����ѡ���ѡ
{
	if(mrsimg) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
//---------------------------------------------------------------------------------------
//-------------------------------------ϵͳ��Ϣ������д----------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------
BOOL CMFCRSDlg::OnEraseBkgnd(CDC* pDC)//˫������� ֱ�ӷ���true
{
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}