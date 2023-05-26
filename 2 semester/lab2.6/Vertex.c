#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
	Файл описує структуру даних, яка використовується для розрахунку та збереження координат вершин;
	а також функції потрібні для роботи з цією структурою.
*/
#define PI 3.1416

typedef struct vertices_list
{
	int num;
	int x;
	int y;
	struct vertices_list *p_next;
} Vertex;

void check_mem_error(Vertex *new_elem)
{
	if (new_elem == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

double to_radians(int degrees)
{
	return degrees * PI / 180.0;
}

int calc_x(double rotate_angle, int num, int coefficient, int margin)
{
	int x = -cos(to_radians(rotate_angle * num)) * coefficient + margin;
	return x;
}

int calc_y(double rotate_angle, int num, int coefficient, int margin)
{
	int y = -sin(to_radians(rotate_angle * num)) * coefficient + margin;
	return y;
}

Vertex *create_vertices(int count, int coefficient, int x_margin, int y_margin)
{
	if (count < 1)
	{
		printf("Error! Invalid count entered.\n");
		return NULL;
	}

	Vertex *p_head = (Vertex *)malloc(sizeof(Vertex));
	check_mem_error(p_head);

	double rotate_angle = 360.0 / (double)count;
	Vertex *current = p_head;
	int i;
	for (i = 0; i < count; i++)
	{
		current->num = i + 1;
		current->x = calc_x(rotate_angle, i, coefficient, x_margin);
		current->y = calc_y(rotate_angle, i, coefficient, y_margin);

		if (i + 1 == count)
		{
			current->p_next = NULL;
			break;
		}
		Vertex *new_elem = (Vertex *)malloc(sizeof(Vertex));
		check_mem_error(new_elem);
		current->p_next = new_elem;
		current = current->p_next;
	}

	return p_head;
}

void delete_vertices(Vertex **pp_head)
{
	Vertex *current = *pp_head;
	while (current != NULL)
	{
		Vertex *previous = current;
		current = current->p_next;
		free(previous);
	}
	(*pp_head) = NULL;
	printf("Vertices have been deleted from memory!\n");
}