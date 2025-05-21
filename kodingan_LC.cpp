#include <iostream>
#include <fstream>
#include <sstream>   
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

#include <iostream>
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

void clearscreen(){
    system("cls");

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

void catatpemasukan();
void ajukanProposal();
void lihatDaftarTransaksi();
void tampilkanRingkasanKeuangan();
void lihatProposal();
bool simpanTransaksiKeCSV(const string& filename);
bool loadTransaksiDariCSV(const string& filename);
bool simpanProposal(const Transaksi& proposal);
void setujuiProposal();
void tampilkanNotifikasi(); 

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


void tampilkanNotifikasi() {
    ifstream file("notifikasi.csv");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        cout << "[NOTIFIKASI]: " << line << endl;
    }
    file.close();

    ofstream clear("notifikasi.csv", ios::trunc);
    clear.close();
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

            ofstream notif("notifikasi.csv", ios::app);
            notif << "Proposal \"" << deskripsi << "\" disetujui oleh manajer.\n";
            notif.close();
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


// === MENU ADMIN === //
void menuAdmin() {
    clearscreen();
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
    clearscreen();
    int pilihan; 
    do{
        cout<<endl<<"----menu akuntan----"<<endl;
        cout<<"1. Catat Pemasukan"<<endl;
        cout<<"2. Ajukan Proposal Pengeluaran"<<endl;
        cout<<"3. Lihat  Daftar Transaksi"<<endl;
        cout<<"4. Laporan Ringkasan Keuangan"<<endl;
        cout<<"5. Kembali Ke Menu Utama"<<endl;
        cout<<"Masukkan pilihanmu; ";
        cin>>pilihan;
        switch (pilihan)
        {
            case 1:
                catatpemasukan();
                break;           
            case 2:
                ajukanProposal();
                break;
            case 3:
                lihatDaftarTransaksi();
                break;
            case 4:
                tampilkanRingkasanKeuangan();
                break;
            case 5:
                cout<<"balik ke menu utama"<<endl;
                break;
            default:
                cout<<"pilihan tidak valid"<<endl;
                break;
        }
    }while (pilihan !=5);
    
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
    proposal.id = totaltransaksi + 1;

    cout << "Deskripsi: ";
    cin.ignore();
    getline(cin, proposal.deskripsi);

    cout << "Jumlah: ";
    cin >> proposal.jumlah;

    if (simpanProposal(proposal)) {
        cout << "Proposal berhasil diajukan.\n";
    } else {
        cout << "Gagal menyimpan proposal.\n";
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

void menuManajer() {
    clearscreen();
    int pilihan;  
    do{
        cout << endl <<"----Menu Manager----" << endl;
        cout <<"1. Tampilkan Laporan Keuangan"<< endl;
        cout <<"2. Lihat Proposal"<< endl;
        cout << "3. Kembali Ke Menu Utama"<< endl;
        cout<<"Masukkan Pilihan: ";
        cin>>pilihan;
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