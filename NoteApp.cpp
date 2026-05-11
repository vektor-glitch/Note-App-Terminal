#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// konstanta ygy
const int titlemax = 200;
const int maxnote = 2000;
const char notes_file[] = "notes.txt";

// struct node linked list ygy
struct Note
{
    int id;
    char judul[titlemax];
    char isi[maxnote];
    Note *next;
};

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
    // error handling ygy - jika file tidak ada, itu normal (belum ada data)
    if (!tunjuk)
    {
        // File belum ada, ini normal untuk pertama kali
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
        // ini untuk memvalidasi jika hasilangka 0 dan file tidak kosong, file mungkin corrupt
        if (hasilangka == 0)
        {
            notehitung = 0;
        }
        else
        {
            notehitung = hasilangka;
        }
    }
    else
    {
        // Error handling
        notehitung = 0;
        fclose(tunjuk);
        return;
    }

    int id;
    char judul[titlemax], isi[maxnote];

    // looping untuk membaca sisa note ygy
    while (fgets(buffer, sizeof(buffer), tunjuk))
    {
        // ini untuk memvalidasi cek apakah buffer bisa dikonversi ke integer
        int hasilid = 0;
        int validid = 0;
        for (int i = 0; buffer[i] >= '0' && buffer[i] <= '9'; i++)
        {
            hasilid = (hasilid * 10) + (buffer[i] - '0');
            validid = 1;
        }

        // error handling
        if (!validid)
        {
            cout << "Data corrupt ditemukan! Skip data yang tidak valid." << endl;
            continue;
        }

        id = hasilid;

        // baris untuk judul ygy
        if (!fgets(judul, sizeof(judul), tunjuk))
        {
            cout << "File tidak lengkap, proses load selesai." << endl;
            break;
        }
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
        {
            cout << "File tidak lengkap, proses load selesai." << endl;
            break;
        }
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

// menambahkan note ke list ygy
void tambahnotekelist(const char *judul, const char *isi)
{
    // Ini buat alokasi memori untuk node baru ygy
    Note *nodebaru = (Note *)malloc(sizeof(Note));

    // Error Handling ygy
    if (nodebaru == NULL)
    {
        cout << "[Fatal Error] Gagal mengalokasikan memori! Catatan batal disimpan." << endl;
        return;
    }

    // Isi data ke dalam node baru ygy
    nodebaru->id = ++notehitung;

    // ini buat menyalin string ke struct ygy
    int i;
    for (i = 0; i < titlemax - 1 && judul[i] != '\0'; i++)
    {
        nodebaru->judul[i] = judul[i];
    }
    nodebaru->judul[i] = '\0';

    for (i = 0; i < maxnote - 1 && isi[i] != '\0'; i++)
    {
        nodebaru->isi[i] = isi[i];
    }
    nodebaru->isi[i] = '\0';

    // Karena ini node baru, next-nya ke NULL dulu geyzt
    nodebaru->next = NULL;

    // Masukkan node ke dalam Linked List ygy
    if (head == NULL)
    {
        head = nodebaru;
    }
    else
    {
        Note *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = nodebaru;
    }
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
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
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

// rekurisif untuk searching ygy
Note *searchingtittle(Note *current, const char *keywordnotes)
{
    // error handling ygy
    if (current == NULL)
    {
        return NULL;
    }

    // konversi keyword ke lowercase untuk case insensitive search ygy
    char keywordlower[titlemax];
    for (int i = 0; i < titlemax && keywordnotes[i] != '\0'; i++)
    {
        if (keywordnotes[i] >= 'A' && keywordnotes[i] <= 'Z')
            keywordlower[i] = keywordnotes[i] + 32;
        else
            keywordlower[i] = keywordnotes[i];
    }
    keywordlower[titlemax - 1] = '\0';

    while (current != NULL)
    {
        // konversi judul ke lowercase ygy
        char judullower[titlemax];
        for (int i = 0; i < titlemax && current->judul[i] != '\0'; i++)
        {
            if (current->judul[i] >= 'A' && current->judul[i] <= 'Z')
                judullower[i] = current->judul[i] + 32;
            else
                judullower[i] = current->judul[i];
        }
        judullower[titlemax - 1] = '\0';

        int found = 0;
        for (int i = 0; judullower[i] != '\0'; i++)
        {
            int j = 0;
            while (keywordlower[j] != '\0' && judullower[i + j] == keywordlower[j])
            {
                j++;
            }
            if (keywordlower[j] == '\0')
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            return current; // Ketemu geyzt!
        }

        current = current->next;
    }

    return NULL;
}

bool yesorno(string yort)
{
    bool validinput = false;
    cin.ignore(1000, '\n'); // Bersihkan buffer input dari system("pause") ygy

    while (!validinput)
    {
        cout << "Kembali ke menu utama? (Y/T) : ";
        cin >> yort;

        // Validasi input tidak kosong ygy
        int panjangInput = 0;
        for (int i = 0; yort[i] != '\0'; i++)
        {
            panjangInput++;
        }

        if (panjangInput == 0)
        {
            cout << "Input tidak boleh kosong! Silahkan coba lagi." << endl;
            continue;
        }

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
            cout << "Opsi tidak tersedia! Input hanya (ya/tidak atau y/t). Silahkan coba lagi." << endl;
            continue;
        }
    }

    return false;
}

// ==menu utama ygy==

// menu tambah note ygy
void tambahnote()
{
    system("cls");
    char judul[titlemax], isi[maxnote];

    cout << "==============================" << endl;
    cout << "      TAMBAH CATATAN BARU     " << endl;
    cout << "==============================" << endl;

    // Error handling ygy
    cin.ignore(1000, '\n');

    cout << "PERINGATAN! \nJudul catatan hanya bisa diisi sebanyak 200 kata dan isi catatan hanya bisa diisi sebanyak 2000 kata. Jika melebihi batas, maka file catatan akan corupt dan tidak akan tersimpan." << endl;
    cout << endl;

    system("pause");

    // Loop untuk validasi judul tidak kosong ygy
    int panjangJudul = 0;
    bool validjudul = false;
    while (!validjudul)
    {
        cout << "Masukkan Judul Catatan: ";
        cin.getline(judul, titlemax);

        // Hitung panjang judul ygy
        panjangJudul = 0;
        for (int i = 0; judul[i] != '\0'; i++)
        {
            panjangJudul++;
        }

        if (panjangJudul == 0)
        {
            cout << "Gagal! Judul catatan tidak boleh kosong! Silahkan coba lagi." << endl;
            continue;
        }
        validjudul = true;
    }

    cout << "Masukkan Isi Catatan  : ";
    cin.getline(isi, maxnote);

    tambahnotekelist(judul, isi);

    cout << "\nCatatan berhasil disimpan ke memory dan file!" << endl;
    system("pause");
}

// menu lihat note ygy
void lihatnote()
{
    system("cls");
    cout << "==============================" << endl;
    cout << "     DAFTAR SELURUH NOTES     " << endl;
    cout << "==============================" << endl;

    // Error handling ygy
    if (head == NULL)
    {
        cout << "\nCatatan masih kosong geyzt, yuk tambah dulu!" << endl;
        system("pause");
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
    system("pause");
}

// menu cari note ygy
void carinote()
{
    system("cls");
    char keyword[titlemax];
    int panjangKeyword = 0;

    // Error handling
    if (head == NULL)
    {
        cout << "Gak ada catatan yang bisa dicari geyzt!" << endl;
        system("pause");
        return;
    }

    cout << "==============================" << endl;
    cout << "         CARI CATATAN         " << endl;
    cout << "==============================" << endl;

    cin.ignore(1000, '\n');
    cout << "Masukkan kata kunci judul: ";
    cin.getline(keyword, titlemax);

    // Validasi keyword tidak kosong
    for (int i = 0; keyword[i] != '\0'; i++)
    {
        panjangKeyword++;
    }

    if (panjangKeyword == 0)
    {
        cout << "\nError: Kata kunci tidak boleh kosong!" << endl;
        system("pause");
        return;
    }

    Note *hasil = searchingtittle(head, keyword);

    if (hasil != NULL)
    {
        cout << "\nData Berhasil Ditemukan!" << endl;
        cout << "ID   : " << hasil->id << endl;
        cout << "Judul: " << hasil->judul << endl;
        cout << "Isi  : " << hasil->isi << endl;
    }
    else
    {
        cout << "\nJudul mengandung '" << keyword << "' tidak ditemukan!" << endl;
    }
    system("pause");
}

// menu mengurutkan note ygy
void urutkannote()
{
    system("cls");
    cout << "==============================" << endl;
    cout << "      URUTKAN NOTES (A-Z)     " << endl;
    cout << "==============================" << endl;

    int n = menghitungnote();
    if (n < 2)
    {
        cout << "\nPerlu minimal 2 notes untuk diurutkan!" << endl;
        system("pause");
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
    system("pause");
}

// menu hapus note ygy
void hapusnote()
{
    system("cls");
    cout << "==============================" << endl;
    cout << "          HAPUS NOTES         " << endl;
    cout << "==============================" << endl;

    if (!head)
    {
        cout << "Belum ada note untuk dihapus bang!" << endl;
        system("pause");
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
    bool found = false;
    cout << endl;

    while (!found)
    {
        cout << "Masukkan ID note yang ingin kamu hapus : ";
        // error handling
        while (!(cin >> id))
        {
            cout << "Input harus angka! Masukkan ID kembali: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if (deletenoteid(id))
        {
            cout << "Catatan ID: " << id << " Berhasil Dihapus!" << endl;
            found = true;
        }
        else
        {
            cout << "ID: " << id << " Tidak ditemukan! Coba lagi." << endl;
        }
    }
    system("pause");
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
    bool validmenu = false;

    loadnotesfile();

    while (kembalikemenuawal)
    {
        validmenu = false;
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
                validmenu = false;
                system("cls");
                cout << "=====================" << endl;
                cout << "  HALO, " << inputusername << endl;
                cout << "=====================" << endl;
                cout << "1. Tambah Catatan Baru" << endl;
                cout << "2. Lihat Semua Catatan" << endl;
                cout << "3. Cari Catatan" << endl;
                cout << "4. Urutkan Catatan" << endl;
                cout << "5. Hapus Catatan" << endl;
                cout << "6. Logout" << endl;
                cout << "Masukkan menu: ";

                // Error handling untuk input harus angka
                while (!(cin >> pilihan))
                {
                    cout << "Input harus angka! Masukkan menu kembali: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }

                // Error handling untuk pilihan harus 1-6
                while (!validmenu)
                {
                    if (pilihan >= 1 && pilihan <= 6)
                    {
                        validmenu = true;
                    }
                    else
                    {
                        system("cls");
                        cout << "Opsi tidak tersedia! Silahkan input kembali." << endl;
                        cout << "=====================" << endl;
                        cout << "  HALO, " << inputusername << endl;
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
            freememolist();
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
        freememolist();
        return 0;
    }
}