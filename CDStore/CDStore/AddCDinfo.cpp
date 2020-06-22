// AddCDinfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "AddCDinfo.h"
#include "afxdialogex.h"


// CAddCDinfo �Ի���

IMPLEMENT_DYNAMIC(CAddCDinfo, CDialogEx)

CAddCDinfo::CAddCDinfo(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDCDINFO, pParent)
{
	m_pConnection = p;
	adminid = id;
	m_cdid = -1;
}

CAddCDinfo::CAddCDinfo(_ConnectionPtr p, int id, int cdid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDCDINFO, pParent)
{
	m_pConnection = p;
	adminid = id;
	m_cdid = cdid;
}

CAddCDinfo::~CAddCDinfo()
{
}

void CAddCDinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDGEN, m_genre);
	DDX_Control(pDX, IDC_ADDCDNAME, m_cdname);
	DDX_Control(pDX, IDC_ADDCDARTIST, m_artist);
}


BEGIN_MESSAGE_MAP(CAddCDinfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddCDinfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddCDinfo ��Ϣ�������


void CAddCDinfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	if (m_cdid == -1)
	{
		CString cdname, artist, genre;
		m_cdname.GetWindowTextW(cdname);
		m_artist.GetWindowTextW(artist);
		m_genre.GetWindowTextW(genre);
		int genreid, insid;
		if (genre == L"δ֪���")
		{
			genreid = 0;
		}
		else
		{
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
			CString sqlHasRecord;
			sqlHasRecord.Format(L"select * from Genre");
			try
			{
				HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
				if (SUCCEEDED(hr))
				{
					while (!m_pRecordset->rsEOF)
					{
						if (m_pRecordset->GetCollect("genre_name") == genre)
						{
							CString str;
							str = m_pRecordset->GetCollect("genre_id");
							genreid = _ttoi(str);
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
				AfxMessageBox(temp1 + L"\r\n" + temp2);
				return;
			}
		}
		
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from CDinfo order by cd_id desc");
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF)
				{
					insid = 1;
				}
				else
				{
					CString str;
					str = m_pRecordset->GetCollect("cd_id");
					insid = _ttoi(str) + 1;
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);
			return;
		}

		CString strSql;
		if (!cdname.IsEmpty() && !artist.IsEmpty())
		{
			strSql.Format(L"insert into CDinfo values(%d,'%s','%s',%d)", insid, cdname, artist, genreid);
		}
		else if (!cdname.IsEmpty() && artist.IsEmpty())
		{
			strSql.Format(L"insert into CDinfo values(%d,'%s',null,%d)", insid, cdname, genreid);
		}
		else if (cdname.IsEmpty() && !artist.IsEmpty())
		{
			strSql.Format(L"insert into CDinfo values(%d,null,'%s',%d)", insid, artist, genreid);
		}
		else
		{
			strSql.Format(L"insert into CDinfo values(%d,null,null,%d)", insid, genreid);
		}

		try
		{
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
		}
		catch (_com_error e)
		{
			MessageBox(e.Description(), L"����CD��Ϣ", MB_OK | MB_ICONWARNING);
			return;
		}
		MessageBox(L"��ӳɹ���", L"����CD��Ϣ", MB_OK | MB_ICONINFORMATION);
		CDialogEx::OnOK();
	}
	else
	{
		CString cdname, artist, genre, strSql;
		m_cdname.GetWindowTextW(cdname);
		m_artist.GetWindowTextW(artist);
		m_genre.GetWindowTextW(genre);
		int genreid;

		if (!genre.IsEmpty())
		{
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
			CString sqlHasRecord;
			sqlHasRecord.Format(L"select * from Genre where genre_name='%s'", genre);
			try
			{
				HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
				if (SUCCEEDED(hr))
				{
					CString str;
					str = m_pRecordset->GetCollect("genre_id");
					genreid = _ttoi(str);
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
				AfxMessageBox(temp1 + L"\r\n" + temp2);
				return;
			}
		}
		
		if (cdname.IsEmpty() && artist.IsEmpty() && genre.IsEmpty())
		{
			MessageBox(L"������Ϣ����ͬʱΪ�գ�", L"�޸�CD��Ϣ", MB_OK | MB_ICONWARNING);
			return;
		}
		else if (cdname.IsEmpty() && artist.IsEmpty() && !genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set genre_id=%d where cd_id=%d", genreid, m_cdid);
		}
		else if (cdname.IsEmpty() && !artist.IsEmpty() && genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set artist_name='%s' where cd_id=%d", artist, m_cdid);
		}
		else if (cdname.IsEmpty() && !artist.IsEmpty() && !genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set artist_name='%s',genre_id=%d where cd_id=%d", artist, genreid, m_cdid);
		}
		else if (!cdname.IsEmpty() && artist.IsEmpty() && genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set cd_name='%s' where cd_id=%d", cdname, m_cdid);
		}
		else if (!cdname.IsEmpty() && artist.IsEmpty() && !genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set cd_name='%s',genre_id=%d where cd_id=%d", cdname, genreid, m_cdid);
		}
		else if (!cdname.IsEmpty() && !artist.IsEmpty() && genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set cd_name='%s',artist_name='%s' where cd_id=%d", cdname, artist, m_cdid);
		}
		else if (!cdname.IsEmpty() && !artist.IsEmpty() && !genre.IsEmpty())
		{
			strSql.Format(L"update CDinfo set cd_name='%s',artist_name='%s',genre_id=%d where cd_id=%d", cdname, artist, genreid, m_cdid);
		}

		try
		{
			m_pConnection->BeginTrans();
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			MessageBox(e.Description(), L"�޸�CD��Ϣ", MB_OK | MB_ICONWARNING);
			return;
		}
		m_pConnection->CommitTrans();
		MessageBox(L"���³ɹ���", L"�޸�CD��Ϣ", MB_OK | MB_ICONINFORMATION);
		CDialogEx::OnOK();
	}
}


void CAddCDinfo::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}


BOOL CAddCDinfo::OnInitDialog()
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
	if (m_cdid == -1)
	{
		m_genre.SetCurSel(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
