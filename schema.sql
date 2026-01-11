-- Samples of data for testing data base functionality 

-- schema.sql
PRAGMA foreign_keys = ON;

-- Drop tables if they exist (for development)
DROP TABLE IF EXISTS copies;
DROP TABLE IF EXISTS book_categories;
DROP TABLE IF EXISTS book_authors;
DROP TABLE IF EXISTS books;
DROP TABLE IF EXISTS categories;
DROP TABLE IF EXISTS authors;

-- 1. Authors Table
CREATE TABLE authors (
    author_id INTEGER PRIMARY KEY,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    email TEXT,
    birth_year INTEGER,
    bio TEXT,
    UNIQUE(first_name, last_name)
);

-- 2. Categories Table
CREATE TABLE categories (
    category_id INTEGER PRIMARY KEY,
    category_name TEXT NOT NULL UNIQUE,
    description TEXT
);

-- 3. Books Table
CREATE TABLE books (
    book_id INTEGER PRIMARY KEY,
    title TEXT NOT NULL,
    isbn TEXT UNIQUE,
    publication_year INTEGER,
    edition INTEGER DEFAULT 1,
    publisher TEXT,
    language TEXT DEFAULT 'English',
    page_count INTEGER,
    summary TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 4. Book-Author Relationship Table
CREATE TABLE book_authors (
    book_id INTEGER,
    author_id INTEGER,
    author_order INTEGER DEFAULT 1,
    PRIMARY KEY (book_id, author_id),
    FOREIGN KEY (book_id) REFERENCES books(book_id) ON DELETE CASCADE,
    FOREIGN KEY (author_id) REFERENCES authors(author_id) ON DELETE CASCADE
);

-- 5. Book-Category Relationship Table
CREATE TABLE book_categories (
    book_id INTEGER,
    category_id INTEGER,
    PRIMARY KEY (book_id, category_id),
    FOREIGN KEY (book_id) REFERENCES books(book_id) ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES categories(category_id) ON DELETE CASCADE
);

-- Optional: Copies Table
CREATE TABLE copies (
    copy_id INTEGER PRIMARY KEY,
    book_id INTEGER,
    barcode TEXT UNIQUE,
    status TEXT DEFAULT 'Available',
    location TEXT DEFAULT 'Main Shelf',
    purchase_date DATE,
    price_paid REAL,
    notes TEXT,
    FOREIGN KEY (book_id) REFERENCES books(book_id) ON DELETE CASCADE
);

-- Insert sample data
INSERT INTO authors (first_name, last_name, birth_year) VALUES
('George', 'Orwell', 1903),
('F. Scott', 'Fitzgerald', 1896),
('Harper', 'Lee', 1926),
('J.K.', 'Rowling', 1965),
('J.R.R.', 'Tolkien', 1892);

INSERT INTO categories (category_name, description) VALUES
('Science Fiction', 'Books about futuristic technology and space'),
('Fiction', 'Imaginative stories and novels'),
('Fantasy', 'Magical worlds and creatures'),
('Classic', 'Enduring works of literature'),
('Dystopian', 'Books about oppressive societies');

INSERT INTO books (title, isbn, publication_year, publisher, page_count) VALUES
('1984', '978-0451524935', 1949, 'Secker & Warburg', 328),
('The Great Gatsby', '978-0743273565', 1925, 'Charles Scribner''s Sons', 180),
('To Kill a Mockingbird', '978-0061120084', 1960, 'J.B. Lippincott & Co.', 281),
('Harry Potter and the Philosopher''s Stone', '978-0747532743', 1997, 'Bloomsbury', 223),
('The Hobbit', '978-0547928227', 1937, 'George Allen & Unwin', 310);

-- Link books to authors
INSERT INTO book_authors (book_id, author_id) VALUES
(1, 1),  -- 1984 by George Orwell
(2, 2),  -- The Great Gatsby by F. Scott Fitzgerald
(3, 3),  -- To Kill a Mockingbird by Harper Lee
(4, 4),  -- Harry Potter by J.K. Rowling
(5, 5);  -- The Hobbit by J.R.R. Tolkien

-- Link books to categories
INSERT INTO book_categories (book_id, category_id) VALUES
(1, 1), (1, 5),  -- 1984 is Sci-Fi and Dystopian
(2, 2), (2, 4),  -- Gatsby is Fiction and Classic
(3, 2), (3, 4),  -- Mockingbird is Fiction and Classic
(4, 3),          -- Harry Potter is Fantasy
(5, 3);          -- The Hobbit is Fantasy

-- Add some copies
INSERT INTO copies (book_id, barcode, status, location) VALUES
(1, 'BK001', 'Available', 'Shelf A1'),
(1, 'BK002', 'Available', 'Shelf A1'),
(2, 'BK003', 'Checked Out', 'Shelf B2'),
(3, 'BK004', 'Available', 'Shelf C3'),
(4, 'BK005', 'Available', 'Shelf D4'),
(5, 'BK006', 'Damaged', 'Repair Room');
