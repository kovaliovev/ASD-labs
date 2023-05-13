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

void get_product_of_matrices(double **result_matrix, double **first_matrix, double **second_matrix, int size)
{
	int i, j, k;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			double result = 0;
			for (k = 0; k < size; k++)
			{
				result += first_matrix[i][k] * second_matrix[k][j];
			}
			result_matrix[i][j] = result;
		}
	}
}

void get_logical_or(double **result_matrix, double **first_matrix, double **second_matrix, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			result_matrix[i][j] = first_matrix[i][j] || second_matrix[i][j];
		}
	}
}

void get_logical_and(double **result_matrix, double **first_matrix, double **second_matrix, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			result_matrix[i][j] = first_matrix[i][j] && second_matrix[i][j];
		}
	}
}

void get_copy_of_matrix(double **result_matrix, double **original_matrix, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			result_matrix[i][j] = original_matrix[i][j];
		}
	}
}

void print_pathes_2(double **matrix_pathes_2, double **matrix, int size)
{
	printf("\n\nAll pathes with length 2:\n");
	int i, j, k;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (matrix_pathes_2)
			{
				for (k = 0; k < size; k++)
				{
					if (matrix[i][k] && matrix[k][j])
					{
						printf("Vertex #%d -> Vertex #%d -> Vertex #%d\n", i + 1, k + 1, j + 1);
					}
				}
			}
		}
	}
}

void print_pathes_3(double **matrix_pathes_3, double **matrix, int size)
{
	printf("\n\nAll pathes with length 3:\n");
	int i, j, k, m;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (matrix_pathes_3)
			{
				for (k = 0; k < size; k++)
				{
					for (m = 0; m < size; m++)
					{
						if (matrix[i][k] && matrix[k][m] && matrix[m][j])
						{
							printf("Vertex #%d -> Vertex #%d -> Vertex #%d -> Vertex #%d\n", i + 1, k + 1, m + 1, j + 1);
						}
					}
				}
			}
		}
	}
}

double **get_reachability_matrix(double **matrix, int size)
{
	double **powered_matrix = create_matrix(size);
	double **temp_matrix = create_matrix(size);
	double **reachability_matrix = create_matrix(size);

	get_copy_of_matrix(powered_matrix, matrix, size);
	int i;
	for (i = 2; i < size + 1; i++)
	{
		get_logical_or(reachability_matrix, reachability_matrix, powered_matrix, size);
		get_copy_of_matrix(temp_matrix, powered_matrix, size);
		get_product_of_matrices(powered_matrix, temp_matrix, matrix, size);
		if (i == 2)
		{
			print_pathes_2(powered_matrix, matrix, size);
		}
		else if (i == 3)
		{
			print_pathes_3(powered_matrix, matrix, size);
		}
	}
	delete_matrix(powered_matrix, size);
	delete_matrix(temp_matrix, size);
	return reachability_matrix;
}

void get_transponed_matrix(double **result_matrix, double **original_matrix, int size)
{
	{
		int i, j;
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				result_matrix[i][j] = original_matrix[j][i];
			}
		}
	}
}

void get_components(double **matrix, int *checked, int *components, int index, int component_num, int size)
{
	checked[index] = 1;								 // позначаємо, що ця вершина була проглянута
	components[index] = component_num; // позначаємо, що ця вершина відноситься до певного компонента
	int i;
	for (i = 0; i < size; i++)
	{
		if (!checked[i] && matrix[index][i])
		{
			get_components(matrix, checked, components, i, component_num, size);
		}
	}
}

int *get_connectivity_components(double **strong_connectivity_matrix, int size) // отримання компонентів зв'язку, до яких належить кожна з вершин
{
	int component_num = 1;

	int *checked = (int *)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		checked[i] = 0;
	}

	int *components = (int *)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		if (!checked[i])
		{
			get_components(strong_connectivity_matrix, checked, components, i, component_num, size);
			component_num++;
		}
	}

	free(checked);
	return components;
}

int get_components_count(int *components, int size)
{
	int result = 0;
	int i;
	for (i = 0; i < size; i++)
	{
		result = components[i] > result ? components[i] : result;
	}
	return result;
}

double **get_components_matrix(double **reachability_matrix, int *components, int components_count, int size)
{
	double **components_matrix = create_matrix(components_count);

	int i, j, k;
	for (i = 0; i < components_count; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (components[j] == i + 1)
			{
				for (k = 0; k < size; k++)
				{
					if (components[k] != i + 1)
					{
						if (reachability_matrix[j][k])
						{
							components_matrix[i][components[k] - 1] = 1;
						}
						else if (reachability_matrix[k][j])
						{
							components_matrix[components[k] - 1][i] = 1;
						}
					}
				}
			}
		}
	}
	return components_matrix;
}