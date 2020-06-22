#pragma once
#include "DotEdit.h"
#include "afxwin.h"


// CAddBal 对话框

class CAddBal : public CDialogEx
{
	DECLARE_DYNAMIC(CAddBal)

public:
	CAddBal(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddBal();
	CString id;
	double balance;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDBAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDotEdit m_balance;
	CStatic m_baldisp;
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	CEdit m_pwdbal;
	CString m_pwdstr;
	CString m_balstr;
};
