# 🗃️ Console-Based File Versioning & Management System

A fully-featured file management system built in **C++**, simulating core functionalities of real-world operating systems like folder navigation, file version control, user roles, file compression, cloud synchronization, and much more — all through a command-line interface.

---

## 🚀 Features

### 🔐 User Authentication & Role-Based Access
- Sign up and login system with **Admin**, **Editor**, and **Viewer** roles.
- Password and security-question-based recovery.
- Permissions enforced at every operation.

### 📁 Folder & File Management
- Create, update, delete files and folders.
- Navigate between folders (like `cd` and `cd ..`).
- View contents of the current directory or all folders.
- Recycle Bin support for deleted files with restore/empty options.

### 🔄 File Version Control
- Automatically saves previous versions of a file when updated.
- View full version history.
- Rollback to any previous version at any time.

### 🗂️ Recent Files
- Tracks files accessed recently.
- Displays recent files for easy access.

### 🧺 Recycle Bin
- Soft-delete functionality to avoid data loss.
- Restore or permanently remove files from Recycle Bin.

### 📤 File Sharing System
- Share files between users.
- View who has access to your files via a sharing graph.

### 🔎 Search & Metadata Indexing
- Search for files by name.
- View detailed metadata (name, path, owner, type, size, creation date) using a **Hash Table**.

### 🗜️ Compression & Decompression
- Compress files using **Run-Length Encoding (RLE)**.
- Decompress `.compressed` files back to original content.
- Maintains separate compressed file for safety.

### ☁️ Simulated Cloud Sync
- Add files to a sync queue (upload/download/delete).
- Simulate syncing to the cloud with timestamps.
- Auto-processing and background execution support.

### 🧠 File System Optimization *(Bonus Placeholder)*
- AVL Tree-based folder structure and garbage collection framework (future extension).

---

## 🧪 Tech Stack

- **Language**: C++
- **Concepts Used**:
  - OOP (Classes, Inheritance, Polymorphism)
  - STL (strings, vectors, maps)
  - Queues, Hash Tables, Trees
  - File I/O
  - Timestamps and formatting
  - Console-based UI

---

## 📁 Folder Structure

- Root/
- ├── - folders/
- │ ├── - subfolders/
- │ └── - files.txt
- ├── - RecycleBin/
- │ └── - deleted_files/
- ├── - UserSystem/
- │ └── - users.txt
- ├── - CloudQueue/
- │ └── - tasks.txt

> Note: This structure is simulated in memory.

---

## 👤 Roles & Permissions

| Action                  | Admin | Editor | Viewer |
|-------------------------|:-----:|:------:|:------:|
| Create/Update Files     | ✅    | ✅     | ❌     |
| Delete Files/Folders    | ✅    | ✅     | ❌     |
| Compress/Decompress     | ✅    | ✅     | ❌     |
| Share Files             | ✅    | ✅     | ❌     |
| View Version History    | ✅    | ✅     | ✅     |
| Restore from Recycle Bin| ✅    | ✅     | ❌     |
| Process Cloud Sync      | ✅    | ❌     | ❌     |
| Empty Recycle Bin       | ✅    | ❌     | ❌     |

---

## 🛠️ How to Run

1. Clone the repo:
   ```bash
    git clone https://github.com/NIMRAH-S/Google-Drive.git
   cd File-Versioning-System

2. Compile the code:
   g++ -std=c++11 main.cpp -o file_system

3. Run the program:
   ./file_system
   
 ---
### 👥 Group Members

- Nimrah Shahid  
- Aameena Shahid  
- Ghulam Dastageer

