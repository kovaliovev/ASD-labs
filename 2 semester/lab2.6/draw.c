#include <windows.h>
#include <stdbool.h>
#include "Vertex.c"
#include "Edge.c"
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
#define GRAPH_X_MARGIN 410
#define GRAPH_Y_MARGIN 395
#define GRAPH_COEF 325
// Кольори:
#define COL_BLACK RGB(0, 0, 0)
#define COL_BLUE RGB(50, 0, 255)
#define COL_RED RGB(255, 0, 0)
#define COL_DARK_ORANGE RGB(255, 140, 0)
#define COL_FOREST_GREEN RGB(34, 139, 34)
#define COL_DARK_VIOLET RGB(148, 0, 211)
#define COL_MEDIUM_VIOLET_RED RGB(199, 21, 133)
#define COL_SIENNA RGB(160, 82, 45)
#define COL_DARK_AQUA RGB(7, 144, 121)

COLORREF get_random_color()
{
	COLORREF COLORS[] = {COL_RED, COL_DARK_ORANGE, COL_FOREST_GREEN, COL_DARK_VIOLET, COL_MEDIUM_VIOLET_RED, COL_SIENNA, COL_BLACK, COL_DARK_AQUA};
	int random_number = round(get_rand_num(0, 7));
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

	TextOut(hdc, text_x, text_y, vertex_name, strlen(vertex_name));
}

void draw_edge(HDC hdc, HPEN edge_pen, int start_x, int start_y, int end_x, int end_y)
{
	MoveToEx(hdc, start_x, start_y, NULL);
	LineTo(hdc, end_x, end_y);
}

void draw_reflect_edge(HDC hdc, HPEN edge_pen, Vertex *vertex)
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
				SetTextColor(hdc, rand_color);
				SelectObject(hdc, edge_pen);
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

					int center_x = (start_x + end_x) / 2 - 10;
					int center_y = (start_y + end_y) / 2 - 10;

					char weight[4];
					sprintf(weight, "%d", current_edge->weight);
					TextOut(hdc, center_x, center_y, weight, strlen(weight));

					draw_edge(hdc, edge_pen, start_x, start_y, end_x, end_y);
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
