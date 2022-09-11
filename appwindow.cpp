#include "main.h"

#define hBtQuit_id 1
#define hBt2_id 2
#define hEdtPath_id 3
#define hBtPathOk_id 4
#define hCB1_id 5
#define hBt3_id 6
#define hEdtSize_id 7
#define hBtSizeOk_id 8
#define hBt4_id 9
#define hBtAbout_id 10
#define hBtHelp_id 11
HWND hWnd;
HWND hCap1;
HWND hCap3;
char path[300] = { 0 };
char cSize[5] = { 0 };
int answ;

union a {
	LPSTR b;
	LPCSTR c;
}tmp;

int CharToInt(char* a) {
	int res = 0;
	for (int i = 0; i < strlen(a); i++) {
		if (a[i] >= '0' && a[i] <= '9')res = res * 10 + (a[i] - '0');
		else return 0;
	}
	return res;
}

char* CharToStr(char a) {
	char text[3]{ 0 };
	if (a == 'я')text[0] = 'Я';
	else text[0] = a;
	text[1] = '\0';
	return text;
}


LRESULT CALLBACK ChildProc(
	HWND hwnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam) {
	char s[300];
	SCROLLINFO si{ 0 };
	switch (Msg) {
	case WM_DESTROY:
		return 0;
		break;
	case WM_COMMAND:

		if (LOWORD(wParam) == hEdtPath_id)
		{
			GetWindowTextA((HWND)lParam, path, 299);
		}
		if (LOWORD(wParam) == hBtPathOk_id) {
			cout << "Тронула меня" << endl;
			strcpy(s, "Путь к файлу: ");
			strcat(s, path);
			SetWindowTextA(hCap1, s);
			CloseWindow(hwnd);
		}
		if (LOWORD(wParam) == hEdtSize_id)
		{
			GetWindowTextA((HWND)lParam, cSize, 4);
		}
		if (LOWORD(wParam) == hBtSizeOk_id) {
			cout << "Тронула меня" << endl;
			strcpy(s, cSize);
			strcat(s, "*");
			strcat(s, cSize);
			SetWindowTextA(hCap3, s);
			CloseWindow(hwnd);
		}
		break;
	case WM_HSCROLL:
	{
		// Получить информацию о горизонтальном scroll bar.
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		// Сохранить позицию бара для дальнейшего сравнения.
		GetScrollInfo(hwnd, SB_HORZ, &si);
		int xPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// Пользователь нажал на левую стрелочку.
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

			// Пользователь нажал на правую стрелочку
		case SB_LINERIGHT:
			si.nPos += 1;
			break;

			// Пользователь нажал на область левее бара
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

			// Пользователь нажал на область правее бара
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// Пользователь переместил бар.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// Задать положение, а потом извлечь его.  В связи с корректировками
		//в Windows оно может не совпадать с установленным значением.
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// Если позиция бара изменилась пролистать окно.
		if (si.nPos != xPos)
		{
			ScrollWindow(hwnd, (xPos - si.nPos), 0, NULL, NULL);
		}

		break;
	}
	case WM_VSCROLL:
	{
		// Получить информацию о вертикальном баре.
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		// Сохранить позицию бара для дальнейшего сравнения.
		int yPos = si.nPos;
		switch (LOWORD(wParam))
		{

			// Пользователь нажал на верхнюю стрелочку.
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// Пользователь нажал на нижнюю стрелочку.
		case SB_LINEDOWN:
			si.nPos += 1;
			break;

			// Пользователь кликнул на область выше бара.
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// Пользователь кликнул на область ниже бара.
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// Пользователь переместил бар.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// Задать положение, а потом извлечь его.  В связи с корректировками
		//в Windows оно может не совпадать с установленным значением.
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &si);

		// Если позиция бара изменилась, пролистать окно.
		if (si.nPos != yPos)
		{
			ScrollWindow(hwnd, 0, (yPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
		break;
	}

}

void CreatePathWindow() {
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.lpfnWndProc = ChildProc;
	w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	w.lpszClassName = L"ChildWClass";
	RegisterClass(&w);

	HWND hWChild;
	hWChild = CreateWindow(L"ChildWClass", L"",
		WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_VISIBLE | WS_SYSMENU, 100, 100,
		400, 115, hWnd, NULL, NULL, NULL);
	ShowWindow(hWChild, SW_NORMAL);
	UpdateWindow(hWChild);

	HWND hCapPath = CreateWindow(L"static", L"Введите путь к файлу",
		WS_VISIBLE | WS_CHILD,
		0, 0, 400, 25,
		hWChild, NULL, NULL, NULL);

	HWND рEdtPath = CreateWindow(L"edit", L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		0, 25, 400, 25,
		hWChild, (HMENU)hEdtPath_id, NULL, NULL);

	HWND hBtPathOk = CreateWindow(L"button", L"Ок",
		WS_VISIBLE | WS_CHILD,
		0, 50, 400, 25,
		hWChild, (HMENU)hBtPathOk_id, NULL, NULL);
}

void CreateSizeWindow() {
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.lpfnWndProc = ChildProc;
	w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	w.lpszClassName = L"ChildWClass";
	RegisterClass(&w);

	HWND hWChild;
	hWChild = CreateWindow(L"ChildWClass", L"",
		WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_VISIBLE | WS_SYSMENU, 100, 100,
		400, 115, hWnd, NULL, NULL, NULL);
	ShowWindow(hWChild, SW_NORMAL);
	UpdateWindow(hWChild);

	HWND hCapSize = CreateWindow(L"static", L"Введите размер поля - n (n*n):",
		WS_VISIBLE | WS_CHILD,
		0, 0, 400, 25,
		hWChild, NULL, NULL, NULL);

	HWND рEdtSize = CreateWindow(L"edit", L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		0, 25, 400, 25,
		hWChild, (HMENU)hEdtSize_id, NULL, NULL);

	HWND hBtSizeOk = CreateWindow(L"button", L"Ок",
		WS_VISIBLE | WS_CHILD,
		0, 50, 400, 25,
		hWChild, (HMENU)hBtSizeOk_id, NULL, NULL);
	UpdateWindow(hWChild);
}

int CreateGridWindow() {
	int n = CharToInt(cSize);
	if (n == 0) {
		MessageBoxA(NULL, "Неверный размер поля", NULL, MB_OK);
		return 0;
	}
	
	char** grid = generate(n, path, answ);
	
	if (grid == NULL)return 0;
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.lpfnWndProc = ChildProc;
	w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	w.lpszClassName = L"ChildWClass";
	RegisterClass(&w);
	HWND hWChild;
	hWChild = CreateWindowA("ChildWClass", "",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_HSCROLL | WS_VSCROLL, 50, 50,
		(n + 1) * 16, (n + 1) * 16, hWnd, NULL, NULL, NULL);

	RECT rc = { 0 };
	GetClientRect(hWChild, &rc);
	SCROLLINFO si = { 0 };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = n * 16;
	si.nPage = (rc.bottom - rc.top);
	si.nPos = 0;
	si.nTrackPos = 0;
	SetScrollInfo(hWChild, SB_VERT, &si, true);
	SetScrollInfo(hWChild, SB_HORZ, &si, true);

	ShowWindow(hWChild, SW_NORMAL);
	UpdateWindow(hWChild);
	HWND** hWGrid;
	hWGrid = new HWND * [sizeof(HWND*) * n];
	for (int i = 0; i < n; i++) {
		hWGrid[i] = new HWND[sizeof(HWND) * n];
	}
	char c[2];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {

			strcpy(c, CharToStr(grid[i][j]));
			hWGrid[i][j] = CreateWindowA("static", c,
				WS_VISIBLE | WS_CHILD,
				j * 16, i * 16, 15, 15,
				hWChild, NULL, NULL, NULL);
		}
	}
	
	return 1;
}

LRESULT CALLBACK WinProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam) {
	switch (Msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == hBtQuit_id) {
			cout << "Тронула меня" << endl;
			PostQuitMessage(0);
		}
		if (LOWORD(wParam) == hBt2_id) {
			cout << "Тронула меня" << endl;
			CreatePathWindow();

		}
		if (LOWORD(wParam) == hBt3_id) {
			cout << "Тронула меня" << endl;
			CreateSizeWindow();
		}
		if (LOWORD(wParam) == hCB1_id) {
			cout << "Тронула меня" << endl;
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				answ = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0) + 1;
				char s[100];
				SendMessageA((HWND)lParam, CB_GETLBTEXT, answ, (LPARAM)s);
			}
		}
		if (LOWORD(wParam) == hBt4_id) {
			cout << "Тронула меня" << endl;
			if (1 == CreateGridWindow()) {
				MessageBoxA(NULL, "Ваш кроссворд успешно сгенерирован", "", MB_OK);
			}
			else MessageBoxA(NULL, "Неудалось сгенерировать кроссворд", NULL, MB_OK);
		}
		if (LOWORD(wParam) == hBtAbout_id) {
			cout << "Тронула меня" << endl;
			MessageBoxA(NULL, "Crossword_v1.01 by _ma.pp_", "О программе", MB_OK);
		}
		if (LOWORD(wParam) == hBtHelp_id) {
			cout << "Тронула меня" << endl;
			MessageBoxA(NULL, "Для создания кроссворда заполните все поля.\nЗатем нажмите кнопу \"Сгенерировать кроссворд\".", "Помощь", MB_OK);
		}

		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
		break;
	}
}

int ShowWindow() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	WNDCLASS lpWndClass;

	memset(&lpWndClass, 0, sizeof(WNDCLASS));
	lpWndClass.lpszClassName = L"my Crossword";//имя окна 
	lpWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//цвет задника
	lpWndClass.lpfnWndProc = WinProc;// указатель на структуру обработки сообщений, все поступающие
	//сообщения будут обрабатываться через DefWindowProcA
	RegisterClass(&lpWndClass);//регистрируем окно

	hWnd = CreateWindow(L"my Crossword",
		L"Crossword",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		10, 10,
		505, 240,
		NULL,
		NULL,
		NULL,
		NULL);

	ShowWindow(
		hWnd,
		SW_SHOWNORMAL
	);

	HWND hBtAbout = CreateWindow(L"button", L"О программе",
		WS_VISIBLE | WS_CHILD,
		0, 0, 245, 25,
		hWnd, (HMENU)hBtAbout_id, NULL, NULL);

	HWND hBtHelp = CreateWindow(L"button", L"Помощь",
		WS_VISIBLE | WS_CHILD,
		245, 0, 245, 25,
		hWnd, (HMENU)hBtHelp_id, NULL, NULL);

	HWND hBt2 = CreateWindow(L"button", L"Указать путь к файлу",
		WS_VISIBLE | WS_CHILD,
		0, 25, 490, 25,
		hWnd, (HMENU)hBt2_id, NULL, NULL);

	hCap1 = CreateWindow(L"static", L"Путь к файлу:",
		WS_VISIBLE | WS_CHILD,
		0, 50, 490, 25,
		hWnd, NULL, NULL, NULL);

	HWND hCap2 = CreateWindow(L"static", L"Выберете модификацию генерации:",
		WS_VISIBLE | WS_CHILD,
		0, 75, 490, 25,
		hWnd, NULL, NULL, NULL);

	HWND hCB = CreateWindow(L"combobox", L"",
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_OVERLAPPED | CBS_HASSTRINGS,
		0, 100, 490, 75,
		hWnd, (HMENU)hCB1_id, NULL, NULL);

	SendMessage(hCB, CB_ADDSTRING, 0, (LPARAM)L"Наибольшее количество пересечений");
	SendMessage(hCB, CB_ADDSTRING, 0, (LPARAM)L"Наименьшее количество пересечений");

	HWND hBt3 = CreateWindow(L"button", L"Указать размер поля",
		WS_VISIBLE | WS_CHILD,
		0, 125, 245, 25,
		hWnd, (HMENU)hBt3_id, NULL, NULL);

	hCap3 = CreateWindow(L"static", L"Размер поля:",
		WS_VISIBLE | WS_CHILD,
		245, 125, 245, 25,
		hWnd, NULL, NULL, NULL);

	HWND hBt4 = CreateWindow(L"button", L"Сгенерировать кроссворд",
		WS_VISIBLE | WS_CHILD,
		0, 150, 490, 25,
		hWnd, (HMENU)hBt4_id, NULL, NULL);

	HWND hBtQuit = CreateWindow(L"button", L"Выход",
		WS_VISIBLE | WS_CHILD,
		0, 175, 490, 25,
		hWnd, (HMENU)hBtQuit_id, NULL, NULL);

	PMSG pMsg = new MSG;
	while (GetMessage(pMsg, NULL, 0, 0)) {
		TranslateMessage(pMsg);
		DispatchMessage(pMsg);
	}
	return 1;
}