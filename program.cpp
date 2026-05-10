#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

#ifndef MILOUTILS_H
#define MILOUTILS_H

namespace MiloUtils {
    void clearScreen() {
        cout << string(50, '\n');
    }
    
    void printHeader(const string& title) {
        cout << "\n+=======================================+" << endl;
        cout << "| " << left << setw(37) << title << " |" << endl;
        cout << "+=======================================+" << endl;
    }
    
    void printSeparator() {
        cout << "=======================================" << endl;
    }
    
    void printSuccess(const string& message) {
        cout << "\n[OK] " << message << endl;
    }
    
    void printError(const string& message) {
        cout << "\n[ERROR] " << message << endl;
    }
    
    void printWarning(const string& message) {
        cout << "\n[WARNING] " << message << endl;
    }
    
    void pause() {
        cout << "\npress enter to continue...";
        cin.ignore();
    }
    
    int getValidatedInput(int minVal, int maxVal, const string& prompt) {
        int input;
        while (true) {
            cout << prompt;
            if (cin >> input) {
                if (input >= minVal && input <= maxVal) {
                    return input;
                } else {
                    printError("input harus antara " + to_string(minVal) + "-" + to_string(maxVal));
                }
            } else {
                printError("input harus berupa angka!");
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
    }
}

#endif


#define ASSERT(kondisi, pesan) \
    if (!(kondisi)) { \
        stringstream ss; \
        ss << "[ASSERT FAILED] " << pesan << " | file: " << __FILE__ \
           << " | line: " << __LINE__ << " | function: " << __func__; \
        throw runtime_error(ss.str()); \
    }


struct user {
    string username;
    string password;
    string role;
};

struct infopaket {
    string hari;    
    string jamsesi;
    int bulanMember;
    int harga;
};

struct member {
    int id;
    string nama;
    string username;
    string namapaket;
    infopaket detail;
};

#define MAX_MEMBER 100
#define MAX_user 20
#define PERTEMUAN_PER_BULAN 4

class PadelException : public exception {
private:
    string message;
public:
    PadelException(const string& msg) : message(msg) {}
    const char* what() const throw() {
        return message.c_str();
    }
};

int hitungpertemuan(int bulan) {
    try {
        ASSERT(bulan >= 0, "bulan tidak boleh negatif");
        if (bulan <= 0) return 0;
        ASSERT(bulan <= 12, "maksimal durasi 12 bulan");
        return PERTEMUAN_PER_BULAN + hitungpertemuan(bulan - 1);
    } catch (const PadelException& e) {
        throw;
    }
}

string infohari(string namapaket) {
    if (namapaket == "SUN") return "SENIN-JUMAT";
    else if (namapaket == "MOON") return "SENIN-JUMAT";
    else if (namapaket == "STAR") return "SABTU-MINGGU";
    else return "-";
}

bool validasihari(string hari, string namapaket) {
    if (namapaket == "SUN" || namapaket == "MOON") {
        return (hari == "Senin" || hari == "senin" ||
                hari == "Selasa" || hari == "selasa" ||
                hari == "Rabu" || hari == "rabu" ||
                hari == "Kamis" || hari == "kamis" ||
                hari == "Jumat" || hari == "jumat");
    } else if (namapaket == "STAR") {
        return (hari == "Sabtu" || hari == "sabtu" ||
                hari == "Minggu" || hari == "minggu");
    }
    return false;
}

string inputharivalid(string namapaket) {
    string hari;
    while (true) {
        try {
            cout << "MAU PADEL DI HARI APA (" << infohari(namapaket) << "): ";
            getline(cin, hari);
            
            ASSERT(!hari.empty(), "hari tidak boleh kosong");
            
            if (validasihari(hari, namapaket)) {
                if (hari.length() > 0) {
                    hari[0] = toupper(hari[0]);
                    for (int i = 1; i < hari.length(); i++) hari[i] = tolower(hari[i]);
                }
                return hari;
            } else {
                throw PadelException("hari tidak valid untuk paket ini!");
            }
        } catch (const PadelException& e) {
            cout << "[!] " << e.what() << " ";
            if (namapaket == "SUN" || namapaket == "MOON")
                cout << "paket weekday hanya bisa: SENIN-JUMAT" << endl;
            else
                cout << "paket weekend hanya bisa: SABTU&MINGGU" << endl;
        }
    }
}

int hargapaket(string namapaket, int bulan) {
    try {
        ASSERT(bulan >= 0, "durasi tidak boleh negatif");
        int hargaPerSesi;
        if (namapaket == "SUN") hargaPerSesi = 450;
        else if (namapaket == "MOON") hargaPerSesi = 500;
        else if (namapaket == "STAR") hargaPerSesi = 650;
        else throw PadelException("paket tidak valid!");
        
        int totalPertemuan = hitungpertemuan(bulan);
        return hargaPerSesi * totalPertemuan;
    } catch (const PadelException& e) {
        throw;
    }
}

int hargapaket(string namapaket) {
    if (namapaket == "SUN") return 450;
    else if (namapaket == "MOON") return 500;
    else if (namapaket == "STAR") return 650;
    else return 0;
}

void infopilihanjam(string namapaket) {
    cout << "\n--- PILIH JAM SESI (max 1 pilihan) ---" << endl;
    if (namapaket == "SUN") {
        for (int i = 1; i <= 10; i++)
            cout << i << ". " << (5+i) << ".00-" << (6+i) << ".00" << endl;
    } else if (namapaket == "MOON" || namapaket == "STAR") {
        for (int i = 1; i <= 7; i++)
            cout << i << ". " << (15+i) << ".00-" << (16+i) << ".00" << endl;
    }
}

string jamsesi(string namapaket, int pilihan) {
    try {
        if (namapaket == "SUN" && pilihan >= 1 && pilihan <= 10) {
            int start = 5 + pilihan;
            return to_string(start) + ".00-" + to_string(start + 1) + ".00";
        } else if ((namapaket == "MOON" || namapaket == "STAR") && pilihan >= 1 && pilihan <= 7) {
            int start = 15 + pilihan;
            return to_string(start) + ".00-" + to_string(start + 1) + ".00";
        } else {
            throw PadelException("pilihan jam tidak valid!");
        }
    } catch (const PadelException& e) {
        cout << "[!] " << e.what() << endl;
        return "-";
    }
    return "-";
}

void tampilkaninfopaket() {
    MiloUtils::printHeader("PAKET SESI DI MILO PADEL");
    cout << "1. SUN   : weekday, jam 06.00-16.00 | Rp 450.000/sesi" << endl;
    cout << "2. MOON  : weekday, jam 16.00-23.00 | Rp 500.000/sesi" << endl;
    cout << "3. STAR  : weekend, jam 16.00-23.00 | Rp 650.000/sesi" << endl;
    MiloUtils::printSeparator();
    cout << "[FYI] 1 bulan = 4 pertemuan (1x seminggu)" << endl;
    MiloUtils::printSeparator();
}


void sortNamaDescending(member arr[], int n) {
    sort(arr, arr + n, [](const member& a, const member& b) {
        return a.nama > b.nama;
    });
}

void sortIdAscending(member arr[], int n) {
    sort(arr, arr + n, [](const member& a, const member& b) {
        return a.id < b.id;
    });
}

void sortHargaAscending(member arr[], int n) {
    sort(arr, arr + n, [](const member& a, const member& b) {
        return a.detail.harga < b.detail.harga;
    });
}


int binarySearchID(member *arr, int n, int target) {
    int low = 0;
    int high = n - 1;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (arr[mid].id == target) {
            return mid;
        } else if (arr[mid].id < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int linearSearchNama(member *arr, int n, string target) {
    for (int i = 0; i < n; i++) {
        if (arr[i].nama == target) {
            return i;
        }
    }
    return -1;
}


vector<string> logAktivitas;

void catatLog(const string& aktivitas) {
    logAktivitas.push_back(aktivitas);
}

void tampilkanLog() {
    if (logAktivitas.empty()) {
        cout << "\n[INFO] belum ada aktivitas tercatat." << endl;
        return;
    }
    cout << "\n=== LOG AKTIVITAS ===" << endl;
    for (size_t i = 0; i < logAktivitas.size(); i++) {
        cout << "[" << (i+1) << "] " << logAktivitas[i] << endl;
    }
    cout << "=====================" << endl;
}


void buatmember(member members[], int &jumlahmember, string username) {
    try {
        for (int i = 0; i < jumlahmember; i++) {
            if (members[i].username == username) {
                throw PadelException("MEMBER SUDAH ADA! pilih menu 'lihat data saya' untuk melihat data sendiri.");
            }
        }
        
        ASSERT(jumlahmember < MAX_MEMBER, "kapasitas member penuh");
        
        cout << "\n--- BUAT AKUN MEMBER ---" << endl;
        member baru;
        baru.id = jumlahmember + 1;
        baru.username = username;
        
        cout << "NAMA: ";
        getline(cin, baru.nama);
        
        ASSERT(!baru.nama.empty(), "nama tidak boleh kosong");
        
        tampilkaninfopaket();
        cout << "PAKET(SUN/MOON/STAR): ";
        getline(cin, baru.namapaket);
        
        if (baru.namapaket != "SUN" && baru.namapaket != "MOON" && baru.namapaket != "STAR") {
            throw PadelException("PAKET TIDAK VALID! Pilih SUN, MOON, atau STAR.");
        }
        
        baru.detail.hari = inputharivalid(baru.namapaket);
        infopilihanjam(baru.namapaket);
        cout << "JAM BERAPA (1-" << (baru.namapaket == "SUN" ? "10" : "7") << "): ";
        int pilihanJam;
        cin >> pilihanJam;
        cin.ignore();
        baru.detail.jamsesi = jamsesi(baru.namapaket, pilihanJam);
        
        cout << "\nMAU JOIN BERAPA BULAN: ";
        cin >> baru.detail.bulanMember;
        cin.ignore();
        
        ASSERT(baru.detail.bulanMember > 0, "durasi harus lebih dari 0 bulan");
        
        baru.detail.harga = hargapaket(baru.namapaket);
        
        members[jumlahmember] = baru;
        jumlahmember++;
        
        int totalPertemuan = hitungpertemuan(baru.detail.bulanMember);
        int total = hargapaket(baru.namapaket, baru.detail.bulanMember);
        
        MiloUtils::printSuccess("YEY AKUN MEMBER UDAH ADA!");
        cout << "      PAKET           : " << baru.namapaket << endl;
        cout << "      HARI            : " << baru.detail.hari << endl;
        cout << "      JAM SESI        : " << baru.detail.jamsesi << endl;
        cout << "      DURASI          : " << baru.detail.bulanMember << " bulan" << endl;
        cout << "      PERTEMUAN /BULAN: " << PERTEMUAN_PER_BULAN << "x" << endl;
        cout << "      TOTAL PERTEMUAN : " << totalPertemuan << "x" << endl;
        cout << "      HARGA /SESI     : Rp " << baru.detail.harga << ".000" << endl;
        cout << "      TOTAL BAYAR     : Rp " << total << ".000 " << endl;
        
        catatLog("member baru: " + baru.nama + " (ID: " + to_string(baru.id) + ")");
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void bacasemuadata(member members[], int jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "data member masih kosong");
        
        cout << "\n====================================================================================" << endl;
        cout << "                                  DAFTAR MEMBER PADEL                                 " << endl;
        cout << "======================================================================================" << endl;
        cout << left << setw(4) << "ID" 
             << setw(15) << "NAMA MEMBER" 
             << setw(15) << "USERNAME"
             << setw(8)  << "PAKET"
             << setw(10) << "HARI"
             << setw(12) << "JAM SESI"
             << setw(10) << "BULAN"
             << setw(12) << "TOTAL" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        
        for (int i = 0; i < jumlahmember; i++) {
            int total = hargapaket(members[i].namapaket, members[i].detail.bulanMember);
            
            cout << left << setw(4) << members[i].id
                 << setw(18) << members[i].nama
                 << setw(15) << members[i].username
                 << setw(8) << members[i].namapaket
                 << setw(12) << members[i].detail.hari
                 << setw(12) << members[i].detail.jamsesi
                 << setw(10) << (to_string(members[i].detail.bulanMember) + " bln")
                 << setw(12) << ("Rp " + to_string(total) + "K") << endl;
        }
        cout << "================================================================================" << endl;
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void bacadatasaya(member members[], int jumlahmember, string username) {
    try {
        bool ditemukan = false;
        
        for (int i = 0; i < jumlahmember; i++) {
            if (members[i].username == username) {
                int totalPertemuan = hitungpertemuan(members[i].detail.bulanMember);
                int total = hargapaket(members[i].namapaket, members[i].detail.bulanMember);
                
                cout << "\n=====================================" << endl;
                cout << "             DATA MEMBER ANDA          " << endl;
                cout << "=======================================" << endl;
                cout << "ID              : " << members[i].id << endl;
                cout << "NAMA            : " << members[i].nama << endl;
                cout << "PAKET           : " << members[i].namapaket << endl;
                cout << "HARI            : " << members[i].detail.hari << endl;
                cout << "JAM SESI        : " << members[i].detail.jamsesi << endl;
                cout << "DURASI          : " << members[i].detail.bulanMember << " bulan" << endl;
                cout << "PERTEMUAN /BULAN: " << PERTEMUAN_PER_BULAN << "x" << endl;
                cout << "TOTAL PERTEMUAN : " << totalPertemuan << "x" << endl;
                cout << "HARGA /SESI     : Rp " << members[i].detail.harga << ".000" << endl;
                cout << "TOTAL BAYAR   : Rp " << total << ".000 " << endl;
                cout << "========================================" << endl;
                
                ditemukan = true;
                break;
            }
        }
        
        ASSERT(ditemukan, "BELUM JOIN MEMBER! YUK buat akun dulu");
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TEJADI KESALAHAN: " + string(e.what()));
    }
}

void updatedatamember(member members[], int jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "data member masih kosong");
        
        cout << "\nDAFTAR MEMBER:" << endl;
        for (int i = 0; i < jumlahmember; i++) {
            cout << "  [" << members[i].id << "] " << members[i].nama 
                 << " | PAKET: " << members[i].namapaket << endl;
        }
        
        int index;
        cout << "\nID yang mau diupdate: ";
        cin >> index;
        cin.ignore();
        
        ASSERT(index > 0 && index <= jumlahmember, "ID MEMBER TIDAK VALID");
        
        int idx = index - 1;
        cout << "\n--- UPDATE DATA MEMBER ---" << endl;
        cout << "NAMA: " << members[idx].nama << endl;
        cout << "PAKET SEKARANG: " << members[idx].namapaket << endl;
        
        tampilkaninfopaket();
        cout << "PAKET BARU (SUN/MOON/STAR): ";
        getline(cin, members[idx].namapaket);
        
        if (members[idx].namapaket != "SUN" && members[idx].namapaket != "MOON" && members[idx].namapaket != "STAR") {
            throw PadelException("PAKET TIDAK VALID!");
        }
        
        members[idx].detail.hari = inputharivalid(members[idx].namapaket);
        infopilihanjam(members[idx].namapaket);
        cout << "Pilih jam sesi (1-" << (members[idx].namapaket == "SUN" ? "10" : "7") << "): ";
        int pilihanJam;
        cin >> pilihanJam;
        cin.ignore();
        members[idx].detail.jamsesi = jamsesi(members[idx].namapaket, pilihanJam);
        
        cout << "MAU JOIN BERAPA BULAN: ";
        cin >> members[idx].detail.bulanMember;
        cin.ignore();
        
        ASSERT(members[idx].detail.bulanMember > 0, "durasi harus lebih dari 0 bulan");
        
        members[idx].detail.harga = hargapaket(members[idx].namapaket);
        
        int totalPertemuan = hitungpertemuan(members[idx].detail.bulanMember);
        int total = hargapaket(members[idx].namapaket, members[idx].detail.bulanMember);
        
        MiloUtils::printSuccess("PAKET MEMBER \"" + members[idx].nama + "\" UPDATE BERHASIL!");
        cout << "   TOTAL PERTEMUAN : " << totalPertemuan << "x" << endl;
        cout << "   TOTAL BAYAR     : Rp " << total << ".000 " << endl;
        
        catatLog("update member ID " + to_string(members[idx].id));
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void deletedatamember(member members[], int &jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "data member masih kosong");
        
        cout << "\nDAFTAR MEMBER" << endl;
        for (int i = 0; i < jumlahmember; i++) {
            cout << "  [" << members[i].id << "] " << members[i].nama << endl;
        }
        
        int index;
        cout << "\nID yang mau dihapus: ";
        cin >> index;
        cin.ignore();
        
        ASSERT(index > 0 && index <= jumlahmember, "ID MEMBER TIDAK VALID");
        
        for (int i = index - 1; i < jumlahmember - 1; i++) {
            members[i] = members[i + 1];
        }
        jumlahmember--;
        
        MiloUtils::printSuccess("MEMBER ID " + to_string(index) + " BERHASIL DIHAPUS!");
        catatLog("hapus member ID " + to_string(index));
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void menuSortingSearching(member members[], int jumlahmember) {
    if (jumlahmember == 0) {
        MiloUtils::printWarning("data member masih kosong!");
        return;
    }

    int pilihan;
    bool kembali = false;
    do {
        try {
            MiloUtils::printHeader("MENU SORT & SEARCH");
            cout << "1. SORTING\n";
            cout << "2. SEARCHING\n";
            cout << "3. MENU ADMIN\n";
            MiloUtils::printSeparator();
            cout << "pilih menu (1-3): ";
            cin >> pilihan;
            cin.ignore();
            ASSERT(pilihan >= 1 && pilihan <= 3, "pilihan tidak valid");

            if (pilihan == 1) { 
                member temp[MAX_MEMBER];
                for(int i=0; i<jumlahmember; i++) temp[i] = members[i];

                cout << "\n--- PILIH JENIS SORT ---\n";
                cout << "1. NAMA (Z -> A)\n2. ID (1 -> N)\n3. HARGA (termurah)\n4. TANPA SORTING\npilihan: ";
                int opsiSort; cin >> opsiSort; cin.ignore();
                ASSERT(opsiSort >= 1 && opsiSort <= 4, "pilihan tidak valid");

                if(opsiSort == 1) { sortNamaDescending(temp, jumlahmember); cout << "DIURUTKAN: NAMA\n"; }
                else if(opsiSort == 2) { sortIdAscending(temp, jumlahmember); cout << "DIURUTKAN: ID\n"; }
                else if(opsiSort == 3) { sortHargaAscending(temp, jumlahmember); cout << "DIURUTKAN: HARGA\n"; }
                else { cout << "tanpa Sorting\n"; }

                cout << "\n====================================================================================\n";
                cout << left << setw(4) << "ID" << setw(15) << "NAMA" << setw(12) << "USERNAME" << setw(8) << "[PAKET]" << setw(10) << "HARI" << setw(12) << "JAM" << setw(8) << "BULAN" << setw(12) << "TOTAL\n";
                cout << "--------------------------------------------------------------------------------\n";
                for (int i = 0; i < jumlahmember; i++) {
                    int total = hargapaket(temp[i].namapaket, temp[i].detail.bulanMember);
                    cout << left << setw(4) << temp[i].id << setw(15) << temp[i].nama << setw(12) << temp[i].username
                         << setw(8) << temp[i].namapaket << setw(10) << temp[i].detail.hari << setw(12) << temp[i].detail.jamsesi
                         << setw(8) << temp[i].detail.bulanMember << setw(12) << ("Rp " + to_string(total) + "K") << endl;
                }
                cout << "================================================================================\n";

            } else if (pilihan == 2) { 
                cout << "\n--- JENIS SEARCH ---\n";
                cout << "1. BERDASARKAN ID\n2. BERDASARKAN NAMA\npilihan: ";
                int opsiSearch; cin >> opsiSearch; cin.ignore();
                ASSERT(opsiSearch >= 1 && opsiSearch <= 2, "pilihan tidak valid");

                member temp[MAX_MEMBER];
                for(int i=0; i<jumlahmember; i++) temp[i] = members[i];

                if (opsiSearch == 1) {
                    sortIdAscending(temp, jumlahmember);
                    int cariID; cout << "masukkan ID yang dicari: "; cin >> cariID; cin.ignore();
                    ASSERT(cariID > 0, "ID harus > 0");
                    int hasil = binarySearchID(temp, jumlahmember, cariID);
                    if (hasil != -1) {
                        MiloUtils::printSuccess("DATA DITEMUKAN!");
                        cout << "  ID: " << temp[hasil].id << " | NAMA: " << temp[hasil].nama << " | PAKET: " << temp[hasil].namapaket << endl;
                    } else {
                        MiloUtils::printWarning("ID TIDAK DITEMUKAN.");
                    }
                } else {
                    string cariNama; cout << "masukkan Nama yang dicari: "; getline(cin, cariNama);
                    ASSERT(!cariNama.empty(), "nama tidak boleh kosong");
                    int hasil = linearSearchNama(temp, jumlahmember, cariNama);
                    if (hasil != -1) {
                        MiloUtils::printSuccess("DATA DITEMUKAN!");
                        cout << "  ID: " << temp[hasil].id << " | NAMA: " << temp[hasil].nama << " | PAKET: " << temp[hasil].namapaket << endl;
                    } else {
                        MiloUtils::printWarning("NAMA TIDAK DITEMUKAN.");
                    }
                }
            } else {
                kembali = true;
            }
            if (!kembali) MiloUtils::pause();
        } catch (const PadelException& e) { MiloUtils::printError(e.what()); MiloUtils::pause(); }
        catch (const exception& e) { MiloUtils::printError("ERROR " + string(e.what())); MiloUtils::pause(); }
    } while (!kembali);
}

void menuAdmin(member members[], int &jumlahmember, string userLogin) {
    int pilihan; 
    bool menuberjalan = true;
    
    do {
        try {
            cout << endl;
            MiloUtils::printHeader("ADMIN MENU - " + userLogin);
            
            cout << "1. TAMPILKAN SEMUA DATA       " << endl;
            cout << "2. UPDATE PAKET MEMBER        " << endl;
            cout << "3. HAPUS MEMBER               " << endl;
            cout << "4. SORT & SEARCH              " << endl;
            cout << "5. LOG AKTIVITAS        " << endl;
            cout << "6. LOGOUT                     " << endl;
            MiloUtils::printSeparator();
            cout << "pilih menu (1-6): ";
            cin >> pilihan;
            cin.ignore();
            
            ASSERT(pilihan >= 1 && pilihan <= 6, "PILIHAN TIDAK VALID! hanya 1-6");
            
            switch (pilihan) {
                case 1: bacasemuadata(members, jumlahmember); break;
                case 2: updatedatamember(members, jumlahmember); break;
                case 3: deletedatamember(members, jumlahmember); break;
                case 4: menuSortingSearching(members, jumlahmember); break;
                case 5: tampilkanLog(); MiloUtils::pause(); break;
                case 6:
                    MiloUtils::printSuccess("BERHASIL LOGOUT! kembali ke menu utama.");
                    menuberjalan = false;
                    break;
            }
            
        } catch (const PadelException& e) {
            MiloUtils::printError(e.what());
        } catch (const exception& e) {
            MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
        }
        
    } while (menuberjalan);
}

void menuMember(member members[], int &jumlahmember, string userLogin) {
    int pilihan; 
    bool menuberjalan = true;
    
    do {
        try {
            cout << endl;
            MiloUtils::printHeader("MEMBER MENU - " + userLogin);
            
            cout << "1. LIHAT JADWAL & HARGA PAKET   " << endl;
            cout << "2. BUAT AKUN MEMBERSHIP             " << endl;
            cout << "3. LIHAT DATA SAYA             " << endl;
            cout << "4. LOGOUT                       " << endl;
            MiloUtils::printSeparator();
            cout << "pilih menu (1-4): ";
            cin >> pilihan;
            cin.ignore();
            
            ASSERT(pilihan >= 1 && pilihan <= 4, "PILIHAN TIDAK VALID! hanya 1-4");
            
            switch (pilihan) {
                case 1: tampilkaninfopaket(); break;
                case 2: buatmember(members, jumlahmember, userLogin); break;
                case 3: bacadatasaya(members, jumlahmember, userLogin); break;
                case 4:
                    MiloUtils::printSuccess("BERHASIL LOGOUT! kembali ke menu utama.");
                    menuberjalan = false;
                    break;
            }
            
        } catch (const PadelException& e) {
            MiloUtils::printError(e.what());
        } catch (const exception& e) {
            MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
        }
        
    } while (menuberjalan);
}

int main() {
    try {
        user users[MAX_user] = {
            {"risma", "102", "member"},
            {"admin", "milopadel", "admin"}
        };
        int jumlahuser = 2;
        
        member members[MAX_MEMBER];
        int jumlahmember = 0;
        
        string inputnama, inputpw; 
        string userLogin, roleLogin;
        
        while (true) {
            try {
                MiloUtils::clearScreen();
                MiloUtils::printHeader("WELCOME TO MILO PADEL");
                
                cout << "\n--- MENU UTAMA---" << endl;
                cout << "1. REGISTRASI AKUN BARU" << endl;
                cout << "2. LOGIN" << endl;
                cout << "3. KELUAR PROGRAM" << endl;
                cout << "pilihan: ";
                int pilihanawal;
                cin >> pilihanawal;
                cin.ignore();
                
                ASSERT(pilihanawal >= 1 && pilihanawal <= 3, "PILIHAN TIDAK VALID");
                
                if (pilihanawal == 1) {
                    cout << "\n--- REGISTRASI AKUN ---" << endl;
                    cout << "username: ";
                    getline(cin, inputnama);
                    
                    bool usernameada = false;
                    for (int i = 0; i < jumlahuser; i++) {
                        if (inputnama == users[i].username) {
                            usernameada = true;
                            break;
                        }
                    }
                    
                    ASSERT(!usernameada, "username \"" + inputnama + "\" SUDAH DIGUNAKAN");
                    
                    cout << "password: ";
                    getline(cin, inputpw);
                    
                    ASSERT(inputpw.length() >= 3, "password minimal 3 karakter");
                    
                    users[jumlahuser].username = inputnama;
                    users[jumlahuser].password = inputpw;
                    users[jumlahuser].role = "user";
                    jumlahuser++;
                    
                    MiloUtils::printSuccess("REGIS BERHASIL! YUK LOGIN");
                    
                } else if (pilihanawal == 2) {
                    cout << "\n--- LOGIN USER ---" << endl;
                    
                    bool loginSukses = false;
                    
                    for (int percobaan = 0; percobaan < 3; percobaan++) {
                        cout << "username: ";
                        getline(cin, inputnama);
                        cout << "password: ";
                        getline(cin, inputpw);
                        
                        bool usnbenar = false;
                        bool pwbenar = false;
                        
                        for (int i = 0; i < jumlahuser; i++) {
                            if (inputnama == users[i].username) {
                                usnbenar = true;
                                if (inputpw == users[i].password) {
                                    pwbenar = true;
                                    userLogin = inputnama;
                                    roleLogin = users[i].role;
                                    break;
                                }
                            }
                        }
                        
                        if (usnbenar && pwbenar) {
                            loginSukses = true;
                            break;
                        }
                        
                        MiloUtils::printError("LOGIN GAGAL");
                        if (!usnbenar) {
                            cout << "    -> USERNAME GA ADA NIH!" << endl;
                        } else {
                            cout << "    -> PASSWORD SALAH!" << endl;
                        }
                        
                        ASSERT(percobaan < 2, "AKSES DITOLAK! GAGAL LOGIN 3x");
                        
                        cout << "    -> sisa percobaan: " << (2 - percobaan) << endl << endl;
                    }
                    
                    ASSERT(loginSukses, "LOGIN GAGAL");
                    
                    if (roleLogin == "admin") {
                        menuAdmin(members, jumlahmember, userLogin);
                    } else {
                        menuMember(members, jumlahmember, userLogin);
                    }
                    
                } else if (pilihanawal == 3) {
                    MiloUtils::printSuccess("THANK YOU! see you milovers.");
                    break;
                }
                
            } catch (const PadelException& e) {
                MiloUtils::printError(e.what());
                MiloUtils::pause();
            } catch (const exception& e) {
                MiloUtils::printError("TERJADI KESALAHAN SISTEM " + string(e.what()));
                MiloUtils::pause();
            }
        }
        
    } catch (const exception& e) {
        MiloUtils::printError("FATAL ERROR: " + string(e.what()));
        return 1;
    }
    
    return 0;
}