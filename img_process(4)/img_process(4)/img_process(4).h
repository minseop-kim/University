
// img_process(4).h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cimg_process4App:
// �� Ŭ������ ������ ���ؼ��� img_process(4).cpp�� �����Ͻʽÿ�.
//

class Cimg_process4App : public CWinApp
{
public:
	Cimg_process4App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Cimg_process4App theApp;