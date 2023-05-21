#include "draw.c"
/*
	Головний файл проекту, описує створення вікна та взаємодію з ним.
*/
#define DRAW_DIRECTED_CODE 7763
#define DRAW_UNDIRECTED_CODE 8812
// n1 = 2
// n2 = 1
// n3 = 1
// n4 = 0

// Число вершин: 11
// Розміщення вершин: коло

HWND show_directed_button;															 // прототип кнопки показу напрямленого графу
HWND show_undirected_button;														 // прототип кнопки показу ненапрямленого графу
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

void draw_window(HWND hWnd, HDC hdc, int drawing_flag)
{
	Rectangle(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	HPEN vertex_pen = CreatePen(PS_SOLID, 3, RGB(50, 0, 255)); // стиль = неперервний; товщина = 3; колір = синій
	HPEN edge_pen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));		 // стиль = неперервний; товщина = 1; колір = чорний

	double **matrix = get_rand_matrix(MATRIX_SIZE);
	Vertex *vertex = create_vertices(VERTICES_COUNT, STANDART_GRAPH_MARGIN, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

	switch (drawing_flag)
	{
	case DRAW_DIRECTED_CODE:
		draw_directed_graph(hdc, vertex_pen, edge_pen, matrix, vertex, VERTICES_COUNT);
		break;
	case DRAW_UNDIRECTED_CODE:
		draw_undirected_graph(hdc, vertex_pen, edge_pen, matrix, vertex, VERTICES_COUNT);
		break;
	default:
		printf("ERROR! Value of drawing flag is not equal to any drawing code!\n");
	}

	// вивід матриці суміжності
	printf("\nAdjacency matrix of the depicted graph:\n\n");
	print_matrix(matrix, MATRIX_SIZE);
	// очищення пам'яті
	delete_matrix(matrix, MATRIX_SIZE);
	delete_vertices(&vertex);
	printf("=========================================================\n");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASS window;

	window.lpszClassName = "Lab #6";							// ім'я програми
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

	hWnd = CreateWindow("Lab #6",														 // ім'я програми
											"Lab #6 by Evgheniy Kovaliov IM-21", // заголовок
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

	show_directed_button = CreateWindow("button", "Show directed graph", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 720, 196, 32, hWnd, NULL, NULL, NULL);
	show_undirected_button = CreateWindow("button", "Show undirected graph", WS_VISIBLE | WS_CHILD | WS_BORDER, 388, 720, 196, 32, hWnd, NULL, NULL, NULL);

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

	static int drawing_flag = DRAW_DIRECTED_CODE;

	switch (messg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		draw_window(hWnd, hdc, drawing_flag);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		if (lParam == show_directed_button)
		{
			drawing_flag = DRAW_DIRECTED_CODE;
		}
		else if (lParam == show_undirected_button)
		{
			drawing_flag = DRAW_UNDIRECTED_CODE;
		}
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}
}
