#include "application.h"
#include "database.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Helper functions
void handle_add_author(sqlite3 *db) {
  char first_name[100], last_name[100], email[100], bio[500];
  int birth_year;

  printf("\n=== ADD NEW AUTHOR ===\n");

  printf("First Name: ");
  get_input_string(first_name, sizeof(first_name));

  printf("Last Name: ");
  get_input_string(last_name, sizeof(last_name));

  printf("Email (optional): ");
  get_input_string(email, sizeof(email));

  printf("Birth Year: ");
  char year_str[20];
  get_input_string(year_str, sizeof(year_str));
  birth_year = atoi(year_str);

  printf("Biography (optional): ");
  get_input_string(bio, sizeof(bio));

  int author_id =
      add_author(db, first_name, last_name, strlen(email) > 0 ? email : NULL,
                 birth_year, strlen(bio) > 0 ? bio : NULL);

  if (author_id > 0) {
    printf("✅ Author added successfully (ID: %d)\n", author_id);
  }
}

void handle_add_category(sqlite3 *db) {
  char name[100], description[500];

  printf("\n=== ADD NEW CATEGORY ===\n");

  printf("Category Name: ");
  get_input_string(name, sizeof(name));

  printf("Description (optional): ");
  get_input_string(description, sizeof(description));

  int cat_id =
      add_category(db, name, strlen(description) > 0 ? description : NULL);

  if (cat_id > 0) {
    printf("✅ Category added successfully (ID: %d)\n", cat_id);
  }
}

void handle_add_book(sqlite3 *db) {
  char title[200], isbn[20], publisher[100], language[50], summary[1000];
  int pub_year, edition, page_count, author_id, category_id;

  printf("\n=== ADD NEW BOOK ===\n");

  // Display existing authors
  printf("\n--- Existing Authors ---\n");
  display_all_authors(db);

  printf("\nEnter Author ID (or 0 to add new): ");
  char auth_str[20];
  get_input_string(auth_str, sizeof(auth_str));
  author_id = atoi(auth_str);

  if (author_id == 0) {
    handle_add_author(db);
    author_id = (int)sqlite3_last_insert_rowid(db);
  }

  // Display existing categories
  printf("\n--- Existing Categories ---\n");
  display_all_categories(db);

  printf("\nEnter Category ID (or 0 to add new): ");
  char cat_str[20];
  get_input_string(cat_str, sizeof(cat_str));
  category_id = atoi(cat_str);

  if (category_id == 0) {
    handle_add_category(db);
    category_id = (int)sqlite3_last_insert_rowid(db);
  }

  // Get book details
  printf("\nBook Title: ");
  get_input_string(title, sizeof(title));

  printf("ISBN (optional): ");
  get_input_string(isbn, sizeof(isbn));

  printf("Publication Year: ");
  char year_str[20];
  get_input_string(year_str, sizeof(year_str));
  pub_year = atoi(year_str);

  printf("Edition (default 1): ");
  char edition_str[20];
  get_input_string(edition_str, sizeof(edition_str));
  edition = (strlen(edition_str) > 0) ? atoi(edition_str) : 1;

  printf("Publisher: ");
  get_input_string(publisher, sizeof(publisher));

  printf("Language (default English): ");
  get_input_string(language, sizeof(language));
  if (strlen(language) == 0)
    strcpy(language, "English");

  printf("Page Count: ");
  char pages_str[20];
  get_input_string(pages_str, sizeof(pages_str));
  page_count = atoi(pages_str);

  printf("Summary (optional): ");
  get_input_string(summary, sizeof(summary));

  // Add the book
  int book_id = add_book_complete(
      db, title, strlen(isbn) > 0 ? isbn : NULL, pub_year, edition, publisher,
      language, page_count, strlen(summary) > 0 ? summary : NULL, author_id,
      category_id);

  if (book_id > 0) {
    printf("✅ Book added successfully (ID: %d)\n", book_id);
  }
}

int main() {
  printf("Initializing Library Database System...\n");

  system("mkdir -p data 2>/dev/null");

  // Initialize database
  sqlite3 *db = init_database();
  if (!db) {
    fprintf(stderr, "Failed to initialize database.\n");
    return 1;
  }

  // Create tables
  create_tables(db);
  printf("Database initialized successfully.\n");

  int running = 1;
  while (running) {
    display_main_menu();
    int choice = get_input_int(1, 7);

    switch (choice) {
    case 1: { // Book Management
      int book_choice;
      do {
        display_book_menu();
        book_choice = get_input_int(1, 5);

        switch (book_choice) {
        case 1:
          handle_add_book(db);
          break;
        case 2:
          display_all_books(db);
          break;
        case 3: {
          printf("Enter Book ID: ");
          char id_str[20];
          get_input_string(id_str, sizeof(id_str));
          int book_id = atoi(id_str);
          if (book_id > 0)
            display_book_details(db, book_id);
          break;
        }
        case 4: {
          printf("Enter Book ID to delete: ");
          char id_str[20];
          get_input_string(id_str, sizeof(id_str));
          int book_id = atoi(id_str);
          if (book_id > 0) {
            printf("Are you sure? (y/n): ");
            char confirm[10];
            get_input_string(confirm, sizeof(confirm));
            if (tolower(confirm[0]) == 'y') {
              delete_book(db, book_id);
              printf("Book deleted.\n");
            }
          }
          break;
        }
        case 5:
          break; // Back
        }
      } while (book_choice != 5);
      break;
    }

    case 2: { // Author Management
      int author_choice;
      do {
        display_author_menu();
        author_choice = get_input_int(1, 4);

        switch (author_choice) {
        case 1:
          handle_add_author(db);
          break;
        case 2:
          display_all_authors(db);
          break;
        case 3: {
          printf("Enter Author ID: ");
          char id_str[20];
          get_input_string(id_str, sizeof(id_str));
          int author_id = atoi(id_str);
          if (author_id > 0) {
            char sql[100];
            sprintf(sql,
                    "SELECT b.book_id, b.title, b.publication_year "
                    "FROM books b "
                    "JOIN book_authors ba ON b.book_id = ba.book_id "
                    "WHERE ba.author_id = %d "
                    "ORDER BY b.publication_year;",
                    author_id);
            execute_sqlite3_command(db, sql);
          }
          break;
        }
        case 4:
          break; // Back
        }
      } while (author_choice != 4);
      break;
    }

    case 3: { // Category Management
      int category_choice;
      do {
        display_category_menu();
        category_choice = get_input_int(1, 4);

        switch (category_choice) {
        case 1:
          handle_add_category(db);
          break;
        case 2:
          display_all_categories(db);
          break;
        case 3: {
          printf("Enter Category ID: ");
          char id_str[20];
          get_input_string(id_str, sizeof(id_str));
          int cat_id = atoi(id_str);
          if (cat_id > 0) {
            char sql[200];
            sprintf(sql,
                    "SELECT b.book_id, b.title, "
                    "a.first_name || ' ' || a.last_name as author "
                    "FROM books b "
                    "JOIN book_categories bc ON b.book_id = bc.book_id "
                    "JOIN book_authors ba ON b.book_id = ba.book_id "
                    "JOIN authors a ON ba.author_id = a.author_id "
                    "WHERE bc.category_id = %d "
                    "ORDER BY b.title;",
                    cat_id);
            execute_sqlite3_command(db, sql);
          }
          break;
        }
        case 4:
          break; // Back
        }
      } while (category_choice != 4);
      break;
    }

    case 4: { // Search & Browse
      int search_choice;
      do {
        display_search_menu();
        search_choice = get_input_int(1, 5);

        switch (search_choice) {
        case 1: {
          printf("Enter search term: ");
          char term[100];
          get_input_string(term, sizeof(term));
          if (strlen(term) > 0) {
            search_books_advanced(db, term);
          }
          break;
        }
        case 2:
          display_all_categories(db);
          break;
        case 3:
          display_all_authors(db);
          break;
        case 4: {
          const char *stats_sql =
              "SELECT "
              "(SELECT COUNT(*) FROM books) as total_books, "
              "(SELECT COUNT(*) FROM authors) as total_authors, "
              "(SELECT COUNT(*) FROM categories) as total_categories, "
              "(SELECT COUNT(*) FROM copies WHERE status = 'Available') as "
              "available_copies;";
          execute_sqlite3_command(db, (char *)stats_sql);
          break;
        }
        case 5:
          break; // Back
        }
      } while (search_choice != 5);
      break;
    }

    case 5: // SQLite3 CLI
      sqlite3_prompt(db);
      break;

    case 6: // Help
      display_help();
      break;

    case 7: // Exit
      running = 0;
      break;
    }
  }

  free_database(db);
  printf("\nThank you for using the Library Management System!\n");
  return 0;
}
