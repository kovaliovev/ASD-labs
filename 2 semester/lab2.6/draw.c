#include <windows.h>
#include <stdbool.h>
#include "Vertex.c"
#include "Edge.c"
#include "matrix.c"
/*
	Файл описує усі функції, необхідні для малювання ненапрямленого зваженого графа,
	та демонстрації роботи алгоритму Краскала.
*/
#define VERTICES_COUNT 11
#define VERTEX_RADIUS 32
#define VERTEX_DIAMETER (2 * VERTEX_RADIUS)
#define TEXT_MARGIN 5
#define WINDOW_RIGHT_TOP_CORNER_X 120
#define WINDOW_RIGHT_TOP_CORNER_Y 5
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 820
#define GRAPH_X_MARGIN 410
#define GRAPH_Y_MARGIN 395
#define GRAPH_COEF 325
// Кольори:
#define COL_BLACK RGB(0, 0, 0)
#define COL_BLUE RGB(50, 0, 255)
#define COL_GREEN RGB(50, 205, 50)
#define COL_DARK_GREEN RGB(0, 128, 0)
#define COL_RED RGB(255, 0, 0)
#define COL_DARK_ORANGE RGB(255, 140, 0)
#define COL_DARK_VIOLET RGB(148, 0, 211)
#define COL_MEDIUM_VIOLET_RED RGB(199, 21, 133)
#define COL_SIENNA RGB(160, 82, 45)
#define COL_DARK_AQUA RGB(7, 144, 121)
#define COL_DARK_PINK RGB(223, 7, 130)

COLORREF get_random_color()
{
	COLORREF COLORS[] = {COL_RED, COL_DARK_ORANGE, COL_BLUE, COL_DARK_VIOLET, COL_MEDIUM_VIOLET_RED, COL_SIENNA, COL_BLACK, COL_DARK_AQUA, COL_DARK_PINK};
	int random_number = round(get_rand_num(0, 8));
	return COLORS[random_number];
}

void draw_vertex(HDC hdc, HPEN vertex_pen, Vertex *vertex)
{
	// малювання вершини
	SelectObject(hdc, vertex_pen);
	Ellipse(hdc, vertex->x - VERTEX_RADIUS, vertex->y - VERTEX_RADIUS, vertex->x + VERTEX_RADIUS, vertex->y + VERTEX_RADIUS);

	// написання номеру вершини
	int text_y = vertex->y - VERTEX_RADIUS / 4;
	int text_x = vertex->num <= 9 ? vertex->x - TEXT_MARGIN : vertex->x - TEXT_MARGIN - 4;

	char vertex_name[3];
	sprintf(vertex_name, "%d", vertex->num);

	SetTextColor(hdc, COL_BLACK);
	TextOut(hdc, text_x, text_y, vertex_name, strlen(vertex_name));
}

void draw_edge(HDC hdc, HPEN edge_pen, int start_x, int start_y, int end_x, int end_y)
{
	SelectObject(hdc, edge_pen);
	MoveToEx(hdc, start_x, start_y, NULL);
	LineTo(hdc, end_x, end_y);
}

void draw_reflect_edge(HDC hdc, HPEN edge_pen, Vertex *vertex)
{
	SelectObject(hdc, edge_pen);
	if (vertex->num <= ceil(VERTICES_COUNT / 4.0)) // 2 чверть
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= ceil(VERTICES_COUNT / 4.0 * 2)) // 1 чверть
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= ceil(VERTICES_COUNT / 4.0 * 3)) // 4 чверть
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else // 3 чверть
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
}

void write_weight(HDC hdc, COLORREF text_color, int weight, int start_x, int start_y, int end_x, int end_y)
{
	SetTextColor(hdc, text_color);

	int text_x = (start_x + end_x) / 2 - 10;
	int text_y = (start_y + end_y) / 2 - 10;

	char str_weight[4];
	sprintf(str_weight, "%d", weight);
	TextOut(hdc, text_x, text_y, str_weight, strlen(str_weight));
}

void draw_undirected_graph(HDC hdc, HPEN vertex_pen, int vertices_count, double **matrix, Vertex *head_vertex, Edge *head_edge)
{
	make_matrix_symmetric(VERTICES_COUNT, matrix);

	Vertex *current_vertex = head_vertex;
	Edge *current_edge = head_edge;

	int row, col;
	for (row = 0; row < vertices_count; row++)
	{
		for (col = row; col < vertices_count; col++)
		{
			if (matrix[row][col])
			{
				COLORREF rand_color = get_random_color();
				HPEN edge_pen = CreatePen(PS_SOLID, 1, rand_color);

				if (row == col)
				{
					draw_reflect_edge(hdc, edge_pen, current_vertex);
				}
				else
				{
					int start_x = current_vertex->x;
					int start_y = current_vertex->y;

					int end_x = calc_x(360.0 / vertices_count, col, GRAPH_COEF, GRAPH_X_MARGIN);
					int end_y = calc_y(360.0 / vertices_count, col, GRAPH_COEF, GRAPH_Y_MARGIN);

					draw_edge(hdc, edge_pen, start_x, start_y, end_x, end_y);
					write_weight(hdc, rand_color, current_edge->weight, start_x, start_y, end_x, end_y);

					current_edge = current_edge->p_next;
				}
			}
		}
		current_vertex = current_vertex->p_next;
	}

	current_vertex = head_vertex;
	SetTextColor(hdc, COL_BLACK);
	while (current_vertex != NULL)
	{
		draw_vertex(hdc, vertex_pen, current_vertex);
		current_vertex = current_vertex->p_next;
	}
}

void draw_minimum_spanning_tree(HDC hdc, HPEN spanning_tree_vertex_pen, HPEN spanning_tree_edge_pen, int vertices_count, int max_step, Vertex *head_vertex, Edge *head_edge)
{
	if (max_step > vertices_count - 1)
		max_step = vertices_count - 1;

	int flags[vertices_count];
	int i;
	for (i = 0; i < vertices_count; i++)
	{
		flags[i] = i;
	}
	Edge *current_edge = head_edge;

	int step;
	for (step = 0; step < max_step; step++)
	{
		while (flags[current_edge->first_vertex_num - 1] == flags[current_edge->second_vertex_num - 1])
		{
			current_edge = current_edge->p_next;
		}

		Vertex *current_vertex = head_vertex;
		Vertex *first_vertex;
		Vertex *second_vertex;

		while (current_vertex != NULL)
		{
			if (current_edge->first_vertex_num == current_vertex->num)
			{
				first_vertex = current_vertex;
			}
			if (current_edge->second_vertex_num == current_vertex->num)
			{
				second_vertex = current_vertex;
			}
			current_vertex = current_vertex->p_next;
		}

		draw_edge(hdc, spanning_tree_edge_pen, first_vertex->x, first_vertex->y, second_vertex->x, second_vertex->y);
		write_weight(hdc, COL_GREEN, current_edge->weight, first_vertex->x, first_vertex->y, second_vertex->x, second_vertex->y);
		draw_vertex(hdc, spanning_tree_vertex_pen, first_vertex);
		draw_vertex(hdc, spanning_tree_vertex_pen, second_vertex);

		int first_vertex_flag = flags[current_edge->first_vertex_num - 1];
		int second_vertex_flag = flags[current_edge->second_vertex_num - 1];

		if (first_vertex_flag < second_vertex_flag)
		{
			int j;
			for (j = 0; j < vertices_count; j++)
			{
				if (flags[j] == second_vertex_flag)
				{
					flags[j] = first_vertex_flag;
				}
			}
		}
		else
		{
			int j;
			for (j = 0; j < vertices_count; j++)
			{
				if (flags[j] == first_vertex_flag)
				{
					flags[j] = second_vertex_flag;
				}
			}
		}
	}
}