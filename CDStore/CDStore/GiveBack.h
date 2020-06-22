#pragma once
#include "afxwin.h"


// CGiveBack 对话框

class CGiveBack : public CDialogEx
{
	DECLARE_DYNAMIC(CGiveBack)

public:
	CGiveBack(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGiveBack();
	CString id;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIVEBACK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_givebackcd;
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
};
