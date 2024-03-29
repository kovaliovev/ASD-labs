#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "List.c"
/*
	Файл описує динамічний двовимірний масив, який використовується як матриця суміжності
	графа, що зображується; а також функції для роботи з цим масивом.
*/
#define MATRIX_SIZE 11
#define SEED 2110
#define ACTIVE_VERTEX_CODE 5590
#define NEW_VERTEX_CODE 6691
#define CLOSED_VERTEX_CODE 7792

double **create_matrix(int size)
{
	double **matrix;
	matrix = (double **)malloc(size * sizeof(double *));
	int i, j;
	for (i = 0; i < size; i++)
	{
		matrix[i] = (double *)malloc(size * sizeof(double));
	}
	// заповнення створеної матриці нулями
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = 0;
		}
	}
	return matrix;
}

void delete_matrix(double **matrix, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
	printf("Matrix have been deleted from memory!\n");
}

double get_rand_num(double min, double max)
{
	double randomNum = ((double)rand() / RAND_MAX) * (max - min);
	return randomNum;
}

double **get_rand_matrix(int size)
{
	srand(SEED);
	double **matrix = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = get_rand_num(0, 2);
		}
	}
	return matrix;
}

void mult_matrix(double **matrix, int size, double coefficient)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = floor(matrix[i][j] * coefficient);
		}
	}
}

void print_matrix(double **matrix, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf("%.0lf ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void bfs(int vertices_count, double **adjacency_matrix, int *visited, double **bfs_matrix, int bfs_step, int *bfs_order)
{
	int order = 0;
	List *queue = create_list(0);
	bfs_order[order++] = 1;

	while (queue != NULL && bfs_step > 0)
	{
		int active = queue->data;
		visited[active] = ACTIVE_VERTEX_CODE;
		if (--bfs_step < 1)
			break;
		int i;

		for (i = 0; i < vertices_count; i++)
		{
			if (adjacency_matrix[active][i] && !visited[i])
			{
				visited[i] = NEW_VERTEX_CODE;
				bfs_order[order++] = i + 1;
				push(&queue, i);
				bfs_matrix[active][i] = 1;

				if (--bfs_step < 1)
					break;
			}
		}
		if (i == vertices_count)
		{
			shift(&queue);
			visited[active] = CLOSED_VERTEX_CODE;
			bfs_step--;
		}
	}

	delete_list(&queue);
}

void dfs(int vertices_count, double **adjacency_matrix, int *visited, double **dfs_matrix, int dfs_step, int *dfs_order)
{
	int order = 0;
	List *stack = create_list(0);
	visited[0] = ACTIVE_VERTEX_CODE;
	dfs_order[order++] = 1;
	--dfs_step;

	while (stack != NULL && dfs_step > 0)
	{
		int active = stack->data;
		visited[active] = ACTIVE_VERTEX_CODE;
		int i;
		int is_new_vertex_found = 0;

		for (i = 0; i < vertices_count; i++)
		{
			if (adjacency_matrix[active][i] && !visited[i])
			{
				visited[i] = ACTIVE_VERTEX_CODE;
				visited[active] = NEW_VERTEX_CODE;
				dfs_order[order++] = i + 1;
				is_new_vertex_found = 1;
				unshift(&stack, i);
				dfs_matrix[active][i] = 1;
				active = i;

				if (--dfs_step < 1)
					break;
			}
		}

		if (!is_new_vertex_found)
		{
			shift(&stack);
			if (stack == NULL)
			{
				int j;
				for (j = 0; j < vertices_count; j++)
				{
					visited[j] = CLOSED_VERTEX_CODE;
				}
			}
			else
			{
				visited[active] = CLOSED_VERTEX_CODE;
				active = stack->data;
				visited[active] = ACTIVE_VERTEX_CODE;
				dfs_step--;
			}
		}
	}

	delete_list(&stack);
}