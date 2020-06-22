#pragma once
#include "afxwin.h"


// CLookCDinfo 对话框

class CLookCDinfo : public CDialogEx
{
	DECLARE_DYNAMIC(CLookCDinfo)

public:
	CLookCDinfo(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLookCDinfo();
	int adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKCDINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_cdinfolist;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedModifyinfo();
};
