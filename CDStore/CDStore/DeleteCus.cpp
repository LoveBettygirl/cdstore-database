// DeleteCus.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "DeleteCus.h"
#include "afxdialogex.h"


// CDeleteCus 对话框

IMPLEMENT_DYNAMIC(CDeleteCus, CDialogEx)

CDeleteCus::CDeleteCus(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CANCEL, pParent)
{
	m_pConnection = p;
	id = cusid;
}

CDeleteCus::~CDeleteCus()
{
}

void CDeleteCus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALPWD, m_calpwd);
}


BEGIN_MESSAGE_MAP(CDeleteCus, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeleteCus::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDeleteCus::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDeleteCus 消息处理程序


void CDeleteCus::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CString pwd;
	m_calpwd.GetWindowTextW(pwd);
	if (pwd.IsEmpty())
	{
		MessageBox(L"密码不能为空！", L"顾客注销帐户", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from Customer where cus_id='%s' and cus_password='%s'", id, pwd);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true),
				adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空
				{
					MessageBox(L"密码错误！", L"顾客注销帐户", MB_OK | MB_ICONWARNING);
					m_pRecordset->Close();
					return;
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	 ///显示错误信息
			return;
		}

		try
		{
			HRESULT hr = m_pCommand.CreateInstance(__uuidof(Command));
			if (FAILED(hr))
			{
				AfxMessageBox(L"Create instance of command failed!!");
				return;
			}
			m_pCommand->Parameters->Refresh();
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("Return"),
				adInteger, adParamReturnValue, sizeof(int)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("cus_id"),
				adVarChar, adParamInput, 30, _variant_t(id)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("tips"),
				adVarChar, adParamOutput, 100));
			m_pCommand->CommandText = _bstr_t("sp_deletecus");
			m_pCommand->ActiveConnection = m_pConnection;
			m_pCommand->CommandType = adCmdStoredProc;
			m_pConnection->BeginTrans();
			m_pCommand->Execute(NULL, NULL, adCmdStoredProc); //执行存储过程
			int nRet = m_pCommand->Parameters->GetItem("Return")->GetValue();
			CString tips = m_pCommand->Parameters->GetItem("tips")->GetValue();
			if (nRet == 0)	 //不满足删除条件
			{
				m_pConnection->RollbackTrans();
				MessageBox(tips, L"顾客注销帐户", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 1)	 //满足删除条件
			{
				m_pConnection->CommitTrans();
				MessageBox(tips, L"顾客注销帐户", MB_OK | MB_ICONINFORMATION);
				CDialogEx::OnOK();
			}
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}
	}
}


void CDeleteCus::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(L"取消注销帐户？注销的请求将会被取消！", L"顾客注销帐户", MB_YESNO | MB_ICONINFORMATION)==IDYES)
	{
		CDialogEx::OnCancel();
	}
}


void CDeleteCus::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
