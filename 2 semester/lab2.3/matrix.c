#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MATRIX_SIZE 11
#define N3 1
#define N4 0
#define SEED 2110

double **createMatrix(int size)
{
	double **matrix;
	matrix = (double **)malloc(size * sizeof(double *));
	int i;
	for (i = 0; i < size; i++)
	{
		matrix[i] = (double *)malloc(size * sizeof(double));
	}
	return matrix;
}

void deleteMatrix(double **matrix, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
	printf("Matrix have been deleted from memory!\n");
}

double getRandomNum(double min, double max)
{
	double randomNum = ((double)rand() / RAND_MAX) * (max - min);
	return randomNum;
}

double **getRandomMatrix(int size)
{
	srand(SEED);
	double **matrix = createMatrix(size);
	int i;
	int j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = getRandomNum(0, 2);
		}
	}
	return matrix;
}

void multMatrix(double** matrix, int size){
	double coefficient = 1.0 - N3 * 0.02 - N4 * 0.005 - 0.25;
	int i;
	int j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = floor(matrix[i][j] * coefficient);
		}
	}
}

void printMatrix(double **matrix, int size)
{
	int i;
	int j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf("%.0lf ", matrix[i][j]);
		}
		printf("\n");
	}
}