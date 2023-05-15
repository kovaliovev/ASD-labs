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

	(*pp_head) = new_elem;
}

void pop(List **pp_head)
{
	List *current = (*pp_head);
	if (current->p_next == NULL)
	{
		(*pp_head) = NULL;
	}
	while (current->p_next != NULL)
	{
		current = current->p_next;
	}
	printf("\nElement was deleted from the end of list.\n");
	free(current);
	printf("Memory was freed! :)\n\n");
}

void shift(List **pp_head)
{
	List *current = (*pp_head);

	(*pp_head) = current->p_next;
	printf("\nElement was deleted from the begin of list.\n");
	free(current);
}