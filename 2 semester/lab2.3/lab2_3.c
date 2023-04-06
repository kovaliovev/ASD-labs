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
#define RIGHT_TOP_CORNER_X 420
#define RIGHT_TOP_CORNER_Y 50
#define WINDOW_WIDTH 760
#define WINDOW_HEIGHT 760
#define GRAPH_MARGIN 350
#define VERTEX_HEIGHT 32
#define VERTEX_WIDTH 32
#define TEXT_MARGIN 5
#define ARROW_LENGTH 16

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

char ProgName[] = "Lab #3"; // ім'я програми

void drawArrowedEdge(HPEN edgePen, Vertex *startVertex, int endVertexNum, HDC hdc)
{
	double startX = startVertex->x;
	double startY = startVertex->y;

	double endX = calcX(360.0 / VERTICES_COUNT, endVertexNum, GRAPH_MARGIN);
	double endY = calcY(360.0 / VERTICES_COUNT, endVertexNum, GRAPH_MARGIN);

	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);

	double angle = atan2(endX, endY);
	double offsetX = VERTEX_HEIGHT/2 * -cos(angle);
	double offsetY = VERTEX_HEIGHT/2 * -sin(angle);

	endX += offsetX;
	endY += offsetY;

	double dx = startX - endX; // відстань по Х
	double dy = startY - endY; // відстань по У
	double edgeLength = sqrt(dx * dx + dy * dy);
	double ratio = ARROW_LENGTH / edgeLength;	// відношення довжини боку стрілки до довжини лінії
	double lx = endX + ratio * (dx * cos(3.1416 / 6.0) + dy * sin(3.1416 / 6.0));
	double ly = endY + ratio * (dy * cos(3.1416 / 6.0) - dx * sin(3.1416 / 6.0));

	double rx = endX + ratio * (dx * cos(3.1416 / 6.0) + dy * sin(3.1416 / 6.0));
	double ry = endY + ratio * (dy * cos(3.1416 / 6.0) - dx * sin(3.1416 / 6.0));
	MoveToEx(hdc, lx, ly, NULL);
	LineTo(hdc, endX, endY);
	LineTo(hdc, rx, ry);
}

void drawVertex(HPEN vertexPen, Vertex *vertex, HDC hdc)
{
	// малювання вершини
	SelectObject(hdc, vertexPen);
	Ellipse(hdc, vertex->x - VERTEX_WIDTH, vertex->y - VERTEX_HEIGHT, vertex->x + VERTEX_WIDTH, vertex->y + VERTEX_HEIGHT);
	char vertexName[3];
	sprintf(vertexName, "%d", vertex->num);
	// написання номеру вершини
	TextOut(hdc, vertex->x - TEXT_MARGIN, vertex->y - VERTEX_HEIGHT / 2 + 5, vertexName, 2);
}

void drawEdge(HPEN edgePen, Vertex *startVertex, int endVertexNum, HDC hdc)
{
	MoveToEx(hdc, startVertex->x, startVertex->y, NULL);
	double endX = calcX(360.0 / VERTICES_COUNT, endVertexNum, GRAPH_MARGIN);
	double endY = calcY(360.0 / VERTICES_COUNT, endVertexNum, GRAPH_MARGIN);
	LineTo(hdc, endX, endY);
}

void drawReflectEdge(HPEN edgePen, Vertex *vertex, int vertexNum, HDC hdc)
{
	if (vertexNum <= (VERTICES_COUNT + 1) / 4)
	{
		Ellipse(hdc, vertex->x - 65, vertex->y - 65, vertex->x, vertex->y);
	}
	else if (vertexNum <= (VERTICES_COUNT + 1) / 2)
	{
		Ellipse(hdc, vertex->x + 65, vertex->y - 65, vertex->x, vertex->y);
	}
	else if (vertexNum <= (VERTICES_COUNT + 1) * 3 / 4)
	{
		Ellipse(hdc, vertex->x + 65, vertex->y + 65, vertex->x, vertex->y);
	}
	else
	{
		Ellipse(hdc, vertex->x - 65, vertex->y + 65, vertex->x, vertex->y);
	}
}

void drawWindow(HWND hWnd, HDC hdc)
{
	HPEN vertexPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255)); // стиль = неперервний; товщина = 2; колір = синій
	HPEN edgePen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));		// стиль = неперервний; товщина = 1; колір = чорний

	// SelectObject(hdc, linePen);
	// MoveToEx(hdc, nodesX[0], nodesY[0], NULL);
	// LineTo(hdc, nodesX[1], nodesY[1]);
	// drawArrow(0, nodesX[1] - dx, nodesY[1], hdc);
	// Arc(hdc, nodesX[0], nodesY[0] - 40, nodesX[2], nodesY[2] + 40, nodesX[2], nodesY[2], nodesX[0], nodesY[0]);
	// drawArrow(-45.0, nodesX[2] - dx * 0.5, nodesY[2] - dy * 0.8, hdc);
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
				if (row == col)
				{
					drawReflectEdge(edgePen, currentVertex, col + 1, hdc);
				}
				else
				{
					drawArrowedEdge(edgePen, currentVertex, col, hdc);
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
											RIGHT_TOP_CORNER_X,									 // положення верхнього лівого кута вікна на екрані по x
											RIGHT_TOP_CORNER_Y,									 // положення верхнього лівого кута вікна на екрані по y
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
