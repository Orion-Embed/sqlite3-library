// database.h - Updated for 5-table design
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

// Database initialization
sqlite3 *init_database(void);
void free_database(sqlite3 *db);
void create_tables(sqlite3 *db);

// Basic operations
void execute_sqlite3_command(sqlite3 *db, char *cmd);
int sqlite3_prompt(sqlite3 *db);
void display_column(sqlite3 *db, char *column);

// Author operations
int add_author(sqlite3 *db, const char *first_name, const char *last_name,
               const char *email, int birth_year, const char *bio);
void display_all_authors(sqlite3 *db);

// Category operations
int add_category(sqlite3 *db, const char *category_name,
                 const char *description);
void display_all_categories(sqlite3 *db);

// Book operations
int add_book_complete(sqlite3 *db, const char *title, const char *isbn,
                      int publication_year, int edition, const char *publisher,
                      const char *language, int page_count, const char *summary,
                      int author_id, int category_id);
void display_all_books(sqlite3 *db);
void display_book_details(sqlite3 *db, int book_id);
void search_books_advanced(sqlite3 *db, const char *search_term);
void delete_book(sqlite3 *db, int book_id);

#endif // !DATABASE_H
