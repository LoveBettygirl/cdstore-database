
// CDStoreDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CustomerDlg.h"


// CCDStoreDlg 对话框
class CCDStoreDlg : public CDialogEx
{
// 构造
public:
	CCDStoreDlg(_ConnectionPtr p,CWnd* pParent = NULL);	// 标准构造函数
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDSTORE_DIALOG };
#endif

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
	virtual void OnFinalRelease();
	afx_msg void OnClose();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedRegister();
	CEdit m_id;
	CEdit m_password;
	CString m_idstr;
	CString m_passwordstr;
	virtual void OnOK();
	afx_msg void OnBnClickedAbout();
};
