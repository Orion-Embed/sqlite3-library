#include "database.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// this function process the output of sqlite commands
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

// clean the memory
void free_database(sqlite3 *db) { sqlite3_close(db); }

// initiate database from data/library.db file
sqlite3 *init_database() {
  sqlite3 *db;
  int status = sqlite3_open("data/library.db", &db);
  if (status != SQLITE_OK) {
    fprintf(stderr, "Error: Failed opening database file: %s\n",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }
  return db;
}

int sqlite3_prompt(sqlite3 *db) {
  char input[256];
  printf("=========== Welcome to SQLite3 prompt ==========\n");
  printf("Type 'help' for commands, 'exit' or 'quit' to return\n");
  while (1) {
    printf("sqlite3 ~>");
    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "help") == 0) {
      printf("Available commands:\n");
      printf("  help          - Show this help\n");
      printf("  exit, quit    - Exit SQLite prompt\n");
      printf("  Any SQL command will be executed\n");
    } else if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
      printf("SQLite3 prompt closed.\n");
      return 0;
    } else if (strlen(input) > 0) {
      execute_sqlite3_command(db, input);
    }
  }
  return 0;
}

// execute sqlite statements with errors checking
void execute_sqlite3_command(sqlite3 *db, char *cmd) {
  if (!db) {
    fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    return;
  }
  char *errmsg = NULL;
  int status = sqlite3_exec(db, cmd, callback, 0, &errmsg);
  if (status != SQLITE_OK) {
    fprintf(stderr, "%s\n", errmsg);
    sqlite3_free(errmsg);
  }
}

void display_column(sqlite3 *db, char *column) {
  if (!db) {
    fprintf(stderr, "data base pointer is null!\n");
    return;
  };
  char cmd[25];
  sprintf(cmd, "select * from %s;", column);
  execute_sqlite3_command(db, cmd);
}

void add_book(sqlite3 *db, const char *title, const char *author,
              const char *category, int quantity, float price) {
  char sql[512];
  char *errmsg = NULL;
  const char *create_sql = "CREATE TABLE IF NOT EXISTS books ("
                           "  id INTEGER PRIMARY KEY,"
                           "  title TEXT NOT NULL,"
                           "  author TEXT,"
                           "  category TEXT,"
                           "  quantity INTEGER,"
                           "  price REAL"
                           ");";
  sqlite3_exec(db, create_sql, 0, 0, &errmsg);
  sprintf(sql,
          "INSERT INTO books (title, author, category, quantity, price) "
          "VALUES ('%s', '%s', '%s', %d, %.2f);",
          title, author, category, quantity, price);

  execute_sqlite3_command(db, sql);

  if (errmsg) {
    sqlite3_free(errmsg);
  }
}
void delete_book(sqlite3 *db, int book_id) {
  char sql[100];
  sprintf(sql, "DELETE FROM books WHERE id = %d;", book_id);
  execute_sqlite3_command(db, sql);
}

void search_books(sqlite3 *db, const char *search_term) {
  char sql[256];
  sprintf(
      sql,
      "SELECT * FROM books WHERE title LIKE '%%%s%%' OR author LIKE '%%%s%%';",
      search_term, search_term);
  execute_sqlite3_command(db, sql);
}

void update_book_quantity(sqlite3 *db, int book_id, int new_quantity) {
  char sql[100];
  sprintf(sql, "UPDATE books SET quantity = %d WHERE id = %d;", new_quantity,
          book_id);
  execute_sqlite3_command(db, sql);
}

void display_book_stats(sqlite3 *db) {
  printf("\n=== Library Statistics ===\n");

  const char *queries[] = {
      "SELECT COUNT(*) as 'Total Books' FROM books;",
      "SELECT SUM(quantity) as 'Total Copies' FROM books;",
      "SELECT COUNT(DISTINCT author) as 'Unique Authors' FROM books;",
      "SELECT COUNT(DISTINCT category) as 'Categories' FROM books;",
      "SELECT AVG(price) as 'Average Price' FROM books;",
      NULL};

  for (int i = 0; queries[i] != NULL; i++) {
    execute_sqlite3_command(db, (char *)queries[i]);
  }
}
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
// Initialize all 5 tables
void create_tables(sqlite3 *db) {
  const char *create_sql =
      "PRAGMA foreign_keys = ON;"

      "CREATE TABLE IF NOT EXISTS authors ("
      "    author_id INTEGER PRIMARY KEY,"
      "    first_name TEXT NOT NULL,"
      "    last_name TEXT NOT NULL,"
      "    email TEXT,"
      "    birth_year INTEGER,"
      "    bio TEXT,"
      "    UNIQUE(first_name, last_name)"
      ");"

      "CREATE TABLE IF NOT EXISTS categories ("
      "    category_id INTEGER PRIMARY KEY,"
      "    category_name TEXT NOT NULL UNIQUE,"
      "    description TEXT"
      ");"

      "CREATE TABLE IF NOT EXISTS books ("
      "    book_id INTEGER PRIMARY KEY,"
      "    title TEXT NOT NULL,"
      "    isbn TEXT UNIQUE,"
      "    publication_year INTEGER,"
      "    edition INTEGER DEFAULT 1,"
      "    publisher TEXT,"
      "    language TEXT DEFAULT 'English',"
      "    page_count INTEGER,"
      "    summary TEXT,"
      "    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
      ");"

      "CREATE TABLE IF NOT EXISTS book_authors ("
      "    book_id INTEGER,"
      "    author_id INTEGER,"
      "    author_order INTEGER DEFAULT 1,"
      "    PRIMARY KEY (book_id, author_id),"
      "    FOREIGN KEY (book_id) REFERENCES books(book_id) ON DELETE CASCADE,"
      "    FOREIGN KEY (author_id) REFERENCES authors(author_id) ON DELETE "
      "CASCADE"
      ");"

      "CREATE TABLE IF NOT EXISTS book_categories ("
      "    book_id INTEGER,"
      "    category_id INTEGER,"
      "    PRIMARY KEY (book_id, category_id),"
      "    FOREIGN KEY (book_id) REFERENCES books(book_id) ON DELETE CASCADE,"
      "    FOREIGN KEY (category_id) REFERENCES categories(category_id) ON "
      "DELETE CASCADE"
      ");"

      "CREATE TABLE IF NOT EXISTS copies ("
      "    copy_id INTEGER PRIMARY KEY,"
      "    book_id INTEGER,"
      "    barcode TEXT UNIQUE,"
      "    status TEXT DEFAULT 'Available',"
      "    location TEXT DEFAULT 'Main Shelf',"
      "    purchase_date DATE,"
      "    price_paid REAL,"
      "    notes TEXT,"
      "    FOREIGN KEY (book_id) REFERENCES books(book_id) ON DELETE CASCADE"
      ");";

  char *errmsg = NULL;
  int rc = sqlite3_exec(db, create_sql, 0, 0, &errmsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error creating tables: %s\n", errmsg);
    sqlite3_free(errmsg);
  }
}

// Add a new author
int add_author(sqlite3 *db, const char *first_name, const char *last_name,
               const char *email, int birth_year, const char *bio) {
  sqlite3_stmt *stmt;
  const char *sql =
      "INSERT INTO authors (first_name, last_name, email, birth_year, bio) "
      "VALUES (?, ?, ?, ?, ?);";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_text(stmt, 1, first_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, last_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 4, birth_year);
  sqlite3_bind_text(stmt, 5, bio, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc == SQLITE_DONE) {
    return (int)sqlite3_last_insert_rowid(db);
  } else {
    fprintf(stderr, "Failed to add author: %s\n", sqlite3_errmsg(db));
    return -1;
  }
}

// Add a new category
int add_category(sqlite3 *db, const char *category_name,
                 const char *description) {
  sqlite3_stmt *stmt;
  const char *sql =
      "INSERT INTO categories (category_name, description) VALUES (?, ?);";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK)
    return -1;

  sqlite3_bind_text(stmt, 1, category_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc == SQLITE_DONE) {
    return (int)sqlite3_last_insert_rowid(db);
  }
  return -1;
}

// Add a new book with author and category
int add_book_complete(sqlite3 *db, const char *title, const char *isbn,
                      int publication_year, int edition, const char *publisher,
                      const char *language, int page_count, const char *summary,
                      int author_id, int category_id) {
  sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);

  // Insert book
  sqlite3_stmt *stmt;
  const char *book_sql =
      "INSERT INTO books (title, isbn, publication_year, edition, "
      "publisher, language, page_count, summary) "
      "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

  int rc = sqlite3_prepare_v2(db, book_sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
    return -1;
  }

  sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, isbn, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, publication_year);
  sqlite3_bind_int(stmt, 4, edition);
  sqlite3_bind_text(stmt, 5, publisher, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 6, language, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 7, page_count);
  sqlite3_bind_text(stmt, 8, summary, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
    return -1;
  }

  int book_id = (int)sqlite3_last_insert_rowid(db);

  // Link to author
  const char *author_sql =
      "INSERT INTO book_authors (book_id, author_id) VALUES (?, ?);";
  rc = sqlite3_prepare_v2(db, author_sql, -1, &stmt, NULL);
  if (rc == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, author_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  // Link to category
  const char *category_sql =
      "INSERT INTO book_categories (book_id, category_id) VALUES (?, ?);";
  rc = sqlite3_prepare_v2(db, category_sql, -1, &stmt, NULL);
  if (rc == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, category_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  sqlite3_exec(db, "COMMIT;", 0, 0, 0);
  return book_id;
}

// Display all books with author and category info
void display_all_books(sqlite3 *db) {
  const char *sql =
      "SELECT "
      "b.book_id, "
      "b.title, "
      "b.isbn, "
      "GROUP_CONCAT(DISTINCT a.first_name || ' ' || a.last_name) as authors, "
      "GROUP_CONCAT(DISTINCT c.category_name) as categories, "
      "b.publication_year, "
      "b.publisher "
      "FROM books b "
      "LEFT JOIN book_authors ba ON b.book_id = ba.book_id "
      "LEFT JOIN authors a ON ba.author_id = a.author_id "
      "LEFT JOIN book_categories bc ON b.book_id = bc.book_id "
      "LEFT JOIN categories c ON bc.category_id = c.category_id "
      "GROUP BY b.book_id "
      "ORDER BY b.title;";
  execute_sqlite3_command(db, (char *)sql);
}

// Display all authors
void display_all_authors(sqlite3 *db) {
  const char *sql =
      "SELECT a.author_id, a.first_name || ' ' || a.last_name as name, "
      "a.birth_year, COUNT(ba.book_id) as book_count "
      "FROM authors a "
      "LEFT JOIN book_authors ba ON a.author_id = ba.author_id "
      "GROUP BY a.author_id "
      "ORDER BY a.last_name, a.first_name;";

  execute_sqlite3_command(db, (char *)sql);
}

// Display all categories with book count
void display_all_categories(sqlite3 *db) {
  const char *sql =
      "SELECT c.category_id, c.category_name, c.description, "
      "COUNT(bc.book_id) as book_count "
      "FROM categories c "
      "LEFT JOIN book_categories bc ON c.category_id = bc.category_id "
      "GROUP BY c.category_id "
      "ORDER BY c.category_name;";

  execute_sqlite3_command(db, (char *)sql);
}

// Search books by title, author, or category
void search_books_advanced(sqlite3 *db, const char *search_term) {
  char sql[512];
  sprintf(sql,
          "SELECT DISTINCT b.book_id, b.title, "
          "a.first_name || ' ' || a.last_name as author, "
          "c.category_name "
          "FROM books b "
          "LEFT JOIN book_authors ba ON b.book_id = ba.book_id "
          "LEFT JOIN authors a ON ba.author_id = a.author_id "
          "LEFT JOIN book_categories bc ON b.book_id = bc.book_id "
          "LEFT JOIN categories c ON bc.category_id = c.category_id "
          "WHERE b.title LIKE '%%%s%%' "
          "OR a.first_name LIKE '%%%s%%' "
          "OR a.last_name LIKE '%%%s%%' "
          "OR c.category_name LIKE '%%%s%%' "
          "ORDER BY b.title;",
          search_term, search_term, search_term, search_term);

  execute_sqlite3_command(db, sql);
}

// Get book details by ID
void display_book_details(sqlite3 *db, int book_id) {
  char sql[512];
  sprintf(sql,
          "SELECT b.*, "
          "GROUP_CONCAT(DISTINCT a.first_name || ' ' || a.last_name, ', ') as "
          "authors, "
          "GROUP_CONCAT(DISTINCT c.category_name, ', ') as categories, "
          "COUNT(DISTINCT cp.copy_id) as total_copies, "
          "SUM(CASE WHEN cp.status = 'Available' THEN 1 ELSE 0 END) as "
          "available_copies "
          "FROM books b "
          "LEFT JOIN book_authors ba ON b.book_id = ba.book_id "
          "LEFT JOIN authors a ON ba.author_id = a.author_id "
          "LEFT JOIN book_categories bc ON b.book_id = bc.book_id "
          "LEFT JOIN categories c ON bc.category_id = c.category_id "
          "LEFT JOIN copies cp ON b.book_id = cp.book_id "
          "WHERE b.book_id = %d "
          "GROUP BY b.book_id;",
          book_id);

  execute_sqlite3_command(db, sql);
}
