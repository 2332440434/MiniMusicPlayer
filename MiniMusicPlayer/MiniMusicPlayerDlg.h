
// MiniMusicPlayerDlg.h: 头文件
//

#pragma once
#include "mmsystem.h"
#include "digitalv.h"
#pragma comment (lib,"winmm.lib")

// CMiniMusicPlayerDlg 对话框
class CMiniMusicPlayerDlg : public CDialogEx
{
// 构造
private:
	MCIDEVICEID m_IDdevice;          //标记打开的音乐文件

public:
	CMiniMusicPlayerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINIMUSICPLAYER_DIALOG };
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
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider;
	UINT m_vol;
};
