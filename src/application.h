#ifndef APPLICATION_H
#define APPLICATION_H
#include <stdlib.h>
void display_main_menu(void);
void display_book_menu(void);
void display_author_menu(void);
void display_category_menu(void);
void display_search_menu(void);
void display_help(void);
void init_library(void);
void cleanup(void);

int get_input_int(int lower, int upper);
void get_input_string(char *buffer, size_t size);

#endif // !APPLICATION_H
