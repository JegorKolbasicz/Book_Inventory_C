# 📚 Book Inventory Manager (GTK+ C)

A desktop application written in C using the GTK+ library. It allows users to manage a simple inventory of books — with features like adding, deleting, updating, and displaying book data stored in a file.

This was a university project developed by me to deepen my understanding of memory management, data structures, and GUI programming in C.

---

## 🧠 Features

- Add new books to the inventory
- Search and display book information by title or ID
- Update book quantity and price
- Delete books from the inventory
- Data persistence using a local text file (`plik.txt`)
- GUI interface built with GTK+

---

## 🖥️ Technologies Used

- **C** – core programming language  
- **GTK+ 3** – graphical user interface  
- **Make / GCC** – for compiling the code  
- **Plain text file** – simple persistent data storage  

---

## 📸 Interface

📌 Interface consists of:
- Entry fields for title, author, quantity, and price
- Buttons for performing actions
- A scrollable output text view for displaying feedback

---

## 🚀 How to Run
### 🧪 Build & Run
  On Ubuntu/Debian:
  '''bash
gcc -o magazyn main.c 'pkg-config --cflags --libs gtk+-3.0'
./magazyn

### 🛠️ Prerequisites

- GTK+ 3 development libraries  
  On Ubuntu/Debian:
  ```bash
  sudo apt-get install libgtk-3-dev
