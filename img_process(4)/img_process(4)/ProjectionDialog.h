#pragma once
#include "afxwin.h"
#include "CvvImage.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

// ProjectionDialog 대화 상자입니다.

class ProjectionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProjectionDialog)

public:
	ProjectionDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ProjectionDialog();
	
	bool is_opend;
	CDC *projectionDC;
	CRect *size;
	IplImage *projection_Iplimg;
	CvvImage projection_cvimg;

	CRect projection_picturecontrol_size;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic projection_picturecontrol;
	afx_msg void OnPaint();
};
