#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>


int global_initialized = 777;

int global_uninitialized;

const int global_const = 123;

void my_function() {
    int local_var;
    static int static_var;
    const int const_var = 123;

    printf("my function:\n");
    printf("local var: %p\n", &local_var);
    printf("static var: %p\n", &static_var);
    printf("const var: %p\n", &const_var);

    printf("\nglobal:\n");
    printf("global initialized var: %p\n", &global_initialized);
    printf("global uninitialized var: %p\n", &global_uninitialized);
    printf("global const var: %p\n", &global_const);
}

void print_local_var_addr() {
    int my_local_var = 123;
    printf("\nlocal initialized var: %p\n", &my_local_var);
}

void buffer_function() {
    printf("\nBuffer function:\n");
    char* my_buffer1 = (char*) malloc(100);
    char string[12] = "Hello World";
    memcpy(my_buffer1, string, 12);
    printf("my_buffer1 before free: %s\n", my_buffer1);

    free(my_buffer1);
    printf("my_buffer1 after free: %s\n", my_buffer1);

    free(my_buffer1);

    char* my_buffer2 = (char*) malloc(100);
    memcpy(my_buffer2, string, 12);
    printf("my_buffer2 before free: %s\n", my_buffer2);

    my_buffer2 += 50;
    free(my_buffer2);
    printf("my_buffer2 after free: %s\n", my_buffer2);
}

void env_function() {
    printf("\nEnvironment_function:\n");

    char* my_env_var = getenv("MY_ENV_VAR");
    printf("my_env_var before: %p\n", my_env_var);



    setenv("MY_ENV_VAR", "SECOND", 1);

    my_env_var = getenv("MY_ENV_VAR");
    printf("my_env_var after: %p\n", my_env_var);
}

int main() {
    printf("my pid: %d\n", getpid());
//    my_function();

//    print_local_var_addr();

//    buffer_function();

    env_function();

    sleep(100);
}