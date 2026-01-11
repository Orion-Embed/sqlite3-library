# Library Management System

A complete library management system built with C and SQLite3, featuring a 5-table normalized database design.

## Features

- 📚 **Book Management**: Add, delete, update, and search books
- 👥 **Author Management**: Manage authors with biography and details
- 📂 **Category System**: Organize books by categories
- 🔍 **Advanced Search**: Search by title, author, or category
- 📊 **Statistics**: View library statistics and reports
- ⚡ **SQLite3 CLI**: Built-in SQL command interface
- 🔐 **5-Table Design**: Normalized database schema

## Database Schema

The system uses a normalized 5-table design:

1. **books** - Book information
2. **authors** - Author details
3. **categories** - Book categories
4. **book_authors** - Many-to-many relationship (books ↔ authors)
5. **book_categories** - Many-to-many relationship (books ↔ categories)

## Prerequisites

- GCC compiler
- SQLite3 development libraries
- Git

## Installation

### Linux/macOS

```bash
# Clone the repository
git clone git@github.com:Orion-Embed/sqlite3-library.git
cd sqlite3-library

# Install dependencies
sudo apt-get install sqlite3 libsqlite3-dev  # Ubuntu/Debian
# or
sudo yum install sqlite-devel                # RHEL/Fedora

# Compile
make

# Run
./library-app
