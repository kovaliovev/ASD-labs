#include <stdio.h>
#include <stdlib.h>

typedef struct list
{
	int data;
	struct list *p_next;
} List;

void check_mem_error(List *new_elem)
{
	if (new_elem == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

List *create_list(int data)
{
	List *p_head = (List *)malloc(sizeof(List));
	check_mem_error(p_head);
	p_head->data = data;
	p_head->p_next = NULL;
	return p_head;
}

void unshift(List **pp_head, int data)
{
	List *new_elem = (List *)malloc(sizeof(List));
	check_mem_error(new_elem);

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
	check_mem_error(new_elem);

	current->p_next = new_elem;
	new_elem->data = data;
	new_elem->p_next = NULL;
}

int shift(List **pp_head)
{
	List *current = (*pp_head);
	int data = current->data;

	(*pp_head) = current->p_next;
	printf("\nElement was deleted from the begin of list.\n");
	free(current);
	return data;
}

void print_list(List *p_head)
{
	List *current = p_head;

	printf("Now list is: ");
	while (current->p_next != NULL)
	{
		printf("%d => ", current->data);
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
		printf("Memory was freed! :)\n");
	}
	(*pp_head) = NULL;
	printf("The list no longer exists!\n\n");
}
