#pragma once
#include "afxwin.h"


// CLookStock 对话框

class CLookStock : public CDialogEx
{
	DECLARE_DYNAMIC(CLookStock)

public:
	CLookStock(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLookStock();
	int adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKSTOCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_stocklist;
	virtual BOOL OnInitDialog();
};
