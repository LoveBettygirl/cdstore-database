// AdminDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "AdminDlg.h"
#include "AddStock.h"
#include "LookCus.h"
#include "ModPassword.h"
#include "LookCDinfo.h"
#include "LookGenre.h"
#include "LookSup.h"
#include "LookStock.h"
#include "afxdialogex.h"


// CAdminDlg �Ի���

IMPLEMENT_DYNAMIC(CAdminDlg, CDialogEx)

CAdminDlg::CAdminDlg(_ConnectionPtr p, int id, CString name, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADMINDLG, pParent)
{
	m_pConnection = p;
	m_id = id;
	m_name = name;
}

CAdminDlg::~CAdminDlg()
{
}

void CAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GREETING2, m_greeting);
}


BEGIN_MESSAGE_MAP(CAdminDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAdminDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAdminDlg ��Ϣ�������


void CAdminDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton *)GetDlgItem(IDC_ADDSTOCK))->GetCheck())
	{
		CAddStock dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOOKCUSTOMER))->GetCheck())
	{
		CLookCus dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOGOUT2))->GetCheck())
	{
		if (MessageBox(L"ȷ���˳���¼��", L"����Աѡ����", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			CDialogEx::OnOK();
		}
	}
	else if (((CButton *)GetDlgItem(IDC_LOOKSUPPLIER))->GetCheck())
	{
		CLookSup dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOOKSTOCK))->GetCheck())
	{
		CLookStock dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOOKCDINFO))->GetCheck())
	{								   
		CLookCDinfo dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOOKGENRE))->GetCheck())
	{
		CLookGenre dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_MODADMINPASSWORD))->GetCheck())
	{
		CModPassword dlg(m_pConnection, m_id);
		dlg.DoModal();
	}
	//CDialogEx::OnOK();
}


BOOL CAdminDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString temp1;
	temp1.Format(L"��ã�%s����ѡ��һ�����ܣ�", m_name);
	m_greeting.SetWindowTextW(temp1);
	((CButton *)GetDlgItem(IDC_LOOKCUSTOMER))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CAdminDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(L"ȷ���˳���¼��", L"����Աѡ����", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnClose();
	}
}
