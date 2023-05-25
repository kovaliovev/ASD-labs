#include "draw.c"
/*
	Головний файл проекту, описує створення вікна та взаємодію з ним.
*/
#define DRAW_UNDIRECTED_CODE 8812
// n1 = 2
// n2 = 1
#define N3 1
#define N4 0

// Число вершин: 11
// Розміщення вершин: коло

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // прототип функції потоку вікна

void draw_window(HWND hWnd, HDC hdc, int drawing_flag)
{
	Rectangle(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	HPEN vertex_pen = CreatePen(PS_SOLID, 3, COL_BLUE);					// стиль = неперервний; товщина = 3; колір = синій
	HPEN spanning_tree_pen = CreatePen(PS_SOLID, 3, COL_GREEN); // стиль = неперервний; товщина = 3; колір = зелений

	Vertex *head_vertex = create_vertices(VERTICES_COUNT, GRAPH_COEF, GRAPH_X_MARGIN, GRAPH_Y_MARGIN);

	double **matrix_A = get_rand_matrix(VERTICES_COUNT);
	mulmr(VERTICES_COUNT, (1.0 - N3 * 0.01 - N4 * 0.005 - 0.05), matrix_A);

	double **matrix_Wt = get_rand_matrix(VERTICES_COUNT);
	mult_matrix_by_num(VERTICES_COUNT, 100, matrix_Wt);
	mult_matrix_by_matrix(VERTICES_COUNT, matrix_Wt, matrix_A);
	make_matrix_rounded(VERTICES_COUNT, matrix_Wt);

	double **matrix_B = get_matrix_B(VERTICES_COUNT, matrix_Wt);

	double **matrix_C = get_matrix_C(VERTICES_COUNT, matrix_B);

	double **matrix_D = get_matrix_D(VERTICES_COUNT, matrix_B);

	double **matrix_Tr = get_matrix_Tr(VERTICES_COUNT);

	mult_matrix_by_matrix(VERTICES_COUNT, matrix_D, matrix_Tr);
	add_matrix_to_matrix(VERTICES_COUNT, matrix_C, matrix_D);
	mult_matrix_by_matrix(VERTICES_COUNT, matrix_Wt, matrix_C);

	double **matrix_W = get_matrix_W(VERTICES_COUNT, matrix_Wt);
	printf("Matrix of weights:\n");
	print_matrix(VERTICES_COUNT, matrix_W);

	Edge *head_edge = create_edges(VERTICES_COUNT, matrix_W);
	print_edges(head_edge);

	switch (drawing_flag)
	{
	case DRAW_UNDIRECTED_CODE:
		draw_undirected_graph(hdc, vertex_pen, VERTICES_COUNT, matrix_A, head_vertex, head_edge);
		break;
	default:
		printf("ERROR! Value of drawing flag is not equal to any drawing code!\n");
	}

	sort_edges_by_weight(head_edge);
	printf("AFTER SORTING:\n");
	print_edges(head_edge);

	draw_minimum_spanning_tree(hdc, spanning_tree_pen, VERTICES_COUNT, head_vertex, head_edge);

	// вивід матриці суміжності
	printf("\nAdjacency matrix of the depicted graph:\n\n");
	print_matrix(VERTICES_COUNT, matrix_A);

	// очищення пам'яті
	delete_matrix(VERTICES_COUNT, matrix_A);
	delete_matrix(VERTICES_COUNT, matrix_Wt);
	delete_matrix(VERTICES_COUNT, matrix_B);
	delete_matrix(VERTICES_COUNT, matrix_C);
	delete_matrix(VERTICES_COUNT, matrix_D);
	delete_matrix(VERTICES_COUNT, matrix_Tr);
	delete_matrix(VERTICES_COUNT, matrix_W);
	delete_vertices(&head_vertex);
	delete_edges(&head_edge);
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
