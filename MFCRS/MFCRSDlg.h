
// MFCRSDlg.h : ͷ�ļ�
//
#include"MyFunc.h"
#include"LoadRSData.h"
#include "resource.h"
#include"FactStatistics.h"
#include"ScatterDlg.h"
#include"PicTool.h"
#include"TransformTool.h"
#pragma once


// CMFCRSDlg �Ի���
class CMFCRSDlg : public CDialogEx
{
// ����
public:
	CMFCRSDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_MFCRS_DIALOG};
//-------˫����---------------------------
	CDC MemDC;//�ڴ��п������Ļ�����������黭������StrechDiBits�������ã���һ���Ը��Ƶ���ʾ����dc���У�������������������
	CBitmap MemBitmap;//�˴��ļ����������Է������У���ʼ����ϡ�Ϳ��Էֱ���ڹ��캯��������������
	CBitmap *pOldBit;
//-------����-----------------------------
	CPen *penwhite;
	CPen *pengreen;
	CPen *penred;
//------�༭����--------------------------
	BOOL StartFlag;
	BOOL Ins_PointFlag;
	BOOL Ins_CurveFlag;
	BOOL IsMouseDown;
//----------------------------------------

//--------����Ի���----------------------
	LoadRSData* mreadbmpdlg;
	CFactStatistics* mRGBStatistic;
	CScatterDlg* mscatterdlg;
	CPicTool* mpictooldlg;
	CTransformTool* mtranformtooldlg;
//-------������ع��̵ļ�¼����-----------
    int odx;//��һ�ε��������
	int ody;
	int modx;//��һ������ƶ�ʱ��׽������
	int mody;
//----------------------------------------
//-------�ƶ�����ͼƬ�Ĳ���---------------
	int bmpoffx;
	int bmpoffy;
	int firstx;
	int firsty;
	float strech;
//----------��������----------------------
	CDC *dc;//��Ļָ��
	CRect rect; //��ȡ��ǰ�Ի����С
	CPoint mousepoint;//����������
	CToolBar mtoolbar;//������
	CString printname;//��Ļ������ļ���
	BOOL ShowToolBar;//��ʾ������
//----------------------------------------
//--------------����----------------------
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//------------��Ϣӳ�亯��---------------------
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
