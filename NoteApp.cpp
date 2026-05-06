#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
using namespace std;

// konstanta ygy
const int titlemax = 100;
const int maxnote = 500;
const char notes_file[] = "notes.txt";

// struct node linked list ygy
struct Note
{
    int id;
    char judul[titlemax];
    char isi[maxnote];
    Note *next;
};
// --- TARUH DI SINI YGY ---
Note* searchingtittleRecursive(Note* current, const char* keywordnotes);
// -------------------------
// variable global ygy
Note *head = NULL;
int notehitung = 0;
FILE *tunjuk;

// simpan notes ke file ygy
void savenotesfile()
{
    tunjuk = fopen(notes_file, "w");
    // error handling ygy
    if (!tunjuk)
    {
        cout << "Gagal membuat atau membuka file untuk menyimpan!" << endl;
        return;
    }

    // tulis penghitung note ygy
    fprintf(tunjuk, "%d\n", notehitung);
    Note *current = head;
    while (current)
    {
        fprintf(tunjuk, "%d\n", current->id);
        fprintf(tunjuk, "%s\n", current->judul);
        fprintf(tunjuk, "%s\n", current->isi);
        current = current->next;
    }
    fclose(tunjuk);
}

// load notes dari file ygy
void loadnotesfile()
{
    tunjuk = fopen(notes_file, "r");
    // error handling ygy
    if (!tunjuk)
    {
        cout << "Gagal memuat file!" << endl;
        return;
    }

    // mendefinisikan buffer untuk membaca per baris ygy
    char buffer[titlemax + 10];

    // baca penghitung note dari baris pertama ygy
    if (fgets(buffer, sizeof(buffer), tunjuk))
    {
        int hasilangka = 0;
        for (int i = 0; buffer[i] >= '0' && buffer[i] <= '9'; i++)
        {
            hasilangka = (hasilangka * 10) + (buffer[i] - '0');
        }
        notehitung = hasilangka;
    }

    int id;
    char judul[titlemax], isi[maxnote];

    // looping untuk membaca sisa note ygy
    while (fgets(buffer, sizeof(buffer), tunjuk))
    {
        int hasilid = 0;
        for (int i = 0; buffer[i] >= '0' && buffer[i] <= '9'; i++)
        {
            hasilid = (hasilid * 10) + (buffer[i] - '0');
        }
        id = hasilid;

        // baris untuk judul ygy
        if (!fgets(judul, sizeof(judul), tunjuk))
            break;
        // cari enter (\n) di judul diganti menjadi \0 atau null ygy
        for (int i = 0; judul[i] != '\0'; i++)
        {
            if (judul[i] == '\n')
            {
                judul[i] = '\0';
                break;
            }
        }

        // baris untuk isi note ygy
        if (!fgets(isi, sizeof(isi), tunjuk))
            break;
        // cari enter (\n) di isi diganti menjadi \0 atau null ygy
        for (int i = 0; isi[i] != '\0'; i++)
        {
            if (isi[i] == '\n')
            {
                isi[i] = '\0';
                break;
            }
        }

        Note *node = (Note *)malloc(sizeof(Note));
        node->id = id;
        for (int i = 0; i < titlemax; i++)
        {
            node->judul[i] = judul[i];
            if (judul[i] == '\0')
                break;
        }
        for (int i = 0; i < maxnote; i++)
        {
            node->isi[i] = isi[i];
            if (isi[i] == '\0')
                break;
        }

        node->next = NULL;

        if (!head)
        {
            head = node;
        }
        else
        {
            Note *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = node;
        }
    }
    fclose(tunjuk);
}

// memory management agar tidak ada kebocoran memori yang dipinjam oleh bang malloc ygy
void freememolist()
{
    Note *current = head;
    while (current)
    {
        Note *next = current->next;
        free(current); // free ini adalah pasanagan dari malloc
        current = next;
    }
    head = NULL;
}

// ===================================================================tugas joyce
void tambahnotekelist(const char *judul, const char *isi)
{
    // 1. Alokasi memori untuk node baru menggunakan malloc ygy
    Note *nodebaru = (Note *)malloc(sizeof(Note));

    // Error Handling: Cek apakah memori berhasil dipesan ygy
    if (nodebaru == NULL)
    {
        cout << "[Fatal Error] Gagal mengalokasikan memori! Catatan batal disimpan." << endl;
        return;
    }

    // 2. Isi data ke dalam node baru
    nodebaru->id = ++notehitung; // Menaikkan counter ID otomatis
    
    // Gunakan loop atau strcpy untuk menyalin string ke struct ygy
    int i;
    for (i = 0; i < titlemax - 1 && judul[i] != '\0'; i++) {
        nodebaru->judul[i] = judul[i];
    }
    nodebaru->judul[i] = '\0'; // Pastikan diakhiri null terminator

    for (i = 0; i < maxnote - 1 && isi[i] != '\0'; i++) {
        nodebaru->isi[i] = isi[i];
    }
    nodebaru->isi[i] = '\0';

    nodebaru->next = NULL; // Karena ini node baru, next-nya ke NULL dulu

    // 3. Masukkan node ke dalam Linked List
    if (head == NULL)
    {
        // Jika list masih kosong, node baru jadi head ygy
        head = nodebaru;
    }
    else
    {
        // Jika sudah ada isinya, cari node terakhir (tail) lalu sambungkan
        Note *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = nodebaru;
    }

    // 4. Simpan perubahan ke file agar permanen ygy
    savenotesfile();
}

// rekursif untuk menghapus note berdasarkan id note ygy
int deletenoteid(int id)
{
    Note *current = head;
    Note *previous = NULL;
    while (current)
    {
        if (current->id == id)
        {
            if (previous)
                previous->next = current->next;
            else
                head = current->next;
            free(current);
            savenotesfile();
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

// rekursif untuk menghitung jumlah notes ygy
int menghitungnote()
{
    int count = 0;
    Note *current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

// function untuk sorting menggunakan bubble sort ygy
void bubblesort()
{
    int n = menghitungnote();
    if (n < 2)
    {
        return;
    }

    // mengumpulkan pointer node ke array sementara dulu ygy
    Note **array = (Note **)malloc(n * sizeof(Note *));
    Note *current = head;
    for (int i = 0; i < n; i++)
    {
        array[i] = current;
        current = current->next;
    }

    // bubble sort ni geyzt
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            int cmp = 0;
            for (int k = 0;; k++)
            {
                if (array[j]->judul[k] < array[j + 1]->judul[k])
                {
                    cmp = -1;
                    break;
                }
                else if (array[j]->judul[k] > array[j + 1]->judul[k])
                {
                    cmp = 1;
                    break;
                }
                else if (array[j]->judul[k] == '\0')
                {
                    cmp = 0;
                    break;
                }
            }

            if (cmp > 0)
            {
                Note *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    // menyusun ulang pointer next ygy
    head = array[0];
    for (int i = 0; i < n - 1; i++)
    {
        array[i]->next = array[i + 1];
    }
    array[n - 1]->next = NULL;
    free(array);
    savenotesfile();
}

// rekursif untuk searching ygy
// ===================================================================tugas joyce
// current: node yang sedang diperiksa sekarang
// keywordnotes: kata kunci yang dicari user
Note *searchingtittleRecursive(Note *current, const char *keywordnotes)
{
    // 1. BASE CASE: Jika current NULL, artinya sudah sampai ujung list tapi gak ketemu
    if (current == NULL)
    {
        return NULL;
    }

    // 2. CHECK: Apakah judul pada node saat ini mengandung keyword?
    // strstr digunakan untuk mencari potongan kata di dalam string
    if (strstr(current->judul, keywordnotes) != NULL)
    {
        return current; // Ketemu geyzt! Kembalikan alamat node ini
    }

    // 3. RECURSIVE STEP: Jika belum ketemu, panggil lagi fungsi ini
    // tapi geser ke node berikutnya (current->next)
    return searchingtittleRecursive(current->next, keywordnotes);
}

// ===================================================================tugas joyce
// Ini fungsi utama yang dipanggil di menu utama
Note *searchingtittle(const char *keywordnotes)
{
    // Kita mulai pencarian rekursif dari 'head'
    return searchingtittleRecursive(head, keywordnotes);
}
	
// yes or no
bool yesorno(string yort)
{
    cout << "Kembali ke menu utama? (Y/T) : ";
    cin >> yort;

    if (yort == "ya" || yort == "Ya" || yort == "YA" || yort == "yA" || yort == "y" || yort == "Y")
    {
        system("cls");
        return false;
    }
    else if (yort == "tidak" || yort == "Tidak" || yort == "TIDAK" || yort == "t" || yort == "T")
    {
        cout << "Program Selesai!" << endl;
        cout << "Terima kasih telah menggunakan program ini!" << endl;
        return true;
    }
    else
    {
        cout << "Opsi tidak tersedia. Silahkan input kembali" << endl;
        return yesorno("");
    }
}
//MENU UTAMA

// ===================================================================tugas joyce
void tambahnote()
{
    system("cls");
    char judul[titlemax], isi[maxnote];

    cout << "==============================" << endl;
    cout << "      TAMBAH CATATAN BARU     " << endl;
    cout << "==============================" << endl;

    // Error handling: membersihkan buffer agar getline tidak terlewat ygy
    cin.ignore(1000, '\n'); 
    
    cout << "Masukkan Judul Catatan: ";
    cin.getline(judul, titlemax);
    
    // Validasi input: judul tidak boleh kosong ygy
    if (strlen(judul) == 0) {
        cout << "\n[Error] Judul catatan tidak boleh kosong!" << endl;
        return;
    }

    cout << "Masukkan Isi Catatan  : ";
    cin.getline(isi, maxnote);

    // Memanggil fungsi penambah list
    tambahnotekelist(judul, isi);
    
    cout << "\n[Sukses] Catatan berhasil disimpan ke memory dan file ygy!" << endl;
}

// ===================================================================tugas joyce
void lihatnote()
{
    system("cls");
    cout << "==============================" << endl;
    cout << "      DAFTAR SELURUH NOTES    " << endl;
    cout << "==============================" << endl;

    // Error handling: cek jika list kosong ygy
    if (head == NULL) {
        cout << "\nCatatan masih kosong geyzt, yuk tambah dulu!" << endl;
        return;
    }

    Note *current = head;
    int index = 1;
    while (current != NULL)
    {
        cout << index++ << ". [" << current->id << "] " << current->judul << endl;
        cout << "   Isi: " << current->isi << endl;
        cout << "------------------------------" << endl;
        current = current->next;
    }
}

// ===================================================================tugas joyce
void carinote()
{
    system("cls");
    char keyword[titlemax];

    // Error handling: cek list kosong sebelum mencari ygy
    if (head == NULL) {
        cout << "Gak ada catatan yang bisa dicari geyzt!" << endl;
        return;
    }

    cout << "==============================" << endl;
    cout << "         CARI CATATAN         " << endl;
    cout << "==============================" << endl;
    
    cin.ignore(1000, '\n');
    cout << "Masukkan kata kunci judul: ";
    cin.getline(keyword, titlemax);

    // Memanggil fungsi searchingtittle yang sudah rekursif tadi ygy
    Note *hasil = searchingtittle(keyword);

    if (hasil != NULL)
    {
        cout << "\n[Data Ditemukan secara Rekursif!]" << endl;
        cout << "ID   : " << hasil->id << endl;
        cout << "Judul: " << hasil->judul << endl;
        cout << "Isi  : " << hasil->isi << endl;
    }
    else
    {
        cout << "\n[Info] Judul mengandung '" << keyword << "' tidak ditemukan ygy." << endl;
    }
}

void urutkannote()
{
    system("cls");
    cout << "==============================" << endl;
    cout << "      URUTKAN NOTES (A-Z)     " << endl;
    cout << "==============================" << endl;

    int n = menghitungnote();
    if (n < 2)
    {
        cout << "\n Perlu minimal 2 notesmu untuk diurutkan!" << endl;
        return;
    }

    bubblesort();
    cout << endl;
    cout << n << " Catatan Berhasil Diurutkan A-Z!" << endl;
    cout << "Urutan setelah sorting:" << endl;

    Note *current = head;
    int no = 1;
    while (current)
    {
        cout << " " << no++ << ". " << current->judul << endl;
        current = current->next;
    }
}

void hapusnote()
{
    system("cls");
    cout << "==============================" << endl;
    cout << "          HAPUS NOTES         " << endl;
    cout << "==============================" << endl;

    if (!head)
    {
        cout << "Belum ada note untuk dihapus bang" << endl;
        return;
    }

    // menampilkan notes dulu ygy
    Note *current = head;
    while (current)
    {
        cout << "ID: " << current->id << " " << current->judul << endl;
        current = current->next;
    }

    int id;
    cout << endl;
    cout << "Masukkan ID note yang ingin kamu hapus : ";
    while (!(cin >> id))
    {
        cout << "Input harus angka! Masukkan ID kembali: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (deletenoteid(id))
    {
        cout << "Catatan ID: " << id << " Berhasil Dihapus!" << endl;
    }
    else
    {
        cout << "ID: " << id << " Tidak ditemukan! Coba lagi." << endl;
    }
}

int main()
{
    int pilihan;
    string inputpassword;
    string inputusername;
    string password = "vekjoy123";
    string username = "vektorjoyce";
    bool found = false;
    int max_attempt = 5;
    bool kembalikemenuawal = true;
    int i = 1;

    while (kembalikemenuawal)
    {
        cout << "=====================" << endl;
        cout << " WELCOME TO NOTE APP " << endl;
        cout << "=====================" << endl;
        cout << "Silahkan login terlebih dahulu!" << endl;
        cout << "Masukkan username: ";
        cin >> inputusername;
        cout << "Masukkan Password: ";
        cin >> inputpassword;

        found = false;
        if (inputusername == username && inputpassword == password)
        {
            found = true;
            i = 1;
            do
            {
                system("cls");
                cout << "=====================" << endl;
                cout << "HALO, " << inputusername << endl;
                cout << "=====================" << endl;
                cout << "1. Tambah Catatan Baru" << endl;
                cout << "2. Lihat Semua Catatan" << endl;
                cout << "3. Cari Catatan" << endl;
                cout << "4. Urutkan Catatan" << endl;
                cout << "5. Hapus Catatan" << endl;
                cout << "6. Logout" << endl;
                cout << "Masukkan menu: ";
                while (!(cin >> pilihan))
                {
                    cout << "Input harus angka! Masukkan menu kembali: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }

                while (pilihan < 1 || pilihan > 6)
                {
                    cout << "Opsi tidak tersedia! Masukkan menu (1-6): ";
                    while (!(cin >> pilihan))
                    {
                        cout << "Input harus angka! Masukkan menu kembali: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                }

                switch (pilihan)
                {
                case 1:
                    do
                    {
                        tambahnote();
                    } while (yesorno(""));
                    break;
                case 2:
                    do
                    {
                        lihatnote();
                    } while (yesorno(""));
                    break;
                case 3:
                    do
                    {
                        carinote();
                    } while (yesorno(""));
                    break;
                case 4:
                    do
                    {
                        urutkannote();
                    } while (yesorno(""));
                    break;
                case 5:
                    do
                    {
                        hapusnote();
                    } while (yesorno(""));
                    break;
                case 6:
                    kembalikemenuawal = true;
                    break;
                }
            } while (pilihan != 6);
            cout << "Terima kasih, " << inputusername << "\nSampai jumpa di lembaran berikutnya!\n";
            system("pause");
            system("cls");
            break;
        }
        else
        {
            if (i < max_attempt)
            {
                int sisakesempatan = max_attempt - i;
                system("cls");
                cout << "Login gagal! Username atau Password salah!" << endl;
                cout << "Kesempatan tersisa " << sisakesempatan << endl;
                i++;
            }
            else
            {
                break;
            }
        }
    }
    if (!found)
    {
        system("cls");
        cout << "Login gagal! Akses ditolak." << endl;
        return 0;
    }
}
