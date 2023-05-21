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
#define CONDENSAT_GRAPH_X_MARGIN 960
#define CONDENSAT_GRAPH_Y_MARGIN 600
#define STANDART_GRAPH_COEF 300
#define CONDENSAT_GRAPH_COEF 150
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

void draw_arrow(HPEN arrow_pen, double rotate_angle, double arrow_x, double arrow_y, double arrow_length, HDC hdc)
{
	double lx, ly, rx, ry;
	lx = arrow_x + arrow_length * cos(rotate_angle + 0.3);
	rx = arrow_x + arrow_length * cos(rotate_angle - 0.3);
	ly = arrow_y + arrow_length * sin(rotate_angle + 0.3);
	ry = arrow_y + arrow_length * sin(rotate_angle - 0.3);
	MoveToEx(hdc, lx, ly, NULL);
	LineTo(hdc, arrow_x, arrow_y);
	LineTo(hdc, rx, ry);
}

void draw_edge(HPEN edge_pen, double start_x, double start_y, double end_x, double end_y, HDC hdc)
{
	MoveToEx(hdc, start_x, start_y, NULL);
	LineTo(hdc, end_x, end_y);
}

void draw_arrowed_edge(HPEN edge_pen, double start_x, double start_y, double end_x, double end_y, HDC hdc)
{
	double rotate_angle = atan2(start_y - end_y, start_x - end_x);
	double arrow_x = end_x + VERTEX_RADIUS * cos(rotate_angle);
	double arrow_y = end_y + VERTEX_RADIUS * sin(rotate_angle);

	draw_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
	draw_arrow(edge_pen, rotate_angle, arrow_x, arrow_y, VERTEX_RADIUS, hdc);
}

void draw_reflect_edge(HPEN edge_pen, Vertex *vertex, HDC hdc)
{
	if (vertex->num <= (VERTICES_COUNT / 4)) // 3 чверть
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 2) // 2 чверть
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 3) // 1 чверть
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else // 4 чверть
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
}

void draw_arrowed_reflect_edge(HPEN edge_pen, Vertex *vertex, HDC hdc)
{
	draw_reflect_edge(edge_pen, vertex, hdc);

	double rotate_angle;
	double arrow_x = vertex->x;
	double arrow_y = vertex->y;

	if (vertex->num <= (VERTICES_COUNT / 4)) // 3 чверть
	{
		rotate_angle = to_radians(165.0);
		arrow_x -= 32;
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 2) // 2 чверть
	{
		rotate_angle = to_radians(195.0);
		arrow_x -= 32;
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 3) // 1 чверть
	{
		rotate_angle = to_radians(345.0);
		arrow_x += 32;
	}
	else // 4 чверть
	{
		rotate_angle = to_radians(15.0);
		arrow_x += 32;
	}
	draw_arrow(edge_pen, rotate_angle, arrow_x, arrow_y, VERTEX_RADIUS / 2, hdc);
}

void draw_arrowed_curve_edge(HPEN edge_pen, double start_x, double start_y, double end_x, double end_y, HDC hdc)
{
	double center_x = (start_x + end_x) / 2 - 32;
	double center_y = (start_y + end_y) / 2 - 32;
	draw_edge(edge_pen, start_x, start_y, center_x, center_y, hdc);
	draw_arrowed_edge(edge_pen, center_x, center_y, end_x, end_y, hdc);
}

void draw_directed_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex, int vertices_count)
{
	double multiplier = 1.0 - N3 * 0.01 - N4 * 0.01 - 0.3;
	mult_matrix(matrix, MATRIX_SIZE, multiplier);

	SelectObject(hdc, edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < vertices_count; row++)
	{
		for (col = 0; col < vertices_count; col++)
		{
			if (matrix[row][col])
			{
				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);
				double end_y = calc_y(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

				if (matrix[col][row] && row > col)
				{
					draw_arrowed_curve_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
				}
				else if (row == col)
				{

					draw_arrowed_reflect_edge(edge_pen, current_vertex, hdc);
				}
				else
				{

					draw_arrowed_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
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
}

void draw_undirected_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex, int vertices_count)
{
	double multiplier = 1.0 - N3 * 0.01 - N4 * 0.01 - 0.3;
	mult_matrix(matrix, MATRIX_SIZE, multiplier);
	make_matrix_symmetric(matrix, MATRIX_SIZE);

	SelectObject(hdc, edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < vertices_count; row++)
	{
		for (col = 0; col < vertices_count; col++)
		{
			if (matrix[row][col])
			{
				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);
				double end_y = calc_y(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

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
}
