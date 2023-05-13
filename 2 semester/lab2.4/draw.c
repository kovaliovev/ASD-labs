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
#define GRAPH_MARGIN 350
#define ANGLE_BETWEEN_VERTICES (360.0 / VERTICES_COUNT)
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

void show_degrees(Vertex *start_vertex, HDC hdc, int start_x, int start_y, bool* is_homogeneous)
{
	Vertex *current_vertex = start_vertex;
	int text_x = start_x;
	int text_y = start_y;

	TextOut(hdc, text_x, text_y, "Degrees of graph's vertices:", 29);
	text_y += 30;

	int degree = current_vertex->deg_in + current_vertex->deg_out;

	int last_degree;
	while (current_vertex != NULL)
	{
		int num = current_vertex->num;

		last_degree = degree;
		degree = current_vertex->deg_in + current_vertex->deg_out;
		if (degree != last_degree)
			*is_homogeneous = false;

		if (num == 1)
		{
			TextOut(hdc, text_x, text_y, "VERTEX | DEGREE", 16);
		}
		text_y += 21;

		char message[20];
		sprintf(message, "Vertex #%d | %d deg", num, degree);
		TextOut(hdc, text_x, text_y, message, 18);

		current_vertex = current_vertex->p_next;
	}
}

void show_semidegrees(Vertex *start_vertex, HDC hdc, int start_x, int start_y)
{
	Vertex *current_vertex = start_vertex;
	int text_x = start_x;
	int text_y = start_y;

	TextOut(hdc, text_x, text_y, "Degrees of graph's vertices:", 29);
	text_y += 30;

	while (current_vertex != NULL)
	{
		int num = current_vertex->num;
		if (num == 1)
		{
			TextOut(hdc, text_x, text_y, "VERTEX | INPUT | OUTPUT", 24);
		}
		text_y += 21;

		char message[29];
		sprintf(message, "Vertex #%d | %d deg- | %d deg+", num, current_vertex->deg_in, current_vertex->deg_out);
		TextOut(hdc, text_x, text_y, message, 28);

		current_vertex = current_vertex->p_next;
	}
}

void show_specific_vertices(Vertex *start_vertex, HDC hdc, int start_x, int start_y)
{
	int text_x = start_x;
	int text_y = start_y;

	int isolated = 0;
	int pendant = 0;

	Vertex *current_vertex = start_vertex;
	while (current_vertex != NULL)
	{
		int num = current_vertex->num;
		int degree = current_vertex->deg_in + current_vertex->deg_out;

		if (degree == 0)
		{
			char message[23];
			sprintf(message, "Vertex #%d is isolated!", num);
			TextOut(hdc, text_x, text_y, message, 23);
			isolated++;
		}
		else if (degree == 1)
		{
			char message[22];
			sprintf(message, "Vertex #%d is pendant!", num);
			TextOut(hdc, text_x, text_y, message, 22);
			pendant++;
		}

		text_y += 21;
		current_vertex = current_vertex->p_next;
	}
	if (!isolated)
	{
		TextOut(hdc, start_x, start_y - 45, "Graph has not isolated vertices!", 33);
		start_y += 20;
	}
	if (!pendant)
	{
		TextOut(hdc, start_x, start_y - 45, "Graph has not pendant vertices!", 32);
	}
}

void print_pathes_2(double **matrix_pathes_2, double **matrix, int size)
{
	printf("\n\nAll pathes with length 2:\n");
	int i, j, k;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (matrix_pathes_2)
			{
				for (k = 0; k < size; k++)
				{
					if (matrix[i][k] && matrix[k][j])
					{
						printf("Vertex #%d -> Vertex #%d -> Vertex #%d\n", i + 1, k + 1, j + 1);
					}
				}
			}
		}
	}
}

void print_pathes_3(double **matrix_pathes_3, double **matrix, int size)
{
	printf("\n\nAll pathes with length 3:\n");
	int i, j, k, m;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (matrix_pathes_3)
			{
				for (k = 0; k < size; k++)
				{
					for (m = 0; m < size; m++)
					{
						if (matrix[i][k] && matrix[k][m] && matrix[m][j])
						{
							printf("Vertex #%d -> Vertex #%d -> Vertex #%d -> Vertex #%d\n", i + 1, k + 1, m + 1, j + 1);
						}
					}
				}
			}
		}
	}
}

double **get_reachability_matrix(double **matrix, int size)
{
	double **powered_matrix = create_matrix(size);
	double **temp_matrix = create_matrix(size);
	double **reachability_matrix = create_matrix(size);

	get_copy_of_matrix(powered_matrix, matrix, size);
	int i;
	for (i = 2; i < size + 1; i++)
	{
		get_logical_or(reachability_matrix, reachability_matrix, powered_matrix, size);
		get_copy_of_matrix(temp_matrix, powered_matrix, size);
		get_product_of_matrices(powered_matrix, temp_matrix, matrix, size);
		if (i == 2)
		{
			print_pathes_2(powered_matrix, matrix, size);
		}
		else if (i == 3)
		{
			print_pathes_3(powered_matrix, matrix, size);
		}
	}
	delete_matrix(powered_matrix, size);
	delete_matrix(temp_matrix, size);
	return reachability_matrix;
}

void draw_directed_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex)
{
	double multiplier = 1.0 - N3 * 0.01 - N4 * 0.01 - 0.3;
	mult_matrix(matrix, MATRIX_SIZE, multiplier);

	SelectObject(hdc, edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < VERTICES_COUNT; row++)
	{
		for (col = 0; col < VERTICES_COUNT; col++)
		{
			if (matrix[row][col])
			{
				current_vertex->deg_out++;

				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);
				double end_y = calc_y(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);

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
			if (matrix[col][row])
			{
				current_vertex->deg_in++;
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
	show_semidegrees(vertex, hdc, 720, 50);
}

void draw_undirected_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex)
{
	double multiplier = 1.0 - N3 * 0.01 - N4 * 0.01 - 0.3;
	mult_matrix(matrix, MATRIX_SIZE, multiplier);
	make_matrix_symmetric(matrix, MATRIX_SIZE);

	SelectObject(hdc, edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < VERTICES_COUNT; row++)
	{
		for (col = 0; col < VERTICES_COUNT; col++)
		{
			if (matrix[row][col])
			{
				current_vertex->deg_out++;

				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);
				double end_y = calc_y(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);

				if (row == col)
				{
					draw_reflect_edge(edge_pen, current_vertex, hdc);
					current_vertex->deg_in++;
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
	bool is_homogeneous = true;
	show_degrees(vertex, hdc, 720, 50, &is_homogeneous);
	if (is_homogeneous)
	{
		char message[42];
		sprintf(message, "Wow! Graph is homogeneous! It's degree: %d", (vertex->deg_in + vertex->deg_out));
		TextOut(hdc, 720, 20, message, 42);
	}
	else
	{
		TextOut(hdc, 720, 20, "Graph is not homogeneous!", 26);
	}

	show_specific_vertices(vertex, hdc, 920, 100);
}

void draw_modified_graph(HDC hdc, HPEN vertex_pen, HPEN edge_pen, double **matrix, Vertex *vertex)
{
	double multiplier = 1.0 - N3 * 0.005 - N4 * 0.005 - 0.27;
	mult_matrix(matrix, MATRIX_SIZE, multiplier);

	SelectObject(hdc, edge_pen);

	Vertex *current_vertex = vertex;
	int row, col;
	for (row = 0; row < VERTICES_COUNT; row++)
	{
		for (col = 0; col < VERTICES_COUNT; col++)
		{
			if (matrix[row][col])
			{
				current_vertex->deg_out++;

				double start_x = current_vertex->x;
				double start_y = current_vertex->y;

				double end_x = calc_x(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);
				double end_y = calc_y(ANGLE_BETWEEN_VERTICES, col, GRAPH_MARGIN);

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
			if (matrix[col][row])
			{
				current_vertex->deg_in++;
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
	// вивід напівстепенів вершин
	show_semidegrees(vertex, hdc, 720, 50);

	// вивід матриці досяжності + шляхів довжиною 2 та 3
	double **reachability_matrix = get_reachability_matrix(matrix, VERTICES_COUNT);
	printf("\nReachability matrix of the depicted graph:\n\n");
	print_matrix(reachability_matrix, VERTICES_COUNT);

	// очищення пам'яті
	delete_matrix(reachability_matrix, VERTICES_COUNT);
}