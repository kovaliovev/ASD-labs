#include <windows.h>
#include "Vertex.c"
#include "matrix.c"
/*
	Файл описує усі функції, необхідні для малювання графа, dfs- та bfs- обходів.
*/
#define VERTICES_COUNT 11
#define VERTEX_RADIUS 32
#define VERTEX_DIAMETER (2 * VERTEX_RADIUS)
#define TEXT_MARGIN 5
#define WINDOW_RIGHT_TOP_CORNER_X 120
#define WINDOW_RIGHT_TOP_CORNER_Y 5
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 820
#define STANDART_GRAPH_MARGIN 335
#define STANDART_GRAPH_COEF 285
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

void draw_arrowed_reflect_edge(HPEN edge_pen, Vertex *vertex, HDC hdc)
{
	draw_reflect_edge(edge_pen, vertex, hdc);

	double rotate_angle;
	double arrow_x = vertex->x;
	double arrow_y = vertex->y;

	if (vertex->num <= ceil(VERTICES_COUNT / 4.0)) // 2 чверть
	{
		rotate_angle = to_radians(195.0);
		arrow_x -= 32;
	}
	else if (vertex->num <= ceil(VERTICES_COUNT / 4.0) * 2) // 1 чверть
	{
		rotate_angle = to_radians(345.0);
		arrow_x += 32;
	}
	else if (vertex->num <= ceil(VERTICES_COUNT / 4.0) * 3) // 4 чверть
	{
		rotate_angle = to_radians(15.0);
		arrow_x += 32;
	}
	else // 3 чверть
	{
		rotate_angle = to_radians(165.0);
		arrow_x -= 32;
	}
	draw_arrow(edge_pen, rotate_angle, arrow_x, arrow_y, VERTEX_RADIUS / 2, hdc);
}

void draw_arrowed_curve_edge(HPEN edge_pen, double start_x, double start_y, double end_x, double end_y, HDC hdc)
{
	int diff_x = abs(start_x - end_x);
	int diff_y = abs(start_y - end_y);
	double center_x, center_y;
	if (diff_x > diff_y)
	{
		center_x = (start_x + end_x) / 2;
		center_y = (start_y + end_y) / 2 - 45;
	}
	else
	{
		center_x = (start_x + end_x) / 2 - 45;
		center_y = (start_y + end_y) / 2;
	}
	draw_edge(edge_pen, start_x, start_y, center_x, center_y, hdc);
	draw_arrowed_edge(edge_pen, center_x, center_y, end_x, end_y, hdc);
}

void draw_legend(HDC hdc, int start_x, int start_y)
{
	HPEN default_pen = CreatePen(PS_SOLID, 3, RGB(50, 0, 255));
	HPEN active_pen = CreatePen(PS_SOLID, 4, RGB(34, 139, 34));
	HPEN new_pen = CreatePen(PS_SOLID, 4, RGB(57, 255, 20));
	HPEN closed_pen = CreatePen(PS_SOLID, 4, RGB(255, 165, 0));

	SelectObject(hdc, default_pen);
	Ellipse(hdc, start_x + VERTEX_DIAMETER, start_y + VERTEX_DIAMETER, start_x, start_y);
	TextOut(hdc, start_x + VERTEX_DIAMETER * 1.2, start_y + VERTEX_RADIUS / 1.2, "NOT VISITED VERTEX", 19);
	start_x += 320;

	SelectObject(hdc, active_pen);
	Ellipse(hdc, start_x + VERTEX_DIAMETER, start_y + VERTEX_DIAMETER, start_x, start_y);
	TextOut(hdc, start_x + VERTEX_DIAMETER * 1.2, start_y + VERTEX_RADIUS / 1.2, "ACTIVE VERTEX", 14);
	start_x += 320;

	SelectObject(hdc, new_pen);
	Ellipse(hdc, start_x + VERTEX_DIAMETER, start_y + VERTEX_DIAMETER, start_x, start_y);
	TextOut(hdc, start_x + VERTEX_DIAMETER * 1.2, start_y + VERTEX_RADIUS / 1.2, "VISITED VERTEX", 15);
	start_x += 320;

	SelectObject(hdc, closed_pen);
	Ellipse(hdc, start_x + VERTEX_DIAMETER, start_y + VERTEX_DIAMETER, start_x, start_y);
	TextOut(hdc, start_x + VERTEX_DIAMETER * 1.2, start_y + VERTEX_RADIUS / 1.2, "CLOSED VERTEX", 14);
	start_x += 320;
}

void draw_directed_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex, int vertices_count)
{
	Rectangle(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

	draw_legend(hdc, 50, 700);
}

void draw_visited_vertices(HDC hdc, int *order, int vertices_count, int start_x, int start_y)
{
	int is_all_visited = 1;
	int i;
	for (i = 0; i < vertices_count; i++)
	{
		if (order[i])
		{
			char message[29];
			sprintf(message, "[%d] Vertex #%d was visited!", i + 1, order[i]);
			TextOut(hdc, start_x, start_y, message, 27);
			start_y += 32;
		}
		else
		{
			is_all_visited = 0;
		}
	}
	if (is_all_visited)
	{
		TextOut(hdc, start_x, start_y, "All vertices were visited!", 27);
	}
}

void draw_bfs(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex, int vertices_count, int bfs_step)
{
	int bfs_visited[VERTICES_COUNT];
	int bfs_order[VERTICES_COUNT];
	int i;
	for (i = 0; i < VERTICES_COUNT; i++)
	{
		bfs_visited[i] = 0;
		bfs_order[i] = 0;
	}
	double **bfs_matrix = create_matrix(VERTICES_COUNT);

	HPEN active_pen = CreatePen(PS_SOLID, 4, RGB(34, 139, 34));
	HPEN new_pen = CreatePen(PS_SOLID, 4, RGB(57, 255, 20));
	HPEN closed_pen = CreatePen(PS_SOLID, 4, RGB(255, 165, 0));
	HPEN bfs_edge_pen = CreatePen(PS_SOLID, 3, RGB(118, 255, 122));

	bfs(VERTICES_COUNT, matrix, bfs_visited, bfs_matrix, bfs_step, bfs_order);
	printf("\nBFS matrix:\n");
	print_matrix(bfs_matrix, VERTICES_COUNT);

	SelectObject(hdc, bfs_edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < vertices_count; row++)
	{
		for (col = 0; col < vertices_count; col++)
		{
			if (bfs_matrix[row][col])
			{
				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);
				double end_y = calc_y(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

				draw_arrowed_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
			}
		}
		current_vertex = current_vertex->p_next;
	}

	current_vertex = vertex;
	while (current_vertex != NULL)
	{
		switch (bfs_visited[current_vertex->num - 1])
		{
		case ACTIVE_VERTEX_CODE:
			draw_vertex(active_pen, current_vertex, hdc);
			break;
		case NEW_VERTEX_CODE:
			draw_vertex(new_pen, current_vertex, hdc);
			break;
		case CLOSED_VERTEX_CODE:
			draw_vertex(closed_pen, current_vertex, hdc);
			break;
		}

		current_vertex = current_vertex->p_next;
	}
	delete_matrix(bfs_matrix, VERTICES_COUNT);

	draw_visited_vertices(hdc, bfs_order, VERTICES_COUNT, 870, 100);
}

void draw_dfs(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex, int vertices_count, int dfs_step)
{
	int dfs_visited[VERTICES_COUNT];
	int dfs_order[VERTICES_COUNT];
	int i;
	for (i = 0; i < VERTICES_COUNT; i++)
	{
		dfs_visited[i] = 0;
		dfs_order[i] = 0;
	}
	double **dfs_matrix = create_matrix(VERTICES_COUNT);

	HPEN active_pen = CreatePen(PS_SOLID, 4, RGB(34, 139, 34));
	HPEN new_pen = CreatePen(PS_SOLID, 4, RGB(57, 255, 20));
	HPEN closed_pen = CreatePen(PS_SOLID, 4, RGB(255, 165, 0));
	HPEN bfs_edge_pen = CreatePen(PS_SOLID, 3, RGB(118, 255, 122));

	dfs(VERTICES_COUNT, matrix, dfs_visited, dfs_matrix, dfs_step, dfs_order);
	printf("\nDFS matrix:\n");
	print_matrix(dfs_matrix, VERTICES_COUNT);

	SelectObject(hdc, bfs_edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < vertices_count; row++)
	{
		for (col = 0; col < vertices_count; col++)
		{
			if (dfs_matrix[row][col])
			{
				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);
				double end_y = calc_y(360.0 / vertices_count, col, STANDART_GRAPH_MARGIN, STANDART_GRAPH_COEF);

				draw_arrowed_edge(edge_pen, start_x, start_y, end_x, end_y, hdc);
			}
		}
		current_vertex = current_vertex->p_next;
	}

	current_vertex = vertex;
	while (current_vertex != NULL)
	{
		switch (dfs_visited[current_vertex->num - 1])
		{
		case ACTIVE_VERTEX_CODE:
			draw_vertex(active_pen, current_vertex, hdc);
			break;
		case NEW_VERTEX_CODE:
			draw_vertex(new_pen, current_vertex, hdc);
			break;
		case CLOSED_VERTEX_CODE:
			draw_vertex(closed_pen, current_vertex, hdc);
			break;
		}

		current_vertex = current_vertex->p_next;
	}
	delete_matrix(dfs_matrix, VERTICES_COUNT);

	draw_visited_vertices(hdc, dfs_order, VERTICES_COUNT, 870, 100);
}
