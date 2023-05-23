#include <stdio.h>
#include <stdlib.h>
/*
	Файл описує структуру даних, яка використовується для створення черги та стеку, 
	для виконання bfs та dfs обходів.
*/
typedef struct list
{
	int data;
	struct list *p_next;
} List;

List *create_list(int data)
{
	List *p_head = (List *)malloc(sizeof(List));
	if (p_head == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
	p_head->data = data;
	p_head->p_next = NULL;
	return p_head;
}

void unshift(List **pp_head, int data)
{
	List *new_elem = (List *)malloc(sizeof(List));
	if (new_elem == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}

	new_elem->p_next = *pp_head;
	new_elem->data = data;

	(*pp_head) = new_elem;
}

void push(List **pp_head, int data)
{
	List *current = (*pp_head);

	while (current->p_next != NULL)
	{
		current = current->p_next;
	}
	List *new_elem = (List *)malloc(sizeof(List));
	if (new_elem == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}

	new_elem->data = data;
	new_elem->p_next = NULL;
	current->p_next = new_elem;
}

void shift(List **pp_head)
{
	List *current = (*pp_head);

	(*pp_head) = current->p_next;
	free(current);
}

void print_list(List *p_head)
{
	List *current = p_head;

	printf("Now list is: ");
	while (current->p_next != NULL)
	{
		printf("%d <= ", current->data);
		current = current->p_next;
	}
	printf("%d\n\n", current->data);
}

void delete_list(List **pp_head)
{
	List *current = *pp_head;
	while (current != NULL)
	{
		List *previous = current;
		current = current->p_next;
		free(previous);
	}
	(*pp_head) = NULL;
	printf("The list has been deleted from memory!\n");
}
