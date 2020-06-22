#pragma once
#include "afxwin.h"


// CCustomerDlg 对话框

class CCustomerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomerDlg)

public:
	CCustomerDlg(_ConnectionPtr p, CString cusid,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustomerDlg();
	CString id;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_greeting;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};
