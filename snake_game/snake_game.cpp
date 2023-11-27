#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK StartWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void StartWindow(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	WNDCLASS	WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("시작화면"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400,
		300,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	MSG msg;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

#define ITEMNUM 10

int board[22][22];
int wormX[100], wormY[100];
int xDirect, yDirect, len;
HWND hWndInput;
wstring inputText; // 전역 변수로 입력된 글자를 저장할 wstring 선언
int score = 0;
static TCHAR str[100];

void ItemGenerator()
{
	int i, x, y;
	for (i = 0; i < ITEMNUM; i++)
	{
		x = rand() % 20 + 1;
		y = rand() % 20 + 1;
		if (board[y][x] == 0)
			board[y][x] = 2;
		else
		{
			i = i - 1;
			continue;
		}
	}
	return;
}

void ItemGenerator2()
{
	int i, x, y;
	for (i = 0; i < 1; i++)
	{
		x = rand() % 20 + 1;
		y = rand() % 20 + 1;
		if (board[y][x] == 0)
			board[y][x] = 2;
		else
		{
			i = i - 1;
			continue;
		}
	}
	return;
}

void DrawGameBoard(HDC hdc)
{
	int i, x, y;
	for (y = 0; y < 22; y++) {
		for (x = 0; x < 22; x++)
		{
			switch (board[y][x])
			{
			case -1:
				Rectangle(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
				break;
			case 2:
				SelectObject(hdc, GetStockObject(BLACK_BRUSH));
				Ellipse(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
				SelectObject(hdc, GetStockObject(WHITE_BRUSH));
				break;
			}
		}
	}

	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));
	Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20);
	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255)));

	for (i = 1; i < len; i++)
		Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20);
	wstring scoreText = L"Score: " + to_wstring(score);
	TextOut(hdc, 500, 20, scoreText.c_str(), scoreText.length());
}

void GameInit()
{
	int i;
	for (i = 0; i < 22; i++)
	{
		board[i][0] = -1;
		board[i][21] = -1;
		board[0][i] = -1;
		board[21][i] = -1;
	}
	wormX[0] = 2; wormY[0] = 1;
	wormX[1] = 1; wormY[1] = 1;
	board[wormY[0]][wormX[0]] = 3;
	board[wormY[1]][wormX[1]] = 3;

	ItemGenerator();

	len = 2;
	xDirect = 1; yDirect = 0;
}

void DirectControl(int DirectKey)
{
	switch (DirectKey) {
	case VK_LEFT:
		if (xDirect == 1)
			break;
		if (board[wormY[0]][wormX[0] - 1] != -1)
		{
			xDirect = -1;
			yDirect = 0;
		}
		break;
	case VK_RIGHT:
		if (xDirect == -1)
			break;
		if (board[wormY[0]][wormX[0] + 1] != -1)
		{
			xDirect = 1;
			yDirect = 0;
		}
		break;
	case VK_UP:
		if (yDirect == 1)
			break;
		if (board[wormY[0] - 1][wormX[0]] != -1)
		{
			xDirect = 0;
			yDirect = -1;
		}
		break;
	case VK_DOWN:
		if (yDirect == -1)
			break;
		if (board[wormY[0] + 1][wormX[0]] != -1)
		{
			xDirect = 0;
			yDirect = 1;
		}
		break;
	}
}

void ShowSnakeInfo(HWND hwnd) {
	char command[100];
	FILE* file;
	sprintf_s(command, "txt_upload.exe %ls %d", str, score);
	system(command);
	//system("txt_upload.exe LSH 200");
	system("txt_maker.exe");
	if (fopen_s(&file, "snake_info.txt", "r") == 0 && file != NULL) {
		char line[100];
		wstring message = L"Snake Ranking:\n\n";
		int lineCount = 0;

		while (fgets(line, sizeof(line), file)) {
			// Assuming the file contains lines in the format: Name: XYZ, Score: N
			wstring lineW = wstring(line, line + strlen(line));
			message += lineW;
			lineCount++;
			if (lineCount >= 10) // Read 10 lines
				break;
		}

		fclose(file);
		//MessageBox(hwnd, message.c_str(), L"Snake Ranking", MB_OK | MB_ICONINFORMATION);
		int result = MessageBox(hwnd, message.c_str(), L"Snake Ranking", MB_OK | MB_ICONINFORMATION);
		if (result == IDOK) {
			exit(0);
		}
	}
	else {
		MessageBox(hwnd, L"Failed to open snake_info.txt", L"Error", MB_OK | MB_ICONERROR);
	}
}

void ShowSnakeInfo_no_quit(HWND hwnd) {
	char command[100];
	FILE* file;
	sprintf_s(command, "txt_upload.exe %ls %d", str, score);
	system(command);
	//system("txt_upload.exe LSH 200");
	system("txt_maker.exe");
	if (fopen_s(&file, "snake_info.txt", "r") == 0 && file != NULL) {
		char line[100];
		wstring message = L"Snake Ranking:\n\n";
		int lineCount = 0;

		while (fgets(line, sizeof(line), file)) {
			// Assuming the file contains lines in the format: Name: XYZ, Score: N
			wstring lineW = wstring(line, line + strlen(line));
			message += lineW;
			lineCount++;
			if (lineCount >= 10) // Read 10 lines
				break;
		}

		fclose(file);
		//MessageBox(hwnd, message.c_str(), L"Snake Ranking", MB_OK | MB_ICONINFORMATION);
		int result = MessageBox(hwnd, message.c_str(), L"Snake Ranking", MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(hwnd, L"Failed to open snake_info.txt", L"Error", MB_OK | MB_ICONERROR);
	}
}

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int count;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (iMsg) {
	case WM_CREATE:
		count = 0;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			ShowSnakeInfo(hwnd);
			return 0; // Return here to stop further processing for VK_RETURN
		}
		break;
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 220, 150, str, _tcslen(str));
		{
			wstring scoreText = L"Score: " + to_wstring(score);
			TextOut(hdc, 155, 70, scoreText.c_str(), scoreText.length());
		}
		TextOut(hdc, 10, 200, _T("점수 등록을 하려면 이름 입력후 enter키를 누르세요"), _tcslen(_T("점수 등록을 하려면 이름 입력후 enter키를 누르세요")));
		TextOut(hdc, 30, 220, _T("등록하지 않으려면 그냥 enter키를 누르세요"), _tcslen(_T("등록하지 않으려면 그냥 enter키를 누르세요")));
		TextOut(hdc, 120, 150, _T("이름(이니셜) : "), _tcslen(_T("이름(이니셜) : ")));
		SetTextColor(hdc, RGB(255, 0, 0)); // Set text color to red
		TextOut(hdc, 150, 50, _T("Game Over"), _tcslen(_T("Game Over")));
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR: // WM_CHAR 메시지 처리 (글자 입력)
		if (wParam == VK_BACK && count > 0)count--;
		else str[count++] = wParam;
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
	return 0;
}

void NewWindow(HWND hwnd) {
	WNDCLASS NewWndClass;
	NewWndClass.style = CS_HREDRAW | CS_VREDRAW;
	NewWndClass.lpfnWndProc = NewWndProc; // 새로운 창의 동작을 위한 새로운 WndProc를 정의하세요.
	NewWndClass.cbClsExtra = 0;
	NewWndClass.cbWndExtra = 0;
	NewWndClass.hInstance = GetModuleHandle(NULL); // 현재 인스턴스를 사용합니다.
	NewWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	NewWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	NewWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	NewWndClass.lpszMenuName = NULL;
	NewWndClass.lpszClassName = _T("New Window Class Name"); // 새로운 클래스 이름을 정의하세요.
	RegisterClass(&NewWndClass);

	// 새로운 창을 생성합니다.
	HWND newHwnd = CreateWindow(_T("New Window Class Name"), // 여기에 새로운 클래스 이름을 사용하세요.
		_T("점수등록"), // 새로운 창의 제목을 설정하세요.
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400,
		300,
		hwnd, // 이 창의 부모로 설정합니다. (NULL을 사용하여 루트 윈도우로 생성 가능)
		NULL,
		GetModuleHandle(NULL), // 현재 인스턴스를 사용합니다.
		NULL
	);

	if (newHwnd != NULL) {
		// 새로운 창을 표시하고 업데이트합니다.
		ShowWindow(newHwnd, SW_SHOW);
		UpdateWindow(newHwnd);
	}
}

void MovingWorm(HWND hwnd)
{
	static bool wallHit = false;
	int tmpx, tmpy, i;

	tmpx = wormX[0];
	tmpy = wormY[0];
	wormX[0] += xDirect; wormY[0] += yDirect;

	if (board[wormY[0]][wormX[0]] == -1 || board[wormY[0]][wormX[0]] == 3)
	{
		if (!wallHit) {
			PlaySound(TEXT("thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
			wallHit = true;
			NewWindow(hwnd); // Show snake info when hitting the wall
			//wallHit = true;
		}
		wormX[0] = tmpx;
		wormY[0] = tmpy;

	}
	else
	{
		if (board[wormY[0]][wormX[0]] == 2)
		{
			len = len + 1;
			board[wormY[0]][wormX[0]] = 0;
			score += 10;
			PlaySound(TEXT("apple.wav"), NULL, SND_FILENAME | SND_ASYNC);
			ItemGenerator2();
		}
		else
			board[wormY[len - 1]][wormX[len - 1]] = 0;

		for (i = len - 1; i > 1; i--)
		{
			wormX[i] = wormX[i - 1];
			wormY[i] = wormY[i - 1];
		}
		wormX[1] = tmpx;
		wormY[1] = tmpy;
		board[wormY[0]][wormX[0]] = 3;
	}
}

LRESULT CALLBACK StartWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMsg) {
	case WM_CREATE:
		//system("txt_maker.exe");
		GameInit();
		SetTimer(hwnd, 1, 100, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawGameBoard(hdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		DirectControl((int)wParam);
		break;
	case WM_TIMER:
		MovingWorm(hwnd);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
	case WM_CREATE:
		// Create the "시작" button
		CreateWindow(_T("BUTTON"), _T("시작"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			30, 180, 150, 50, hwnd, (HMENU)101, NULL, NULL);

		// Create the "정보" button
		CreateWindow(_T("BUTTON"), _T("랭킹"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			200, 180, 150, 50, hwnd, (HMENU)102, NULL, NULL);
		CreateWindow(_T("BUTTON"), _T("바로가기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			200, 90, 100, 20, hwnd, (HMENU)103, NULL, NULL);
		break;

	case WM_COMMAND:
		// Handle button clicks here if needed
		switch (LOWORD(wParam)) {
		case 101: // 처리할 버튼의 ID를 여기에 입력하세요 (시작 버튼)
			StartWindow(hwnd);
			ShowWindow(hwnd, SW_HIDE);

			break;
		case 102:
			ShowSnakeInfo_no_quit(hwnd);
			break;
		case 103:
			ShellExecute(NULL, _T("open"), _T("https://github.com/sunbaklee/snake_game"), NULL, NULL, SW_SHOWNORMAL);
			break;

		}
		break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		SetTextAlign(hdc, TA_CENTER | TA_TOP);
		//TextOut(hdc, 200, 70, _T("Snake Game"), _tcslen(_T("Snake Game")));
		TextOut(hdc, 140, 90, _T("업데이트 정보"), _tcslen(_T("업데이트 정보")));
		TextOut(hdc, 190,120, _T("업데이트 날짜: 2023-11-28"), _tcslen(_T("업데이트 날짜: 2023-11-28")));
	
		HFONT hFont = CreateFont(50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, _T("Arial"));
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		RECT rc;
		SetRect(&rc,40, 25, 620, 100); // 폰트 크기 및 위치 지정
		DrawText(hdc, _T("Snake Game"), -1, &rc, DT_CENTER | DT_TOP | DT_SINGLELINE);

		hFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, _T("Arial"));
		hOldFont = (HFONT)SelectObject(hdc, hFont);

		SetRect(&rc, 200, 240, 550, 260); // 폰트 크기 및 위치 지정
		DrawText(hdc, _T("@copy right 2023, sunbaklee all rights reserved."), -1, &rc, DT_CENTER | DT_TOP | DT_SINGLELINE);

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	// Other message handling code

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


void StartWindow(HWND hwnd) {
	WNDCLASS StartWndClass;
	StartWndClass.style = CS_HREDRAW | CS_VREDRAW;
	StartWndClass.lpfnWndProc = StartWndProc; // 시작 창의 동작을 위한 StartWndProc 사용
	StartWndClass.cbClsExtra = 0;
	StartWndClass.cbWndExtra = 0;
	StartWndClass.hInstance = GetModuleHandle(NULL);
	StartWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	StartWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	StartWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	StartWndClass.lpszMenuName = NULL;
	StartWndClass.lpszClassName = _T("Start Window Class Name"); // 시작 창의 클래스 이름 변경
	RegisterClass(&StartWndClass);

	HWND startHwnd = CreateWindow(_T("Start Window Class Name"), // 새 클래스 이름으로 업데이트
		_T("뱀게임"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);


	if (startHwnd != NULL) {
		ShowWindow(startHwnd, SW_SHOW);
		UpdateWindow(startHwnd);
	}
}
