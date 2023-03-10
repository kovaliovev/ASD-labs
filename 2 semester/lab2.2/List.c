#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct linked_list
{
	int data;
	struct linked_list *p_next;
	struct linked_list *p_prev;
} List;
// error-check functions
void check_mem_error(List *new_elem)
{
	if (new_elem == NULL)
	{
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

bool is_list_exist(List *p_head)
{
	if (p_head == NULL)
	{
		printf("Error! The list does not exist!\n\n");
		return false;
	}
	return true;
}
// basic functions
void push(List *p_head)
{
	if (!is_list_exist(p_head))
		return;

	List *new_elem = (List *)malloc(sizeof(List));
	check_mem_error(new_elem);

	printf("Please enter value to add in the end of list:\n");
	scanf("%d", &(new_elem->data));

	new_elem->p_next = NULL;
	new_elem->p_prev = NULL;

	printf("New list element was created successfully.\nIts value: %d\nIts address: %d\n", new_elem->data, new_elem);

	List *current = p_head;

	while (current->p_next != NULL)
	{
		current = current->p_next;
	}

	current->p_next = new_elem;
	new_elem->p_prev = current;
	printf("This element was pushed in the end of list.\n\n");
}

void unshift(List **pp_head)
{
	if (!is_list_exist(*pp_head))
		return;

	List *new_elem = (List *)malloc(sizeof(List));
	check_mem_error(new_elem);

	printf("Please enter value to add in the beginning of list:\n");
	scanf("%d", &(new_elem->data));

	new_elem->p_next = *pp_head;
	new_elem->p_prev = NULL;
	printf("New list element was created successfully.\nIts value: %d\nIts address: %d\n", new_elem->data, new_elem);

	(*pp_head)->p_prev = new_elem;
	(*pp_head) = new_elem;
	printf("This element was added in the begin of list.\n\n");
}

void pop(List **pp_head)
{
	if (!is_list_exist(*pp_head))
		return;

	List *current = (*pp_head);
	while (current->p_next != NULL)
	{
		current = current->p_next;
	}

	if (current->p_prev == NULL)
	{
		free(current);
		printf("Element was deleted from the end of list.\nThe list no longer exists.\n");
		(*pp_head) = NULL;
	}
	else
	{
		List *previous = current->p_prev;
		previous->p_next = NULL;
		printf("Element was deleted from the end of list.\n");
		free(current);
	}
	printf("Memory was freed! :)\n\n");
}

void shift(List **pp_head)
{
	if (!is_list_exist(*pp_head))
		return;

	List *current = (*pp_head);

	if (current->p_next == NULL)
	{
		free(current);
		printf("Element was deleted from the begin of list.\nThe list no longer exists.\n");
		(*pp_head) = NULL;
	}
	else
	{
		List *next = current->p_next;
		next->p_prev = NULL;
		(*pp_head) = next;
		printf("Element was deleted from the begin of list.\n");
		free(current);
	}
	printf("Memory was freed! :)\n\n");
}

void print_list(List *p_head)
{
	if (!is_list_exist(p_head))
		return;

	List *current = p_head;

	printf("Now list is: ");
	while (current->p_next != NULL)
	{
		printf("%d => ", current->data);
		current = current->p_next;
	}
	printf("%d", current->data);
	printf("\n\n");
}

void log_list(List *p_head)
{
	if (!is_list_exist(p_head))
		return;

	List *current = p_head;
	int counter = 1;
	printf("List logging:\n----------------------\n");
	while (current != NULL)
	{
		printf("Element of list #%d\n", counter);
		printf("Data: %d\n", current->data);
		printf("Address: %d\n", current);
		printf("Previous: %d\n", current->p_prev);
		printf("Next: %d\n", current->p_next);
		printf("----------------------\n");
		current = current->p_next;
		counter++;
	}
}

List *create_list(int length)
{
	if (length < 1)
	{
		printf("Error! Invalid length entered.\n\n");
		return NULL;
	}

	List *p_head = (List *)malloc(sizeof(List));
	check_mem_error(p_head);

	int counter = 1;
	printf("Please enter value to add at the #%d position of the list:\n", counter);
	scanf("%d", &(p_head->data));

	p_head->p_next = NULL;
	p_head->p_prev = NULL;

	List *current = p_head;
	while (counter < length)
	{
		List *new_elem = (List *)malloc(sizeof(List));
		check_mem_error(new_elem);

		printf("Please enter value to add at the #%d position of the list:\n", ++counter);
		scanf("%d", &(new_elem->data));
		new_elem->p_prev = current;
		new_elem->p_next = NULL;

		current->p_next = new_elem;
		current = current->p_next;
	}

	printf("List was created succsessfully!\n");
	print_list(p_head);
	return p_head;
}

void delete_list(List **pp_head)
{
	if (!is_list_exist(*pp_head))
		return;

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
// functions to do the task
int count_bigger_than(List *p_head, int number)
{
	if (!is_list_exist(p_head))
		return -1;

	int counter = 0;
	List *current = p_head;
	while (current != NULL)
	{
		if (current->data > number)
			counter++;
		current = current->p_next;
	}
	return counter;
}

void insert_after(List *p_head, int index)
{
	if (!is_list_exist(p_head))
		return;
	if (index < 1)
	{
		printf("Error! Invalid index of insertion.\n");
		return;
	}

	List *left_elem = p_head;
	int counter = 1;

	while (counter < index && left_elem->p_next != NULL)
	{
		left_elem = left_elem->p_next;
		counter++;
	}

	List *right_elem = left_elem->p_next;

	List *new_elem = (List *)malloc(sizeof(List));
	check_mem_error(new_elem);

	printf("Please enter value to add at the #%d position of list:\n", index + 1);
	scanf("%d", &(new_elem->data));
	new_elem->p_prev = left_elem;
	new_elem->p_next = right_elem;
	printf("New list element was created successfully.\nIts value: %d\nIts address: %d\n", new_elem->data, new_elem);

	left_elem->p_next = new_elem;
	if (right_elem != NULL)
	{
		right_elem->p_prev = new_elem;
	}
	printf("New list element was successfully added at #%d position.\n\n", index + 1);
}
