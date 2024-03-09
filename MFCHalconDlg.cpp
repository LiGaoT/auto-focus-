
// MFCHalconDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCHalcon.h"
#include "MFCHalconDlg.h"
#include "afxdialogex.h"
#include "pzt.h"
#include "OPTIONS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCHalconDlg 对话框
using namespace HalconCpp;

CMFCHalconDlg::CMFCHalconDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCHALCON_DIALOG, pParent)
	, const_dis(0.0)
	, const_vol(0.0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCHalconDlg::DoDataExchange(CDataExchange* pDX)
{

}

BEGIN_MESSAGE_MAP(CMFCHalconDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN() // 左击消息映射表
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_OPENPIC_BUTTON, &CMFCHalconDlg::OnBnClickedclimb)
	ON_BN_CLICKED(IDC_COM, &CMFCHalconDlg::OnBnClickedCom)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_U, &CMFCHalconDlg::OnBnClickedU)
	ON_BN_CLICKED(IDC_LOCALIMG, &CMFCHalconDlg::OnBnClickedLocalimg)
	ON_BN_CLICKED(IDC_L, &CMFCHalconDlg::OnBnClickedL)
	ON_BN_CLICKED(IDC_D, &CMFCHalconDlg::OnBnClickedD)
	ON_BN_CLICKED(IDC_R, &CMFCHalconDlg::OnBnClickedR)
	ON_BN_CLICKED(IDC_EDIT_inputvol, &CMFCHalconDlg::OnBnClickedEditinputvol)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_objective4, &CMFCHalconDlg::OnBnClickedobjective4)
	ON_BN_CLICKED(IDC_objective10, &CMFCHalconDlg::OnBnClickedobjective10)
	ON_BN_CLICKED(IDC_Objective40, &CMFCHalconDlg::OnBnClickedObjective40)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_Test, &CMFCHalconDlg::OnBnClickedTest)
	ON_COMMAND(1, &CMFCHalconDlg::OnOptions)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMFCHalconDlg 消息处理程序

BOOL CMFCHalconDlg::OnInitDialog()//初始化
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("苏州大学 | 苏州迈客荣有限公司"));
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);

	/* 设置字体 */
	CFont* f;
	f = new CFont;
	f->CreateFont(
	24,								// nHeight
	0,								// nWidth
	0,								// nEscapement
	0,								// nOrientation
	800,							// nWeight
	FALSE,							// bItalic
	FALSE,							// bUnderline
	0,								// cStrikeOut
	ANSI_CHARSET,					// nCharSet
	OUT_DEFAULT_PRECIS,				// nOutPrecision
	CLIP_TT_ALWAYS,					// nClipPrecision
	PROOF_QUALITY,					// nQuality
	DEFAULT_PITCH | FF_SWISS,		// nPitchAndFamily
	_T("Microsoft YaHei"));			// lpszFac

	/* 按钮 */
	GetDlgItem(IDC_COM)->SetFont(f);
	GetDlgItem(IDC_LOCALIMG)->SetFont(f);
	GetDlgItem(IDC_OPENPIC_BUTTON)->SetFont(f);
	GetDlgItem(IDC_U)->SetFont(f);
	GetDlgItem(IDC_D)->SetFont(f);
	GetDlgItem(IDC_L)->SetFont(f);
	GetDlgItem(IDC_R)->SetFont(f);
	GetDlgItem(IDC_LOCALTEXT_STATIC)->SetFont(f);
	GetDlgItem(IDC_MOVEDIS_STATIC)->SetFont(f);
	GetDlgItem(IDC_EDIT_inputvol)->SetFont(f);
	GetDlgItem(IDC_objective4)->SetFont(f);
	GetDlgItem(IDC_objective10)->SetFont(f);
	GetDlgItem(IDC_Objective40)->SetFont(f);
	GetDlgItem(IDC_Test)->SetFont(f); 

	// TODO: 在此添加额外的初始化代码
	//菜单初始化
	CMenu menu;
	menu.LoadMenu(IDR_MENU1); //IDR_MENU1为菜单栏ID号
	SetMenu(&menu);

	pMFCHalcondlg = this;

	//初始化串口、初始化操作
	opencom("COM3");
	//uint8_t data[6] = { 0xff , 'R' , 65000 / 256 , 'N' ,65000 % 256, 0xfe };
	//m_SerialPort.writeData(data, 6);
	CreateImageWindows();
	StartCapture();

	//从相机中获取图像
	GrabImage(&ho_Image, hv_AcqHandle);
	// 获取图像大小
	HTuple hv_Width, hv_Height;
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	imageWidth = hv_Width.I();
	imageHeight = hv_Height.I();
	SetPart(hWndID, 0, 0, hv_Height, hv_Width);
	HomMat2dIdentity(&hv_HomMat2DIdentity);
	HomMat2dScale(hv_HomMat2DIdentity, m_ZoomFactor, m_ZoomFactor, hv_Width / 2, hv_Height / 2, &hv_HomMat2DScale);
	AffineTransImage(ho_Image, &ho_ImageAffinTrans, hv_HomMat2DScale, "constant", "false");
	DispObj(ho_ImageAffinTrans, hWndID);
	CGdi::init();
	SetTimer(0, 20, NULL);

#ifndef _WIN64
	SkinH_Attach();
#endif 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
pztCtrol mypzt;
CMFCHalconDlg* CMFCHalconDlg::pMFCHalcondlg = NULL;
//创建图像显示窗口
void CMFCHalconDlg::CreateImageWindows() {
	CRect Picturebox_Regions;   // CRect矩形类
	HTuple hv_Exception;

	HWND hPictureBox_Regions = GetDlgItem(IDC_PIC)->m_hWnd;
	GetDlgItem(IDC_PIC)->GetClientRect(&Picturebox_Regions);

	try {
		OpenWindow(Picturebox_Regions.left, Picturebox_Regions.top, Picturebox_Regions.Width(),
		Picturebox_Regions.Height(), (Hlong)hPictureBox_Regions, "visible", "", &hWndID);
	}
	catch (HalconCpp::HException& HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		throw HalconCpp::HException(hv_Exception);
	}
}
//

//开启停止摄像头采集
void CMFCHalconDlg::StartCapture() {
	try {
		OpenFramegrabber(
			/*"DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false","default", "[1]  Mshot", 0, -1, &hv_AcqHandle*/
			"DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false", "default", "[0] XiaoMi USB 2.0 Webcam", 0, -1, &hv_AcqHandle
		);
	}
	catch (HalconCpp::HException& e) {
		// 处理异常
		e.ErrorMessage();
	}
}
void CMFCHalconDlg::StopCapture() {
	try {
		CloseFramegrabber(hv_AcqHandle);
	}
	catch (HalconCpp::HException& e) {
		// 处理异常
		e.ErrorMessage();
	}
}
//

//DAC串口发送
void CMFCHalconDlg::WriteDAC(int value){
	uint8_t sendBuf[6];
	sendBuf[0] = 0xff;			//起始表示符
	sendBuf[1] = 'A';
	sendBuf[2] = 'B';
	sendBuf[3] = value / 256;
	sendBuf[4] = value % 256;
	sendBuf[5] = 0xfe;			//结束表示符
	m_SerialPort.writeData(sendBuf, 6);
}
//

void CMFCHalconDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CMFCHalconDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//小地图坐标获取
CPoint CMFCHalconDlg::getpt(CWnd* org, CWnd* des, CPoint pt)
{
	CPoint p = CPoint(-1, -1);
	if (org != NULL && des != NULL)
	{
		org->ClientToScreen(&pt);  //相对屏幕坐标
		des->ScreenToClient(&pt);  //相对控件坐标
		p = pt;
	}
	return p;
}
CPoint CMFCHalconDlg::getxy(CWnd* org, CPoint pt)
{
	CPoint p = CPoint(0, 0);
	if (org != NULL)
	{
		CRect rc;
		org->GetClientRect(rc);
		p.x = pt.x - rc.CenterPoint().x; //相对控件中心坐标
		p.y = rc.Height() / 2 - pt.y;
	}
	return p;
}
CRect CMFCHalconDlg::getctlrc(CWnd* ctl)
{
	CRect rect;
	if (ctl != NULL)
	{
		ctl->GetClientRect(rect);
		ctl->ClientToScreen(rect);
		ScreenToClient(rect); //控件位置
	}
	return rect;
}
CRect CMFCHalconDlg::getrc(CPoint pt, int w, int h)
{
	CRect rc;
	rc.left = pt.x - w / 2;
	rc.right = pt.x + w / 2;
	rc.top = pt.y - h / 2;
	rc.bottom = pt.y + h / 2;
	return rc;
}
//

//MINPIC控件和PIC控件点击控制电机运动
void CMFCHalconDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(TRUE);

	KillTimer(0);

	CPoint pt1 = getpt(this, GetDlgItem(IDC_MINPIC), point); //相对控件坐标
	CPoint pt = getxy(GetDlgItem(IDC_MINPIC), pt1); //相对控件中心坐标，计算xy的坐标差值

	//判断鼠标是否在MINPIC控件内
	if(pt.x >= -240 && pt.x <= 240 && pt.y >= -169 && pt.y <= 169)
	{ 
		rcClk = getrc(pt1, rcWidth, rcHeight);		//点击矩形

		Invalidate();		//刷新绘制 OnCtlColor

		int Motor_x_dis = 0, Motor_y_dis = 0;
		int ADIS = 0;
		int BDIS = 0;

		//计算电机需要走的位移
		Motor_x_dis = pt.x - Motor_nowpos_x;
		Motor_y_dis = pt.y - Motor_nowpos_y;

		//给电机的位移取整
		ADIS = abs(Motor_y_dis) * distance_para_x;
		BDIS = abs(Motor_x_dis) * distance_para_y;

		//更新电机位置
		Motor_nowpos_x = pt.x;
		Motor_nowpos_y = pt.y;

		if (m_SerialPort.isOpen())
		{
			if (Motor_x_dis >= 0 && Motor_y_dis >= 0){
				uint8_t data[6] = { 0xff , 'D' , ADIS , 'L' , BDIS , 0xfe };
				m_SerialPort.writeData(data, 6);
			}
			else if (Motor_x_dis < 0 && Motor_y_dis >= 0){
				uint8_t data[6] = { 0xff , 'D' , ADIS , 'R' , BDIS , 0xfe };
				m_SerialPort.writeData(data, 6);
			}
			else if (Motor_x_dis < 0 && Motor_y_dis < 0){
				uint8_t data[6] = { 0xff , 'U' , ADIS , 'R' , BDIS , 0xfe };
				m_SerialPort.writeData(data, 6);
			}
			else{
				uint8_t data[6] = { 0xff , 'U' , ADIS , 'L' , BDIS , 0xfe };
				m_SerialPort.writeData(data, 6);
			}
		}
		else
		{
			AfxMessageBox(_T("串口未打开"));
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
	SetTimer(0, 20, NULL);
}
//

//PIC控件控制电机运动和自动对焦
BOOL CMFCHalconDlg::PreTranslateMessage(MSG* pMsg) {
	// TODO: 在此添加专用代码和/或调用基类

	KillTimer(0);

	CPoint point;

	int mx = 0;
	int my = 0;

	if (KEY_DOWN(VK_LBUTTON)) {

		SetCursor(LoadCursor(NULL, IDC_HAND));

		GetCursorPos(&point);

		CRect src;
		GetDlgItem(IDC_PIC)->GetClientRect(src);
		GetDlgItem(IDC_PIC)->ClientToScreen(src);


		if (src.PtInRect(point) && imageWidth > 0) {  //判断鼠标点击位置是否的IDC_PIC控件内

			mx = point.x - src.left;
			my = point.y - src.top;

			int Motor_x_dis = 0, Motor_y_dis = 0,
				ADIS = 0, BDIS = 0;

			CPoint center = src.CenterPoint();//得到IDC_PIC控件相对于Client的中心点坐标

			int dx = point.x - center.x;
			int dy = -(point.y - center.y);

			//计算电机的移动像素距离
			Motor_x_dis = dx - Nowpos_x;
			Motor_y_dis = dy - Nowpos_y;


			//给电机的位移取整
			ADIS = abs(Motor_y_dis) * Distance_para_x;
			BDIS = abs(Motor_x_dis) * Distance_para_y;

#if 1       // "PIC" 控件电机运动
			if (m_SerialPort.isOpen())
			{
				if (Motor_x_dis >= 0 && Motor_y_dis >= 0) {
					uint8_t data[6] = { 0xff , 'D' , ADIS , 'L' , BDIS , 0xfe };
					m_SerialPort.writeData(data, 6);
				}
				else if (Motor_x_dis < 0 && Motor_y_dis >= 0) {
					uint8_t data[6] = { 0xff , 'D' , ADIS , 'R' , BDIS , 0xfe };
					m_SerialPort.writeData(data, 6);
				}
				else if (Motor_x_dis < 0 && Motor_y_dis < 0) {
					uint8_t data[6] = { 0xff , 'U' , ADIS , 'R' , BDIS , 0xfe };
					m_SerialPort.writeData(data, 6);
				}
				else {
					uint8_t data[6] = { 0xff , 'U' , ADIS , 'L' , BDIS , 0xfe };
					m_SerialPort.writeData(data, 6);
				}
			}
			else
			{
				AfxMessageBox(_T("串口未打开"));
			}
#endif

			rcClk.left += dx * 0.021;
			rcClk.right += dx * 0.021;
			rcClk.top += -dy * 0.032;
			rcClk.bottom += -dy * 0.032;
			Invalidate();								//刷新绘制 OnCtlColor

		}
	}

	if (KEY_DOWN(VK_RBUTTON)) {

		/* 对焦测试 */
		//压电对焦爬山
		//pzthillclimb(15, 5, 1, 0.2, 80); //单位：V，MS
		//电机对焦爬山
		hillclimb(100, 50, 5, 10);	//单位：脉冲个数，MS
		//double x;
		//int step;
		//double fx;
		//std::tie(step, fx) = local_min(0.0, 120, 1.0, x);
		//Sleep(2);
		//WriteDAC(vol2ad(x));
	}
	delayMs(2);
	SetTimer(0, 20, NULL);

	return CDialogEx::PreTranslateMessage(pMsg);
}
//
//不懂
void CMFCHalconDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	CWnd* pwnd = GetDlgItem(IDC_MINPIC);
	if (pwnd != NULL)
	{
		rc = getctlrc(pwnd);
		if (rc.PtInRect(point))
		{
			CPoint pt = getpt(this, pwnd, point);  //相对控件坐标
			pt = getxy(pwnd, pt); //相对控件中心坐标
			//CString cs;
			//cs.Format(_T("%d,%d"), pt.x, pt.y);
			//GetDlgItem(IDC_STATIC1)->SetWindowText(cs);
		}
		else
		{
			//GetDlgItem(IDC_STATIC1)->SetWindowText(_T(""));
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
//

//绘制点击矩形
HBRUSH CMFCHalconDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT id = pWnd->GetDlgCtrlID();
	CRect rc;
	pWnd->GetClientRect(rc);
	if (id == IDC_MINPIC)
	{
		CGdi::drawimg(pDC, bkimg, rc); //图片绘制

		if (!rcClk.IsRectEmpty())  //点击矩形绘制
		{
			pDC->MoveTo(rcClk.left, rcClk.top);
			pDC->LineTo(rcClk.right, rcClk.top);

			pDC->MoveTo(rcClk.right, rcClk.top);
			pDC->LineTo(rcClk.right, rcClk.bottom);

			pDC->MoveTo(rcClk.right, rcClk.bottom);
			pDC->LineTo(rcClk.left, rcClk.bottom);

			pDC->MoveTo(rcClk.left, rcClk.bottom);
			pDC->LineTo(rcClk.left, rcClk.top);
		}
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
//

//串口相关
void CMFCHalconDlg::opencom(char *com) {
	const char* portName = com; //电机串口号，注意修改
	int SelBaudRate = 9600;
	int SelParity = 0;
	int SelDataBits = 8;
	int SelStop = 1;

	UpdateData(true);

	m_SerialPort.init(portName, SelBaudRate, itas109::Parity(SelParity), itas109::DataBits(SelDataBits), itas109::StopBits(SelStop));

	if (!m_SerialPort.isOpen())
	{
		m_SerialPort.open();
		GetDlgItem(IDC_COM)->SetWindowText(_T("关闭串口"));
	}
	else
	{
		m_SerialPort.close();
		GetDlgItem(IDC_COM)->SetWindowText(_T("打开串口"));
	}
}
void CMFCHalconDlg::OnBnClickedCom()
{
	// TODO: 在此添加控件通知处理程序代码

	//使用CSerial库控制电机串口
	//打开串口操作
	const char* portName = "COM3"; //电机串口号，注意修改
	int SelBaudRate = 9600;
	int SelParity = 0;
	int SelDataBits = 8;
	int SelStop = 1;

	UpdateData(true);

	m_SerialPort.init(portName, SelBaudRate, itas109::Parity(SelParity), itas109::DataBits(SelDataBits), itas109::StopBits(SelStop));
	
	if (!m_SerialPort.isOpen())
	{
		m_SerialPort.open();
		GetDlgItem(IDC_COM)->SetWindowText(_T("关闭串口"));
	}
	else
	{
		m_SerialPort.close();
		GetDlgItem(IDC_COM)->SetWindowText(_T("打开串口"));
	}
}
//

//相机定时器
void CMFCHalconDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 0:
		GrabImage(&ho_Image, hv_AcqHandle);
		GetImageSize(ho_Image, &hW, &hH); //获取图片大小
		SetPart(hWndID, 0, 0, hH, hW);    //设置显示的范围
		HomMat2dIdentity(&hv_HomMat2DIdentity);
		HomMat2dScale(hv_HomMat2DIdentity, m_ZoomFactor, m_ZoomFactor, hH / 2, hW / 2, &hv_HomMat2DScale);
		AffineTransImage(ho_Image, &ho_ImageAffinTrans, hv_HomMat2DScale, "constant", "false");
		DispObj(ho_ImageAffinTrans, hWndID);        //显示图片
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
//

//加载当前图像到MINPIC
void CMFCHalconDlg::OnBnClickedLocalimg()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(0);//关闭计时器

	HTuple hv_FilePath = "./";
	HTuple hv_Filename = hv_FilePath + "Snap shot.bmp";

	GrabImage(&ho_Image, hv_AcqHandle);
	WriteImage(ho_Image, "bmp", 0, hv_Filename);

	// 字符集不同处理方式不同
	CStringA cmsgA = hv_Filename.S();
	CString cmsg(cmsgA);

	bkimg = CGdi::Loadimg(cmsg); //加载图片
	GetDlgItem(IDC_MINPIC)->Invalidate();
}
//

//没用但是不能没有
void CMFCHalconDlg::OnBnClickedEditinputvol()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}
//

//方向键控制载物台
void CMFCHalconDlg::OnBnClickedU()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	KillTimer(0);
	int dx = 50 * 1;
	/* 调整小窗位置  左右方向键 */
	rcClk.top -= dx * 2;
	rcClk.bottom -= dx * 2;

	if (m_SerialPort.isOpen()){
		uint8_t data[6] = { 0xff , 'U' , motorresolution , 'N' , 00 , 0xfe };
		m_SerialPort.writeData(data, 6);
	}
	else {
		AfxMessageBox(_T("串口未打开"));
	}
	Invalidate();		//刷新绘制 OnCtlColor
	SetTimer(0, 20, NULL);
}
void CMFCHalconDlg::OnBnClickedD()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	KillTimer(0);
	int dx = 50 * 1;
	/* 调整小窗位置  左右方向键 */
	rcClk.top += dx * 2;
	rcClk.bottom += dx * 2;
	if (m_SerialPort.isOpen()) {
		uint8_t data[6] = { 0xff , 'D' , motorresolution , 'N' , 00 , 0xfe };
		m_SerialPort.writeData(data, 6);
	}
	else {
		AfxMessageBox(_T("串口未打开"));
	}
	Invalidate();		//刷新绘制 OnCtlColor
	SetTimer(0, 20, NULL);//开启计时器，功能为每20ms刷新一次窗口
}
void CMFCHalconDlg::OnBnClickedL()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	KillTimer(0);
	int dx = 50 * 1;
	/* 调整小窗位置  左右方向键 */
	rcClk.left -= dx * 2;
	rcClk.right -= dx * 2;
	if (m_SerialPort.isOpen()) {
		uint8_t data[6] = { 0xff , 'N' , 00 , 'L' , motorresolution , 0xfe };
		m_SerialPort.writeData(data, 6);
	}
	else {
		AfxMessageBox(_T("串口未打开"));
	}
	Invalidate();		//刷新绘制 OnCtlColor
	SetTimer(0, 20, NULL);
} 
void CMFCHalconDlg::OnBnClickedR()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	KillTimer(0);
	int dx = 1 * 50;
	/* 调整小窗位置  左右方向键 */
	rcClk.left += dx * 2;
	rcClk.right += dx * 2;
	if (m_SerialPort.isOpen()) {
		uint8_t data[6] = { 0xff , 'N' , 00 , 'R' , motorresolution , 0xfe };
		m_SerialPort.writeData(data, 6);
	}
	else {
		AfxMessageBox(_T("串口未打开"));
	}
	Invalidate();		//刷新绘制 OnCtlColor
	SetTimer(0, 20, NULL);
}
//

//切换物镜操作
void CMFCHalconDlg::OnBnClickedobjective4()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8_t data[6] = { 0xff , 'O' , 1 , 'O' , 1 , 0xfe };
	if (m_SerialPort.isOpen()){
		m_SerialPort.writeData(data, 6);
	} 
	else{
		AfxMessageBox(_T("串口未打开"));
	}
	Distance_para_x = 0.015;//大地图位移增益
	Distance_para_y = 0.015;
	rcWidth = 126;
	rcHeight = 108;
}
void CMFCHalconDlg::OnBnClickedobjective10()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8_t data[6] = { 0xff , 'O' , 2 , 'O' , 2 , 0xfe };
	if (m_SerialPort.isOpen()){
		m_SerialPort.writeData(data, 6);
	}
	else{
		AfxMessageBox(_T("串口未打开"));
	}
	rcWidth = 96;
	rcHeight = 78;
	Distance_para_x = 0.015;//大地图位移增益
	Distance_para_y = 0.015;
	distance_para_x = 0.6;//小地图位移增益
	distance_para_y = 0.3;

	Invalidate();
}
void CMFCHalconDlg::OnBnClickedObjective40()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8_t data[6] = { 0xff , 'O' , 3 , 'O' , 3 , 0xfe };
	if (m_SerialPort.isOpen()){
		m_SerialPort.writeData(data, 6);
	}
	else{
		AfxMessageBox(_T("串口未打开"));
	}
	rcWidth = 48;
	rcHeight = 39;
	Distance_para_x = 0.005;//大地图位移增益
	Distance_para_y = 0.005;
	Invalidate();
}
//

//鼠标滚轮控件
BOOL CMFCHalconDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetCursorPos(&pt);

	CRect src;
	GetDlgItem(IDC_PIC)->GetClientRect(src);
	GetDlgItem(IDC_PIC)->ClientToScreen(src);					

	if (src.PtInRect(pt) && imageWidth > 0) {//判断鼠标指针是否在控件PIC中
		if (zDelta > 0) {
			m_ZoomFactor *= 1.1;// 向上滚动，放大图像
			if (m_ZoomFactor >= 1.8) {
				KillTimer(0);
				switch (sign)
				{
				case 1:
					if (m_SerialPort.isOpen()){
						uint8_t data1[6] = { 0xff , 'O' , 2 , 'O' , 2 , 0xfe };
						m_SerialPort.writeData(data1, 6);
					}
					else{
						AfxMessageBox(_T("串口未打开"));
					}
					rcWidth = 96;
					rcHeight = 78;
					Distance_para_x = 0.01;//大地图位移增益
					Distance_para_y = 0.01;
					distance_para_x = 0.6;//小地图位移增益
					distance_para_y = 0.3;
					m_ZoomFactor = 1.0;
					sign = 2;
					delayMs(2000);
					SetTimer(0, 30, NULL);
				break;
				case 2:
					if (m_SerialPort.isOpen()){
						uint8_t data1[6] = { 0xff , 'O' , 3 , 'O' , 3 , 0xfe };
						m_SerialPort.writeData(data1, 6);
					}
					else{
						AfxMessageBox(_T("串口未打开"));
					}
					rcWidth = 48;
					rcHeight = 39;
					Distance_para_x = 0.005;//大地图位移增益
					Distance_para_y = 0.005;
					distance_para_x = 0.26;//小地图位移增益
					distance_para_y = 0.65;
					m_ZoomFactor = 1.0;
					sign = 3;
					delayMs(1200);
					SetTimer(0, 30, NULL);
				}
			}
		}
		if (zDelta < 0) {
			m_ZoomFactor /= 1.1;// 向下滚动，缩小图像
			if (m_ZoomFactor <= 1) {
				KillTimer(0);
				switch (sign)
				{
				case 2:
					if (m_SerialPort.isOpen()){
						uint8_t data1[6] = { 0xff , 'O' , 1 , 'O' , 1 , 0xfe };
						m_SerialPort.writeData(data1, 6);
					}
					else{
						AfxMessageBox(_T("串口未打开"));
					}
					rcWidth = 126;
					rcHeight = 108;
					Distance_para_x = 0.015;//大地图位移增益
					Distance_para_y = 0.015;
					distance_para_x = 0;//小地图位移增益
					distance_para_y = 0;
					m_ZoomFactor = 1.0;
					sign = 1;
					delayMs(1200);
					SetTimer(0, 20, NULL);
					break;
				case 3:
					if (m_SerialPort.isOpen()){
						uint8_t data1[6] = { 0xff , 'O' , 2 , 'O' , 2 , 0xfe };
						m_SerialPort.writeData(data1, 6);
					}
					else{
						AfxMessageBox(_T("串口未打开"));
					}
					rcWidth = 96;
					rcHeight = 78;
					Distance_para_x = 0.01;
					Distance_para_y = 0.01;
					distance_para_x = 1.0;
					distance_para_y = 0.6;
					m_ZoomFactor = 1.0;
					sign = 2;
					delayMs(1200);
					SetTimer(0, 20, NULL);
					break;
				}
			}

		}
		else {
			return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
		}
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	}
}
//

//电机自动对焦
void CMFCHalconDlg::hillclimb(int fstep_size1, int rstep_size1, int fstep_size2, int delaytime){
	double current_fx = 0.0;//定义当前清晰度值
	double last_fx = 0.0;//定义上一时刻清晰度值
	GrabImage(&ho_Image, hv_AcqHandle);
	current_fx = HTenegrad(ho_Image);
	int step = 0;

	while (current_fx > last_fx) {
		uint8_t data1[6] = { 0xff , 'R' , (fstep_size1 / 256) , 'N' , (fstep_size1 % 256) , 0xfe };
		m_SerialPort.writeData(data1, 6);
		delayMs(delaytime);
		last_fx = current_fx;
		GrabImage(&ho_Image, hv_AcqHandle);
		current_fx = HTenegrad(ho_Image);
		step++;
		if (step > 20) { break; }
	}
	while (current_fx < last_fx) {
		uint8_t data2[6] = { 0xff , 'F' , (rstep_size1 / 256) , 'N' , (rstep_size1 % 256) , 0xfe };
		m_SerialPort.writeData(data2, 6);
		delayMs(delaytime);
		last_fx = current_fx;
		GrabImage(&ho_Image, hv_AcqHandle);
		current_fx = HTenegrad(ho_Image);
		step++;
		if (step > 20) { break; }
	}
	while (current_fx > last_fx) {
		uint8_t data3[6] = { 0xff , 'R' , (fstep_size2 / 256) , 'N' , (fstep_size2 % 256) , 0xfe };
		m_SerialPort.writeData(data3, 6);
		delayMs(delaytime);
		last_fx = current_fx;
		GrabImage(&ho_Image, hv_AcqHandle);
		current_fx = HTenegrad(ho_Image);
		step++;
		if (step > 20) { break; }
	}
}
//

//测试
void CMFCHalconDlg::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteDAC(vol2ad(0));
}
//

//图像处理部分
//电机调焦搜索
void CMFCHalconDlg::OnBnClickedclimb()
{
	// TODO: 在此添加控件通知处理程序代码
	hillclimb(motorclimba, motorclimbb, motorclimbc, delaytime);
}
//

//清晰度计算
HTuple CMFCHalconDlg::HTenegrad(HObject& ho_Image) {
	ZoomImageFactor(ho_Image, &ho_ImageZoomed, 0.5, 0.5, "bilinear");
	Rgb1ToGray(ho_ImageZoomed, &ho_GrayImage);
	MedianImage(ho_GrayImage, &ho_Imageres, "circle", 3, "continued");
	SobelAmp(ho_Imageres, &ho_EdgeAmplitudeTenengrad, "sum_sqrt", 3);
	Intensity(ho_EdgeAmplitudeTenengrad, ho_EdgeAmplitudeTenengrad, &hv_ValueTenengrad, &hv_DeviationTenengrad);
	return hv_ValueTenengrad;
}
//

//压电自动对焦
tuple<int, double> CMFCHalconDlg::local_min(const double a, const double b, const double t, double& x)
{
	vector<double> b_array;
	vector<double> fb_array;
	vector<double> adc_array;
	int temp_adc;

	double c;
	double d;
	double e;
	double eps;
	double fu, fv, fw;
	double fx;
	double m;
	double p, q, r;
	double sa, sb;
	double t2, tol;
	double u, v, w;
	int step = 0;

	bool parabola = false;
	//
	//  C is the square of the inverse of the golden ratio.
	//
	c = 0.5 * (3.0 - sqrt(5.0));

	eps = sqrt(r8_epsilon());

	sa = a;
	sb = b;
	x = sa + c * (b - a);
	w = x;
	v = w;
	e = 0.0;

	WriteDAC(vol2ad(0));
	delayMs(5);

	WriteDAC(vol2ad(x));
	b_array.emplace_back(x);
	delayMs(10);

	GrabImage(&ho_Image, hv_AcqHandle);
	fx = -HTenegrad(ho_Image);
	fb_array.emplace_back(fx);
	step += 1;

	WriteDAC(vol2ad(0));
	delayMs(5);

	fw = fx;
	fv = fw;

	for (;;)
	{
		m = 0.5 * (sa + sb);
		tol = eps * fabs(x) + t;
		t2 = 2.0 * tol;
		//
		//  Check the stopping criterion.
		//
		if (fabs(x - m) <= t2 - 0.5 * (sb - sa))
		{
			break;
		}
		//  Fit a parabola.
		//
		r = 0.0;
		q = r;
		p = q;

		if (tol < fabs(e))
		{
			r = (x - w) * (fx - fv);
			q = (x - v) * (fx - fw);
			p = (x - v) * q - (x - w) * r;
			q = 2.0 * (q - r);
			if (0.0 < q)
			{
				p = -p;
			}
			q = fabs(q);
			r = e;
			e = d;
		}

		if (fabs(p) < fabs(0.5 * q * r) &&
			q * (sa - x) < p &&
			p < q * (sb - x))
		{
			//  Take the parabolic interpolation step.
			//
			d = p / q;
			u = x + d;

			//  F must not be evaluated too close to A or B.
			//
			if ((u - sa) < t2 || (sb - u) < t2)
			{
				if (x < m)
				{
					d = tol;
				}
				else
				{
					d = -tol;
				}
			}

			//if (parabola) break;
			parabola = true;
		}
		//
		//  A golden-section step.
		//
		else
		{
			if (x < m)
			{
				e = sb - x;
			}
			else
			{
				e = sa - x;
			}
			d = c * e;
		}
		//
		//  F must not be evaluated too close to X.
		//
		if (tol <= fabs(d))
		{
			u = x + d;
		}
		else if (0.0 < d)
		{
			u = x + tol;
		}
		else
		{
			u = x - tol;
		}

		WriteDAC(vol2ad(u));
		b_array.emplace_back(u);
		delayMs(10);

		GrabImage(&ho_Image, hv_AcqHandle);
		fu = -HTenegrad(ho_Image);
		fb_array.emplace_back(fu);


		WriteDAC(vol2ad(0));
		delayMs(5);

		step += 1;
		//
		//  Update A, B, V, W, and X.
		//
		if (fu <= fx)
		{
			if (u < x)
			{
				sb = x;
			}
			else
			{
				sa = x;
			}
			v = w;
			fv = fw;
			w = x;
			fw = fx;
			x = u;
			fx = fu;
		}
		else
		{
			if (u < x)
			{
				sa = u;
			}
			else
			{
				sb = u;
			}

			if (fu <= fw || w == x)
			{
				v = w;
				fv = fw;
				w = u;
				fw = fu;
			}
			else if (fu <= fv || v == x || v == w)
			{
				v = u;
				fv = fu;
			}
		}
	}

	WriteDAC(vol2ad(x));
	delayMs(10);

	GrabImage(&ho_Image, hv_AcqHandle);
	double fb = -HTenegrad(ho_Image);
	return make_tuple(step, fb);
}
void CMFCHalconDlg::pzthillclimb(double fstep_size1, double rstep_size1, double fstep_size2, double rstep_size2, int delaytime) {
	double current_fx = 0.0;//定义当前清晰度值
	double last_fx = 0.0;//定义上一时刻清晰度值
	double vol = 0;
	double i = 0;
	int step = 0;

	GrabImage(&ho_Image, hv_AcqHandle);
	current_fx = HTenegrad(ho_Image);

	while (current_fx > last_fx) {
		vol = i += fstep_size1;
		WriteDAC(vol2ad(vol));
		delayMs(delaytime);
		last_fx = current_fx;
		GrabImage(&ho_Image, hv_AcqHandle);
		current_fx = HTenegrad(ho_Image);
		step++;
		if (vol >= 100 || step >= 20) { WriteDAC(vol2ad(0)); break; }
	}
	while (current_fx < last_fx) {
		vol = i -= rstep_size1;
		WriteDAC(vol2ad(vol));
		delayMs(delaytime);
		last_fx = current_fx;
		GrabImage(&ho_Image, hv_AcqHandle);
		current_fx = HTenegrad(ho_Image);
		step++;
		if (vol <= 1 || step >= 20) { WriteDAC(vol2ad(0)); break; }
	}
	while (current_fx > last_fx) {
		vol = i += fstep_size2;
		WriteDAC(vol2ad(vol));
		delayMs(delaytime);
		last_fx = current_fx;
		GrabImage(&ho_Image, hv_AcqHandle);
		current_fx = HTenegrad(ho_Image);
		step++;
		if (vol >= 100 || step >= 20) { WriteDAC(vol2ad(0)); break; }
	}
}
//

//菜单对话框
void CMFCHalconDlg::OnOptions()
{
	// TODO: 在此添加命令处理程序代码
	OPTIONS Dlg;
	Dlg.DoModal();
}

//关闭程序处理
void CMFCHalconDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	uint8_t data[6] = { 0xff , 'O' , 1 , 'O' , 1 , 0xfe };
	m_SerialPort.writeData(data, 6);
	//uint8_t data1[6] = { 0xff , 'F' , 65000 / 256 , 'N' ,65000 % 256, 0xfe };
	//m_SerialPort.writeData(data1, 6);

	CDialogEx::OnClose();
}
