#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
	Файл описує динамічний двовимірний масив, який використовується як матриця суміжності
	графа, що зображується; а також функції для роботи з цим масивом. Також додані функції,
	що використовуються для отримання матриці ваг.
*/
#define SEED 2110

double **create_matrix(int size)
{
	double **matrix;
	matrix = (double **)malloc(size * sizeof(double *));
	int i, j;
	for (i = 0; i < size; i++)
	{
		matrix[i] = (double *)malloc(size * sizeof(double));

		for (j = 0; j < size; j++)
		{
			matrix[i][j] = 0;
		}
	}
	return matrix;
}

void delete_matrix(int size, double **matrix)
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
	printf("Matrix have been deleted from memory!\n");
}

double get_rand_num(int min, int max)
{
	double random_num = ((double)rand() / RAND_MAX) * (max - min);
	return random_num;
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

void make_matrix_symmetric(int size, double **matrix)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (matrix[i][j])
			{
				matrix[j][i] = 1;
			}
		}
	}
}

void make_matrix_rounded(int size, double **matrix)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = round(matrix[i][j]);
		}
	}
}

void mulmr(int size, double coefficient, double **matrix)
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

void mult_matrix_by_num(int size, int number, double **matrix)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] *= number;
		}
	}
}

void mult_matrix_by_matrix(int size, double **result_matrix, double **matrix)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			result_matrix[i][j] *= matrix[i][j];
		}
	}
}

void add_matrix_to_matrix(int size, double **result_matrix, double **matrix)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			result_matrix[i][j] += matrix[i][j];
		}
	}
}

void print_matrix(int size, double **matrix)
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

double **get_matrix_Wt(int size, double **matrix_A)
{
	double **matrix_Wt = get_rand_matrix(size);
	mult_matrix_by_num(size, 100, matrix_Wt);
	mult_matrix_by_matrix(size, matrix_Wt, matrix_A);
	make_matrix_rounded(size, matrix_Wt);
	return matrix_Wt;
}

double **get_matrix_B(int size, double **matrix_Wt)
{
	double **matrix_B = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_B[i][j] = !matrix_Wt[i][j] ? 0 : 1;
		}
	}
	return matrix_B;
}

double **get_matrix_C(int size, double **matrix_B)
{
	double **matrix_C = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_C[i][j] = matrix_B[i][j] != matrix_B[j][i] ? 1 : 0;
		}
	}
	return matrix_C;
}

double **get_matrix_D(int size, double **matrix_B)
{
	double **matrix_D = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_D[i][j] = (matrix_B[i][j] == matrix_B[j][i] && matrix_B[i][j]) ? 1 : 0;
		}
	}
	return matrix_D;
}

double **get_matrix_Tr(int size)
{
	double **matrix_Tr = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			matrix_Tr[i][j] = 1;
		}
	}
	return matrix_Tr;
}

double **get_matrix_W(int size, double **matrix_Wt)
{
	double **matrix_W = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_W[i][j] = matrix_Wt[i][j] ? matrix_Wt[i][j] : matrix_Wt[j][i];
		}
	}
	return matrix_W;
}
