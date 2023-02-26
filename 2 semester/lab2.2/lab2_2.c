#include "List.c"

int main()
{
	List *p_list = create_list(0);
	delete_list(&p_list);
	print_list(p_list);
	return 0;
}
