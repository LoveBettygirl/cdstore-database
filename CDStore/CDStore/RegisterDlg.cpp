// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(_ConnectionPtr p,CWnd* pParent)
	: CDialogEx(IDD_REGISTER, pParent)
	, m_balancedata(0)
	, m_passwordstr(_T(""))
{
	m_pConnection = p;
}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BALANCE, m_balance);
	DDX_Control(pDX, IDC_PASSWORD1, m_password1);
	DDX_Control(pDX, IDC_PASSWORD2, m_password2);
	DDX_Text(pDX, IDC_BALANCE, m_balancedata);
	DDX_Text(pDX, IDC_PASSWORD1, m_passwordstr);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegisterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRegisterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序


void CRegisterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString pwd1, pwd2, bal;
	m_password1.GetWindowTextW(pwd1);
	m_password2.GetWindowTextW(pwd2);
	m_balance.GetWindowTextW(bal);
	if (pwd1.IsEmpty())
	{
		MessageBox(L"密码不能为空！", L"Register", MB_OK | MB_ICONWARNING);
	}
	else if (pwd2.IsEmpty())
	{
		MessageBox(L"确认密码不能为空！", L"Register", MB_OK | MB_ICONWARNING);
	}
	else if (bal.IsEmpty())
	{
		MessageBox(L"充值余额不能为空！", L"Register", MB_OK | MB_ICONWARNING);
	}
	else
	{
		if (pwd1 != pwd2)
		{
			MessageBox(L"两次密码输入不一致！请重新输入！", L"Register", MB_OK | MB_ICONWARNING);
		}
		else if (pwd1.GetLength() < 6 || pwd1.GetLength() > 30)
		{
			MessageBox(L"密码必须不少于6个字符且不多于30个字符！", L"Register", MB_OK | MB_ICONWARNING);
		}
		else
		{
			UpdateData(true);
			CString strTime, cusid, strSql, sqlHasRecord, temp1;
			CTime tm;
			tm = CTime::GetCurrentTime();
			strTime = tm.Format("%Y%m%d");
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
			sqlHasRecord.Format(L"select * from Customer where cus_id like '%s", strTime);
			temp1 = L"%' order by register_date desc";
			sqlHasRecord += temp1;
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, 
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				//生成新顾客会员ID
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空
				{
					cusid.Format(L"%s_%d", strTime, 1);
				}
				else
				{
					CString temp2;
					temp2 = m_pRecordset->GetCollect(L"cus_id");
					int down = temp2.Find('_');
					temp2 = temp2.Mid(down + 1);
					cusid.Format(L"%s_%d", strTime, _ttoi(temp2) + 1);
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();

			//执行插入语句
			strSql.Format(L"insert into Customer values('%s','%s',%lf,getdate())", 
				cusid, m_passwordstr, m_balancedata);
			try
			{
				m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
			}
			catch (_com_error e)
			{
				MessageBox(e.Description(), L"Register", MB_OK | MB_ICONWARNING);
				return;
			}
			CString success;
			success.Format(L"注册成功！\r\n帐户ID：%s\r\n请牢记你的帐户ID和密码以便下次使用！", cusid);
			MessageBox(success, L"Register", MB_OK | MB_ICONINFORMATION);
			CDialogEx::OnOK();
		}
	}
	//CDialogEx::OnOK();
}


void CRegisterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CRegisterDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_balance.SetWindowTextW(L"");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
