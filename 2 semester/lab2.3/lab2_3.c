#include <windows.h>
#include "Vertex.c"
#include "matrix.c"

// n1 = 2
// n2 = 1
// n3 = 1
// n4 = 0

// Число вершин: 11
// Розміщення вершин: коло

#define VERTICES_COUNT 11
#define WINDOW_RIGHT_TOP_CORNER_X 420
#define WINDOW_RIGHT_TOP_CORNER_Y 50
#define WINDOW_WIDTH 760
#define WINDOW_HEIGHT 760
#define GRAPH_MARGIN 350
#define VERTEX_RADIUS 32
#define VERTEX_DIAMETER 64
#define TEXT_MARGIN 5
#define ARROW_LENGTH 16

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

char ProgName[] = "Lab #3"; // ім'я програми

void drawVertex(HPEN vertexPen, Vertex *vertex, HDC hdc)
{
	// малювання вершини
	SelectObject(hdc, vertexPen);
	Ellipse(hdc, vertex->x - VERTEX_RADIUS, vertex->y - VERTEX_RADIUS, vertex->x + VERTEX_RADIUS, vertex->y + VERTEX_RADIUS);
	char vertexName[3];
	sprintf(vertexName, "%d", vertex->num);
	// написання номеру вершини
	if (vertex->num <= 9)
	{
		TextOut(hdc, vertex->x - TEXT_MARGIN, vertex->y - VERTEX_RADIUS / 2 + 8, vertexName, 1);
	}
	else
	{
		TextOut(hdc, vertex->x - TEXT_MARGIN - 4, vertex->y - VERTEX_RADIUS / 2 + 8, vertexName, 2);
	}
}

void drawArrow(HPEN arrowPen, double fi, double arrowX, double arrowY, double arrowLength, HDC hdc)
{
	double lx, ly, rx, ry;
	lx = arrowX + arrowLength * cos(fi + 0.3);
	rx = arrowX + arrowLength * cos(fi - 0.3);
	ly = arrowY + arrowLength * sin(fi + 0.3);
	ry = arrowY + arrowLength * sin(fi - 0.3);
	MoveToEx(hdc, lx, ly, NULL);
	LineTo(hdc, arrowX, arrowY);
	LineTo(hdc, rx, ry);
}

void drawEdge(HPEN edgePen, double startX, double startY, double endX, double endY, HDC hdc)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

void drawArrowedEdge(HPEN edgePen, double startX, double startY, double endX, double endY, HDC hdc)
{
	double arrowX = endX + VERTEX_RADIUS * cos(atan2(startY - endY, startX - endX));
	double arrowY = endY + VERTEX_RADIUS * sin(atan2(startY - endY, startX - endX));

	drawEdge(edgePen, startX, startY, endX, endY, hdc);
	drawArrow(edgePen, atan2((startY - endY), (startX - endX)), arrowX, arrowY, VERTEX_RADIUS, hdc);
}

void drawReflectEdge(HPEN edgePen, Vertex *vertex, HDC hdc)
{
	if (vertex->num <= (VERTICES_COUNT / 4))
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 2)
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 3)
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
}

void drawArrowedReflectEdge(HPEN edgePen, Vertex *vertex, HDC hdc)
{
	drawReflectEdge(edgePen, vertex, hdc);

	double rotateAngle;
	double arrowX = vertex->x;
	double arrowY = vertex->y;

	if (vertex->num <= (VERTICES_COUNT / 4))
	{
		rotateAngle = 165.0 * M_PI / 180.0;
		arrowX -= 32;
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 2)
	{
		rotateAngle = 195.0 * M_PI / 180.0;
		arrowX -= 32;
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 3)
	{
		rotateAngle = 345.0 * M_PI / 180.0;
		arrowX += 32;
	}
	else
	{
		rotateAngle = 15.0 * M_PI / 180.0;
		arrowX += 32;
	}
	drawArrow(edgePen, rotateAngle, arrowX, arrowY, VERTEX_RADIUS / 2, hdc);
}

void drawArrowedCurveEdge(HPEN edgePen, double startX, double startY, double endX, double endY, HDC hdc)
{
	double centerX = (startX + endX) / 2 - 32;
	double centerY = (startY + endY) / 2 - 32;
	drawEdge(edgePen, startX, startY, centerX, centerY, hdc);
	drawArrowedEdge(edgePen, centerX, centerY, endX, endY, hdc);
}

void drawWindow(HWND hWnd, HDC hdc)
{
	HPEN vertexPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255)); // стиль = неперервний; товщина = 2; колір = синій
	HPEN edgePen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));		// стиль = неперервний; товщина = 1; колір = чорний

	double **matrix = getRandomMatrix(MATRIX_SIZE);
	multMatrix(matrix, MATRIX_SIZE);
	printMatrix(matrix, MATRIX_SIZE);

	Vertex *vertex = create_vertices(VERTICES_COUNT, GRAPH_MARGIN);

	Vertex *currentVertex = vertex;
	int row;
	int col;
	SelectObject(hdc, edgePen);
	for (row = 0; row < VERTICES_COUNT; row++)
	{
		for (col = 0; col < VERTICES_COUNT; col++)
		{
			if (matrix[row][col])
			{
				if (row > col && matrix[col][row])
				{
					double startX = currentVertex->x;
					double startY = currentVertex->y;
					double endX = calcX(360.0 / VERTICES_COUNT, col, GRAPH_MARGIN);
					double endY = calcY(360.0 / VERTICES_COUNT, col, GRAPH_MARGIN);
					drawArrowedCurveEdge(edgePen, startX, startY, endX, endY, hdc);
				}
				else if (row == col)
				{
					drawArrowedReflectEdge(edgePen, currentVertex, hdc);
				}
				else
				{
					double startX = currentVertex->x;
					double startY = currentVertex->y;
					double endX = calcX(360.0 / VERTICES_COUNT, col, GRAPH_MARGIN);
					double endY = calcY(360.0 / VERTICES_COUNT, col, GRAPH_MARGIN);
					drawArrowedEdge(edgePen, startX, startY, endX, endY, hdc);
				}
			}
		}
		currentVertex = currentVertex->p_next;
	}

	currentVertex = vertex;
	while (currentVertex != NULL)
	{
		drawVertex(vertexPen, currentVertex, hdc);
		currentVertex = currentVertex->p_next;
	}
	// очищення пам'яті
	deleteMatrix(matrix, MATRIX_SIZE);
	delete_vertices(&vertex);
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
											WINDOW_RIGHT_TOP_CORNER_X,					 // положення верхнього лівого кута вікна на екрані по x
											WINDOW_RIGHT_TOP_CORNER_Y,					 // положення верхнього лівого кута вікна на екрані по y
											WINDOW_WIDTH,												 // ширина вікна
											WINDOW_HEIGHT,											 // висота вікна
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
