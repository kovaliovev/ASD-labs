#include "List.c"

int main()
{
	int len;
	printf("Enter the length of list: \n");
	scanf("%d", &len);

	List *p_list = create_list(len);

	int num;
	printf("Enter the number to compare with list's elements: \n");
	scanf("%d", &num);
	printf("Number of list's elements that bigger than %d: %d\n\n", num, count_bigger_than(p_list, num));

	int amount;
	int index;
	printf("Enter the amount of elements you want to insert: \n");
	scanf("%d", &amount);
	printf("Enter the index of the element after which you want to insert: (>= 1) \n");
	scanf("%d", &index);

	for (int i = 0; i < amount; i++)
	{
		insert_after(p_list, index++);
	}

	print_list(p_list);

	delete_list(&p_list);
	return 0;
}
