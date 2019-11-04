
// img_process(4)Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CvvImage.h"
#include "Histogram.h"
#include "DibFilter.h"

// Cimg_process4Dlg 대화 상자
class Cimg_process4Dlg : public CDialogEx
{
// 생성입니다.
public:
	Cimg_process4Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	// 원본파일 선택 관련
	CStringA file_path;
	bool file_selected;
	CDC *original_DC;	
	IplImage *original_Iplimg;
	CvvImage original_cvimg;

	// 히스토그램 선택관련
	bool histo_original_selected;
	CDC *original_histoDC;	
	IplImage *original_histo_Iplimg;
	CvvImage original_histo_cvimg;
	int cRGBG;

	bool histo_change_selected;
	CDC *change_histoDC;	
	IplImage *change_histo_Iplimg;
	CvvImage change_histo_cvimg;
	int RGBG;

	RGBHISTO *bgr;

	// 변환파일 선택 관련
	bool change_img_selectd;
	CDC *change_DC;	
	IplImage *change_Iplimg;
	CvvImage change_cvimg;
	int combo_selected;

	CRect imgrect;
	CRect position;
	CRect position2;
	CRect histogramrect;

	
	IplImage *gra;
	IplImage *chan;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMG_PROCESS4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic original_picbox;
	CStatic change_picbox;
	CStatic original_histopicbox;
	CStatic change_histopicbox;
	CSliderCtrl original_slider;
	CEdit original_editbox;
	CButton file_select_button;
	CComboBox change_combobox;
	afx_msg void OnBnClickedFileSelect();
	afx_msg void OnBnClickedRedRadio();
	afx_msg void OnBnClickedGreenRadio();
	afx_msg void OnBnClickedBlueRadio();
	afx_msg void OnBnClickedRedRadio2();
	afx_msg void OnBnClickedGreenRadio2();
	afx_msg void OnBnClickedBlueRadio2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedGrayRadio();
	afx_msg void OnBnClickedGrayRadio2();
	CButton projection_x;
	CButton projection_y;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnSave();
	afx_msg void OnSavetogray();
};
