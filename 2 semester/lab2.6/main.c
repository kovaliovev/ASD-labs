#include "draw.c"
/*
	Головний файл проекту, описує створення вікна та взаємодію з ним.
*/
#define DRAW_UNDIRECTED_CODE 8812
// n1 = 2
// n2 = 1
// n3 = 1
// n4 = 0

// Число вершин: 11
// Розміщення вершин: коло

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

void draw_window(HWND hWnd, HDC hdc, int drawing_flag)
{
	Rectangle(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	HPEN vertex_pen = CreatePen(PS_SOLID, 3, RGB(50, 0, 255)); // стиль = неперервний; товщина = 3; колір = синій
	HPEN edge_pen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));		 // стиль = неперервний; товщина = 1; колір = чорний

	Vertex *vertex = create_vertices(VERTICES_COUNT, STANDART_GRAPH_MARGIN, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

	double **matrix_A = get_rand_matrix(MATRIX_SIZE);
	mulmr(matrix_A, MATRIX_SIZE, (1.0 - N3 * 0.01 - N4 * 0.005 - 0.05));

	double **matrix_Wt = get_rand_matrix(MATRIX_SIZE);
	// print_matrix(matrix_Wt, MATRIX_SIZE);
	mult_matrix_by_num(matrix_Wt, VERTICES_COUNT, 100);
	// print_matrix(matrix_Wt, MATRIX_SIZE);
	mult_matrix_by_matrix(matrix_Wt, VERTICES_COUNT, matrix_A);
	// print_matrix(matrix_Wt, MATRIX_SIZE);
	make_matrix_rounded(matrix_Wt, VERTICES_COUNT);
	// print_matrix(matrix_Wt, MATRIX_SIZE);

	double **matrix_B = get_matrix_B(matrix_Wt, VERTICES_COUNT);
	// print_matrix(matrix_B, MATRIX_SIZE);

	double **matrix_C = get_matrix_C(matrix_B, VERTICES_COUNT);
	// print_matrix(matrix_C, MATRIX_SIZE);

	double **matrix_D = get_matrix_D(matrix_B, VERTICES_COUNT);
	// print_matrix(matrix_D, MATRIX_SIZE);

	switch (drawing_flag)
	{
	case DRAW_UNDIRECTED_CODE:
		draw_undirected_graph(hdc, vertex_pen, edge_pen, matrix_A, vertex, VERTICES_COUNT);
		break;
	default:
		printf("ERROR! Value of drawing flag is not equal to any drawing code!\n");
	}

	// вивід матриці суміжності
	printf("\nAdjacency matrix of the depicted graph:\n\n");
	print_matrix(matrix_A, MATRIX_SIZE);
	// очищення пам'яті
	delete_matrix(matrix_A, MATRIX_SIZE);
	delete_matrix(matrix_Wt, MATRIX_SIZE);
	delete_matrix(matrix_B, MATRIX_SIZE);
	delete_matrix(matrix_C, MATRIX_SIZE);
	delete_matrix(matrix_D, MATRIX_SIZE);
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

	static int drawing_flag = DRAW_UNDIRECTED_CODE;

	switch (messg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		draw_window(hWnd, hdc, drawing_flag);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}
}
