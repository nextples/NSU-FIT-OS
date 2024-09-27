#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFF_SIZE 1024

#define log_error() fprintf(stderr, "%s: unexpected error.\nLine: %d\nError: %s", __FILE__, __LINE__, strerror(errno))

long get_last_slash_index(const char* string) {
    char* slash_ptr = strrchr(string, '/');
    if (slash_ptr == NULL) {
        return -1;
    }
    return (long)(slash_ptr - string)/ sizeof(char);
}

void invert_string(char* src_str, char* dst_str, long length) {
    char tmp;
    for (long i = 0; i < length / 2; i++) {
        tmp = src_str[i];
        dst_str[i] = src_str[length - i - 1];
        dst_str[length - i - 1] = tmp;
    }
}

int invert_file(char* src_path, char* dst_path) {
    errno = 0;
    int src_file = open(src_path, O_RDONLY);
    if (src_file == -1) {
        log_error();
        fprintf(stderr, "\n%s", src_path);
        return errno;
    }

    struct stat src_stat;
    stat(src_path, &src_stat);

    mode_t file_permissions = src_stat.st_mode & 0777;

    errno = 0;
    int dst_file = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    chmod(dst_path, file_permissions);
    if (dst_file == -1) {
        log_error();
        fprintf(stderr, "\n%s", dst_path);
        return errno;
    }

    // determining the file size
    long file_size = lseek(src_file, 0, SEEK_END);

    char buffer[BUFF_SIZE];
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
            fprintf(stderr, "\n%s", src_path);
            return errno;
        }

        invert_string(buffer, buffer, read_count);

        errno = 0;
        if (write(dst_file, buffer, read_count) == -1) {
            log_error();
            fprintf(stderr, "\n%s", dst_path);
            return errno;
        }
    }
    return 0;
}

void create_inverted_path(char* original_path, char* inverted_path) {
    unsigned long path_length = strlen(original_path);
    if (original_path[path_length - 1] == '/') {
        original_path[path_length - 1] = '\0';
        path_length--;
    }

    long last_slash_index = get_last_slash_index(original_path);

    if (last_slash_index != -1) {
        memcpy(inverted_path, original_path, last_slash_index);
        inverted_path[last_slash_index] = '/';
    }
    invert_string(original_path + last_slash_index + 1, inverted_path + last_slash_index + 1, path_length - last_slash_index - 1);
}

void add_filename_to_path(char* path, char* filename) {
    unsigned long path_len = strlen(path);
    unsigned long name_len = strlen(filename);

    path[path_len] = '/';
    path[path_len + 1] = '\0';
    strncat(path, filename, name_len);
}

void update_path(char* original_path, unsigned long op_length, char* inverted_path, unsigned long ip_length) {
    original_path[op_length] = '\0';
    inverted_path[ip_length] = '\0';
}

int invert_full_catalog(char* original_path, char* inverted_path) {
    errno = 0;
    DIR* original_dir = opendir(original_path);
    if (original_dir == NULL) {
        log_error();
        fprintf(stderr, "\n%s", original_path);
        return errno;
    }

    errno = 0;
    if (mkdir(inverted_path, 0777) == -1) {
        log_error();
        return errno;
    }

    errno = 0;
    struct dirent* current_file = readdir(original_dir);
    if (current_file == NULL) {
        log_error();
        fprintf(stderr, "\n%s", original_path);
        return errno;
    }


    unsigned long orig_path_length = strlen(original_path);
    unsigned long inv_path_length = strlen(inverted_path);

    while (1) {
        char new_file_name[512] = {0};

        if (current_file->d_type == DT_REG) {
            add_filename_to_path(original_path, current_file->d_name);

            if (strcmp(current_file->d_name, ".") == 0 || strcmp(current_file->d_name, "..") == 0) {

                errno = 0;
                current_file = readdir(original_dir);
                if (current_file == NULL) {
                    if (errno != 0) {
                        log_error();
                        return errno;
                    }
                    break;
                }
                continue;
            }

            invert_string(current_file->d_name, new_file_name, (long)strlen(current_file->d_name));
            add_filename_to_path(inverted_path, new_file_name);

            errno = 0;
            if (invert_file(original_path, inverted_path) == -1) {
                log_error();
                return errno;
            }
        }


        if (current_file->d_type == DT_DIR && strcmp(current_file->d_name, ".") != 0 && strcmp(current_file->d_name, "..") != 0) {
            add_filename_to_path(inverted_path, current_file->d_name);
            add_filename_to_path(original_path, current_file->d_name);
            create_inverted_path(inverted_path, inverted_path);

            errno = 0;
            int err_flag = invert_full_catalog(original_path, inverted_path);
            if (err_flag != 0) {
                log_error();
                return errno;
            }
        }


        update_path(original_path, orig_path_length, inverted_path, inv_path_length);
        errno = 0;
        current_file = readdir(original_dir);

        if (current_file == NULL) {
            if (errno != 0) {
                log_error();
                return errno;
            }
            break;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "incorrect number of arguments received\n");
        return -1;
    }
    char* original_path = argv[1];

    char inverted_path[1024] = {0};
    create_inverted_path(original_path, inverted_path);

    return invert_full_catalog(original_path, inverted_path);
}