#include <stdio.h>
#include <stdlib.h>
struct pollfd
{
	int                   			    fd;
	int                   			    type;
//	struct   fd_operations              fdopt;
};

int pollfd_array_input(struct pollfd **pfd_array, int pfd_array_len, struct pollfd *pfd_new)
{
	int i = 0;
	for(; i < pfd_array_len; ++i)
	{
		if(NULL == pfd_array[i])
		{
			pfd_array[i] = pfd_new;
			return 0;
		}
	}
	return -1;
}

void pollfd_array_foreach(struct pollfd **pfd_array, int pfd_array_len)
{	
	int i = 0;
	for(; i < pfd_array_len; ++i)
	{
		if(pfd_array[i])
		{
			printf("%d\t", pfd_array[i]->fd);
		}
	}
	printf("\n");
	
}

void pollfd_array_insert_sort(struct pollfd **pfd_array, int pfd_array_len)
{
	struct pollfd * tmp = NULL;
	int i = 1;
	int j = 0;
	for(; i < pfd_array_len; ++i)
	{
		tmp = pfd_array[i];
		j = i - 1;
		while((0 <= j) && (tmp->fd < pfd_array[j]->fd))
		{
			pfd_array[j+1] = pfd_array[j];
			--j;
		}
		pfd_array[j+1] = tmp;
	}
}

int pollfd_array_binary_get(struct pollfd **pfd_array, int pfd_array_len, int fd)
{
	int left = 0;
	int right = pfd_array_len - 1;
	while(left < right)
	{
		int mid = (left + right)/2;
		if(pfd_array[mid]->fd > fd)
		{
			right = mid - 1;
		}
		else if(pfd_array[mid]->fd < fd)
		{
			left = mid + 1;
		}
		else
			return mid;
	}
	return -1;
}

int main()
{
	struct pollfd **pfd = (struct pollfd **)malloc(sizeof(struct pollfd *)*10);

	srand(time(NULL));
	int i = 0;
	for(; i < 10; ++i)
	{
		pfd[i] = NULL;
	}
	
	for(i=0; i < 10; ++i)
	{
		struct pollfd *new = (struct pollfd *)malloc(sizeof(struct pollfd));
//		new->fd = new->type = rand()%100;
		new->fd = new->type = i;
		pollfd_array_input(pfd, 10, new);
		printf("%d\t", pfd[i]->fd);
	}
	printf("\n");
	pollfd_array_foreach(pfd, 10);

	pollfd_array_insert_sort(pfd, 10);
	pollfd_array_foreach(pfd, 10);

	printf("%d\n", pollfd_array_binary_get(pfd, 10, 8));
	return 0;
}
