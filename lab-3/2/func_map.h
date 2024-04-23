
typedef struct function {
    char* name;
    int func_id;
} function;

enum func_types {
    create_dir,
    print_dir,
    remove_dir,
    create_file,
    print_file,
    remove_file,
    create_slink,
    print_file_by_slink,
    print_slink,
    remove_slink,
    create_hlink,
    remove_hlink,
    print_file_info,
    change_mode
};

function functions[] = {
        {"create_dir", create_dir},
        {"print_dir", print_dir},
        {"remove_dir", remove_dir},
        {"create_file", create_file},
        {"print_file", print_file},
        {"remove_file", remove_file},
        {"create_slink", create_slink},
        {"print_file_by_slink", print_file_by_slink},
        {"print_slink", print_slink},
        {"remove_slink", remove_slink},
        {"create_hlink", create_hlink},
        {"remove_hlink", remove_hlink},
        {"print_file_info", print_file_info},
        {"change_mod", change_mode}};