// LookStock.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookStock.h"
#include "afxdialogex.h"


// CLookStock 对话框

IMPLEMENT_DYNAMIC(CLookStock, CDialogEx)

CLookStock::CLookStock(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKSTOCK, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookStock::~CLookStock()
{
}

void CLookStock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STOCKLIST, m_stocklist);
}


BEGIN_MESSAGE_MAP(CLookStock, CDialogEx)
END_MESSAGE_MAP()


// CLookStock 消息处理程序


BOOL CLookStock::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
	CString sqlHasRecord;
	sqlHasRecord = L"select * from Stock order by pur_date";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//遍历返回的每一条记录
			{
				CString str, all;
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("stock_id"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("admin_id"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("sup_id"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_id"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("stock_id"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("pur_price"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("lent_count"));
				all = all + str + L"、";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("pur_date"));
				all = all + str;
				m_stocklist.InsertString(m_stocklist.GetCount(), all);
				m_pRecordset->MoveNext();
			}
		}
		else
		{
			AfxMessageBox(L"Create instance of recordset failed!");
			return FALSE;
		}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		CString temp1 = e.Description(), temp2 = e.ErrorMessage();
		AfxMessageBox(temp1 + L"\r\n" + temp2);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
