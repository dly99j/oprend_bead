//
// Created by htomi on 3/28/2022.
//

#ifndef OPREND_BEAD_UTIL_H
#define OPREND_BEAD_UTIL_H

#define BUFSIZE 1024

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct nyuszi;
struct region_data;

struct nyuszi* init(const char*, const char*, int);
void check_successful_alloc(const char*);
void check_successful_alloc2(const char**);
void append_to_file(const char*, struct nyuszi*);
void check_file_pointer(FILE*);
void check_null_char_ptr(const char*);
void delete_nyuszi(struct nyuszi*);
struct nyuszi* get_nyuszi_from_user();
int check_lakoterulet(const char*, int);
int count_nyuszi(const char*,int);
char** read_all_nyuszi(const char*, int);
void list_nyuszi(const char*, const char*);
void copy_back_temp(const char*);
void modify_entry_in_file(const char*, const char*);
void delete_from_file(const char*, const char*);
void clear_database(const char*);
char* get_name();
char* get_place();
void get_region_data(const char*, int, struct region_data*);
void delete_region_data(struct region_data*);
void print_winner(char*, int);
void run_easter(const char*);
void menu_wrong_format();
void menu_goodbye();
void menu_wrong_number();
void menu_add(const char*);
void menu_modify(const char*);
void menu_delete(const char*);
void menu_list(const char*);
void menu_list_specific_place(const char*);
void menu_delete_database(const char*);
void menu_run_easter(const char*);
void print_menu();
int get_menu_point();
void run(const char*);

#endif //OPREND_BEAD_UTIL_H
