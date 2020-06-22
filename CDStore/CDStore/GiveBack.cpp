// GiveBack.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "GiveBack.h"
#include "afxdialogex.h"


// CGiveBack 对话框

IMPLEMENT_DYNAMIC(CGiveBack, CDialogEx)

CGiveBack::CGiveBack(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GIVEBACK, pParent)
{
	m_pConnection = p;
	id = cusid;
}

CGiveBack::~CGiveBack()
{
}

void CGiveBack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GIVEBACK, m_givebackcd);
}


BEGIN_MESSAGE_MAP(CGiveBack, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGiveBack::OnBnClickedOk)
END_MESSAGE_MAP()


// CGiveBack 消息处理程序


void CGiveBack::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cdid;
	m_givebackcd.GetWindowTextW(cdid);
	if (cdid.IsEmpty())
	{
		MessageBox(L"CD号不能为空！", L"归还CD", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		CString strSql;

		//执行存储过程
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
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("cd_id"), 
				adVarChar, adParamInput, 30, _variant_t(cdid)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("tips"), 
				adVarChar, adParamOutput, 100));
			m_pCommand->CommandText = _bstr_t("sp_giveback");
			m_pCommand->ActiveConnection = m_pConnection;
			m_pCommand->CommandType = adCmdStoredProc;
			m_pCommand->Execute(NULL, NULL, adCmdStoredProc);
			int nRet = m_pCommand->Parameters->GetItem("Return")->GetValue();
			CString tips= m_pCommand->Parameters->GetItem("tips")->GetValue();
			if (nRet == 0)
			{
				MessageBox(tips, L"归还CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else
			{
				MessageBox(tips, L"归还CD", MB_OK | MB_ICONINFORMATION);
			}
		}
		catch (_com_error e)
		{
			MessageBox(e.Description(), L"归还CD", MB_OK | MB_ICONWARNING);
			return;
		}
		CDialogEx::OnOK();
	}
	//CDialogEx::OnOK();
}


void CGiveBack::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
