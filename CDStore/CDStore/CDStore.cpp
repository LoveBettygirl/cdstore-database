
// CDStore.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CDStore.h"
#include "CDStoreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCDStoreApp

BEGIN_MESSAGE_MAP(CCDStoreApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCDStoreApp ����

CCDStoreApp::CCDStoreApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCDStoreApp ����

CCDStoreApp theApp;


// CCDStoreApp ��ʼ��

BOOL CCDStoreApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//�������ݿ�
	CoInitialize(NULL);   //��ʼ��OLE/COM�⻷��
	HRESULT hr = NULL;
	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");
		if (SUCCEEDED(hr))
		{
			//��ʼ�����Ӵ�
			_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=CDStore;uid=Bettygirl;pwd=betty981022cyy";
			hr = m_pConnection->Open(strConnect, "", "", adModeUnknown);//�����ݿ�
			if (FAILED(hr))
			{
				AfxMessageBox(_T("Open Failed!"));
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox(_T("Create instance of connection failed!"));
			return FALSE;
		}

	}
	catch (_com_error e)
	{
		CString temp1, temp2;
		temp1.Format(_T("�������ݿ�ʧ�ܣ�������Ϣ��%s\r\n"), e.ErrorMessage());
		temp2.Format(e.Description());
		AfxMessageBox(temp1 + temp2);///��ʾ������Ϣ
		return FALSE;
	}

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CCDStoreDlg dlg(m_pConnection);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CCDStoreApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_pConnection->State)
	{
		m_pConnection->Close();
		m_pConnection = NULL;
	}
	CoUninitialize();
	return CWinApp::ExitInstance();
}
