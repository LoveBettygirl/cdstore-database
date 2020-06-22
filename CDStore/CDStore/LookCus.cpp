// LookCus.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookCus.h"
#include "afxdialogex.h"


// CLookCus 对话框

IMPLEMENT_DYNAMIC(CLookCus, CDialogEx)

CLookCus::CLookCus(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKCUS, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookCus::~CLookCus()
{
}

void CLookCus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOOKCUSID, m_cusid);
	DDX_Control(pDX, IDC_CUSLENT, m_cuslent);
	DDX_Control(pDX, IDC_CUSBUY, m_cusbuy);
	DDX_Control(pDX, IDC_CUSBAL, m_cusbal);
	DDX_Control(pDX, IDC_CUSTIME, m_custime);
}


BEGIN_MESSAGE_MAP(CLookCus, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCHCUS, &CLookCus::OnBnClickedSearchcus)
END_MESSAGE_MAP()


// CLookCus 消息处理程序


void CLookCus::OnBnClickedSearchcus()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cusid;
	m_cusid.GetWindowTextW(cusid);
	((CStatic*)GetDlgItem(IDC_CUSBAL))->SetWindowTextW(L"余额：");
	((CStatic*)GetDlgItem(IDC_CUSTIME))->SetWindowTextW(L"注册时间：");
	m_cuslent.ResetContent();
	m_cusbuy.ResetContent();
	if (cusid.IsEmpty())
	{
		MessageBox(L"顾客帐户ID不能为空！", L"查看顾客帐户", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from Customer where cus_id='%s'", cusid);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空
				{
					MessageBox(L"不存在此顾客！", L"查看顾客帐户", MB_OK | MB_ICONWARNING);
					m_pRecordset->Close();
					return;
				}
				else
				{
					CString ttime, str1, str2;
					double bal;
					bal = _ttof(_bstr_t(m_pRecordset->GetCollect("balance")));
					ttime = m_pRecordset->GetCollect("register_date");
					((CStatic*)GetDlgItem(IDC_CUSBAL))->GetWindowTextW(str1);
					str2.Format(L"%lf", bal);
					str2 = str2.Left(str2.Find('.') + 3);
					str1 += str2;
					((CStatic*)GetDlgItem(IDC_CUSBAL))->SetWindowTextW(str1);
					((CStatic*)GetDlgItem(IDC_CUSTIME))->GetWindowTextW(str1);
					str2.Format(L"%s", ttime);
					str1 += str2;
					((CStatic*)GetDlgItem(IDC_CUSTIME))->SetWindowTextW(str1);
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();
		}
		catch (_com_error e)
		{
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}

		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		sqlHasRecord.Format(L"select * from Trade where cus_id='%s'", cusid);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				while (!m_pRecordset->rsEOF)//遍历返回的每一条记录
				{
					CString cdid, type;
					cdid = m_pRecordset->GetCollect("stock_id");
					type = m_pRecordset->GetCollect("trade_type");
					if (_ttoi(type) == 1)
					{
						m_cusbuy.InsertString(m_cusbuy.GetCount(), cdid);
					}
					else
					{
						m_cuslent.InsertString(m_cuslent.GetCount(), cdid);
					}
					m_pRecordset->MoveNext();
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();
		}
		catch (_com_error e)
		{
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}
	}
}


void CLookCus::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedSearchcus();
	//CDialogEx::OnOK();
}
