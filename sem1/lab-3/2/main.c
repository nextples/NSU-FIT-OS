#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "func_map.h"

#define BUFF_SIZE 1024

#define log_error() fprintf(stderr, "%s: unexpected error.\nLine: %d\nError: %s", __FILE__, __LINE__, strerror(errno))

int create_dir_func(char* path_to_dir) {
    errno = 0;
    if (mkdir(path_to_dir, 0777) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int dir_info_func(char* path_to_dir) {
    errno = 0;
    DIR* dir = opendir(path_to_dir);
    if (dir == NULL) {
        log_error();
        fprintf(stderr, "\n%s", path_to_dir);
        return errno;
    }

    errno = 0;
    struct dirent* current_file = readdir(dir);
    if (current_file == NULL) {
        log_error();
        fprintf(stderr, "\n%s", path_to_dir);
        return errno;
    }

    while (current_file != NULL) {
        printf("%s \n", current_file->d_name);

        errno = 0;
        current_file = readdir(dir);
        if (current_file == -1) {
            log_error();
            fprintf(stderr, "\n%s", path_to_dir);
            return errno;
        }
    }
    return 0;
}

int remove_dir_func(char* path_to_dir) {
    errno = 0;
    if (rmdir(path_to_dir) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int create_file_func(char* path_to_file) {
    errno = 0;
    int file = open(path_to_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file == -1) {
        log_error();
        fprintf(stderr, "\n%s", path_to_file);
        return errno;
    }
    close(file);
    return 0;
}

int read_file_func(char* path_to_file) {
    errno = 0;
    int src_file = open(path_to_file, O_RDONLY);
    if (src_file == -1) {
        log_error();
        fprintf(stderr, "\n%s", path_to_file);
        return errno;
    }
    long file_size = lseek(src_file, 0, SEEK_END);
    char buffer[BUFF_SIZE] = {0};
    long txt_pointer = file_size;

    while (txt_pointer != 0) {
        if (txt_pointer - BUFF_SIZE < 0) {
            txt_pointer = 0;
        }
        else {
            txt_pointer -= BUFF_SIZE;
        }
        lseek(src_file, txt_pointer, SEEK_SET);

        errno = 0;
        long read_count = read(src_file, buffer, BUFF_SIZE);
        if (read_count == -1) {
            log_error();
            fprintf(stderr, "\n%s", path_to_file);
            return errno;
        }
        errno = 0;
        if (write(1, buffer, read_count) == -1) {
            log_error();
            fprintf(stderr, "\n%s", path_to_file);
            return errno;
        }
    }
    close(src_file);
    return 0;

}

int create_slink_func(char* path_to_file, char* path_to_link) {
    errno = 0;
    if (symlink(path_to_file, path_to_link) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int read_slink_func(char* path_to_link) {
    errno = 0;
    char buffer[BUFF_SIZE] = {0};
    if (readlink(path_to_link, buffer, BUFF_SIZE) == -1) {
        log_error();
        return errno;
    }
    printf("%s\n", buffer);
    return 0;
}

int remove_slink_func(char* path_to_link) {
    errno = 0;
    if (unlink(path_to_link) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int read_file_by_slink_func(char* path_to_link) {
    return read_file_func(path_to_link);
}

int remove_file_func(char* path_to_file) {
    errno = 0;
    if (remove(path_to_file) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int create_hlink_func(char* path_to_file, char* path_to_link) {
    errno = 0;
    if (link(path_to_file, path_to_link) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int remove_hlink_func(char* path_to_link) {
    errno = 0;
    if (unlink(path_to_link) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int print_file_info_func(char* path_to_file) {
    struct stat file_stat;
    errno = 0;
    if (stat(path_to_file, &file_stat) == -1) {
        log_error();
        return errno;
    }

    printf("File access rights: %o\n", file_stat.st_mode & 0777);
    printf("Count hard links: %lu\n", file_stat.st_nlink);

    return 0;
}

int change_mode_func(char* path_to_file, char* mode) {
    int permissions = 0;
    for (int i = 0; i < 9; i++) {
        if (mode[i] == 'r') {
            if (i/3 == 0) {
                permissions |= S_IRUSR;
            }
            else if (i/3 == 1) {
                permissions |= S_IRGRP;
            }
            else {
                permissions |= S_IROTH;
            }
        }
        else if (mode[i] == 'w') {
            if (i/3 == 0) {
                permissions |= S_IWUSR;
            }
            else if (i/3 == 1) {
                permissions |= S_IWGRP;
            }
            else {
                permissions |= S_IWOTH;
            }
        }
        else if (mode[i] == 'x') {
            if (i/3 == 0) {
                permissions |= S_IXUSR;
            }
            else if (i/3 == 1) {
                permissions |= S_IXGRP;
            }
            else {
                permissions |= S_IXOTH;
            }
        }
        else if (mode[i] != '-') {
            fprintf(stderr, "invalid permission character %c\n", mode[i]);
            return -1;
        }
    }
    errno = 0;
    if (chmod(path_to_file, permissions) == -1) {
        log_error();
        return errno;
    }
    return 0;
}

int get_function_id(char* name) {
    char *pt = strrchr(name, '/');
    if (pt != NULL) {
        name = pt + 1;
    }
    size_t funcs_num = sizeof(functions) / sizeof(functions[0]);
    for (int i = 0; i < funcs_num; i++) {
        if (strcmp(name, functions[i].name) == 0) {
            return functions[i].func_id;
        }
    }
    return -1;
}

int call_utility(int argc, char *argv[]) {
    int func_id = get_function_id(argv[0]);

    switch (func_id) {
        case create_dir:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return create_dir_func(argv[1]);

        case print_dir:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return dir_info_func(argv[1]);

        case remove_dir:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return remove_dir_func(argv[1]);

        case create_file:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return create_file_func(argv[1]);

        case print_file:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return read_file_func(argv[1]);

        case remove_file:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return remove_file_func(argv[1]);

        case create_slink:
            if (argc < 3) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return create_slink_func(argv[1], argv[2]);

        case print_file_by_slink:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return read_file_by_slink_func(argv[1]);

        case remove_slink:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return remove_slink_func(argv[1]);

        case create_hlink:
            if (argc < 3) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return create_hlink_func(argv[1], argv[2]);

        case remove_hlink:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return remove_hlink_func(argv[1]);

        case print_file_info:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return print_file_info_func(argv[1]);

        case change_mode:
            if (argc < 3) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return change_mode_func(argv[1], argv[2]);

        case print_slink:
            if (argc < 2) {
                fprintf(stderr, "incorrect number of arguments\n");
                return -1;
            }
            return read_slink_func(argv[1]);

        default:
            printf("undefined command received\n");
            return -1;
    }
}

int main(int argc, char *argv[]) {
    return call_utility(argc, argv);
}