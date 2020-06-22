// ModPassword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "ModPassword.h"
#include "afxdialogex.h"


// CModPassword �Ի���

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


// CModPassword ��Ϣ�������


void CModPassword::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString oldpwd, pwd1, pwd2;
	m_oldpassword.GetWindowTextW(oldpwd);
	m_newpassword.GetWindowTextW(pwd1);
	m_cerpassword.GetWindowTextW(pwd2);
	if (oldpwd.IsEmpty())
	{
		MessageBox(L"�����벻��Ϊ�գ�", L"�޸��ʻ�����", MB_OK | MB_ICONWARNING);
		return;
	}
	else if (pwd1.IsEmpty())
	{
		MessageBox(L"�����벻��Ϊ�գ�", L"�޸��ʻ�����", MB_OK | MB_ICONWARNING);
		return;
	}
	else if (pwd2.IsEmpty())
	{
		MessageBox(L"ȷ�����벻��Ϊ�գ�", L"�޸��ʻ�����", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		if (pwd1 != pwd2)
		{
			MessageBox(L"�����������벻һ�£����������룡", L"�޸��ʻ�����", MB_OK | MB_ICONWARNING);
			return;
		}
		else if (pwd1.GetLength() < 6 || pwd1.GetLength() > 30)
		{
			MessageBox(L"������벻����6���ַ��Ҳ�����30���ַ���", L"�޸��ʻ�����", MB_OK | MB_ICONWARNING);
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
				MessageBox(e.Description(), L"�޸��ʻ�����", MB_OK | MB_ICONWARNING);
				return;
			}  
			CString success;
			success = L"�޸�����ɹ������μ���������Ա��´�ʹ�ã�";
			MessageBox(success, L"�޸��ʻ�����", MB_OK | MB_ICONINFORMATION);
			CDialogEx::OnOK();
		}
	}
	//CDialogEx::OnOK();
}


void CModPassword::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
