#pragma once
#include "afxwin.h"


// CTrade 对话框

class CTrade : public CDialogEx
{
	DECLARE_DYNAMIC(CTrade)

public:
	CTrade(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTrade();
	CString id;
	double balance;
	int tradetype;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRADE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	CEdit m_cdid;
	CString m_cdidstr;
};
