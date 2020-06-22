// AddCDinfo.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "AddCDinfo.h"
#include "afxdialogex.h"


// CAddCDinfo 对话框

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


// CAddCDinfo 消息处理程序


void CAddCDinfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	if (m_cdid == -1)
	{
		CString cdname, artist, genre;
		m_cdname.GetWindowTextW(cdname);
		m_artist.GetWindowTextW(artist);
		m_genre.GetWindowTextW(genre);
		int genreid, insid;
		if (genre == L"未知类别")
		{
			genreid = 0;
		}
		else
		{
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
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
		
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
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
			MessageBox(e.Description(), L"增加CD信息", MB_OK | MB_ICONWARNING);
			return;
		}
		MessageBox(L"添加成功！", L"增加CD信息", MB_OK | MB_ICONINFORMATION);
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
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
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
			MessageBox(L"三个信息不能同时为空！", L"修改CD信息", MB_OK | MB_ICONWARNING);
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
			MessageBox(e.Description(), L"修改CD信息", MB_OK | MB_ICONWARNING);
			return;
		}
		m_pConnection->CommitTrans();
		MessageBox(L"更新成功！", L"修改CD信息", MB_OK | MB_ICONINFORMATION);
		CDialogEx::OnOK();
	}
}


void CAddCDinfo::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}


BOOL CAddCDinfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
	CString sqlHasRecord;
	sqlHasRecord = L"select * from Genre";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//遍历返回的每一条记录
			{
				CString str;
				_variant_t vValue = m_pRecordset->GetCollect("genre_name");
				if (vValue.vt != VT_NULL)
				{
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
				}
				else
				{
					str = L"未知类别";
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
				  // 异常: OCX 属性页应返回 FALSE
}
