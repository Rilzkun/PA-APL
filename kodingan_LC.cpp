#include <iostream>
#include <fstream>
#include <sstream>   
#include <string>
using namespace std;

// === STRUCT SECTION === //
struct Akun {
    string email;
    string password;
    string nama;
    string role; // admin, akuntan, manajer
};

// === GLOBALS === //
const int MAKS_AKUN = 100;
Akun daftarAkun[MAKS_AKUN];
int totalAkun = 0;

string currentUserEmail;
string currentUserRole;

// === FUNCTION DECLARATIONS === //
bool loadAkunDariCSV(const string& filename);
bool simpanAkunKeCSV(const string& filename);
int menuUtama();
int login();
void menuAdmin();
void menuAkuntan();
void menuManajer();

bool tambahAkun(Akun akunBaru);
void lihatDaftarAkun();
bool hapusAkun(const string& emailTarget);

// === MAIN FUNCTION === //
int main() {
    if (!loadAkunDariCSV("akun.csv")) {
        cout << "Gagal memuat data akun dari CSV.\n";
    }

    while (true) {
        int pilih = menuUtama();
        if (pilih == 1) {
            int idxLogin = login();
            if (idxLogin != -1) {
                currentUserEmail = daftarAkun[idxLogin].email;
                currentUserRole = daftarAkun[idxLogin].role;

                if (currentUserRole == "admin") menuAdmin();
                else if (currentUserRole == "akuntan") menuAkuntan();
                else if (currentUserRole == "manajer") menuManajer();
                else cout << "Role tidak dikenali.\n";
            } else {
                cout << "Login gagal 3x. Kembali ke menu utama.\n";
            }
        } else if (pilih == 2) {
            cout << "Keluar dari program.\n";
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }

    return 0;
}

// === MENU UTAMA === //
int menuUtama() {
    int pilihan;
    cout << "\n=== MENU UTAMA ===\n";
    cout << "1. Login\n";
    cout << "2. Keluar\n";
    cout << "Pilih menu: ";
    cin >> pilihan;
    return pilihan;
}

// === LOGIN FUNCTION === //
int login() {
    string emailInput, passwordInput;
    int percobaan = 0;

    while (percobaan < 3) {
        cout << "\n=== LOGIN ===\n";
        cout << "Email: ";
        cin >> emailInput;
        cout << "Password: ";
        cin >> passwordInput;

        for (int i = 0; i < totalAkun; i++) {
            if (daftarAkun[i].email == emailInput && daftarAkun[i].password == passwordInput) {
                cout << "Login berhasil sebagai " << daftarAkun[i].role << "!\n";
                return i;
            }
        }

        cout << "Login gagal!\n";
        percobaan++;
    }

    return -1;
}

// === CSV FUNCTIONS === //
bool loadAkunDariCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); // skip header

    totalAkun = 0;
    while (getline(file, line) && totalAkun < MAKS_AKUN) {
        stringstream ss(line);
        string email, password, nama, role;

        getline(ss, email, ',');
        getline(ss, password, ',');
        getline(ss, nama, ',');
        getline(ss, role, ',');

        daftarAkun[totalAkun++] = {email, password, nama, role};
    }

    file.close();
    return true;
}

bool simpanAkunKeCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "email,password,nama,role\n";
    for (int i = 0; i < totalAkun; i++) {
        file << daftarAkun[i].email << ","
             << daftarAkun[i].password << ","
             << daftarAkun[i].nama << ","
             << daftarAkun[i].role << "\n";
    }

    file.close();
    return true;
}

// === MENU ADMIN === //
void menuAdmin() {
    int pilihan;
    do {
        cout << "\n=== MENU ADMIN ===\n";
        cout << "1. Tambah Akun Baru\n";
        cout << "2. Lihat Daftar Akun\n";
        cout << "3. Hapus Akun\n";
        cout << "4. Kembali ke Menu Utama\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                Akun akunBaru;
                cout << "\n--- Tambah Akun Baru ---\n";
                cout << "Email    : ";
                cin >> akunBaru.email;
                cout << "Password : ";
                cin >> akunBaru.password;
                cin.ignore();
                cout << "Nama     : ";
                getline(cin, akunBaru.nama);
                cout << "Role (admin/akuntan/manajer): ";
                cin >> akunBaru.role;

                if (tambahAkun(akunBaru))
                    cout << "Akun berhasil ditambahkan!\n";
                else
                    cout << "Gagal menambahkan akun.\n";
                break;
            }

            case 2:
                lihatDaftarAkun();
                break;

            case 3: {
                string emailHapus;
                cout << "Masukkan email akun yang ingin dihapus: ";
                cin >> emailHapus;

                if (hapusAkun(emailHapus))
                    cout << "Akun berhasil dihapus.\n";
                else
                    cout << "Akun tidak ditemukan.\n";
                break;
            }

            case 4:
                cout << "Kembali ke menu utama...\n";
                break;

            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 4);
}

// === ADMIN FUNCTIONS === //
bool tambahAkun(Akun akunBaru) {
    if (totalAkun >= MAKS_AKUN) return false;

    for (int i = 0; i < totalAkun; i++) {
        if (daftarAkun[i].email == akunBaru.email) {
            cout << "Email sudah terdaftar.\n";
            return false;
        }
    }

    daftarAkun[totalAkun++] = akunBaru;
    simpanAkunKeCSV("akun.csv");
    return true;
}

void lihatDaftarAkun() {
    cout << "\n=== DAFTAR AKUN ===\n";
    for (int i = 0; i < totalAkun; i++) {
        cout << i + 1 << ". "
             << daftarAkun[i].nama << " | "
             << daftarAkun[i].email << " | "
             << daftarAkun[i].role << endl;
    }
}

bool hapusAkun(const string& emailTarget) {
    int index = -1;
    for (int i = 0; i < totalAkun; i++) {
        if (daftarAkun[i].email == emailTarget) {
            index = i;
            break;
        }
    }

    if (index == -1) return false;

    for (int i = index; i < totalAkun - 1; i++) {
        daftarAkun[i] = daftarAkun[i + 1];
    }

    totalAkun--;
    simpanAkunKeCSV("akun.csv");
    return true;
}

// === MENU AKUNTAN & MANAJER (placeholder) === //
void menuAkuntan() {
    cout << "\n[AKUNTAN MODE] - Menu masih kosong.\n";
}

void menuManajer() {
    cout << "\n[MANAJER MODE] - Menu masih kosong.\n";
}
