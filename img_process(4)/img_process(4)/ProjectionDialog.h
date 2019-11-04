#pragma once
#include "afxwin.h"
#include "CvvImage.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

// ProjectionDialog ��ȭ �����Դϴ�.

class ProjectionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProjectionDialog)

public:
	ProjectionDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ProjectionDialog();
	
	bool is_opend;
	CDC *projectionDC;
	CRect *size;
	IplImage *projection_Iplimg;
	CvvImage projection_cvimg;

	CRect projection_picturecontrol_size;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CStatic projection_picturecontrol;
	afx_msg void OnPaint();
};
