#pragma once
#include "afxwin.h"


// CLookGenre 对话框

class CLookGenre : public CDialogEx
{
	DECLARE_DYNAMIC(CLookGenre)

public:
	CLookGenre(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLookGenre();
	int adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKGENRE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_genrelist;
	CEdit m_addgenre;
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
