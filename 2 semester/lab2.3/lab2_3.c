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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[] = "Lab #3";

void arrow(float fi, int px, int py, HDC hdc)
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

void drawGraph(HWND hWnd, HDC hdc)
{
	char *nn[3] = {"1", "2", "3"};
	int nx[3] = {100, 200, 300};
	int ny[3] = {100, 100, 100};
	int dx = 16, dy = 16, dtx = 5;
	int i;
	HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
	HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
	SelectObject(hdc, KPen);
	MoveToEx(hdc, nx[0], ny[0], NULL);
	LineTo(hdc, nx[1], ny[1]);
	arrow(0, nx[1] - dx, ny[1], hdc);
	Arc(hdc, nx[0], ny[0] - 40, nx[2], ny[2] + 40, nx[2], ny[2], nx[0], ny[0]);
	arrow(-45.0, nx[2] - dx * 0.5, ny[2] - dy * 0.8, hdc);
	SelectObject(hdc, BPen);
	for (i = 0; i <= 2; i++)
	{
		Ellipse(hdc, nx[i] - dx, ny[i] - dy, nx[i] + dx, ny[i] + dy);
		TextOut(hdc, nx[i] - dtx, ny[i] - dy / 2, nn[i], 1);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASS window;

	window.lpszClassName = ProgName;
	window.hInstance = hInstance;
	window.lpfnWndProc = WndProc;
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hIcon = 0;
	window.lpszMenuName = 0;
	window.hbrBackground = WHITE_BRUSH;
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;

	if (!RegisterClass(&window))
	{
		printf("Error! Window was not registered!");
		return 0;
	}

	HWND hWnd;
	MSG lpMsg;

	hWnd = CreateWindow(ProgName,
											"Lab #3 by Evgheniy Kovaliov IM-21",
											WS_OVERLAPPEDWINDOW,
											100,
											100,
											460,
											240,
											(HWND)NULL,
											(HMENU)NULL,
											(HINSTANCE)hInstance,
											(HINSTANCE)NULL);

	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&lpMsg, hWnd, 0, 0))
	{
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}

	return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (messg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		drawGraph(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}
}
