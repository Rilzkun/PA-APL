#include <iostream>    
// assalamualaikum
#include <string>
using namespace std;

// === STRUCT SECTION === //
struct Akun {
    string email;
    string password;
    string nama;
    string role; // admin, akuntan, manajer
};

struct Transaksi {
    string jenis; // pemasukan/pengeluaran
    string deskripsi;
    float jumlah;
    string status; // pending, disetujui, ditolak (untuk pengeluaran)
};

// === GLOBAL VARIABLE SECTION === //
const int MAKS_AKUN = 100;
const int MAKS_TRANSAKSI = 100;

Akun daftarAkun[MAKS_AKUN];
int totalAkun = 0;

Transaksi transaksi[MAKS_TRANSAKSI];
int totalTransaksi = 0;

string currentUserEmail;
string currentUserRole;

// === FUNCTION DECLARATION SECTION === //
// Login
bool login();

// Menu
void menuUtama();
void menuAdmin();
void menuAkuntan();
void menuManajer();

// Fungsi Admin
void tambahAkun();
void lihatDaftarAkun();
void hapusAkun();
void lihatHistory();

// Fungsi Akuntan
void catatPemasukan();
void ajukanPengeluaran();
void lihatTransaksi();
void ringkasanPemasukan(); // placeholder

// Fungsi Manajer
void ringkasanKeuangan(); // placeholder
void setujuiAnggaran();
void sortingTransaksi(); // placeholder

// Util
void clearScreen();
void pause();

int main() {
    menuUtama();
    return 0;
}

// === FUNCTION IMPLEMENTATION SECTION === //

void menuUtama() {
    int pilihan;
    bool isRunning = true;

    while (isRunning) {
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Login\n";
        cout << "2. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                int percobaan = 0;
                bool sukses = false;

                while (percobaan < 3 && !sukses) {
                    sukses = login();
                    if (!sukses) {
                        percobaan++;
                        cout << "Login gagal! Percobaan ke-" << percobaan << endl;
                    }
                }

                if (!sukses) {
                    cout << "Gagal login sebanyak 3 kali. Kembali ke menu utama.\n";
                } else {
                    // Arahkan ke menu sesuai role
                    if (currentUserRole == "admin") {
                        menuAdmin();
                    } else if (currentUserRole == "akuntan") {
                        menuAkuntan();
                    } else if (currentUserRole == "manajer") {
                        menuManajer();
                    } else {
                        cout << "Role tidak dikenal!\n";
                    }
                }
                break;
            }
            case 2:
                cout << "Terima kasih telah menggunakan program ini!\n";
                isRunning = false;
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    }
}


bool login() {
    string emailInput, passwordInput;
    cout << "\n=== LOGIN ===\n";
    cout << "Email: ";
    cin >> emailInput;
    cout << "Password: ";
    cin >> passwordInput;

    for (int i = 0; i < totalAkun; i++) {
        if (daftarAkun[i].email == emailInput && daftarAkun[i].password == passwordInput) {
            currentUserEmail = emailInput;
            currentUserRole = daftarAkun[i].role;
            cout << "Login berhasil sebagai " << currentUserRole << "!\n";
            return true;
        }
    }
    return false;
}


int main() {
    menuUtama();
    return 0;
}