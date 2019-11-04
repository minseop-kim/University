#include <windows.h>
#include<stdio.h>
#include "resource.h"
#define MAXSTR 256
#define LINESPACE 20
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, 
	WPARAM wParam, LPARAM lParam);


LPCTSTR lpszClass = TEXT("메모장");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : 윈도우 프로그램이라는 의미
	LPSTR lpszCmdLine, int nCmdShow)						 //hInstance : 운영체제의 커널이 응용 프로그램에 부여한 ID
{																 //szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
	HWND	hwnd;												 //iCmdShow : 윈도우가 화면에 출력될 형태
	MSG		msg;
	WNDCLASS WndClass;											 //WndClass 라는 구조체 정의									 
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;			 //출력스타일 : 수직/수평의 변화시 다시 그림
	WndClass.lpfnWndProc	= WndProc;							 //프로시저 함수명
	WndClass.cbClsExtra		= 0;								 //O/S 사용 여분 메모리 (Class)
	WndClass.cbWndExtra		= 0;								 //O/s 사용 여분 메모리 (Window)
	WndClass.hInstance		= hInstance;						 //응용 프로그램 ID
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	 //아이콘 유형
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);		 //커서 유형
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);//배경색   
	WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);								 //메뉴 이름
	WndClass.lpszClassName	= lpszClass;						 //클래스 이름
	RegisterClass(&WndClass);									 //앞서 정의한 윈도우 클래스의 주소

	hwnd = CreateWindow(lpszClass,								 //윈도우가 생성되면 핸들(hwnd)이 반환
		lpszClass,												 //윈도우 클래스, 타이틀 이름
		WS_OVERLAPPEDWINDOW,									 //윈도우 스타일
		CW_USEDEFAULT,											 //윈도우 위치, x좌표
		CW_USEDEFAULT,											 //윈도우 위치, y좌표
		300,													 //윈도우 폭   
		160,													 //윈도우 높이   
		NULL,													 //부모 윈도우 핸들	 
		NULL,													 //메뉴 핸들
		hInstance,    											 //응용 프로그램 ID
		NULL     												 //생성된 윈도우 정보
		);
	ShowWindow(hwnd, nCmdShow);									 //윈도우의 화면 출력
	UpdateWindow(hwnd);											 //O/S 에 WM_PAINT 메시지 전송

	while(GetMessage(&msg, NULL, 0, 0))							 //WinProc()에서 PostQuitMessage() 호출 때까지 처리
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
// caret_y의 y좌표
// 한화면에 37줄씩 입력
// k는 입력받은 문자열 개수
// m은 출력을 위해 사용
// x, y 는 각각의 좌표
// i는 문자열의 각각의 문자를 나타냄
// offset -> 캐럿 좌표설정 변수
// size 캐럿의 위치를 기억
// length 문자열의 길이를 기억
// index_length 캐럿의 위치에 문자열 입력을 위해 사용
// m_2 문자일일이 대입하기위해 사용
// now_cha 지금 입력한 캐릭터를 갖고 있다.(리소스에서 열기, 새로만들기, 저장에서 사용)
// flag 저장 한번 된 이후로는 save파일 다이어로그를 다시 열지 않는다
// down_flag 스크롤바 엔터로 내려갈때만 yInc 20 해줌
// up_flag 스크롤바 백스페이스로 올라갈때 yInc -20 해줌
// scroll_remeber_Ycoordinate 위로 올라가서 보이지않는 문자열의 개수를 나타냅니다.
// scroll_max_length x축 스크롤바 작업 영역의 최대값을 갖는 변수입니다.

void CaretPosition(HWND hwnd){
	HDC hdc;

	hdc = GetDC(hwnd);
	GetTextExtentPoint32(hdc, inStr[caret_y], i, &sz); //캐릭터마다 다른 길이때문에 각 캐릭터의 길이를 구합니다.
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
	OPENFILENAME ofn;						// 오픈파일 다이어로그 를 위해 사용한 변수
	OPENFILENAME sfd;						// 세이브파일 다이어로그 를 위해 사용한 변수

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
		for(m=0, y=0; m<=k; m++, y+=LINESPACE){ // 스크롤을 내렸다 다시 올리면 지워지는 것을 방지하기위해 스크롤 바 x, y 위치를 빼준다
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
			now_cha = (TCHAR)wParam;									//현재캐릭터를 넣어줍니다.
			char_size_char[0] = (TCHAR)wParam;
			length = lstrlen(inStr[caret_y]);

			for(index_length = length ; index_length >= i;index_length--)// 현재 캐럿에 문자 삽입
				inStr[caret_y][index_length] = inStr[caret_y][index_length-1];

			inStr[caret_y][i++] = (TCHAR)wParam;
			inStr[caret_y][lstrlen(inStr[caret_y])] = NULL;
			
			if(sz.cx + 16 > rect.right){			// 글자가 윈도우 창보다 넘어갔을때 클라이언트 작업영역 늘려줌
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
				for(m_2=0; m_2 < lstrlen(inStr[m]); m_2++){			// 맨 밑부터 한줄씩 내리기 엔터진줄 제외
					inStr[m+1][m_2] = inStr[m][m_2];
				}
				for(m_2=lstrlen(inStr[m]); m_2 >= 0; m_2--){			// 내린줄 널로 만들기
					inStr[m][m_2] = NULL;
				}
			}
			for(m_2=0; m_2 < (lstrlen(inStr[caret_y])-i); m_2++){	// 뒤에있는 글자 뒷줄에 붙이기
				inStr[caret_y+1][m_2] = inStr[caret_y][i+m_2];
			}
			inStr[caret_y+1][m_2]=NULL;								// 붙은 문자열 마지막에 널값 넣기
			for(m_2=lstrlen(inStr[caret_y]); m_2 >= i; m_2--){		// 내린줄 널값만들기
				inStr[caret_y][m_2] = NULL;
			}

			k++;						// 총 문자열의 갯수 증가
			i=0;						// \r기능을 해줌
			caret_y++;					// 캐럿의 y축 증가

			if( (caret_y+2)*LINESPACE > rect.bottom){				// 글자가 윈도우 창보다 넘어갔을때 스크롤바 생김
				down_flag = 0;
				SendMessage(hwnd, WM_SIZE, 0, 0);
				SendMessage(hwnd, WM_VSCROLL, 0, 0);
				scroll_remeber_Ycoordinate++;
			}
			break;

		case VK_BACK:					// 문자열 처음에 백스페이스 하면 엔터와는반대로 한줄올라가고 i가 올라간 맨마지막을 나타냄
			if(i>0){					// 캐럿의 위치가 시작부분이 아닐때
				length = lstrlen(inStr[caret_y]);
				for(index_length = --i ; index_length < length; index_length++)// 현재 캐럿에 문자 삽입
					inStr[caret_y][index_length] = inStr[caret_y][index_length+1];
			}else if(k>0){				// 커서의 위치가 맨위 처음부분이 아닐때
				if(caret_y == 0)		// 캐럿의 위치가 맨위 처음일때 움직이지 않게한다.
					break;

				i = lstrlen(inStr[caret_y-1]);

				for(m=0; m<lstrlen(inStr[caret_y]) ; m++)		// 뒤에있는 문장 앞줄에 붙이기
					inStr[caret_y-1][i+m] = inStr[caret_y][m];

				for(m=lstrlen(inStr[caret_y]); m >= 0; m--)		// 붙인줄 전부 널값으로 만들기
					inStr[caret_y][m]=NULL;

				for(m=caret_y; m < k; m++){						// 밑에 문자열 한칸씩 위로 올리기
					for(m_2=0; m_2 < lstrlen(inStr[m+1]); m_2++)
						inStr[m][m_2] = inStr[m+1][m_2];
					inStr[m][m_2] = NULL;
				}

				for(m=lstrlen(inStr[k]); m >= 0; m--)			// 맨 밑에 있던줄 널값으로 만들기
					inStr[k][m]=NULL;
				
				caret_y--;
				k--;
				
				if( scroll_remeber_Ycoordinate != 0){			// 글자가 윈도우 창위로 넘어갔을때 스크롤바 이벤트에 메시지 주기
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

		if(sz.cx + 16 > rect.right && right_flag == 0){			// 글자가 윈도우 창보다 넘어갔을때 오른쪽으로 한번화면이동
			xInc = char_size.cx;
			sz.cx -= char_size.cx;
			right_flag++;											// 한번 스크롤 화면이동 하고 나서 그이후로 체크
		}

		// 새로운 위치는 최소한 0 이상
		if(xPos+xInc<0)
			xInc=-xPos;

		// 새로운 위치는 최대한 yMax 이하
		if(xPos+xInc>xMax)
			xInc=xMax-xPos;

		// 새로운 위치 계산
		xPos += xInc;

		// 스크롤시키고 썸 위치를 다시 계산
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
		if( (caret_y+2)*LINESPACE >= rect.bottom && down_flag == 0){				// 캐럿이 넘어갔을때 스크롤바 위치 선정
			yInc=LINESPACE;
			down_flag++;
		}
		if( up_flag == 0 && scroll_remeber_Ycoordinate != 0 && yInc == 0){			// 캐럿이 올라갈대 한칸 땡겨옴
			yInc=-LINESPACE;
			up_flag++;
			scroll_remeber_Ycoordinate--;
		}
		// 새로운 위치는 최소한 0 이상
		if(yPos+yInc<0)
			yInc=-yPos;
		// 새로운 위치는 최대한 yMax 이하
		if(yPos+yInc>yMax)
			yInc=yMax-yPos;
		// 새로운 위치 계산
		//
		yPos=yPos+yInc;

		// 스크롤시키고 썸 위치를 다시 계산
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
		case ID_40001:				// 새로만들기
			if(lstrlen(inStr[0]) != 0){
				if(flag == 0){
					ZeroMemory(&sfd, sizeof(sfd));							
					sfd.lStructSize= sizeof(sfd);
					sfd.hwndOwner = hwnd;
					sfd.lpstrTitle = L"저장";
					sfd.lpstrFilter = L"Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
					sfd.lpstrFile = (LPWSTR)szSaveFileName;
					sfd.nMaxFile = MAX_PATH;
					sfd.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
					sfd.lpstrDefExt = L"txt";
					if(GetSaveFileName(&sfd) != 0){
						hFile = CreateFile(sfd.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//한글자 읽음

						for(m=0; m<=k; m++)
						{
							lstrcat(cpy_inStr,inStr[m]);
							lstrcat(cpy_inStr, L"\r\n");						//멀티바이트와 유니코드의 차이점 알아두기
							str_len = lstrlen(cpy_inStr) * sizeof(WORD);
							WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
							cpy_inStr[0] = NULL;
						}
						for(m=k; m>=0; m--){
							for(m_2=lstrlen(inStr[m]); m_2>=0; m_2--){
								inStr[m][m_2] = NULL;
							}
						}
						for(m_2 = lstrlen(Path); m_2 >=0; m_2--) // 문자열 전부 널로 초기화
							Path[m_2] = NULL;
						flag = 0;
						i=0;
						caret_y=0;
					}
				}else{
					hFile = CreateFile((LPCWSTR)Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//한글자 읽음
					for(m=0; m<=k; m++)
					{
						lstrcat(cpy_inStr,inStr[m]);
						lstrcat(cpy_inStr, L"\r\n");						//멀티바이트와 유니코드의 차이점 알아두기
						str_len = lstrlen(cpy_inStr) * sizeof(WORD);
						WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
						for(m_2 = lstrlen(cpy_inStr); m_2 >=0; m_2--) // 문자열 전부 널로 초기화
							cpy_inStr[m_2] = NULL;
					}
					for(m=k; m>=0; m--){
						for(m_2=lstrlen(inStr[m]); m_2>=0; m_2--){
							inStr[m][m_2] = NULL;
						}
					}
					for(m_2 = lstrlen(Path); m_2 >=0; m_2--) // 문자열 전부 널로 초기화
						Path[m_2] = NULL;
					flag = 0;
					i=0;
					caret_y=0;
					
				}
				
				CloseHandle(hFile);							//오픈한 파일 닫기
				CaretPosition(hwnd);
				InvalidateRect(hwnd, NULL, TRUE);

			}

			break;

		case ID_40002:				// 열기
			for(m=k; m>=0; m--){								// 원래 있던 문자열 다 초기화
				for(m_2=lstrlen(inStr[m]); m_2>=0; m_2--){
					inStr[m][m_2] = NULL;
				}
			}
			k=0;
			i=0;
			caret_y=0;

			memset(&ofn, 0, sizeof(OPENFILENAME));	// 오픈파일 다이어로그를 그 크기만큼 메모리 세팅
			ofn.lStructSize = sizeof(OPENFILENAME);	// 오픈파일 다이어로그 스트럭트 사이즈설정
			ofn.hwndOwner = hwnd;					// 헨들러설정
			ofn.lpstrFilter = L"EveryFile(*.*)\0*.*\0Text File\0*.txt;*.doc\0"; // 필터설정
			ofn.lpstrFile = (LPWSTR)lpstrFile;	// 파일 이름 에디트에 처음 나타낼 파일명을 지정	
			ofn.nMaxFile=256;					// 파일 이름 총 길이
			ofn.lpstrInitialDir = L"c:\\";		// 파일의 초기경로
			if(GetOpenFileName(&ofn)!=0){		
				hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				// 파일 경로 읽어옴
				now_cha = NULL;
				ReadFile(hFile, &now_cha, sizeof(TCHAR), &dwRead, NULL);	//한글자 읽음

				for(m_2 = lstrlen(Path); m_2 >=0; m_2--) // 경로 문자열 전부 널로 초기화
					Path[m_2] = NULL;
				lstrcat(Path, (LPWSTR)lpstrFile);

				while(dwRead!=0){
					
					inStr[k][i++] = now_cha;				// 화면에 한글자씩 뿌려줌 다음 글자는 널
					inStr[k][i] = NULL;						//
					if (inStr[k][i-1] == '\n' && inStr[k][i-2] == '\r') 
					{										//  개행됬을대 처리
						inStr[k][--i] = NULL;				// |를 널처리해주고
						inStr[k][--i] = NULL;
						k++;								// 개행
						i=0;								// 리와인드
					}
					ReadFile(hFile, &now_cha, sizeof(TCHAR), &dwRead, NULL);	//한글자 또읽음
				}
				caret_y = k;								//캐럿의 위치를 맨 마지막으로 설정
				i = lstrlen(inStr[k]);						//

				CloseHandle(hFile);							//오픈한 파일 닫기
				CaretPosition(hwnd);						//캐럿 위치 다시 선정
				InvalidateRect(hwnd, NULL, TRUE);			//다시 그려주기
			}
			flag++;
			break;

		case ID_40003:		// 저장
			if(flag == 0){
				ZeroMemory(&sfd, sizeof(sfd));
				sfd.lStructSize= sizeof(sfd);
				sfd.hwndOwner = hwnd;
				sfd.lpstrTitle = L"저장";
				sfd.lpstrFilter = L"Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
				sfd.lpstrFile = (LPWSTR)szSaveFileName;
				sfd.nMaxFile = MAX_PATH;
				sfd.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
				sfd.lpstrDefExt = L"txt";
				if(GetSaveFileName(&sfd) != 0){
					hFile = CreateFile(sfd.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//한글자 읽음
					lstrcat(Path, (LPWSTR)szSaveFileName);

					for(m=0; m<=k; m++)
					{
						lstrcat(cpy_inStr,inStr[m]);
						lstrcat(cpy_inStr, L"\r\n");						//멀티바이트와 유니코드의 차이점 알아두기
						str_len = lstrlen(cpy_inStr) * sizeof(WORD);
						WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
						for(m_2 = lstrlen(cpy_inStr); m_2 >=0; m_2--) // 문자열 전부 널로 초기화
							cpy_inStr[m_2] = NULL;
					}
				}
			}else{
				hFile = CreateFile((LPCWSTR)Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				ReadFile(hFile, &now_cha, sizeof(char), &dwRead, NULL);	//한글자 읽음
				for(m=0; m<=k; m++)
				{
					lstrcat(cpy_inStr,inStr[m]);
					lstrcat(cpy_inStr, L"\r\n");						//멀티바이트와 유니코드의 차이점 알아두기
					str_len = lstrlen(cpy_inStr) * sizeof(WORD);
					WriteFile(hFile, cpy_inStr, str_len, &dwWritten, NULL);
					for(m_2 = lstrlen(cpy_inStr); m_2 >=0; m_2--) // 문자열 전부 널로 초기화
						cpy_inStr[m_2] = NULL;
				}
			}	
			flag++;
			CloseHandle(hFile);							//오픈한 파일 닫기
			CaretPosition(hwnd);
			break;

		case ID_40004:		// 끝내기
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
	return DefWindowProc(hwnd, iMsg, wParam, lParam);			 //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}