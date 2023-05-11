#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
	Файл описує структуру даних, яка використовується для розрахунку та збережння координат вершин;
	а також функції потрібні для роботи з цією структурою.
*/
#define PI 3.1416

typedef struct vertices_list
{
	int num;
	int deg_in;
	int deg_out;
	double x;
	double y;
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

double to_radians(double degrees){
	return degrees * PI / 180.0;
}

double calc_x(double rotate_angle, int i, int margin){
	double x = -cos(to_radians(rotate_angle * i)) * 300 + margin;
	return x;
}

double calc_y(double rotate_angle, int i, int margin){
	double y = -sin(to_radians(rotate_angle * i)) * 300 + margin;
	return y;
}

Vertex *create_vertices(double count, int margin)
{
	if (count < 1)
	{
		printf("Error! Invalid count entered.\n");
		return NULL;
	}

	Vertex *p_head = (Vertex *)malloc(sizeof(Vertex));
	check_mem_error(p_head);

	double i;
	double rotate_angle = 360 / count;
	Vertex* current = p_head;
	for (i = 0; i < count; i++) {
		current->num = i + 1;
		current->deg_in = 0;
		current->deg_out = 0;
		current->x = calc_x(rotate_angle, i, margin);
		current->y = calc_y(rotate_angle, i, margin);

		if (i + 1 == count) {
			current->p_next = NULL;
			break;
		}
		Vertex * new_elem = (Vertex*)malloc(sizeof(Vertex));
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