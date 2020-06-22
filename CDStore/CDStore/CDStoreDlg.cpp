
// CDStoreDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCDStoreDlg �Ի���



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


// CCDStoreDlg ��Ϣ�������

BOOL CCDStoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	((CButton *)GetDlgItem(IDC_CUSTOMER))->SetCheck(TRUE); 
	((CButton *)GetDlgItem(IDC_ADMIN))->SetCheck(FALSE);
	CString temp;
	((CStatic*)GetDlgItem(IDC_TIME))->GetWindowTextW(temp);
	CString strTime;
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	SetDlgItemText(IDC_TIME, temp + strTime);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCDStoreDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCDStoreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCDStoreDlg::OnFinalRelease()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialogEx::OnFinalRelease();
}


void CCDStoreDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(L"ȷ���˳�ϵͳ��", L"Login", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnClose();
	}
}


void CCDStoreDlg::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString idstr, passwordstr;
	m_id.GetWindowTextW(idstr);
	m_password.GetWindowTextW(passwordstr);
	if (idstr.IsEmpty())
	{
		MessageBox(L"�ʻ�ID����Ϊ�գ�", L"Login", MB_OK | MB_ICONWARNING);
	}
	else if (passwordstr.IsEmpty())
	{
		MessageBox(L"���벻��Ϊ�գ�", L"Login", MB_OK | MB_ICONWARNING);
	}
	else 
	{
		UpdateData(true);
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		CString sqlHasRecord;
		if (((CButton *)GetDlgItem(IDC_CUSTOMER))->GetCheck())
		{
			sqlHasRecord.Format(L"select * from Customer where cus_id='%s' and cus_password='%s'", m_idstr, m_passwordstr);
		}
		else
		{
			//int adminID = _ttoi(m_idstr);	//CStringתint
			sqlHasRecord.Format(L"select * from Administrator where admin_id=%d and admin_password='%s'", _ttoi(m_idstr), m_passwordstr);
		}
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ�գ���������ʻ�ID���������û�����κμ�¼ƥ����
				{
					MessageBox(L"�ʻ�ID���������", L"Login", MB_OK | MB_ICONWARNING);
				}
				else
				{
					MessageBox(L"��¼�ɹ���", L"Login", MB_OK | MB_ICONINFORMATION);
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	 ///��ʾ������Ϣ
			return;
		}
	}
}


void CCDStoreDlg::OnBnClickedRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRegisterDlg dlg(m_pConnection);
	dlg.DoModal();
}


void CCDStoreDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedLogin();	//��ֹ�ڱ༭���а��س����Զ��˳�
	//CDialogEx::OnOK();
}


void CCDStoreDlg::OnBnClickedAbout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg dlg;
	dlg.DoModal();
}
