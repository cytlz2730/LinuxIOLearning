#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>

// 读取文件信息
int display_dir(DIR *dirp, char *filename)
{
	struct dirent *dir = readdir(dirp);
	if (dir == NULL)
		return -1;

	if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
		return -3;

	char temp[30] = {0};
	strcpy(temp, dir->d_name);
	if (strstr(temp,filename) == NULL)
	{
		return -2;
	}

	// printf("type:%o\n",dir->d_type);
	if (dir->d_type == DT_DIR)
	{
		printf("目    录: %s\n", dir->d_name);
		return 1;
	}
	else if (dir->d_type == DT_REG)
	{
		printf("普通文件: %s\n", dir->d_name);
		return 2;
	}
	// return true;
}

// 编写一个程序搜索指定目录下的所有指定文件
// search_file("./",".c") ；
// 表示搜索当前目录下的所有.c文件
bool search_file(char *dirname, char *filename)
{
	DIR *dirp = opendir(dirname);
	if (dirp == NULL)
	{
		perror("open");
		return false;
	}

	while (1)
	{
		if (display_dir(dirp, filename) == -1)
			break;
	}

	closedir(dirp);
	return true;
}

int main(void)
{
	char dirname[30] = {0};
	char filename[30] = {0};
	printf("请输入要搜索的目录: ");
	scanf("%s", dirname);
	printf("请输入要搜索的文件特征: ");
	scanf("%s", filename);

	search_file(dirname, filename);

	return 0;
}