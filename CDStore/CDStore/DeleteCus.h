#pragma once
#include "afxwin.h"


// CDeleteCus 对话框

class CDeleteCus : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteCus)

public:
	CDeleteCus(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeleteCus();
	CString id;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANCEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_calpwd;
	virtual void OnOK();
};
