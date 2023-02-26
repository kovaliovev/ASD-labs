#include "List.c"

int main()
{
	List *p_list = create_list(3);
	log_list(p_list);
	push(p_list);
	print_list(p_list);
	delete_list(&p_list);
	push(p_list);
	return 0;
}
