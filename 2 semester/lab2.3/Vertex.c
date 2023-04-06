#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct vertices_list
{
	int num;
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

double calcX(double rotateAngle, int i, int margin){
	double x = -cos((rotateAngle * 3.1416 / 180) * i) * 300 + margin;
	return x;
}

double calcY(double rotateAngle, int i, int margin){
	double y = -sin((rotateAngle * 3.1416 / 180) * i) * 300 + margin;
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
	double rotateAngle = 360 / count;
	Vertex* current = p_head;
	for (i = 0; i < count; i++) {
		current->num = i + 1;
		current->x = calcX(rotateAngle, i, margin);
		current->y = calcY(rotateAngle, i, margin);

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