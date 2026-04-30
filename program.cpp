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
        cout << "\nPress Enter to continue...";
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
                    printError("Input harus antara " + to_string(minVal) + "-" + to_string(maxVal));
                }
            } else {
                printError("Input harus berupa angka!");
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
        ss << "[ASSERT FAILED] " << pesan << " | File: " << __FILE__ \
           << " | Line: " << __LINE__ << " | Function: " << __func__; \
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
        ASSERT(bulan >= 0, "Bulan tidak boleh negatif");
        if (bulan <= 0) return 0;
        ASSERT(bulan <= 12, "Maksimal durasi 12 bulan");
        return PERTEMUAN_PER_BULAN + hitungpertemuan(bulan - 1);
    } catch (const PadelException& e) {
        throw;
    }
}

string infohari(string namapaket) {
    if (namapaket == "SUN") return "Senin-Jumat";
    else if (namapaket == "MOON") return "Senin-Jumat";
    else if (namapaket == "STAR") return "Sabtu-Minggu";
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
            
            ASSERT(!hari.empty(), "Hari tidak boleh kosong");
            
            if (validasihari(hari, namapaket)) {
                if (hari.length() > 0) {
                    hari[0] = toupper(hari[0]);
                    for (int i = 1; i < hari.length(); i++) hari[i] = tolower(hari[i]);
                }
                return hari;
            } else {
                throw PadelException("Hari tidak valid untuk paket ini!");
            }
        } catch (const PadelException& e) {
            cout << "[!] " << e.what() << " ";
            if (namapaket == "SUN" || namapaket == "MOON")
                cout << "Paket weekday hanya bisa: Senin-Jumat" << endl;
            else
                cout << "Paket weekend hanya bisa: Sabtu&Minggu" << endl;
        }
    }
}

int hargapaket(string namapaket, int bulan) {
    try {
        ASSERT(bulan >= 0, "Durasi tidak boleh negatif");
        int hargaPerSesi;
        if (namapaket == "SUN") hargaPerSesi = 450;
        else if (namapaket == "MOON") hargaPerSesi = 500;
        else if (namapaket == "STAR") hargaPerSesi = 650;
        else throw PadelException("Paket tidak valid!");
        
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
            throw PadelException("Pilihan jam tidak valid!");
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

void updateHargaDenganReference(int &harga, int bonus) {
    harga += bonus;
}

void updateNamaDenganPointer(string *namaPtr, string namaBaru) {
    if (namaPtr != nullptr) {
        *namaPtr = namaBaru;
    }
}

void tampilkanDetailMemberDenganPointer(member *mPtr) {
    if (mPtr != nullptr) {
        cout << "\nDetail via Pointer:" << endl;
        cout << "  ID: " << mPtr->id << " | Nama: " << mPtr->nama << endl;
        cout << "  Paket: " << mPtr->namapaket << " | Harga: Rp " << mPtr->detail.harga << ".000" << endl;
    }
}

void demoPointerInternal(member *m, int idx) {
    cout << "\n[Pointer Info] Member #" << idx << ":" << endl;
    cout << "  Alamat memori: " << m << endl;
    cout << "  Alamat nama: " << &(m->nama) << endl;
    cout << "  Alamat harga: " << &(m->detail.harga) << endl;
    
    int hargaLama = m->detail.harga;
    updateHargaDenganReference(m->detail.harga, 100);
    cout << "  Harga setelah update (+100 via &): Rp " << m->detail.harga << ".000" << endl;
    m->detail.harga = hargaLama;
    
    string namaLama = m->nama;
    updateNamaDenganPointer(&(m->nama), namaLama + " [VIP]");
    cout << "  Nama setelah update (via *): " << m->nama << endl;
    m->nama = namaLama;
    
    tampilkanDetailMemberDenganPointer(m);
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
        cout << "\n[INFO] Belum ada aktivitas tercatat." << endl;
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
                throw PadelException("MEMBER SUDAH ADA! Gunakan menu 'lihat data saya' untuk melihat data sendiri.");
            }
        }
        
        ASSERT(jumlahmember < MAX_MEMBER, "Kapasitas member penuh");
        
        cout << "\n--- BUAT AKUN MEMBER ---" << endl;
        member baru;
        baru.id = jumlahmember + 1;
        baru.username = username;
        
        cout << "Nama: ";
        getline(cin, baru.nama);
        
        ASSERT(!baru.nama.empty(), "Nama tidak boleh kosong");
        
        tampilkaninfopaket();
        cout << "Paket (SUN/MOON/STAR): ";
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
        
        ASSERT(baru.detail.bulanMember > 0, "Durasi harus lebih dari 0 bulan");
        
        baru.detail.harga = hargapaket(baru.namapaket);
        
        updateHargaDenganReference(baru.detail.harga, 0);
        
        members[jumlahmember] = baru;
        tampilkanDetailMemberDenganPointer(&members[jumlahmember]);
        
        jumlahmember++;
        
        int totalPertemuan = hitungpertemuan(baru.detail.bulanMember);
        int total = hargapaket(baru.namapaket, baru.detail.bulanMember);
        
        MiloUtils::printSuccess("YEY AKUN MEMBER UDAH ADA!");
        cout << "      Paket           : " << baru.namapaket << endl;
        cout << "      Hari            : " << baru.detail.hari << endl;
        cout << "      Jam sesi        : " << baru.detail.jamsesi << endl;
        cout << "      Durasi          : " << baru.detail.bulanMember << " bulan" << endl;
        cout << "      Pertemuan/Bulan : " << PERTEMUAN_PER_BULAN << "x" << endl;
        cout << "      Total Pertemuan : " << totalPertemuan << "x" << endl;
        cout << "      Harga /Sesi     : Rp " << baru.detail.harga << ".000" << endl;
        cout << "      TOTAL BAYAR   : Rp " << total << ".000 " << endl;
        
        catatLog("Member baru: " + baru.nama + " (ID: " + to_string(baru.id) + ")");
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
    }
}

void bacasemuadata(member members[], int jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "Data member masih kosong");
        
        cout << "===================" << endl;
        cout << "      SORTING       " << endl;
        cout << "===================" << endl;
        cout << "  1. Nama (Z -> A) [std::sort]      " << endl;
        cout << "  2. ID (1 -> 100) [std::sort]      " << endl;
        cout << "  3. Harga (Termurah) [std::sort]   " << endl;
        cout << "  4. Tanpa Sorting                  " << endl;
        cout << "===================" << endl;
        cout << "Pilihan: ";
        
        int opsi;
        cin >> opsi;
        cin.ignore();
        
        ASSERT(opsi >= 1 && opsi <= 4, "Pilihan sorting tidak valid");
        
        member temp[MAX_MEMBER];
        for (int i = 0; i < jumlahmember; i++) temp[i] = members[i];
        
        if (opsi == 1) {
            sortNamaDescending(temp, jumlahmember);
            cout << "\nData diurutkan: Nama (Z -> A) - std::sort" << endl;
        } else if (opsi == 2) {
            sortIdAscending(temp, jumlahmember);
            cout << "\nData diurutkan: ID (1 -> 100) - std::sort" << endl;
        } else if (opsi == 3) {
            sortHargaAscending(temp, jumlahmember);
            cout << "\nData diurutkan: Harga (Termurah) - std::sort" << endl;
        } else {
            cout << "\nMenampilkan data tanpa sorting" << endl;
        }
        
        cout << "\n====================================================================================" << endl;
        cout << "                                  DAFTAR MEMBER PADEL                                 " << endl;
        cout << "======================================================================================" << endl;
        cout << left << setw(4) << "ID" 
             << setw(15) << "Nama member" 
             << setw(15) << "username"
             << setw(8)  << "Paket"
             << setw(10) << "Hari"
             << setw(12) << "Jam sesi"
             << setw(10) << "Bulan"
             << setw(12) << "Total" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        
        for (int i = 0; i < jumlahmember; i++) {
            int totalPertemuan = hitungpertemuan(temp[i].detail.bulanMember);
            int total = hargapaket(temp[i].namapaket, temp[i].detail.bulanMember);
            
            cout << left << setw(4) << temp[i].id
                 << setw(18) << temp[i].nama
                 << setw(15) << temp[i].username
                 << setw(8) << temp[i].namapaket
                 << setw(12) << temp[i].detail.hari
                 << setw(12) << temp[i].detail.jamsesi
                 << setw(10) << (to_string(temp[i].detail.bulanMember) + " bln")
                 << setw(12) << ("Rp " + to_string(total) + "K") << endl;
        }
        cout << "================================================================================" << endl;
        
        if (jumlahmember > 0) {
            cout << "\nMenampilkan info member pertama via pointer:" << endl;
            demoPointerInternal(&temp[0], 1);
        }
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
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
                cout << "Nama            : " << members[i].nama << endl;
                cout << "Paket           : " << members[i].namapaket << endl;
                cout << "Hari            : " << members[i].detail.hari << endl;
                cout << "Jam sesi        : " << members[i].detail.jamsesi << endl;
                cout << "Durasi          : " << members[i].detail.bulanMember << " bulan" << endl;
                cout << "Pertemuan/Bulan : " << PERTEMUAN_PER_BULAN << "x" << endl;
                cout << "Total Pertemuan : " << totalPertemuan << "x" << endl;
                cout << "Harga /Sesi     : Rp " << members[i].detail.harga << ".000" << endl;
                cout << " TOTAL BAYAR   : Rp " << total << ".000 " << endl;
                cout << "========================================" << endl;
                
                cout << "\n[Pointer Demo] Akses data via pointer:" << endl;
                tampilkanDetailMemberDenganPointer(&members[i]);
                
                ditemukan = true;
                break;
            }
        }
        
        ASSERT(ditemukan, "BELUM JOIN MEMBER! YUK buat akun dulu");
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
    }
}

void updatedatamember(member members[], int jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "Data member masih kosong");
        
        cout << "\nDaftar Member:" << endl;
        for (int i = 0; i < jumlahmember; i++) {
            cout << "  [" << members[i].id << "] " << members[i].nama 
                 << " | Paket: " << members[i].namapaket << endl;
        }
        
        int index;
        cout << "\nID yang mau diupdate: ";
        cin >> index;
        cin.ignore();
        
        ASSERT(index > 0 && index <= jumlahmember, "ID MEMBER TIDAK VALID");
        
        int idx = index - 1;
        cout << "\n--- UPDATE DATA MEMBER ---" << endl;
        cout << "Nama: " << members[idx].nama << endl;
        cout << "Paket sekarang: " << members[idx].namapaket << endl;
        
        tampilkaninfopaket();
        cout << "Paket Baru (SUN/MOON/STAR): ";
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
        
        ASSERT(members[idx].detail.bulanMember > 0, "Durasi harus lebih dari 0 bulan");
        
        members[idx].detail.harga = hargapaket(members[idx].namapaket);
        updateHargaDenganReference(members[idx].detail.harga, 0);
        
        int totalPertemuan = hitungpertemuan(members[idx].detail.bulanMember);
        int total = hargapaket(members[idx].namapaket, members[idx].detail.bulanMember);
        
        MiloUtils::printSuccess("Paket member \"" + members[idx].nama + "\" UPDATE BERHASIL!");
        cout << "    Total Pertemuan : " << totalPertemuan << "x" << endl;
        cout << "     TOTAL BAYAR   : Rp " << total << ".000 " << endl;
        
        cout << "\n[Pointer Demo] Member setelah update:" << endl;
        demoPointerInternal(&members[idx], members[idx].id);
        
        catatLog("Update member ID " + to_string(members[idx].id));
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
    }
}

void deletedatamember(member members[], int &jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "Data member masih kosong");
        
        cout << "\nDaftar Member:" << endl;
        for (int i = 0; i < jumlahmember; i++) {
            cout << "  [" << members[i].id << "] " << members[i].nama << endl;
        }
        
        int index;
        cout << "\nID yang mau dihapus: ";
        cin >> index;
        cin.ignore();
        
        ASSERT(index > 0 && index <= jumlahmember, "ID MEMBER TIDAK VALID");
        
        cout << "\n[Pointer Demo] Member yang akan dihapus:" << endl;
        demoPointerInternal(&members[index - 1], index);
        
        for (int i = index - 1; i < jumlahmember - 1; i++) {
            members[i] = members[i + 1];
        }
        jumlahmember--;
        
        MiloUtils::printSuccess("MEMBER ID " + to_string(index) + " BERHASIL DIHAPUS!");
        catatLog("Hapus member ID " + to_string(index));
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
    }
}

void searchingMember(member members[], int jumlahmember) {
    try {
        ASSERT(jumlahmember > 0, "Data member masih kosong");
        
        cout << "\n=========================" << endl;
        cout << "        CARI MEMBER      " << endl;
        cout << "=========================" << endl;
        cout << "  1. Cari berdasarkan ID" << endl;
        cout << "  2. Cari berdasarkan Nama" << endl;
        cout << "=========================" << endl;
        cout << "Pilihan: ";
        
        int opsi;
        cin >> opsi;
        cin.ignore();
        
        ASSERT(opsi >= 1 && opsi <= 2, "Pilihan tidak valid");
        
        member temp[MAX_MEMBER];
        for (int i = 0; i < jumlahmember; i++) temp[i] = members[i];
        
        if (opsi == 1) {
            sortIdAscending(temp, jumlahmember);
            
            int cariID;
            cout << "Masukkan ID yang dicari: ";
            cin >> cariID;
            
            ASSERT(cariID > 0, "ID harus lebih dari 0");
            
            int hasil = binarySearchID(temp, jumlahmember, cariID);
            
            if (hasil != -1) {
                MiloUtils::printSuccess("Data ditemukan!");
                cout << "  ID       : " << temp[hasil].id << endl;
                cout << "  Nama     : " << temp[hasil].nama << endl;
                cout << "  Paket    : " << temp[hasil].namapaket << endl;
                cout << "  Indeks   : " << hasil << endl;
            } else {
                MiloUtils::printWarning("ID " + to_string(cariID) + " tidak ditemukan.");
            }
        } else if (opsi == 2) {
            string cariNama;
            cout << "Masukkan Nama yang dicari: ";
            getline(cin, cariNama);
            
            ASSERT(!cariNama.empty(), "Nama tidak boleh kosong");
            
            int hasil = linearSearchNama(temp, jumlahmember, cariNama);
            
            if (hasil != -1) {
                MiloUtils::printSuccess("Data ditemukan!");
                cout << "  ID       : " << temp[hasil].id << endl;
                cout << "  Nama     : " << temp[hasil].nama << endl;
                cout << "  Paket    : " << temp[hasil].namapaket << endl;
                cout << "  Indeks   : " << hasil << endl;
            } else {
                MiloUtils::printWarning("Nama \"" + cariNama + "\" tidak ditemukan.");
            }
        }
        
    } catch (const PadelException& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
    }
}

void menuAdmin(member members[], int &jumlahmember, string userLogin) {
    int pilihan; 
    bool menuberjalan = true;
    
    do {
        try {
            cout << endl;
            MiloUtils::printHeader("ADMIN MENU - " + userLogin);
            
            cout << "1. Buat Akun Member Baru      " << endl;
            cout << "2. Tampilkan Semua Data       " << endl;
            cout << "3. Update Paket Member        " << endl;
            cout << "4. Hapus Member               " << endl;
            cout << "5. Cari Member                " << endl;
            cout << "6. Lihat Log Aktivitas        " << endl;
            cout << "7. Logout                     " << endl;
            MiloUtils::printSeparator();
            cout << "Pilih menu (1-7): ";
            cin >> pilihan;
            cin.ignore();
            
            ASSERT(pilihan >= 1 && pilihan <= 7, "PILIHAN TIDAK VALID! hanya 1-7");
            
            switch (pilihan) {
                case 1: buatmember(members, jumlahmember, "admin_managed"); break;
                case 2: bacasemuadata(members, jumlahmember); break;
                case 3: updatedatamember(members, jumlahmember); break;
                case 4: deletedatamember(members, jumlahmember); break;
                case 5: searchingMember(members, jumlahmember); break;
                case 6: tampilkanLog(); MiloUtils::pause(); break;
                case 7:
                    MiloUtils::printSuccess("BERHASIL LOGOUT! kembali ke menu utama.");
                    menuberjalan = false;
                    break;
            }
            
        } catch (const PadelException& e) {
            MiloUtils::printError(e.what());
        } catch (const exception& e) {
            MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
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
            
            cout << "1. Lihat Jadwal & Harga Paket   " << endl;
            cout << "2. Buat Akun Member             " << endl;
            cout << "3. Lihat Data Saya              " << endl;
            cout << "4. Logout                       " << endl;
            MiloUtils::printSeparator();
            cout << "Pilih menu (1-4): ";
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
            MiloUtils::printError("Terjadi kesalahan: " + string(e.what()));
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
                cout << "1. Registrasi Akun Baru" << endl;
                cout << "2. Login" << endl;
                cout << "3. Keluar" << endl;
                cout << "Pilihan: ";
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
                    
                    cout << "Password: ";
                    getline(cin, inputpw);
                    
                    ASSERT(inputpw.length() >= 3, "Password minimal 3 karakter");
                    
                    users[jumlahuser].username = inputnama;
                    users[jumlahuser].password = inputpw;
                    users[jumlahuser].role = "user";
                    jumlahuser++;
                    
                    MiloUtils::printSuccess("REGIS BERHASIL! YUK LOGIN");
                    
                } else if (pilihanawal == 2) {
                    cout << "\n--- LOGIN user ---" << endl;
                    
                    bool loginSukses = false;
                    
                    for (int percobaan = 0; percobaan < 3; percobaan++) {
                        cout << "username: ";
                        getline(cin, inputnama);
                        cout << "Password: ";
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
                        
                        cout << "    -> Sisa percobaan: " << (2 - percobaan) << endl << endl;
                    }
                    
                    ASSERT(loginSukses, "Login gagal");
                    
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
                MiloUtils::printError("Terjadi kesalahan sistem: " + string(e.what()));
                MiloUtils::pause();
            }
        }
        
    } catch (const exception& e) {
        MiloUtils::printError("Fatal Error: " + string(e.what()));
        return 1;
    }
    
    return 0;
}