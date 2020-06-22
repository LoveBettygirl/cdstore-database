// LookCD.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookCD.h"
#include "afxdialogex.h"


// CLookCD �Ի���

IMPLEMENT_DYNAMIC(CLookCD, CDialogEx)

CLookCD::CLookCD(_ConnectionPtr p, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKCD, pParent)
{
	m_pConnection = p;
}

CLookCD::~CLookCD()
{
}

void CLookCD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CDNAME, m_cdname);
	DDX_Control(pDX, IDC_ARTIST, m_artistname);
	DDX_Control(pDX, IDC_GENRE, m_genre);
	DDX_Control(pDX, IDC_RESULTLIST, m_resultlist);
}


BEGIN_MESSAGE_MAP(CLookCD, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCH, &CLookCD::OnBnClickedSearch)
END_MESSAGE_MAP()


// CLookCD ��Ϣ�������


void CLookCD::OnBnClickedSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_resultlist.ResetContent(); //����ϴβ�ѯ�Ľ��
	CString cdname, artist, genre, genre2;
	m_cdname.GetWindowTextW(cdname);
	m_artistname.GetWindowTextW(artist);
	int judge;
	m_genre.GetWindowTextW(genre);
	judge = m_genre.FindString(0, genre);
	if (judge >= 0)
	{
		m_genre.GetLBText(judge, genre2);
	}
	if (!genre.IsEmpty()&&genre!=genre2)
	{
		MessageBox(L"û�����CD���", L"�鿴����CD", MB_OK | MB_ICONWARNING);
		return;
	}
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	if (!cdname.IsEmpty() && !artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD��]='%s' and [������]='%s' and [���]='%s'",
				cdname, artist, genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD��]='%s' and [������]='%s' and [���] "
				"is null", cdname, artist);
		}
	}
	else if (!cdname.IsEmpty() && !artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist where [CD��]='%s' and [������]='%s'", cdname, artist);
	}
	else if (!cdname.IsEmpty() && artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD��]='%s' and [���]='%s'", cdname, genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD��]='%s' and [���] is null", cdname);
		}
	}
	else if (!cdname.IsEmpty() && artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist where [CD��]='%s'", cdname);
	}
	else if (cdname.IsEmpty() && !artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [������]='%s' and [���]='%s'", artist, genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [������]='%s' and [���] is null", artist);
		}
	}
	else if (cdname.IsEmpty() && !artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist where [������]='%s'", artist);
	}
	else if (cdname.IsEmpty() && artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [���]='%s'", genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [���] is null");
		}
	}
	else if (cdname.IsEmpty() && artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist");
	}
	sqlHasRecord += L" order by [���ʱ��]";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, 
			_variant_t((IDispatch*)m_pConnection, true), 
			adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ�գ�û�����κμ�¼ƥ����
			{
				MessageBox(L"���û����Ӧ�Ŀ���CD��", L"�鿴����CD", MB_OK | MB_ICONWARNING);
			}
			else
			{
				while (!m_pRecordset->rsEOF)//�������ص�ÿһ����¼
				{
					CString str, all;
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("CD��"));
					all = all + str + L"��";
					_variant_t vValue = m_pRecordset->GetCollect("CD��");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("CD��"));
					}
					else
					{
						str = L"δ֪";
					}
					all = all + str + L"��";
					vValue = m_pRecordset->GetCollect("������");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("������"));
					}
					else
					{
						str = L"����";
					}
					all = all + str + L"��";
					vValue = m_pRecordset->GetCollect("���");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("���"));
					}
					else
					{
						str = L"δ֪���";
					}
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("�ۼ�"));
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("���ü۸�"));
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("���ô���"));
					all = all + str;
					m_resultlist.InsertString(m_resultlist.GetCount(), all);
					m_pRecordset->MoveNext();
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
		AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
		return;
	}
}


BOOL CLookCD::OnInitDialog()
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
				m_genre.InsertString(m_genre.GetCount(), str);
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


void CLookCD::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedSearch();
	//CDialogEx::OnOK();
}

