
// MFCRSDlg.h : 头文件
//
#include"MyFunc.h"
#include"LoadRSData.h"
#include "resource.h"
#include"FactStatistics.h"
#include"ScatterDlg.h"
#include"PicTool.h"
#include"TransformTool.h"
#pragma once


// CMFCRSDlg 对话框
class CMFCRSDlg : public CDialogEx
{
// 构造
public:
	CMFCRSDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_MFCRS_DIALOG};
//-------双缓冲---------------------------
	CDC MemDC;//内存中看不见的画布，先在这块画布上用StrechDiBits函数画好，再一次性复制到显示屏的dc当中，这样可消除闪屏现象
	CBitmap MemBitmap;//此处的几个变量可以放来类中，初始化和稀释可以分别放在构造函数和析构函数中
	CBitmap *pOldBit;
//-------画笔-----------------------------
	CPen *penwhite;
	CPen *pengreen;
	CPen *penred;
//------编辑点线--------------------------
	BOOL StartFlag;
	BOOL Ins_PointFlag;
	BOOL Ins_CurveFlag;
	BOOL IsMouseDown;
//----------------------------------------

//--------界面对话框----------------------
	LoadRSData* mreadbmpdlg;
	CFactStatistics* mRGBStatistic;
	CScatterDlg* mscatterdlg;
	CPicTool* mpictooldlg;
	CTransformTool* mtranformtooldlg;
//-------画线相关过程的记录坐标-----------
    int odx;//上一次点击的坐标
	int ody;
	int modx;//上一次鼠标移动时捕捉的坐标
	int mody;
//----------------------------------------
//-------移动缩放图片的参数---------------
	int bmpoffx;
	int bmpoffy;
	int firstx;
	int firsty;
	float strech;
//----------其他变量----------------------
	CDC *dc;//画幕指针
	CRect rect; //获取当前对话框大小
	CPoint mousepoint;//存放鼠标坐标
	CToolBar mtoolbar;//工具栏
	CString printname;//屏幕输出的文件名
	BOOL ShowToolBar;//显示工具栏
//----------------------------------------
//--------------函数----------------------
	void PointCrossLine(int x,int y);
	void Ins_Begin(CDC* dc,int x,int y);
	void CPointText(int x,int y);
	void InsOnePointToCurve(CDC* dc,int x,int y);
	void LineTo(int x,int y);
	void OnEditL_RMenu(CPoint point);
	void Open000File(CString filename,BOOL DownBol);
	void MoveBMPFunc(CPoint point);
	void GetBMPOffYX();
	void SavePreInfo(CPoint point);
	void ShowCurBMP(CPoint point);
	void DeleteRSImg_Paint();
	void ScreenPrintf(int nWidth,int nHeight);
	void OnSaveFile(CString filename);
	BOOL ShowRSImg();
	BOOL ShowRSImg(int width,int height);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//------------消息映射函数---------------------
	afx_msg void OnEditBegin();
	afx_msg void OnUpdateEditBegin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIns_Point(CCmdUI *pCmdUI);
	afx_msg void OnIns_Point();
	afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
	afx_msg void OnUpdateIns_Curve(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditStop(CCmdUI *pCmdUI);
	afx_msg void OnEditStop();
	afx_msg void OnNewFile();
	afx_msg void OnOpenFile();
	afx_msg void OnSaveFile();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnIns_Curve();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFinishLineEdit();
	afx_msg void OnOpenBMPFile();
	afx_msg void OnRGBStatistics();
	afx_msg void OnOpenScatterDlg();
	afx_msg void OnClickBar1();
	afx_msg void OnClockRotate90();
	afx_msg void OnInvClockRotate90();
	afx_msg void OnOpenTFToolDlg();
	afx_msg void OnSetCenter();
	afx_msg void OnPictureTool();
	afx_msg void OnSetPicSuitable();
	afx_msg void OnPicToolFunc();
	afx_msg void OnUpdateFactStatistics(CCmdUI *pCmdUI);
	afx_msg void OnUpdateScatter(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTFTool(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePicTool(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoveBMP(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetPicCenter(CCmdUI *pCmdUI);
	afx_msg void OnSaveOtherPlace();
	afx_msg void OnUpdateSaveFile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSaveFileOther(CCmdUI *pCmdUI);
	afx_msg void OnShowToolBar();
	afx_msg void OnUpdateToolBar(CCmdUI *pCmdUI);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnMenuGetObject(MENUGETOBJECTINFO* pMenuGetObjectInfo);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
