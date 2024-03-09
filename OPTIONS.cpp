// OPTIONS.cpp: 实现文件
//

#include "pch.h"
#include "MFCHalcon.h"
#include "afxdialogex.h"
#include "OPTIONS.h"
#include "MFCHalconDlg.h"


// OPTIONS 对话框

IMPLEMENT_DYNAMIC(OPTIONS, CDialogEx)

OPTIONS::OPTIONS(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS_DIALOG1, pParent)
{

}

OPTIONS::~OPTIONS()
{
}

void OPTIONS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
}


BEGIN_MESSAGE_MAP(OPTIONS, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &OPTIONS::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &OPTIONS::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &OPTIONS::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &OPTIONS::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &OPTIONS::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &OPTIONS::OnEnChangeEdit5)
END_MESSAGE_MAP()


// OPTIONS 消息处理程序


void OPTIONS::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

//设置爬山参数
void OPTIONS::OnEnChangeEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CMFCHalconDlg* pdlg = CMFCHalconDlg::pMFCHalcondlg;
	CString str1;
	GetDlgItemText(IDC_EDIT1, str1);
	pdlg->motorclimba = _tstoi(str1);
}
void OPTIONS::OnEnChangeEdit2()
{	
	// TODO:  在此添加控件通知处理程序代码
	CMFCHalconDlg* pdlg1 = CMFCHalconDlg::pMFCHalcondlg;
	CString str2;
	GetDlgItemText(IDC_EDIT2, str2);
	pdlg1->motorclimbb = _tstoi(str2);

}
void OPTIONS::OnEnChangeEdit3()
{
	// TODO:  在此添加控件通知处理程序代码
	CMFCHalconDlg* pdlg2 = CMFCHalconDlg::pMFCHalcondlg;
	CString str3;
	GetDlgItemText(IDC_EDIT3, str3);
	pdlg2->motorclimbc = _tstoi(str3);
}
void OPTIONS::OnEnChangeEdit4()
{
	// TODO:  在此添加控件通知处理程序代码
	CMFCHalconDlg* pdlg3 = CMFCHalconDlg::pMFCHalcondlg;
	CString str4;
	GetDlgItemText(IDC_EDIT4, str4);
	pdlg3->delaytime = _tstoi(str4);
}
void OPTIONS::OnEnChangeEdit5()
{
	// TODO:  在此添加控件通知处理程序代码
	CMFCHalconDlg* pdlg4 = CMFCHalconDlg::pMFCHalcondlg;
	CString str5;
	GetDlgItemText(IDC_EDIT5, str5);
	pdlg4->motorresolution = _tstoi(str5);
}
//