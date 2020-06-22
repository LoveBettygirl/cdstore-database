// ModPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "ModPassword.h"
#include "afxdialogex.h"


// CModPassword 对话框

IMPLEMENT_DYNAMIC(CModPassword, CDialogEx)

CModPassword::CModPassword(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODPASSWORD, pParent)
	, m_oldpasswordstr(_T(""))
	, m_newpasswordstr(_T(""))
	, m_cerpasswordstr(_T(""))
{
	m_pConnection = p;
	m_cusid = cusid;
	m_adminid = -1;
}

CModPassword::CModPassword(_ConnectionPtr p, int adminid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODPASSWORD, pParent)
{
	m_pConnection = p;
	m_adminid = adminid;
	m_cusid = L"";
}

CModPassword::~CModPassword()
{
}

void CModPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OLDPASSWORD, m_oldpassword);
	DDX_Control(pDX, IDC_NEWPASSWORD, m_newpassword);
	DDX_Control(pDX, IDC_CERPASSWORD, m_cerpassword);
	DDX_Text(pDX, IDC_OLDPASSWORD, m_oldpasswordstr);
	DDX_Text(pDX, IDC_NEWPASSWORD, m_newpasswordstr);
	DDX_Text(pDX, IDC_CERPASSWORD, m_cerpasswordstr);
}


BEGIN_MESSAGE_MAP(CModPassword, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// CModPassword 消息处理程序


void CModPassword::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString oldpwd, pwd1, pwd2;
	m_oldpassword.GetWindowTextW(oldpwd);
	m_newpassword.GetWindowTextW(pwd1);
	m_cerpassword.GetWindowTextW(pwd2);
	if (oldpwd.IsEmpty())
	{
		MessageBox(L"旧密码不能为空！", L"修改帐户密码", MB_OK | MB_ICONWARNING);
		return;
	}
	else if (pwd1.IsEmpty())
	{
		MessageBox(L"新密码不能为空！", L"修改帐户密码", MB_OK | MB_ICONWARNING);
		return;
	}
	else if (pwd2.IsEmpty())
	{
		MessageBox(L"确认密码不能为空！", L"修改帐户密码", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		if (pwd1 != pwd2)
		{
			MessageBox(L"两次密码输入不一致！请重新输入！", L"修改帐户密码", MB_OK | MB_ICONWARNING);
			return;
		}
		else if (pwd1.GetLength() < 6 || pwd1.GetLength() > 30)
		{
			MessageBox(L"密码必须不少于6个字符且不多于30个字符！", L"修改帐户密码", MB_OK | MB_ICONWARNING);
			return;
		}
		else
		{
			UpdateData(true);
			CString strSql;
			if (m_adminid == -1)
			{
				strSql.Format(L"exec sp_modifycuspassword @cus_id='%s',@oldpassword='%s',@newpassword='%s'",
					m_cusid, m_oldpasswordstr, m_newpasswordstr);
			}
			else
			{
				strSql.Format(L"exec sp_modifyadminpassword @admin_id=%d,@oldpassword='%s',@newpassword='%s'",
					m_adminid, m_oldpasswordstr, m_newpasswordstr);
			}
			try
			{
				m_pConnection->BeginTrans();
				m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
				m_pConnection->CommitTrans();
			}
			catch (_com_error e)
			{
				m_pConnection->RollbackTrans();
				MessageBox(e.Description(), L"修改帐户密码", MB_OK | MB_ICONWARNING);
				return;
			}  
			CString success;
			success = L"修改密码成功！请牢记你的密码以便下次使用！";
			MessageBox(success, L"修改帐户密码", MB_OK | MB_ICONINFORMATION);
			CDialogEx::OnOK();
		}
	}
	//CDialogEx::OnOK();
}


void CModPassword::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
