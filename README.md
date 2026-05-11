# 📝 NoteApp CLI (C++)

NoteApp adalah aplikasi manajemen catatan berbasis Command Line Interface (CLI) yang dikembangkan menggunakan bahasa C++. Aplikasi ini memungkinkan pengguna untuk membuat, membaca, mencari, mengurutkan, dan menghapus catatan. Data catatan akan disimpan secara lokal sehingga tidak akan hilang ketika program ditutup.

Aplikasi ini dibangun dengan mengimplementasikan struktur data dinamis dan algoritma pengurutan, menjadikannya project yang bagus untuk pembelajaran fundamental Computer Science dan manajemen memori.

# Fitur Utama

1. Sistem Login: Akses aplikasi dibatasi oleh sistem autentikasi sederhana (maksimal 5 kali percobaan).
2. Tambah Catatan: Menambahkan catatan baru berupa judul dan isi tulisan.
3. Lihat Catatan: Menampilkan daftar seluruh catatan yang tersimpan.
4. Cari Catatan: Mencari catatan spesifik berdasarkan kata kunci pada judul.
5. Urutkan Catatan: Mengurutkan daftar catatan secara alfabetis (A-Z).
6. Hapus Catatan: Menghapus catatan secara spesifik berdasarkan ID catatan.
7. Penyimpanan Permanen (File I/O): Data terintegrasi dengan file notes.txt. Catatan otomatis di-load saat program berjalan dan di-save setiap ada perubahan.

# Under the Hood (Teknologi & Algoritma)

Aplikasi ini tidak sekadar menyimpan data di array biasa, melainkan menggunakan beberapa konsep teknis berikut:

1. Struktur Data: Menggunakan Singly Linked List (struct Note) untuk menyimpan kumpulan catatan secara dinamis.
2. Algoritma Sorting: Menggunakan Bubble Sort untuk fitur pengurutan catatan (A-Z) berdasarkan judul.
3. Manajemen Memori: Menerapkan alokasi memori dinamis menggunakan malloc() dan mencegah kebocoran memori (memory leak) menggunakan free() (aman dari memory leak yak geyzt yak!).

# Cara Menjalankan Program

1. Kompilasi Kode
   Pastikan kamu sudah memiliki compiler C++ (seperti MinGW/GCC). Buka terminal dan kompilasi kode dengan perintah:
   g++ main.cpp -o noteapp

2. Jalankan Aplikasi
   Setelah berhasil dikompilasi, jalankan program:
   Windows: noteapp.exe
   Linux/Mac: ./noteapp

3. Kredensial Login Default
   Saat program pertama kali dijalankan, kamu akan diminta untuk login. Gunakan credentials bawaan berikut:
   Username: vektorjoyce
   Password: vekjoy123

# Kontributor / Task Division

Vektor:

1. Pembuatan kerangka program
2. Linked List
3. Bubble Sort
4. File I/O (savenotesfile, loadnotesfile)
5. Delete logic
6. Menu handling
7. dan Memory Management.

Joyce:

1. tambahnotekelist()
2. searchingtittle()
3. tambahnote()
4. lihatnote()
5. carinote()
