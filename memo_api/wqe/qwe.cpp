#include <windows.h>
#include<stdio.h>
#include "resource.h"
#define MAXSTR 256
#define LINESPACE 20
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, 
	WPARAM wParam, LPARAM lParam);


LPCTSTR lpszClass = TEXT("�޸���");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : ������ ���α׷��̶�� �ǹ�
	LPSTR lpszCmdLine, int nCmdShow)						 //hInstance : �ü���� Ŀ���� ���� ���α׷��� �ο��� ID
{																 //szCmdLine : Ŀ��Ʈ���� �󿡼� ���α׷� ���� �� ���޵� ���ڿ�
	HWND	hwnd;												 //iCmdShow : �����찡 ȭ�鿡 ��µ� ����
	MSG		msg;
	WNDCLASS WndClass;											 //WndClass ��� ����ü ����									 
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;			 //��½�Ÿ�� : ����/������ ��ȭ�� �ٽ� �׸�
	WndClass.lpfnWndProc	= WndProc;							 //���ν��� �Լ���
	WndClass.cbClsExtra		= 0;								 //O/S ��� ���� �޸� (Class)
	WndClass.cbWndExtra		= 0;								 //O/s ��� ���� �޸� (Window)
	WndClass.hInstance		= hInstance;						 //���� ���α׷� ID
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	 //������ ����
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);		 //Ŀ�� ����
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);//����   
	WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);								 //�޴� �̸�
	WndClass.lpszClassName	= lpszClass;						 //Ŭ���� �̸�
	RegisterClass(&WndClass);									 //�ռ� ������ ������ Ŭ������ �ּ�

	hwnd = CreateWindow(lpszClass,								 //�����찡 �����Ǹ� �ڵ�(hwnd)�� ��ȯ
		lpszClass,												 //������ Ŭ����, Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW,									 //������ ��Ÿ��
		CW_USEDEFAULT,											 //������ ��ġ, x��ǥ
		CW_USEDEFAULT,											 //������ ��ġ, y��ǥ
		300,													 //������ ��   
		160,													 //������ ����   
		NULL,													 //�θ� ������ �ڵ�	 
		NULL,													 //�޴� �ڵ�
		hInstance,    											 //���� ���α׷� ID
		NULL     												 //������ ������ ����
		);
	ShowWindow(hwnd, nCmdShow);									 //�������� ȭ�� ���
	UpdateWindow(hwnd);											 //O/S �� WM_PAINT �޽��� ����

	while(GetMessage(&msg, NULL, 0, 0))							 //WinProc()���� PostQuitMessage() ȣ�� ������ ó��
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);									 //WinMain -> WinProc  
	}   
	return (int)msg.wParam;
}


TCHAR inStr[MAXSTR][MAXSTR] = {0,};
TCHAR cpy_inStr[MAXSTR] = {0,};
int i=0, x=0, y=0, k=0, m=0, caret_y=0, length, index_length=0, m_2, str_len, flag=0, down_flag=0, up_flag=0, right_flag=0;
int oneTime1=0, oneTime2=0, oneTime3=0, oneTime4=0;
int scroll_remeber_Ycoordinate = 0;
int scroll_max_length;
TCHAR now_cha;
TCHAR char_size_char[2];
TCHAR Path[MAXSTR] = {0,};
SIZE sz, char_size;
RECT rect;
TCHAR rec[MAXSTR];

size_t bytes_in, bytes_out;
FILE *in_file, *out_file;

int yPos, xPos;
int yMax, xMax;
// caret_y�� y��ǥ
// ��ȭ�鿡 37�پ� �Է�
// k�� �Է¹��� ���ڿ� ����
// m�� ����� ���� ���
// x, y �� ������ ��ǥ
// i�� ���ڿ��� ������ ���ڸ� ��Ÿ��
// offset -> ĳ�� ��ǥ���� ����
// size ĳ���� ��ġ�� ���
// length ���ڿ��� ���̸� ���
// index_length ĳ���� ��ġ�� ���ڿ� �Է��� ���� ���
// m_2 ���������� �����ϱ����� ���
// now_cha ���� �Է��� ĳ���͸� ���� �ִ�.(���ҽ����� ����, ���θ����, ���忡�� ���)
// flag ���� �ѹ� �� ���ķδ� save���� ���̾�α׸� �ٽ� ���� �ʴ´�
// down_flag ��ũ�ѹ� ���ͷ� ���������� yInc 20 ����
// up_flag ��ũ�ѹ� �齺���̽��� �ö󰥶� yInc -20 ����
// scroll_remeber_Ycoordinate ���� �ö󰡼� �������ʴ� ���ڿ��� ������ ��Ÿ���ϴ�.
// scroll_max_length x�� ��ũ�ѹ� �۾� ������ �ִ밪�� ���� �����Դϴ�.

void CaretPosition(HWND hwnd){
	HDC hdc;

	hdc = GetDC(hwnd);
	GetTextExtentPoint32(hdc, inStr[caret_y], i, &sz); //ĳ���͸��� �ٸ� ���̶����� �� ĳ������ ���̸� ���մϴ�.
	GetTextExtentPoint32(hdc, (LPCWSTR)char_size_char, 1, &char_size); 
	ReleaseDC(hwnd, hdc);

	SetCaretPos(sz.cx - xPos, caret_y*LINESPACE - yPos);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, 
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int xInc=0, yInc=0;
	OPENFILENAME ofn;						// �������� ���̾�α� �� ���� ����� ����
	OPENFILENAME sfd;						// ���̺����� ���̾�α� �� ���� ����� ����

	char lpstrFile[MAX_PATH]="";
	char szSaveFileName[MAX_PATH]="";
	HANDLE hFile;
	DWORD dwRead;
	DWORD dwWritten;

	GetClientRect(hwnd, &rect);
	scroll_max_length = rect.right;

	switch (iMsg) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for(m=0, y=0; m<=k; m++, y+=LINESPACE){ // ��ũ���� ���ȴ� �ٽ� �ø��� �������� ���� �����ϱ����� ��ũ�� �� x, y ��ġ�� ���ش�
			TextOut(hdc, x - xPos, y - yPos, inStr[m], lstrlen(inStr[m]));
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch(wParam)
		{
		case VK_RETURN:
			break;
		case VK_BACK:
			break;
		default:
			now_cha = (TCHAR)wParam;									//����ĳ���͸� �־��ݴϴ�.
			char_size_char[0] = (TCHAR)wParam;
			length = lstrlen(inStr[caret_y]);

			for(index_length = length ; index_length >= i;index_length--)// ���� ĳ���� ���� ����
				inStr[caret_y][index_length] = inStr[caret_y][index_length-1];

			inStr[caret_y][i++] = (TCHAR)wParam;
			inStr[caret_y][lstrlen(inStr[caret_y])] = NULL;
			
			if(sz.cx + 16 > rect.right){			// ���ڰ� ������ â���� �Ѿ���� Ŭ���̾�Ʈ �۾����� �÷���
				right_flag=0;
				SendMessage(hwnd, WM_SIZE,0,0);
				SendMessage(hwnd, WM_HSCROLL, 0, 0);
			}
			CaretPosition(hwnd);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:
		switch((TCHAR)wParam)
		{
		case VK_RETURN:
			for(m=k+1; m > caret_y; m--){						
				for(m_2=0; m_2 < lstrlen(inStr[m]); m_2++){			// �� �غ��� ���پ� ������ �������� ����
					inStr[m+1][m_2] = inStr[m][m_2];
				}
				for(m_2=lstrlen(inStr[m]); m_2 >= 0; m_2--){			// ������ �η� �����
					inStr[m][m_2] = NULL;
				}
			}
			for(m_2=0; m_2 < (lstrlen(inStr[caret_y])-i); m_2++){	// �ڿ��ִ� ���� ���ٿ� ���̱�
				inStr[caret_y+1][m_2] = inStr[caret_y][i+m_2];
			}
			inStr[caret_y+1][m_2]=NULL;								// ���� ���ڿ� �������� �ΰ� �ֱ�
			for(m_2=lstrlen(inStr[caret_y]); m_2 >= i; m_2--){		// ������ �ΰ������
				inStr[caret_y][m_2] = NULL;
			}

			k++;						// �� ���ڿ��� ���� ����
			i=0;						// \r����� ����
			caret_y++;					// ĳ���� y�� ����

			if( (caret_y+2)*LINESPACE > rect.bottom){				// ���ڰ� ������ â���� �Ѿ���� ��ũ�ѹ� ����
				down_flag = 0;
				SendMessage(hwnd, WM_SIZE, 0, 0);
				SendMessage(hwnd, WM_VSCROLL, 0, 0);
				scroll_remeber_Ycoordinate++;
			}
			break;

		case VK_BACK:					// ���ڿ� ó���� �齺���̽� �ϸ� ���Ϳʹ¹ݴ�� ���ٿö󰡰� i�� �ö� �Ǹ������� ��Ÿ��
			if(i>0){					// ĳ���� ��ġ�� ���ۺκ��� �ƴҶ�
				length = lstrlen(inStr[caret_y]);
				for(index_length = --i ; index_length < length; index_length++)// ���� ĳ���� ���� ����
					inStr[caret_y][index_length] = inStr[caret_y][index_length+1];
			}else if(k>0){				// Ŀ���� ��ġ�� ���� ó���κ��� �ƴҶ�
				if(caret_y == 0)		// ĳ���� ��ġ�� ���� ó���϶� �������� �ʰ��Ѵ�.
					break;

				i = lstrlen(inStr[caret_y-1]);

				for(m=0; m<lstrlen(inStr[caret_y]) ; m++)		// �ڿ��ִ� ���� ���ٿ� ���̱�
					inStr[caret_y-1][i+m] = inStr[caret_y][m];

				for(m=lstrlen(inStr[caret_y]); m >= 0; m--)		// ������ ���� �ΰ����� �����
					inStr[caret_y][m]=NULL;

				for(m=caret_y; m < k; m++){						// �ؿ� ���ڿ� ��ĭ�� ���� �ø���
					for(m_2=0; m_2 < lstrlen(inStr[m+1]); m_2++)
						inStr[m][m_2] = inStr[m+1][m_2];
					inStr[m][m_2] = NULL;
				}

				for(m=lstrlen(inStr[k]); m >= 0; m--)			// �� �ؿ� �ִ��� �ΰ����� �����
					inStr[k][m]=NULL;
				
				caret_y--;
				k--;
				
				if( scroll_remeber_Ycoordinate != 0){			// ���ڰ� ������ â���� �Ѿ���� ��ũ�ѹ� �̺�Ʈ�� �޽��� �ֱ�
					up_flag = 0;
					SendMessage(hwnd, WM_SIZE, 0, 0);
					SendMessage(hwnd, WM_VSCROLL, 0, 0);
				}
			}
			break;

		case VK_LEFT:
			if(i!=0)
				i--;
			else if(caret_y > 0){
				i = lstrlen(inStr[--caret_y]);
			}
			break;

		case VK_RIGHT:
			if( i < lstrlen(inStr[caret_y]))
				i++;
			else if(caret_y < k){
				i=0;
				caret_y++;
			}
			break;

		case VK_UP:
			if(caret_y != 0){
				caret_y--;
				if(i>lstrlen(inStr[caret_y]))
					i=lstrlen(inStr[caret_y]);
			}
			break;

		case VK_DOWN:
			if(caret_y != k){
				caret_y++;
				if(i>lstrlen(inStr[caret_y]))
					i=lstrlen(inStr[caret_y]);
			}
			break;

		case VK_HOME:
			i = 0;
			break;

		case VK_END:
			i = lstrlen(inStr[caret_y]);
			break;
		default:;
		}

		CaretPosition(hwnd);
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_SETFOCUS:
		CreateCaret(hwnd,NULL, 1,16);
		ShowCaret(hwnd);
		break;

	case WM_KILLFOCUS:
		DestroyCaret();
		break;

	case WM_HSCROLL:
		xInc = 0;
		switch(LOWORD(wParam)){
		case SB_LINERIGHT:
			xInc = char_size.cx;
			break;
		case SB_LINELEFT:
			xInc = -char_size.cx;
			break;
		case SB_PAGELEFT:
			xInc = -60;
			break;
		case SB_PAGERIGHT:
			xInc = 60;
			break;
		case SB_THUMBTRACK:
			xInc=HIWORD(wParam) - xPos;
			break;
		}

		if(sz.cx + 16 > rect.right && right_flag == 0){			// ���ڰ� ������ â���� �Ѿ���� ���������� �ѹ�ȭ���̵�
			xInc = char_size.cx;
			sz.cx -= char_size.cx;
			right_flag++;											// �ѹ� ��ũ�� ȭ���̵� �ϰ� ���� �����ķ� üũ
		}

		// ���ο� ��ġ�� �ּ��� 0 �̻�
		if(xPos+xInc<0)
			xInc=-xPos;

		// ���ο� ��ġ�� �ִ��� yMax ����
		if(xPos+xInc>xMax)
			xInc=xMax-xPos;

		// ���ο� ��ġ ���
		xPos += xInc;

		// ��ũ�ѽ�Ű�� �� ��ġ�� �ٽ� ���
		ScrollWindow(hwnd,-xInc, 0, NULL, NULL);
		SetScrollPos(hwnd, SB_HORZ, xPos, true);
		break;

	case WM_VSCROLL:
		yInc = 0;
		switch(LOWORD(wParam)){
		case SB_LINEUP:
			yInc=-LINESPACE;
			break;
		case SB_LINEDOWN:
			yInc=LINESPACE;
			break;
		case SB_PAGEUP:
			yInc=-LINESPACE*5;
			break;
		case SB_PAGEDOWN:
			yInc=LINESPACE*5;
			break;
		case SB_THUMBTRACK:
			yInc=HIWORD(wParam) - yPos;
			break;
		default:
			break;
		}
		if( (caret_y+2)*LINESPACE >= rect.bottom && down_flag == 0){				// ĳ���� �Ѿ���� ��ũ�ѹ� ��ġ ����
			yInc=LINESPACE;
			down_flag++;
		}
		if( up_flag == 0 && scroll_remeber_Ycoordinate != 0 && yInc == 0){			// ĳ���� �ö󰥴� ��ĭ ���ܿ�
			yInc=-LINESPACE;
			up_flag++;
			scroll_remeber_Ycoordinate--;
		}
		// ���ο� ��ġ�� �ּ��� 0 �̻�
		if(yPos+yInc<0)
			yInc=-yPos;
		// ���ο� ��ġ�� �ִ��� yMax ����
		if(yPos+yInc>yMax)
			yInc=yMax-yPos;
		// ���ο� ��ġ ���
		//
		yPos=yPos+yInc;

		// ��ũ�ѽ�Ű�� �� ��ġ�� �ٽ� ���
		ScrollWindow(hwnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hwnd, SB_VERT, yPos, true);
		break;

	case WM_SIZE:
		xMax=sz.cx - LOWORD(lParam);
		yMax=(caret_y+2)*LINESPACE-HIWORD(lParam);
		SetScrollRange(hwnd, SB_VERT, 0, yMax, true);
		SetScrollPos(hwnd, SB_VERT, 0, true);
		SetScrollRange(hwnd, SB_HORZ, 0, xMax, true);
		SetScrollPos(hwnd, SB_HORZ, 0, true);
		break;
		
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case ID_40001:				// ���θ����
			if(lstrlen(inStr[0]) != 0){
				if(flag == 0){
					ZeroMemory(&sfd, sizeof(sfd));							
					sfd.lStructSize= sizeof(sfd);
					sfd.hwndOwner = hwnd;
					sfd.lpstrTitle = L"����";
					sfd.lpstrFilter = L"Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
					sfd.lpstrFile = (LPWSTR)szSaveFileName;
					sfd.nMaxFile = MAX_PATH;
					sfd.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
					sfd.lpstrDefExt = L"txt";
					if(GetSaveFileName(&sfd) != 0){
						hFile = CreateFile(sfd.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//�ѱ��� ����

						for(m=0; m<=k; m++)
						{
							lstrcat(cpy_inStr,inStr[m]);
							lstrcat(cpy_inStr, L"\r\n");						//��Ƽ����Ʈ�� �����ڵ��� ������ �˾Ƶα�
							str_len = lstrlen(cpy_inStr) * sizeof(WORD);
							WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
							cpy_inStr[0] = NULL;
						}
						for(m=k; m>=0; m--){
							for(m_2=lstrlen(inStr[m]); m_2>=0; m_2--){
								inStr[m][m_2] = NULL;
							}
						}
						for(m_2 = lstrlen(Path); m_2 >=0; m_2--) // ���ڿ� ���� �η� �ʱ�ȭ
							Path[m_2] = NULL;
						flag = 0;
						i=0;
						caret_y=0;
					}
				}else{
					hFile = CreateFile((LPCWSTR)Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//�ѱ��� ����
					for(m=0; m<=k; m++)
					{
						lstrcat(cpy_inStr,inStr[m]);
						lstrcat(cpy_inStr, L"\r\n");						//��Ƽ����Ʈ�� �����ڵ��� ������ �˾Ƶα�
						str_len = lstrlen(cpy_inStr) * sizeof(WORD);
						WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
						for(m_2 = lstrlen(cpy_inStr); m_2 >=0; m_2--) // ���ڿ� ���� �η� �ʱ�ȭ
							cpy_inStr[m_2] = NULL;
					}
					for(m=k; m>=0; m--){
						for(m_2=lstrlen(inStr[m]); m_2>=0; m_2--){
							inStr[m][m_2] = NULL;
						}
					}
					for(m_2 = lstrlen(Path); m_2 >=0; m_2--) // ���ڿ� ���� �η� �ʱ�ȭ
						Path[m_2] = NULL;
					flag = 0;
					i=0;
					caret_y=0;
					
				}
				
				CloseHandle(hFile);							//������ ���� �ݱ�
				CaretPosition(hwnd);
				InvalidateRect(hwnd, NULL, TRUE);

			}

			break;

		case ID_40002:				// ����
			for(m=k; m>=0; m--){								// ���� �ִ� ���ڿ� �� �ʱ�ȭ
				for(m_2=lstrlen(inStr[m]); m_2>=0; m_2--){
					inStr[m][m_2] = NULL;
				}
			}
			k=0;
			i=0;
			caret_y=0;

			memset(&ofn, 0, sizeof(OPENFILENAME));	// �������� ���̾�α׸� �� ũ�⸸ŭ �޸� ����
			ofn.lStructSize = sizeof(OPENFILENAME);	// �������� ���̾�α� ��Ʈ��Ʈ �������
			ofn.hwndOwner = hwnd;					// ��鷯����
			ofn.lpstrFilter = L"EveryFile(*.*)\0*.*\0Text File\0*.txt;*.doc\0"; // ���ͼ���
			ofn.lpstrFile = (LPWSTR)lpstrFile;	// ���� �̸� ����Ʈ�� ó�� ��Ÿ�� ���ϸ��� ����	
			ofn.nMaxFile=256;					// ���� �̸� �� ����
			ofn.lpstrInitialDir = L"c:\\";		// ������ �ʱ���
			if(GetOpenFileName(&ofn)!=0){		
				hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				// ���� ��� �о��
				now_cha = NULL;
				ReadFile(hFile, &now_cha, sizeof(TCHAR), &dwRead, NULL);	//�ѱ��� ����

				for(m_2 = lstrlen(Path); m_2 >=0; m_2--) // ��� ���ڿ� ���� �η� �ʱ�ȭ
					Path[m_2] = NULL;
				lstrcat(Path, (LPWSTR)lpstrFile);

				while(dwRead!=0){
					
					inStr[k][i++] = now_cha;				// ȭ�鿡 �ѱ��ھ� �ѷ��� ���� ���ڴ� ��
					inStr[k][i] = NULL;						//
					if (inStr[k][i-1] == '\n' && inStr[k][i-2] == '\r') 
					{										//  ���������� ó��
						inStr[k][--i] = NULL;				// |�� ��ó�����ְ�
						inStr[k][--i] = NULL;
						k++;								// ����
						i=0;								// �����ε�
					}
					ReadFile(hFile, &now_cha, sizeof(TCHAR), &dwRead, NULL);	//�ѱ��� ������
				}
				caret_y = k;								//ĳ���� ��ġ�� �� ���������� ����
				i = lstrlen(inStr[k]);						//

				CloseHandle(hFile);							//������ ���� �ݱ�
				CaretPosition(hwnd);						//ĳ�� ��ġ �ٽ� ����
				InvalidateRect(hwnd, NULL, TRUE);			//�ٽ� �׷��ֱ�
			}
			flag++;
			break;

		case ID_40003:		// ����
			if(flag == 0){
				ZeroMemory(&sfd, sizeof(sfd));
				sfd.lStructSize= sizeof(sfd);
				sfd.hwndOwner = hwnd;
				sfd.lpstrTitle = L"����";
				sfd.lpstrFilter = L"Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
				sfd.lpstrFile = (LPWSTR)szSaveFileName;
				sfd.nMaxFile = MAX_PATH;
				sfd.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
				sfd.lpstrDefExt = L"txt";
				if(GetSaveFileName(&sfd) != 0){
					hFile = CreateFile(sfd.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//�ѱ��� ����
					lstrcat(Path, (LPWSTR)szSaveFileName);

					for(m=0; m<=k; m++)
					{
						lstrcat(cpy_inStr,inStr[m]);
						lstrcat(cpy_inStr, L"\r\n");						//��Ƽ����Ʈ�� �����ڵ��� ������ �˾Ƶα�
						str_len = lstrlen(cpy_inStr) * sizeof(WORD);
						WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
						for(m_2 = lstrlen(cpy_inStr); m_2 >=0; m_2--) // ���ڿ� ���� �η� �ʱ�ȭ
							cpy_inStr[m_2] = NULL;
					}
				}
			}else{
				hFile = CreateFile((LPCWSTR)Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//�ѱ��� ����
				for(m=0; m<=k; m++)
				{
					lstrcat(cpy_inStr,inStr[m]);
					lstrcat(cpy_inStr, L"\r\n");						//��Ƽ����Ʈ�� �����ڵ��� ������ �˾Ƶα�
					str_len = lstrlen(cpy_inStr) * sizeof(WORD);
					WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
					for(m_2 = lstrlen(cpy_inStr); m_2 >=0; m_2--) // ���ڿ� ���� �η� �ʱ�ȭ
						cpy_inStr[m_2] = NULL;
				}
			}	
			flag++;
			CloseHandle(hFile);							//������ ���� �ݱ�
			CaretPosition(hwnd);
			break;

		case ID_40004:		// ������
			PostQuitMessage(0);
			break;

		default:;
		}
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	} 
	return DefWindowProc(hwnd, iMsg, wParam, lParam);			 //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}