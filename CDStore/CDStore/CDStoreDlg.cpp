
// CDStoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "CDStoreDlg.h"
#include "RegisterDlg.h"
#include "AdminDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCDStoreDlg 对话框



CCDStoreDlg::CCDStoreDlg(_ConnectionPtr p,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CDSTORE_DIALOG, pParent)
	, m_idstr(_T(""))
	, m_passwordstr(_T(""))
{
	m_pConnection = p;
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_SMILE);
}

void CCDStoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID, m_id);
	DDX_Control(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_ID, m_idstr);
	DDX_Text(pDX, IDC_PASSWORD, m_passwordstr);
}

BEGIN_MESSAGE_MAP(CCDStoreDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOGIN, &CCDStoreDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_REGISTER, &CCDStoreDlg::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_ABOUT, &CCDStoreDlg::OnBnClickedAbout)
END_MESSAGE_MAP()


// CCDStoreDlg 消息处理程序

BOOL CCDStoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	((CButton *)GetDlgItem(IDC_CUSTOMER))->SetCheck(TRUE); 
	((CButton *)GetDlgItem(IDC_ADMIN))->SetCheck(FALSE);
	CString temp;
	((CStatic*)GetDlgItem(IDC_TIME))->GetWindowTextW(temp);
	CString strTime;
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	SetDlgItemText(IDC_TIME, temp + strTime);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCDStoreDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCDStoreDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCDStoreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCDStoreDlg::OnFinalRelease()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnFinalRelease();
}


void CCDStoreDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(L"确定退出系统？", L"Login", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnClose();
	}
}


void CCDStoreDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString idstr, passwordstr;
	m_id.GetWindowTextW(idstr);
	m_password.GetWindowTextW(passwordstr);
	if (idstr.IsEmpty())
	{
		MessageBox(L"帐户ID不能为空！", L"Login", MB_OK | MB_ICONWARNING);
	}
	else if (passwordstr.IsEmpty())
	{
		MessageBox(L"密码不能为空！", L"Login", MB_OK | MB_ICONWARNING);
	}
	else 
	{
		UpdateData(true);
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		CString sqlHasRecord;
		if (((CButton *)GetDlgItem(IDC_CUSTOMER))->GetCheck())
		{
			sqlHasRecord.Format(L"select * from Customer where cus_id='%s' and cus_password='%s'", m_idstr, m_passwordstr);
		}
		else
		{
			//int adminID = _ttoi(m_idstr);	//CString转int
			sqlHasRecord.Format(L"select * from Administrator where admin_id=%d and admin_password='%s'", _ttoi(m_idstr), m_passwordstr);
		}
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空，即输入的帐户ID和密码错误，没有与任何记录匹配上
				{
					MessageBox(L"帐户ID或密码错误！", L"Login", MB_OK | MB_ICONWARNING);
				}
				else
				{
					MessageBox(L"登录成功！", L"Login", MB_OK | MB_ICONINFORMATION);
					if (((CButton *)GetDlgItem(IDC_CUSTOMER))->GetCheck())
					{
						CCustomerDlg cusdlg(m_pConnection, m_idstr);
						cusdlg.DoModal();
					}
					else
					{
						CString temp;
						temp = m_pRecordset->GetCollect(L"admin_name");
						CAdminDlg admindlg(m_pConnection, _ttoi(m_idstr), temp);
						admindlg.DoModal();
					}
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
	}
}


void CCDStoreDlg::OnBnClickedRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	CRegisterDlg dlg(m_pConnection);
	dlg.DoModal();
}


void CCDStoreDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedLogin();	//防止在编辑框中按回车键自动退出
	//CDialogEx::OnOK();
}


void CCDStoreDlg::OnBnClickedAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}
