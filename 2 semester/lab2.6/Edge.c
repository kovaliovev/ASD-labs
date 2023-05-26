#include <stdio.h>
#include <stdlib.h>
/*
	Файл описує структуру даних, яка використовується для збереження інформації,
	пов'язаної з ребрами графа; а також функції потрібні для роботи із цією структурою;
	та бульбашкове сортування, що виконується для оптимізації роботи алгоритму Краскала.
*/
typedef struct edges_list
{
	int first_vertex_num;
	int second_vertex_num;
	int weight;
	struct edges_list *p_next;
} Edge;

Edge *create_edges(int size, double **weights_matrix)
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

void sort_edges_by_weight(Edge *p_head)
{
	int is_sorted = 0;
	while (!is_sorted)
	{
		is_sorted = 1;
		Edge *current = p_head;
		while (current->p_next != NULL)
		{
			Edge *next = current->p_next;
			if (current->weight > next->weight)
			{
				is_sorted = 0;

				int temp_first_vertex = current->first_vertex_num;
				int temp_second_vertex = current->second_vertex_num;
				int temp_weight = current->weight;

				current->first_vertex_num = next->first_vertex_num;
				current->second_vertex_num = next->second_vertex_num;
				current->weight = next->weight;

				next->first_vertex_num = temp_first_vertex;
				next->second_vertex_num = temp_second_vertex;
				next->weight = temp_weight;
			}
			current = current->p_next;
		}
	}
}