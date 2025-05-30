#include <iostream>
#include <fstream>
#include <sstream>   
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
using namespace std;

void tampilkanJudul() {
    cout << R"( 
 __          ___   ____    __    ____  _______        ______   ______   .___  ___. .______      ___      .__   __. ____    ____ 
|  |        /   \  \   \  /  \  /   / |   ____|      /      | /  __  \  |   \/   | |   _  \    /   \     |  \ |  | \   \  /   / 
|  |       /  ^  \  \   \/    \/   /  |  |__        |  ,----'|  |  |  | |  \  /  | |  |_)  |  /  ^  \    |   \|  |  \   \/   /  
|  |      /  /_\  \  \            /   |   __|       |  |     |  |  |  | |  |\/|  | |   ___/  /  /_\  \   |  . `  |   \_    _/   
|  `----./  _____  \  \    /\    /    |  |____      |  `----.|  `--'  | |  |  |  | |  |     /  _____  \  |  |\   |     |  |     
|_______/__/     \__\  \__/  \__/     |_______|      \______| \______/  |__|  |__| | _|    /__/     \__\ |__| \__|     |__|     
                                                                                                                             
)" << endl;
}



// === STRUCT SECTION === //
struct Akun {
    string email;
    string password;
    string nama;
    string role; // admin, akuntan, manajer
};

struct Transaksi {

    string deskripsi;
    string jenis;
    int id;
    int jumlah;
};

// === GLOBALS === //
const int MAKS_AKUN = 100;
Akun daftarAkun[MAKS_AKUN];
int totalAkun = 0;

const int MAKS_TRANSAKSI = 100;
Transaksi daftartransaksi[MAKS_TRANSAKSI];
int totaltransaksi = 0;

const int MAKS_PROPOSAL = 100;
Transaksi daftarProposal[MAKS_PROPOSAL];
int totalProposal = 0;

string currentUserEmail;
string currentUserRole;



// === MENU UTAMA === //
int menuUtama() {
    int pilihan;
    cout << R"(
=============================================================================================================
| .___  ___.  _______ .__   __.  __    __      __    __  .___________.    ___      .___  ___.      ___      | 
| |   \/   | |   ____||  \ |  | |  |  |  |    |  |  |  | |           |   /   \     |   \/   |     /   \     | 
| |  \  /  | |  |__   |   \|  | |  |  |  |    |  |  |  | `---|  |----`  /  ^  \    |  \  /  |    /  ^  \    | 
| |  |\/|  | |   __|  |  . `  | |  |  |  |    |  |  |  |     |  |      /  /_\  \   |  |\/|  |   /  /_\  \   | 
| |  |  |  | |  |____ |  |\   | |  `--'  |    |  `--'  |     |  |     /  _____  \  |  |  |  |  /  _____  \  | 
| |__|  |__| |_______||__| \__|  \______/      \______/      |__|    /__/     \__\ |__|  |__| /__/     \__\ | 
=============================================================================================================
    )" << endl;
    cout << "1. Login\n";
    cout << "2. Keluar\n";
    cout << "Pilih menu: ";
    cin >> pilihan;
    while (cin.fail()) {
        cin.clear(); // Clear the error flag
        cin.ignore(1000, '\n'); // Ignore invalid input
        cout << "Input tidak valid. Silakan masukkan angka 1/2.\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
    }
    return pilihan;
}

// === LOGIN FUNCTION === //
int login() {
    string emailInput, passwordInput;
    int percobaan = 0;

    while (percobaan < 3) {
        cout << R"(
    ======================================================
    |     __        ______     _______  __  .__   __.    | 
    |    |  |      /  __  \   /  _____||  | |  \ |  |    | 
    |    |  |     |  |  |  | |  |  __  |  | |   \|  |    | 
    |    |  |     |  |  |  | |  | |_ | |  | |  . `  |    | 
    |    |  `----.|  `--'  | |  |__| | |  | |  |\   |    | 
    |    |_______| \______/   \______| |__| |__| \__|    |
    ======================================================
        )" << endl; 
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

bool simpanTransaksiKeCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "id,deskripsi,jenis,jumlah\n";
    for (int i = 0; i < totaltransaksi; i++) {
        file << daftartransaksi[i].id << ","
             << daftartransaksi[i].deskripsi << ","
             << daftartransaksi[i].jenis << ","
             << daftartransaksi[i].jumlah << "\n";
    }
    file.close();
    return true;
}

bool loadTransaksiDariCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line);

    totaltransaksi = 0;
    while (getline(file, line) && totaltransaksi < MAKS_TRANSAKSI) {
        stringstream ss(line);
        string idStr, deskripsi, jenis, jumlahStr;

        getline(ss, idStr, ',');
        getline(ss, deskripsi, ',');
        getline(ss, jenis, ',');
        getline(ss, jumlahStr, ',');

        daftartransaksi[totaltransaksi++] = {deskripsi, jenis, stoi(idStr), stoi(jumlahStr)};
    }
    file.close();
    return true;
}

bool simpanProposal(const Transaksi& proposal) {
    bool fileBaru = false;
    ifstream cek("proposal.csv");
    if (!cek.good()) fileBaru = true;
    cek.close();

    ofstream file("proposal.csv", ios::app);
    if (!file.is_open()) return false;

    if (fileBaru) {
        file << "id,deskripsi,jenis,jumlah\n";  // Tulis header jika file baru
    }

    file << proposal.id << ","
         << proposal.deskripsi << ","
         << proposal.jenis << ","
         << proposal.jumlah << "\n";

    file.close();
    return true;
}

void setujuiProposal() {
    ifstream file("proposal.csv");
    if (!file.is_open()) {
        cout << "Tidak ada proposal untuk ditinjau.\n";
        return;
    }

    string line;
    vector<Transaksi> disetujui;

    if (!getline(file, line)) {  // file kosong (tidak ada header)
        cout << "Tidak ada proposal untuk ditinjau.\n";
        return;
    }

    bool adaProposal = false;
    while (getline(file, line)) {
        adaProposal = true;
        stringstream ss(line);
        string idStr, deskripsi, jenis, jumlahStr;

        getline(ss, idStr, ',');
        getline(ss, deskripsi, ',');
        getline(ss, jenis, ',');
        getline(ss, jumlahStr, ',');

        cout << "Proposal: " << deskripsi << " - Jumlah: " << jumlahStr << endl;
        cout << "Setujui? (y/n): ";
        char jawab;
        cin >> jawab;

        if (jawab == 'y' || jawab == 'Y') {
            Transaksi t = {deskripsi, jenis, totaltransaksi + 1, stoi(jumlahStr)};
            daftartransaksi[totaltransaksi++] = t;
            disetujui.push_back(t);
        }
    }

    if (!adaProposal) {
        cout << "Tidak ada proposal untuk ditinjau.\n";
    }

    file.close();
    simpanTransaksiKeCSV("transaksi.csv");
    ofstream clearFile("proposal.csv", ios::trunc);
    clearFile.close();
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
    cout << "\n=============================== DAFTAR AKUN ===============================\n";
    if (totalAkun == 0) {
        cout << "Tidak ada akun yang terdaftar.\n";
        return;
    }
    cout << "+-----+--------------------+------------------------------+---------------+" << endl;
    cout << "|" << left << setw(5) << "No"
         << "|" << left << setw(20) << "Nama"
         << "|" << left << setw(30) << "Email"
         << "|" << left << setw(15) << "Role" << "|" << endl;
    cout << "+-----+--------------------+------------------------------+---------------+" << endl;
    for (int i = 0; i < totalAkun; i++) {
        cout << "|" << left << setw(5) << (i + 1)
             << "|" << left << setw(20) << daftarAkun[i].nama
             << "|" << left << setw(30) << daftarAkun[i].email
             << "|" << left << setw(15) << daftarAkun[i].role << "|" << endl;
    }
    cout << "+-----+--------------------+------------------------------+---------------+" << endl;
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


// === MENU ADMIN === //
void menuAdmin() {
    int pilihan;
    
    do {
        cout << R"(
===================================================================================================
|  .___  ___.  _______ .__   __.  __    __          ___       _______  .___  ___.  __  .__   __.  |
|  |   \/   | |   ____||  \ |  | |  |  |  |        /   \     |       \ |   \/   | |  | |  \ |  |  |
|  |  \  /  | |  |__   |   \|  | |  |  |  |       /  ^  \    |  .--.  ||  \  /  | |  | |   \|  |  |
|  |  |\/|  | |   __|  |  . `  | |  |  |  |      /  /_\  \   |  |  |  ||  |\/|  | |  | |  . `  |  |
|  |  |  |  | |  |____ |  |\   | |  `--'  |     /  _____  \  |  '--'  ||  |  |  | |  | |  |\   |  |
|  |__|  |__| |_______||__| \__|  \______/     /__/     \__\ |_______/ |__|  |__| |__| |__| \__|  |
===================================================================================================
)";
        cout << "|  1  | Tambah Akun Baru                                                                          |\n";
        cout << "|  2  | Lihat Daftar Akun                                                                         |\n";
        cout << "|  3  | Hapus Akun                                                                                |\n";
        cout << "|  4  | Kembali ke Menu Utama                                                                     |\n";
        cout << "+-----+-------------------------------------------------------------------------------------------+\n";
        cout << "Pilih menu: ";

        cin >> pilihan;
        while (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(1000, '\n'); // Ignore invalid input
            cout << "Input tidak valid. Silakan masukkan angka.\n";
            cout << "Pilih menu: ";
            cin >> pilihan;
        }

        switch (pilihan) {
            case 1: {
                Akun akunBaru;
                cout << "\n--- Tambah Akun Baru ---\n";
                // Validasi email
                while (true) {
                    cout << "Email : ";
                    getline(cin, akunBaru.email);
                    if (akunBaru.email.size() >= 9 && akunBaru.email.substr(akunBaru.email.size() - 9) == "@lawe.com") {
                        break;
                    } else {
                        cout << "Email harus diakhiri dengan '@lawe.com'. Silakan ulangi.\n";
                    }
                }
                cout << "Password : ";
                getline(cin, akunBaru.password);
                cout << "Nama : ";
                getline(cin, akunBaru.nama);
                // Validasi role
                while (true) {
                    cout << "Role (admin/akuntan/manajer): ";
                    getline(cin, akunBaru.role);
                    if (akunBaru.role == "admin" || akunBaru.role == "akuntan" || akunBaru.role == "manajer") {
                        break;
                    } else {
                        cout << "Role hanya boleh 'admin', 'akuntan', atau 'manajer'. Silakan ulangi.\n";
                    }
                }
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

void urutkanJumlahAscending() {
    for (int i = 0; i < totaltransaksi - 1; i++) {
        for (int j = 0; j < totaltransaksi - i - 1; j++) {
            if (daftartransaksi[j].jumlah > daftartransaksi[j + 1].jumlah) {
                Transaksi temp = daftartransaksi[j];
                daftartransaksi[j] = daftartransaksi[j + 1];
                daftartransaksi[j + 1] = temp;
            }
        }
    }
}

void urutkanJumlahDescending() {
    for (int i = 0; i < totaltransaksi - 1; i++) {
        for (int j = 0; j < totaltransaksi - i - 1; j++) {
            if (daftartransaksi[j].jumlah < daftartransaksi[j + 1].jumlah) {
                Transaksi temp = daftartransaksi[j];
                daftartransaksi[j] = daftartransaksi[j + 1];
                daftartransaksi[j + 1] = temp;
            }
        }
    }
}

void urutkanIDAscending() {
    for (int i = 0; i < totaltransaksi - 1; i++) {
        for (int j = 0; j < totaltransaksi - i - 1; j++) {
            if (daftartransaksi[j].id > daftartransaksi[j + 1].id) {
                Transaksi temp = daftartransaksi[j];
                daftartransaksi[j] = daftartransaksi[j + 1];
                daftartransaksi[j + 1] = temp;
            }
        }
    }
}

void urutkanIDDescending() {
    for (int i = 0; i < totaltransaksi - 1; i++) {
        for (int j = 0; j < totaltransaksi - i - 1; j++) {
            if (daftartransaksi[j].id < daftartransaksi[j + 1].id) {
                Transaksi temp = daftartransaksi[j];
                daftartransaksi[j] = daftartransaksi[j + 1];
                daftartransaksi[j + 1] = temp;
            }
        }
    }
}

void catatpemasukan() {
    Transaksi t;
    t.jenis = "pemasukan";
    t.id = totaltransaksi + 1;
    cout << "Deskripsi: ";
    cin.ignore();
    getline(cin, t.deskripsi);
    cout << "Jumlah: ";
    cin >> t.jumlah ;

    daftartransaksi[totaltransaksi++] = t;
    simpanTransaksiKeCSV("transaksi.csv");
    cout << "Pemasukan dicatat.\n";
}

void ajukanProposal() {
    Transaksi proposal;
    proposal.jenis = "pengeluaran";
    proposal.id = totalProposal + 1;  
    
    cout << "Deskripsi: ";
    cin.ignore();
    getline(cin, proposal.deskripsi);
    
    cout << "Jumlah: ";
    cin >> proposal.jumlah;

    daftarProposal[totalProposal++] = proposal;
    if (simpanProposal(proposal)) {
        cout << "Proposal berhasil diajukan dengan ID: " << proposal.id << ".\n";
    }
}

void lihatDaftarTransaksi() {
    cout << "\n=== DAFTAR TRANSAKSI ===\n";
    for (int i = 0; i < totaltransaksi; i++) {
        cout << daftartransaksi[i].id << ". " << daftartransaksi[i].deskripsi << " | " << daftartransaksi[i].jenis << " | " << daftartransaksi[i].jumlah << endl;
    }
}

void tampilkanRingkasanKeuangan() {
    int totalPemasukan = 0, totalPengeluaran = 0;
    for (int i = 0; i < totaltransaksi; i++) {
        if (daftartransaksi[i].jenis == "pemasukan")
            totalPemasukan += daftartransaksi[i].jumlah;
        else if (daftartransaksi[i].jenis == "pengeluaran")
            totalPengeluaran += daftartransaksi[i].jumlah;
    }
    int saldo = totalPemasukan - totalPengeluaran;

    cout << "\n=== RINGKASAN KEUANGAN ===\n";
    cout << "Total Pemasukan : " << totalPemasukan << endl;
    cout << "Total Pengeluaran: " << totalPengeluaran << endl;
    cout << "Saldo Saat Ini  : " << saldo << endl;
}

void menuAkuntan() {
    int pilihan; 
    do {
        cout << R"(
=====================================================================================================================
| .___  ___.  _______ .__   __.  __    __          ___       __  ___  __    __  .___________.    ___      .__   __. |
| |   \/   | |   ____||  \ |  | |  |  |  |        /   \     |  |/  / |  |  |  | |           |   /   \     |  \ |  | |
| |  \  /  | |  |__   |   \|  | |  |  |  |       /  ^  \    |  '  /  |  |  |  | `---|  |----`  /  ^  \    |   \|  | |
| |  |\/|  | |   __|  |  . `  | |  |  |  |      /  /_\  \   |    <   |  |  |  |     |  |      /  /_\  \   |  . `  | |
| |  |  |  | |  |____ |  |\   | |  `--'  |     /  _____  \  |  .  \  |  `--'  |     |  |     /  _____  \  |  |\   | |
| |__|  |__| |_______||__| \__|  \______/     /__/     \__\ |__|\__\  \______/      |__|    /__/     \__\ |__| \__| |
=====================================================================================================================
)";
        cout << "|  1  | Catat Pemasukan                                                                                             |\n";
        cout << "|  2  | Ajukan Proposal Pengeluaran                                                                                 |\n";
        cout << "|  3  | Lihat Daftar Transaksi                                                                                      |\n";
        cout << "|  4  | Urutkan Daftar Transaksi                                                                                    |\n";
        cout << "|  5  | Laporan Ringkasan Keuangan                                                                                  |\n";
        cout << "|  6  | Kembali Ke Menu Utama                                                                                       |\n";
        cout << "+-----+-------------------------------------------------------------------------------------------------------------+\n";
        cout << "Masukkan pilihanmu: ";
        cin >> pilihan;
        while (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(1000, '\n'); // Ignore invalid input
            cout << "Input tidak valid. Silakan masukkan angka 1-6.\n";
            cout << "Masukkan pilihanmu: ";
            cin >> pilihan;
        }

        switch (pilihan) {
            case 1:
                catatpemasukan();
                break;           
            case 2:
                ajukanProposal();
                break;
            case 3:
                lihatDaftarTransaksi();
                break;
            case 4: {
                int pilihsort;;
                cout << "\n=== URUTKAN TRANSAKSI ===" << endl;
                cout << "1. Berdasarkan Jumlah (Ascending)" << endl;
                cout << "2. Berdasarkan Jumlah (Descending)" << endl;
                cout << "3. Berdasarkan ID (Ascending)" << endl;
                cout << "4. Berdasarkan ID (Descending)" << endl;
                cout << "Pilih metode pengurutan: ";
                cin >> pilihsort;
                while (cin.fail() || pilihsort < 1 || pilihsort > 4) {
                    cin.clear(); // Clear the error flag
                    cin.ignore(1000, '\n'); // Ignore invalid input
                    cout << "Input tidak valid. Silakan masukkan angka 1-4.\n";
                    cout << "Pilih metode pengurutan: ";
                    cin >> pilihsort;
                }

                switch (pilihsort) {
                    case 1:
                        urutkanJumlahAscending();
                        cout << "Transaksi diurutkan berdasarkan jumlah (ascending)." << endl;
                        lihatDaftarTransaksi();
                        break;
                    case 2:
                        urutkanJumlahDescending();
                        cout << "Transaksi diurutkan berdasarkan jumlah (descending)." << endl;
                        lihatDaftarTransaksi();
                        break;
                    case 3:
                        urutkanIDAscending();
                        cout << "Transaksi diurutkan berdasarkan ID (ascending)." << endl;
                        lihatDaftarTransaksi();
                        break;
                    case 4:
                        urutkanIDDescending();
                        cout << "Transaksi diurutkan berdasarkan ID (descending)." << endl;
                        lihatDaftarTransaksi();
                        break;
                    default:
                        cout << "Pilihan tidak valid." << endl;
                }
                break;
            }
            case 5:
                tampilkanRingkasanKeuangan();
                break;
            case 6:
                cout << "Kembali ke menu utama" << endl;
                break;
            default:
                cout << "Pilihan tidak valid" << endl;
        }
    } while (pilihan != 6);
}

void menuManajer() {
    int pilihan;  
    do{
        cout << R"(
===============================================================================================================================
| .___  ___.  _______ .__   __.  __    __     .___  ___.      ___      .__   __.      ___       _______  _______ .______      |
| |   \/   | |   ____||  \ |  | |  |  |  |    |   \/   |     /   \     |  \ |  |     /   \     /  _____||   ____||   _  \     |
| |  \  /  | |  |__   |   \|  | |  |  |  |    |  \  /  |    /  ^  \    |   \|  |    /  ^  \   |  |  __  |  |__   |  |_)  |    |
| |  |\/|  | |   __|  |  . `  | |  |  |  |    |  |\/|  |   /  /_\  \   |  . `  |   /  /_\  \  |  | |_ | |   __|  |      /     |
| |  |  |  | |  |____ |  |\   | |  `--'  |    |  |  |  |  /  _____  \  |  |\   |  /  _____  \ |  |__| | |  |____ |  |\  \----.|
| |__|  |__| |_______||__| \__|  \______/     |__|  |__| /__/     \__\ |__| \__| /__/     \__\ \______| |_______|| _| `._____||
===============================================================================================================================
)";

        cout << "|  1  | Tampilkan Laporan Keuangan                                                                                            |\n";
        cout << "|  2  | Lihat Proposal                                                                                                        |\n";
        cout << "|  3  | Kembali Ke Menu Utama                                                                                                 |\n";
        cout << "+-----+-----------------------------------------------------------------------------------------------------------------------+\n";
        cout<<"Masukkan Pilihan: ";
        cin>>pilihan;
        while (cin.fail() || pilihan < 1 || pilihan > 3) {
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            cout << "Input tidak valid. Silakan masukkan angka 1-3.\n";
            cout << "Masukkan Pilihan: ";
            cin >> pilihan;
        }

        switch (pilihan)
        {
            case 1:
                tampilkanRingkasanKeuangan();
                break;
            case 2:
                setujuiProposal();
                break;
            case 3:
                cout << "Kembali Ke Menu Utama" << endl;
                break;
            default:
                cout<<"pilihan tidak valid"<<endl;        
                break;
        }
    }while (pilihan !=3);
}

// === MAIN FUNCTION === //
int main() {
    if (!loadAkunDariCSV("akun.csv")) {
        cout << "Gagal memuat data akun dari CSV.\n";
    }
        if (!loadTransaksiDariCSV("transaksi.csv")) {
        cout << "Gagal memuat data transaksi dari CSV.\n";
    }
    while (true) {
        tampilkanJudul();
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
            cout << R"(
              _____        _              _  __        _ _                                   
             |_   _|__ _ _(_)_ __  __ _  | |/ /__ _ __(_) |_                                 
               | |/ -_) '_| | '  \/ _` | | ' </ _` (_-< | ' \                                
               |_|\___|_| |_|_|_|_\__,_| |_|\_\__,_/__/_|_||_|                               
              _____    _      _      __  __                                   _              
             |_   _|__| |__ _| |_   |  \/  |___ _ _  __ _ __ _ _  _ _ _  __ _| |____ _ _ _   
               | |/ -_) / _` | ' \  | |\/| / -_) ' \/ _` / _` | || | ' \/ _` | / / _` | ' \  
               |_|\___|_\__,_|_||_| |_|  |_\___|_||_\__, \__, |\_,_|_||_\__,_|_\_\__,_|_||_| 
              ___                                _  |___/|___/                               
             | _ \_ _ ___  __ _ _ _ __ _ _ __   (_)_ _ (_)                                   
             |  _/ '_/ _ \/ _` | '_/ _` | '  \  | | ' \| |                                   
             |_| |_| \___/\__, |_| \__,_|_|_|_| |_|_||_|_|                                   
                          |___/                                                              
            )" << endl;
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }

    return 0;
}