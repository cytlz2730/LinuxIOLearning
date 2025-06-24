#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	FILE *fp = fopen("random.txt", "w+");
	if (fp == NULL)
	{
		perror("open");
		return -1;
	}

	srand(time(NULL));
	int size = random() % 5 + 10;
	for (int i = 0; i < size; i++)
	{
		fprintf(fp, "%d", random() % 100);
		if (i != size - 1)
			fprintf(fp, ",");
	}

	fseek(fp, 0, SEEK_SET);
	while (1)
	{
		int data;
		int ret = fscanf(fp, "%d", &data);
		if (ret == 0)
		{
			char c;
			int ret1 = fscanf(fp, "%c", &c);
			if (ret1 == 1)
				continue;
			else
				break;
			// getchar();
		}
		else if(ret==EOF)
			break;
		else
			printf("%d ", data);
		// getchar();
	}
	printf("\n");
	return 0;
}