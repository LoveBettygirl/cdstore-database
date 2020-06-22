#pragma once
#include "afxwin.h"


// CLookSup 对话框

class CLookSup : public CDialogEx
{
	DECLARE_DYNAMIC(CLookSup)

public:
	CLookSup(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLookSup();
	int m_adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKSUPPLIER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_supidlist;
	afx_msg void OnLbnSelchangeSupidlist();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedAddsup();
};
