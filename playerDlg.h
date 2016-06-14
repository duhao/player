
// playerDlg.h : 头文件
//

#pragma once
#include "Enconn.h"
#include "Decoderav.h"

// CplayerDlg 对话框
class CplayerDlg : public CDialogEx
{
// 构造
public:
	CplayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持	
// 实现
protected:
	HICON m_hIcon;
	Enconn *conn;
	Decoderav* m_Dec;
	
	//生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_msg;
	afx_msg void OnBnClickedButton4();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CRichEditCtrl m_RichEdit;
};
