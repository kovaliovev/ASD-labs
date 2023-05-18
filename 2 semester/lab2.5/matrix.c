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

void bfs(int vertices_count, double **adjacency_matrix, int *visited, double **bfs_matrix, int bfs_n)
{
	List *queue = create_list(0);
	printf("Vertex %d was visited!\n", 1);
	while (queue != NULL && bfs_n > 0)
	{
		int active = queue->data;
		visited[active] = 1;
		bfs_n--;
		if (bfs_n < 1)
			break;
		int i;
		for (i = 0; i < vertices_count; i++)
		{
			if (adjacency_matrix[active][i])
			{
				if (!visited[i])
				{
					visited[i] = 2;
					push(&queue, i);
					bfs_matrix[active][i] = 1;
					printf("Vertex %d was visited!\n", i + 1);
					bfs_n--;
					if (bfs_n < 1)
						break;
				}
			}
		}
		if (i == vertices_count)
		{
			shift(&queue);
			visited[active] = 3;
			bfs_n--;
		}
	}
	printf("BFS STOPED!\n");
	delete_list(&queue);
}
