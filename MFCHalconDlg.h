
// MFCHalconDlg.h: 头文件
//

#pragma once
#include "Gdi.h"
//#define rcWidth 48
//#define rcHeight 39
#include "OPTIONS.h"
//About Halcon start
#include "HalconCpp.h"
#include "HDevThread.h"
//About helcon end

//About CSerialPort start
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
using namespace itas109;
//About CSerialPort end

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)//MFC自带鼠标按键检测宏

using namespace HalconCpp;

// CMFCHalconDlg 对话框
class CMFCHalconDlg : public CDialogEx
{
// 构造  绘制相关
public:
	CMFCHalconDlg(CWnd* pParent = nullptr);			//标准构造函数
	CPoint getpt(CWnd* org, CWnd* des, CPoint pt);  //坐标转换
	CPoint getxy(CWnd* org, CPoint pt);				//相对控件中心坐标
	CRect getctlrc(CWnd* ctl);						//控件位置
	Gdiplus::Image* bkimg;							//背景图片
	CString imgpath;								//图片目录
	CRect rcClk;									//点击矩形
	CRect getrc(CPoint pt, int w, int h);			//按中心获取矩形
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int rcWidth = 126, rcHeight = 108;
	static CMFCHalconDlg* pMFCHalcondlg;


// 构造  动作相关
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

// 构造  电机串口相关
private:
	void CMFCHalconDlg::opencom(char* com);
	int rx;
	int tx;
	double const_dis;								// 电机距离
	double const_vol;								// 压电电压

// 构造 halcon相关
public:
	CSerialPort m_SerialPort;							//About CSerialPort 
	HTuple  hv_AcqHandle, hWndID;						//窗口句柄
	HTuple  hW, hH;										//图片的宽高
	HObject ho_Image, ho_Imageres, ho_GrayImage, ho_ImageZoomed, ho_ImageAffinTrans, ho_EdgeAmplitudeTenengrad;
	HTuple  hv_ValueTenengrad, hv_DeviationTenengrad, hv_HomMat2DIdentity, hv_HomMat2DScale;
	void StartCapture();								//开始图像采集
	void StopCapture();									//停止图像采集
	void WriteDAC(int value);							//DAC发送
	int davalue[7] = { 0,10000,20000,30000,40000,50000,60000 };
	double m_ZoomFactor = 1.0;							//图片放大倍数
	int motorresolution = 50;								//载物台电机移动分辨率
	int imageWidth, imageHeight;
	int sign = 1;										//鼠标滚轮物镜标志位
	int mx,	my;											//定义鼠标实时坐标（PIC控件）
	int Motor_nowpos_x = 0, Motor_nowpos_y = 0;			//记录电机的当前位置MINPIC控件使用
	double distance_para_x = 0,	distance_para_y = 0;	//电机位移增益MINPIC控件使用
	int Nowpos_x = 0, Nowpos_y = 0;						//记录电机的当前位置PIC控件使用
	double Distance_para_x,	Distance_para_y;			//电机位移增益PIC控件使用
	int motorclimba = 100, motorclimbb = 50, motorclimbc = 10, delaytime = 20;//电机爬山参数
	void CreateImageWindows();							//图像显示窗口
	HTuple HTenegrad(HObject& ho_Image);				//清晰度评价函数
	void CMFCHalconDlg::pzthillclimb(double fstep_size1, double rstep_size1, double fstep_size2, double rstep_size2, int delaytime);//压电爬山调焦
	void CMFCHalconDlg::hillclimb(int fstep_size1, int rstep_size1, int fstep_size2, int delaytime);//电机调焦爬山搜索
	tuple<int, double> local_min(const double a, const double b, const double t, double& x);//自动对焦搜索函数
	double hillclimb_withBrent(double& x, double& fx);
	void capture(int delaytime, int index, double interval);
	std::tuple<int, int, double> golden_section(int delaytime);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCHALCON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedclimb();
	afx_msg void OnBnClickedCom();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedU();
	afx_msg void OnBnClickedLocalimg();
	afx_msg void OnBnClickedL();
	afx_msg void OnBnClickedD();
	afx_msg void OnBnClickedR();
	afx_msg void OnBnClickedEditinputvol();
	afx_msg void OnBnClickedobjective4();
	afx_msg void OnBnClickedobjective10();
	afx_msg void OnBnClickedObjective40();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CStatic m_picCtrl;
	afx_msg void OnBnClickedTest();
	afx_msg void OnOptions();
	afx_msg void OnClose();
};
