#include <windows.h>
#include <stdbool.h>
#include "Vertex.c"
/*
	Файл описує усі функції, необхідні для малювання напрямнелого/ненапрямленого графа.
*/
#define VERTICES_COUNT 11
#define VERTEX_RADIUS 32
#define VERTEX_DIAMETER (2 * VERTEX_RADIUS)
#define TEXT_MARGIN 5

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

void show_degrees(Vertex *start_vertex, HDC hdc, int start_x, int start_y, bool is_directed)
{
	Vertex *current_vertex = start_vertex;
	int text_x = start_x;
	int text_y = start_y;

	TextOut(hdc, text_x, text_y, "Degrees of graph's vertices:", 29);
	text_y += 30;

	bool is_homogeneous = true;
	int degree, last_degree;
	while (current_vertex != NULL)
	{
		int num = current_vertex->num;
		if (is_directed)
		{
			if (num == 1)
			{
				TextOut(hdc, text_x, text_y, "VERTEX | INPUT | OUTPUT", 24);
			}
			text_y += 21;
			char message[29];
			sprintf(message, "Vertex #%d | %d deg- | %d deg+", num, current_vertex->deg_in, current_vertex->deg_out);
			TextOut(hdc, text_x, text_y, message, 28);
		}
		else
		{
			last_degree = degree;
			degree = current_vertex->deg_in + current_vertex->deg_out;
			if (degree != last_degree)
				is_homogeneous = false;

			if (num == 1)
			{
				TextOut(hdc, text_x, text_y, "VERTEX | DEGREE", 16);
			}
			text_y += 21;
			char message[20];
			sprintf(message, "Vertex #%d | %d deg", num, degree);
			TextOut(hdc, text_x, text_y, message, 18);
		}
		current_vertex = current_vertex->p_next;
	}
	if (!is_directed)
	{
		if (is_homogeneous)
		{
			char message[42];
			sprintf(message, "Wow! Graph is homogeneous! It's degree: %d", degree);
			TextOut(hdc, start_x, start_y - 30, message, 42);
		}
		else
		{
			TextOut(hdc, start_x, start_y - 30, "Graph is not homogeneous!", 26);
		}
	}
}

void show_specific_vertices(Vertex *start_vertex, HDC hdc, int start_x, int start_y)
{
	int text_x = start_x;
	int text_y = start_y;
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
		}
		else if (degree == 1)
		{
			char message[22];
			sprintf(message, "Vertex #%d is pendant!", num);
			TextOut(hdc, text_x, text_y, message, 22);
		}

		text_y += 21;
		current_vertex = current_vertex->p_next;
	}
}
