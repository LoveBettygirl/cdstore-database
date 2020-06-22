#pragma once
#include "afxwin.h"


// CAdminDlg 对话框

class CAdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminDlg)

public:
	CAdminDlg(_ConnectionPtr p,int id,CString name,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminDlg();
	int m_id;
	CString m_name;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMINDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CStatic m_greeting;
	afx_msg void OnClose();
};
