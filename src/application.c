#include "application.h"
#include <stdio.h>
#include <string.h>
void display_main_menu() {
  printf("\n========== LIBRARY MANAGEMENT SYSTEM ==========\n");
  printf("1. Book Management\n");
  printf("2. Author Management\n");
  printf("3. Category Management\n");
  printf("4. Search & Browse\n");
  printf("5. SQLite3 CLI\n");
  printf("6. Help\n");
  printf("7. Exit\n");
  printf("==============================================\n");
}

void display_book_menu() {
  printf("\n---------- BOOK MANAGEMENT ----------\n");
  printf("1. Add New Book\n");
  printf("2. View All Books\n");
  printf("3. View Book Details\n");
  printf("4. Delete Book\n");
  printf("5. Back to Main Menu\n");
  printf("------------------------------------\n");
}

void display_author_menu() {
  printf("\n---------- AUTHOR MANAGEMENT ----------\n");
  printf("1. Add New Author\n");
  printf("2. View All Authors\n");
  printf("3. View Author's Books\n");
  printf("4. Back to Main Menu\n");
  printf("--------------------------------------\n");
}

void display_category_menu() {
  printf("\n---------- CATEGORY MANAGEMENT ----------\n");
  printf("1. Add New Category\n");
  printf("2. View All Categories\n");
  printf("3. View Books by Category\n");
  printf("4. Back to Main Menu\n");
  printf("----------------------------------------\n");
}

void display_search_menu() {
  printf("\n---------- SEARCH & BROWSE ----------\n");
  printf("1. Search Books\n");
  printf("2. Browse by Category\n");
  printf("3. Browse by Author\n");
  printf("4. Library Statistics\n");
  printf("5. Back to Main Menu\n");
  printf("-------------------------------------\n");
}

void display_help() {
  printf("\n=== LIBRARY SYSTEM HELP ===\n");
  printf("This system uses a 5-table database design:\n");
  printf("1. authors - Stores author information\n");
  printf("2. categories - Stores book categories\n");
  printf("3. books - Stores book information\n");
  printf("4. book_authors - Links books to authors (many-to-many)\n");
  printf("5. book_categories - Links books to categories (many-to-many)\n");
  printf("\nKey Features:\n");
  printf("- One book can have multiple authors\n");
  printf("- One book can belong to multiple categories\n");
  printf("- Full-text search across all fields\n");
  printf("- Complete author and category management\n");
  printf("=======================================\n");
}

int get_input_int(int lower, int upper) {
  int input;
  char buffer[100];
  do {
    printf("Enter your choice [%d-%d]~>", lower, upper);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      return lower - 1; // error
    }

    if (sscanf(buffer, "%d", &input) != 1) {
      printf("Invalide input. please enter a number. \n");
      input = lower - 1;
    }
  } while (input > upper || input < lower);
  return input;
}

void get_input_string(char *buffer, size_t size) {
  if (fgets(buffer, size, stdin) == NULL) {
    buffer[0] = '\0';
    return;
  }
  buffer[strcspn(buffer, "\n")] = '\0';
}

void display_edit_menu() {
  printf("----------Edit menu--------\n");
  printf("1.Edit books info\n  4.Back\n");
}

void display_read_menu() {
  printf("----------Read Menu----------\n");
  printf("1.Display Books\n 2.Display Categories\n 3.Display Authors\n "
         "4.Custom Queries\n 5.Back\n");
}
