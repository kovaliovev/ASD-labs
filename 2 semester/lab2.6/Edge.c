#include <stdio.h>
#include <stdlib.h>
/*
	...
*/
typedef struct edges_list
{
	int first_vertex_num;
	int second_vertex_num;
	int weight;
	struct edges_list *p_next;
} Edge;

Edge *create_edges(double **weights_matrix, int size)
{
	Edge *p_head = (Edge *)malloc(sizeof(Edge));
	if (p_head == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
	Edge *current = p_head;
	Edge *last;
	int row, col;
	for (row = 0; row < size; row++)
	{
		for (col = row + 1; col < size; col++)
		{
			if (weights_matrix[row][col])
			{
				current->first_vertex_num = row + 1;
				current->second_vertex_num = col + 1;
				current->weight = (int)weights_matrix[row][col];

				Edge *next = (Edge *)malloc(sizeof(Edge));
				if (next == NULL)
				{
					printf("Memory allocation failed!\n");
					exit(1);
				}
				current->p_next = next;
				last = current;
				current = next;
			}
		}
	}
	free(current);
	last->p_next = NULL;

	return p_head;
}

void print_edges(Edge *p_head)
{
	Edge *current = p_head;
	while (current != NULL)
	{
		printf("Edge info: [%d] ----- (%d) ----- [%d]\n", current->first_vertex_num, current->weight, current->second_vertex_num);
		current = current->p_next;
	}
}

void delete_edges(Edge **pp_head)
{
	Edge *current = *pp_head;
	while (current != NULL)
	{
		Edge *next = current;
		current = current->p_next;
		free(next);
	}
	(*pp_head) = NULL;
	printf("Edges have been deleted from memory!\n");
}