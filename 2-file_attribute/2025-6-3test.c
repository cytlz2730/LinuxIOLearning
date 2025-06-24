#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

// void display_dir()

int main() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_type==DT_DIR) printf("目录: ");
        else if(entry->d_type==DT_REG) printf("普通文件: ");
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
