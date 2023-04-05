#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node_list
{
	int num;
	double x;
	double y;
	struct node_list *p_next;
} Nodes;

void check_mem_error(Nodes *new_elem)
{
	if (new_elem == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

Nodes *create_nodes(double count, int margin)
{
	if (count < 1)
	{
		printf("Error! Invalid count entered.\n\n");
		return NULL;
	}

	Nodes *p_head = (Nodes *)malloc(sizeof(Nodes));
	check_mem_error(p_head);

	double i;
	double rotateAngle = 360 / count;
	Nodes* current = p_head;
	for (i = 0; i < count; i++) {
		current->num = i + 1;
		current->x = -cos((rotateAngle * 3.1416 / 180) * i) * 250.0 + margin;
		current->y = -sin((rotateAngle * 3.1416 / 180) * i) * 250.0 + margin;

		if (i + 1 == count) {
			current->p_next = NULL;
			break;
		}
		Nodes * new_elem = (Nodes*)malloc(sizeof(Nodes));
		check_mem_error(new_elem);
		current->p_next = new_elem;
		current = current->p_next;
	}

	return p_head;
}

void delete_nodes(Nodes **pp_head)
{
	Nodes *current = *pp_head;
	while (current != NULL)
	{
		Nodes *previous = current;
		current = current->p_next;
		free(previous);
		printf("Memory was freed! :)\n");
	}
	(*pp_head) = NULL;
	printf("Nodes are no exist!\n\n");
}