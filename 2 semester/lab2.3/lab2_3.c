#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

// n1 = 2
// n2 = 1
// n3 = 1
// n4 = 0

// Число вершин: 11
// Розміщення вершин: коло

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

char ProgName[] = "Lab #3"; // ім'я програми

void drawArrow(float fi, int px, int py, HDC hdc)
{
	fi = 3.1416 * (180.0 - fi) / 180.0;
	int lx, ly, rx, ry;
	lx = px + 15 * cos(fi + 0.3);
	rx = px + 15 * cos(fi - 0.3);
	ly = py + 15 * sin(fi + 0.3);
	ry = py + 15 * sin(fi - 0.3);
	MoveToEx(hdc, lx, ly, NULL);
	LineTo(hdc, px, py);
	LineTo(hdc, rx, ry);
}

void drawWindow(HWND hWnd, HDC hdc)
{
	char *nodeNames[3] = {"1", "2", "3"};
	int nodeX[3] = {100, 200, 300};
	int nodeY[3] = {100, 100, 100};
	int dx = 16, dy = 16, dtx = 5;

	HPEN nodePen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
	HPEN linePen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

	SelectObject(hdc, linePen);
	MoveToEx(hdc, nodeX[0], nodeY[0], NULL);
	LineTo(hdc, nodeX[1], nodeY[1]);
	drawArrow(0, nodeX[1] - dx, nodeY[1], hdc);
	Arc(hdc, nodeX[0], nodeY[0] - 40, nodeX[2], nodeY[2] + 40, nodeX[2], nodeY[2], nodeX[0], nodeY[0]);
	drawArrow(-45.0, nodeX[2] - dx * 0.5, nodeY[2] - dy * 0.8, hdc);
	SelectObject(hdc, nodePen);

	int i;
	for (i = 0; i <= 2; i++)
	{
		Ellipse(hdc, nodeX[i] - dx, nodeY[i] - dy, nodeX[i] + dx, nodeY[i] + dy);
		TextOut(hdc, nodeX[i] - dtx, nodeY[i] - dy / 2, nodeNames[i], 1);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASS window;

	window.lpszClassName = "Lab #3";							// ім'я програми
	window.hInstance = hInstance;									// ідентифікатор застосунку
	window.lpfnWndProc = WindowProc;							// вказівник на функцію вікна
	window.hCursor = LoadCursor(NULL, IDC_ARROW); // завантажений курсор
	window.hIcon = 0;															// піктограми(відсутня)
	window.lpszMenuName = 0;											// меню(відсутнє)
	window.hbrBackground = WHITE_BRUSH;						// колір фона вікна
	window.style = CS_HREDRAW | CS_VREDRAW;				// стиль: можна перемальовувати
	window.cbClsExtra = 0;												// кількість додаткових байтів для цього класу
	window.cbWndExtra = 0;												// кількість додаткових байтів для цього вікна

	if (!RegisterClass(&window))
	{
		printf("Error! Window was not registered!");
		return 0;
	}

	HWND hWnd;
	MSG lpMsg;

	hWnd = CreateWindow(ProgName,														 // ім'я програми
											"Lab #3 by Evgheniy Kovaliov IM-21", // заголовок
											WS_OVERLAPPEDWINDOW,								 // стиль вікна: комплексний
											100,																 // положення верхнього правого кута вікна на екрані по x
											100,																 // положення верхнього правого кута вікна на екрані по y
											460,																 // ширина вікна
											240,																 // висота вікна
											(HWND)NULL,													 // ідентифікатор породжуючого вікна(відсутній)
											(HMENU)NULL,												 // ідентифікатор меню(відсутній)
											(HINSTANCE)hInstance,								 // ідентифікатор екземпляра вікна
											(HINSTANCE)NULL);										 // додаткові параметри(відсутні)

	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&lpMsg, hWnd, 0, 0))
	{
		TranslateMessage(&lpMsg); // перетворення повідомлення
		DispatchMessage(&lpMsg);	// передача повідомлення до функції вікна
	}

	return (lpMsg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (messg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		drawWindow(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}
}
