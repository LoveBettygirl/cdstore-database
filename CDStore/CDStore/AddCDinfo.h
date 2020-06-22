#pragma once
#include "afxwin.h"


// CAddCDinfo 对话框

class CAddCDinfo : public CDialogEx
{
	DECLARE_DYNAMIC(CAddCDinfo)

public:
	CAddCDinfo(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // 标准构造函数
	CAddCDinfo(_ConnectionPtr p, int id, int cdid, CWnd* pParent = NULL);
	virtual ~CAddCDinfo();
	int adminid;
	int m_cdid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDCDINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	CComboBox m_genre;
	CEdit m_cdname;
	CEdit m_artist;
};
