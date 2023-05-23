#include <windows.h>
#include <stdbool.h>
#include "Vertex.c"
#include "matrix.c"
/*
	Файл описує усі функції, необхідні для малювання напрямнелого/ненапрямленого графа.
*/
#define VERTICES_COUNT 11
#define VERTEX_RADIUS 32
#define VERTEX_DIAMETER (2 * VERTEX_RADIUS)
#define TEXT_MARGIN 5
#define WINDOW_RIGHT_TOP_CORNER_X 120
#define WINDOW_RIGHT_TOP_CORNER_Y 5
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 820
#define STANDART_GRAPH_MARGIN 350
#define STANDART_GRAPH_COEF 300
#define N3 1
#define N4 0

void draw_vertex(HPEN vertex_pen, Vertex *vertex, HDC hdc)
{
	// малювання вершини
	SelectObject(hdc, vertex_pen);
	Ellipse(hdc, vertex->x - VERTEX_RADIUS, vertex->y - VERTEX_RADIUS, vertex->x + VERTEX_RADIUS, vertex->y + VERTEX_RADIUS);

	char vertex_name[3];
	sprintf(vertex_name, "%d", vertex->num);
	// написання номеру вершини
	if (vertex->num <= 9)
	{
		TextOut(hdc, vertex->x - TEXT_MARGIN, vertex->y - VERTEX_RADIUS / 4, vertex_name, 1);
	}
	else
	{
		TextOut(hdc, vertex->x - TEXT_MARGIN - 4, vertex->y - VERTEX_RADIUS / 4, vertex_name, 2);
	}
}

void draw_edge(HPEN edge_pen, double start_x, double start_y, double end_x, double end_y, HDC hdc)
{
	MoveToEx(hdc, start_x, start_y, NULL);
	LineTo(hdc, end_x, end_y);
}

void draw_reflect_edge(HPEN edge_pen, Vertex *vertex, HDC hdc)
{
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

void draw_undirected_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex, int vertices_count, double **weights_matrix)
{
	SelectObject(hdc, edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < vertices_count; row++)
	{
		for (col = 0; col < vertices_count; col++)
		{
			if (matrix[row][col])
			{
				int start_x = current_vertex->x;
				int start_y = current_vertex->y;

				int end_x = calc_x(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);
				int end_y = calc_y(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

				int center_x = (start_x + end_x) / 2;
				int center_y = (start_y + end_y) / 2;

				char weight[4];
				sprintf(weight, "%.0lf", weights_matrix[row][col]);
				TextOut(hdc, center_x, center_y, weight, 4);

				if (row == col)
				{
					draw_reflect_edge(edge_pen, current_vertex, hdc);
				}
				else
				{
					draw_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
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

	make_matrix_symmetric(matrix, MATRIX_SIZE);
}
