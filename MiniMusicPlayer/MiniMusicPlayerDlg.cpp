
// MiniMusicPlayerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MiniMusicPlayer.h"
#include "MiniMusicPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniMusicPlayerDlg 对话框



CMiniMusicPlayerDlg::CMiniMusicPlayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINIMUSICPLAYER_DIALOG, pParent)
	, m_vol(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Text(pDX, IDC_VOICE, m_vol);
}

BEGIN_MESSAGE_MAP(CMiniMusicPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CMiniMusicPlayerDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_PLAY, &CMiniMusicPlayerDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CMiniMusicPlayerDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, &CMiniMusicPlayerDlg::OnBnClickedStop)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMiniMusicPlayerDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// CMiniMusicPlayerDlg 消息处理程序

BOOL CMiniMusicPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);   //打开时默认最大化

	//ShowWindow(SW_MINIMIZE);      //打开时默认最小化

	// TODO: 在此添加额外的初始化代码
	m_slider.SetRange(0, 1000);
	m_slider.SetPos(300);
	m_vol = 300;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMiniMusicPlayerDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMiniMusicPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMiniMusicPlayerDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog filedlg(TRUE,       //打开文件TRUE，另存为False
		NULL,             //扩展名  
		NULL,            //文件名
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"MP3文件(*.mp3)|*.mp3|WMA文件.wma|*.wma|所有文件(*.*)|*.*||"
	);
	filedlg.DoModal();
	mciSendCommand(m_IDdevice, MCI_CLOSE, 0, 0);

	CString musicname = filedlg.GetPathName();      //获取打开文件的路径
	//MessageBox(musicname);
	MCI_OPEN_PARMS mciopenparms;                   //定义打开设备
	mciopenparms.lpstrElementName = musicname;       //填充设备
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)(PVOID)&mciopenparms);    //发送命令
	m_IDdevice = mciopenparms.wDeviceID;

	GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}


void CMiniMusicPlayerDlg::OnBnClickedPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	MCI_PLAY_PARMS mciplayparms;        //定义播放设备
	mciplayparms.dwCallback = NULL;   //填充设备
	mciplayparms.dwFrom = 0;
	mciSendCommand(m_IDdevice, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(PVOID)&mciplayparms);

	GetDlgItemText(IDC_PAUSE, str);
	if (str == "继续")
	{
		SetDlgItemText(IDC_PAUSE, _T("暂停"));
	}

	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}


void CMiniMusicPlayerDlg::OnBnClickedPause()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_PAUSE,str);
	if(str == "暂停")
	{
		mciSendCommand(m_IDdevice, MCI_PAUSE, 0, 0);
		SetDlgItemText(IDC_PAUSE, _T("继续"));
	}
	else if (str == "继续")
	{
		mciSendCommand(m_IDdevice, MCI_RESUME, 0, 0);
		SetDlgItemText(IDC_PAUSE, _T("暂停"));
	}
	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
}


void CMiniMusicPlayerDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	mciSendCommand(m_IDdevice, MCI_STOP, 0, 0);
	//mciSendCommand(m_IDdevice, MCI_CLOSE, 0, 0);

	GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}


void CMiniMusicPlayerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	MCI_DGV_SETAUDIO_PARMS mcisetvol;
	mcisetvol.dwCallback = NULL;
	mcisetvol.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mcisetvol.dwValue = m_slider.GetPos();
	m_vol = mcisetvol.dwValue / 10;

	UpdateData(FALSE);
	mciSendCommand(m_IDdevice, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE| MCI_DGV_SETAUDIO_ITEM, (DWORD)(PVOID)&mcisetvol);
	*pResult = 0;
}
