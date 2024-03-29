#include "matrix.c"
#include "draw.c"
#include <stdbool.h>
/*
	Головний файл проекту, описує створення вікна та взаємодію з ним.
*/
#define WINDOW_RIGHT_TOP_CORNER_X 420
#define WINDOW_RIGHT_TOP_CORNER_Y 5
#define WINDOW_WIDTH 760
#define WINDOW_HEIGHT 820
#define GRAPH_MARGIN 350
#define ANGLE_BETWEEN_VERTICES (360.0 / VERTICES_COUNT)
#define N3 1
#define N4 0

// n1 = 2
// n2 = 1
// n3 = 1
// n4 = 0

// Число вершин: 11
// Розміщення вершин: коло

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

void draw_window(HWND hWnd, HDC hdc, bool is_directed)
{
	if (!is_directed) // якщо граф ненапрямлений, потрібно замалювати напрямлений, що був намальований до цього
	{
		Rectangle(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	HPEN vertex_pen = CreatePen(PS_SOLID, 3, RGB(50, 0, 255)); // стиль = неперервний; товщина = 3; колір = синій
	HPEN edge_pen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));		 // стиль = неперервний; товщина = 1; колір = чорний

	double **matrix = get_rand_matrix(MATRIX_SIZE);
	mult_matrix(matrix, MATRIX_SIZE, (1.0 - N3 * 0.02 - N4 * 0.005 - 0.25));

	Vertex *vertex = create_vertices(VERTICES_COUNT, GRAPH_MARGIN);

	Vertex *current_vertex = vertex;
	int row, col;
	SelectObject(hdc, edge_pen);
	for (row = 0; row < VERTICES_COUNT; row++)
	{
		for (col = 0; col < VERTICES_COUNT; col++)
		{
			if (matrix[row][col])
			{
				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);
				double end_y = calc_y(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);

				if (matrix[col][row] && row > col)
				{
					if (is_directed)
					{
						draw_arrowed_curve_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
					}
				}
				else if (row == col)
				{
					if (is_directed)
					{
						draw_arrowed_reflect_edge(edge_pen, current_vertex, hdc);
					}
					else
					{
						draw_reflect_edge(edge_pen, current_vertex, hdc);
					}
				}
				else
				{
					if (is_directed)
					{
						draw_arrowed_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
					}
					else
					{
						draw_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
					}
				}
			}
		}
		current_vertex = current_vertex->p_next;
	}

	current_vertex = vertex;
	while (current_vertex != NULL)
	{
		draw_vertex(vertex_pen, current_vertex, hdc);
		current_vertex = current_vertex->p_next;
	}
	// вивід матриці
	if (!is_directed) // якщо граф ненапрямлений, матриця приводиться до симетричного вигляду
	{
		get_symmetric_matrix(matrix, MATRIX_SIZE);
	}
	print_matrix(matrix, MATRIX_SIZE);
	// очищення пам'яті
	delete_matrix(matrix, MATRIX_SIZE);
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

	hWnd = CreateWindow("Lab #3",														 // ім'я програми
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

	HWND change_orientation_button;
	change_orientation_button = CreateWindow("button", "Change graph orientation", WS_VISIBLE | WS_CHILD | WS_BORDER, 284, 720, 196, 32, hWnd, NULL, NULL, NULL);

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

	static bool is_directed = true;

	switch (messg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		draw_window(hWnd, hdc, is_directed);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		is_directed = !is_directed;
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}
}
