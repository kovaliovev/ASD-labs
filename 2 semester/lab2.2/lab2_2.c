#include "List.c"

int main()
{
	List *p_list = list_init();
	print_list(p_list);
	push(p_list);
	push(p_list);
	print_list(p_list);
	unshift(&p_list);
	unshift(&p_list);
	push(p_list);
	print_list(p_list);
	insert_after(p_list, 3);
	print_list(p_list);
	log_list(p_list);
	delete_list(&p_list);
	print_list(p_list);
	return 0;
}
