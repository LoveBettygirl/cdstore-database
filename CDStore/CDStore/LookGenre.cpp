// LookGenre.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookGenre.h"
#include "afxdialogex.h"


// CLookGenre �Ի���

IMPLEMENT_DYNAMIC(CLookGenre, CDialogEx)

CLookGenre::CLookGenre(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKGENRE, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookGenre::~CLookGenre()
{
}

void CLookGenre::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GENRELIST, m_genrelist);
	DDX_Control(pDX, IDC_ADDGENRE, m_addgenre);
}


BEGIN_MESSAGE_MAP(CLookGenre, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLookGenre::OnBnClickedOk)
END_MESSAGE_MAP()


// CLookGenre ��Ϣ�������


void CLookGenre::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_addgenre.GetWindowTextW(str);
	if (str.IsEmpty())
	{
		MessageBox(L"���������Ϊ�գ�", L"����CD���", MB_OK | MB_ICONWARNING);
	}
	else if (str == L"δ֪���")
	{
		MessageBox(L"���Ϸ��ķ�������", L"����CD���", MB_OK | MB_ICONWARNING);
	}
	else
	{
		CString sqlHasRecord,strSql;
		int genreid;
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		sqlHasRecord = L"select * from Genre order by genre_id desc";
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
				{
					genreid = 1;
				}
				else
				{
					CString temp;
					temp = m_pRecordset->GetCollect(L"genre_id");
					genreid = _ttoi(temp) + 1;
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
			return;
		}

		strSql.Format(L"insert into Genre values(%d,'%s')", genreid, str);
		try
		{
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
		}
		catch (_com_error e)
		{
			MessageBox(e.Description(), L"����CD���", MB_OK | MB_ICONWARNING);
			return;
		}
		MessageBox(L"��ӳɹ���", L"����CD���", MB_OK | MB_ICONINFORMATION);
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		sqlHasRecord.Format(L"select * from Genre where genre_id=%d", genreid);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (!m_pRecordset->rsEOF) 
				{
					str = m_pRecordset->GetCollect("genre_name");
					m_genrelist.InsertString(m_genrelist.GetCount(), str);
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
			return;
		}
	}
	//CDialogEx::OnOK();
}


void CLookGenre::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}

BOOL CLookGenre::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	sqlHasRecord = L"select * from Genre";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//�������ص�ÿһ����¼
			{
				CString str;
				_variant_t vValue = m_pRecordset->GetCollect("genre_name");
				if (vValue.vt != VT_NULL)
				{
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
				}
				else
				{
					str = L"δ֪���";
				}
				m_genrelist.InsertString(m_genrelist.GetCount(), str);
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}
