#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

double **get_matrix_B(double **matrix_Wt, int size)
{
	double **matrix_B = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_B[i][j] = matrix_Wt[i][j] == 0.0 ? 0.0 : 1.0;
		}
	}
	return matrix_B;
}

double **get_matrix_C(double **matrix_B, int size)
{
	double **matrix_C = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_C[i][j] = matrix_B[i][j] != matrix_B[j][i] ? 1.0 : 0.0;
		}
	}
	return matrix_C;
}

double **get_matrix_D(double **matrix_B, int size)
{
	double **matrix_D = create_matrix(size);
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix_D[i][j] = (matrix_B[i][j] == matrix_B[j][i] && matrix_B[i][j]) ? 1.0 : 0.0;
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
		for (j = 0; j < size; j++)
		{
			matrix_Tr[i][j] = i < j ? 1.0 : 0.0;
		}
	}
	return matrix_Tr;
}

double **get_matrix_W(double **matrix_Wt, int size)
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

void make_matrix_symmetric(double **matrix, int size)
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

void make_matrix_rounded(double **matrix, int size)
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

void mulmr(double **matrix, int size, double coefficient)
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

void mult_matrix_by_num(double **matrix, int size, double number)
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

void mult_matrix_by_matrix(double **result_matrix, int size, double **matrix)
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

void add_matrix_to_matrix(double **result_matrix, int size, double **matrix)
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
