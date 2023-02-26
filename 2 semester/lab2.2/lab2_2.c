#include "List.c"

int main()
{
	List *p_list = create_list(10);
	log_list(p_list);
	delete_list(&p_list);
	return 0;
}
