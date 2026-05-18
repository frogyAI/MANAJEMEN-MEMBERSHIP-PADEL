#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "table.h"
using namespace std;

#ifndef MILOUTILS_H
#define MILOUTILS_H

namespace MiloUtils {
    void clearScreen() {

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
    bool diskon_aktif = false;
    int nominal_diskon = 0;
};

#define MAX_MEMBER 100
#define MAX_user 20
#define PERTEMUAN_PER_BULAN 4

class PADELEXCEPTION : public exception {
private:
    string message;
public:
    PADELEXCEPTION(const string& msg) : message(msg) {}
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
    } catch (const PADELEXCEPTION& e) {
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
            ASSERT(!hari.empty(), "hari tidak boleh kosong");
            if (validasihari(hari, namapaket)) {
                if (hari.length() > 0) {
                    hari[0] = toupper(hari[0]);
                    for (int i = 1; i < hari.length(); i++) hari[i] = tolower(hari[i]);
                }
                return hari;
            } else {
                throw PADELEXCEPTION("hari tidak valid untuk paket ini!");
            }
        } catch (const PADELEXCEPTION& e) {
            cout << "[!] " << e.what() << " ";
            if (namapaket == "SUN" || namapaket == "MOON")
                cout << "paket weekday hanya bisa: Senin-Jumat" << endl;
            else
                cout << "paket weekend hanya bisa: Sabtu&Minggu" << endl;
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
        else throw PADELEXCEPTION("PAKET TIDAK VALID");
        int totalPertemuan = hitungpertemuan(bulan);
        return hargaPerSesi * totalPertemuan;
    } catch (const PADELEXCEPTION& e) {
        throw;
    }
}

int hargapaket(string namapaket) {
    if (namapaket == "SUN") return 450;
    else if (namapaket == "MOON") return 500;
    else if (namapaket == "STAR") return 650;
    else return 0;
}

int hitungdiskon(int bulan) {
    if (bulan >= 6 && bulan <= 8) return 15;
    else if (bulan >= 9 && bulan <= 12) return 30;
    return 0;
}

void infopilihanjam(string namapaket) {

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
            throw PADELEXCEPTION("pilihan jam tidak valid!");
        }
    } catch (const PADELEXCEPTION& e) {
        cout << "[!] " << e.what() << endl;
        return "-";
    }
    return "-";
}

void tampilkaninfopaket() {
 clitable::Table tabel;
    
    tabel.addColumn(clitable::Column("PAKET", clitable::Column::CENTER_ALIGN, 
                    clitable::Column::CENTER_ALIGN, 1, 8, clitable::Column::RESIZABLE));
    tabel.addColumn(clitable::Column("JADWAL", clitable::Column::CENTER_ALIGN, 
                    clitable::Column::LEFT_ALIGN, 1, 25, clitable::Column::RESIZABLE));
    tabel.addColumn(clitable::Column("HARGA/SESI", clitable::Column::CENTER_ALIGN, 
                    clitable::Column::RIGHT_ALIGN, 1, 15, clitable::Column::RESIZABLE));
    
    tabel.addTitle("PAKET SESI MILO PADEL");
    
    string row1[3] = {"SUN", "weekday, jam 06.00-16.00", "Rp 450.000"};
    string row2[3] = {"MOON", "weekday, jam 16.00-23.00", "Rp 500.000"};
    string row3[3] = {"STAR", "weekend, jam 16.00-23.00", "Rp 650.000"};
    
    tabel.addRow(row1);
    tabel.addRow(row2);
    tabel.addRow(row3);
    
    cout << tabel.draw();   
    cout << "\n[INFO] 1 bulan = 4 pertemuan (1x seminggu)" << endl;
}


void sortnamadescending(member arr[], int n) {
    sort(arr, arr + n, [](const member& a, const member& b) {
        return a.nama > b.nama;
    });
}

void sorthargadescending(member arr[], int n) {
    sort(arr, arr + n, [](const member& a, const member& b) {
        return a.id < b.id;
    });
}

void sorthargaascending(member arr[], int n) {
    sort(arr, arr + n, [](const member& a, const member& b) {
        int totalA = hargapaket(a.namapaket, a.detail.bulanMember);
        int totalB = hargapaket(b.namapaket, b.detail.bulanMember);
        
        if (a.diskon_aktif) totalA = totalA - (totalA * a.nominal_diskon / 100);
        if (b.diskon_aktif) totalB = totalB - (totalB * b.nominal_diskon / 100);
        
        return totalA < totalB;
    });
}

int binarysearchid(member *arr, int n, int target) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].id == target) return mid;
        else if (arr[mid].id < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int linearsearchname(member *arr, int n, string target) {
    for (int i = 0; i < n; i++)
        if (arr[i].nama == target) return i;
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


const string FILENAME = "data_member.csv";

void loadmembersfromCSV(member members[], int &jumlahmember) {
    ifstream file(FILENAME);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line) && jumlahmember < MAX_MEMBER) {
        stringstream ss(line);
        string token;
        member m;
        
        getline(ss, token, ','); m.id = stoi(token);
        getline(ss, token, ','); m.nama = token;
        getline(ss, token, ','); m.username = token;
        getline(ss, token, ','); m.namapaket = token;
        getline(ss, token, ','); m.detail.hari = token;
        getline(ss, token, ','); m.detail.jamsesi = token;
        getline(ss, token, ','); m.detail.bulanMember = stoi(token);
        getline(ss, token, ','); m.detail.harga = stoi(token);
        getline(ss, token, ','); m.diskon_aktif = (token == "1");
        getline(ss, token, ','); m.nominal_diskon = stoi(token);
        
        members[jumlahmember++] = m;
    }
    file.close();
}

void savememberstoCSV(member members[], int jumlahmember) {
    ofstream file(FILENAME);
    if (!file.is_open()) {
        cout << "[ERROR] gagal menyimpan ke " << FILENAME << endl;
        return;
    }
    
    file << "id,nama,username,namapaket,hari,jamsesi,bulanMember,harga,diskon_aktif,nominal_diskon\n";
    
    for (int i = 0; i < jumlahmember; i++) {
        file << members[i].id << ","
             << members[i].nama << ","
             << members[i].username << ","
             << members[i].namapaket << ","
             << members[i].detail.hari << ","
             << members[i].detail.jamsesi << ","
             << members[i].detail.bulanMember << ","
             << members[i].detail.harga << ","
             << (members[i].diskon_aktif ? "1" : "0") << ","
             << members[i].nominal_diskon << "\n";
    }
    file.close();
}

void createdatamember(member members[], int &jumlahmember, string username) {
    MiloUtils::clearScreen();
    try {
        for (int i = 0; i < jumlahmember; i++) {
            if (members[i].username == username) {
                throw PADELEXCEPTION("MEMBER SUDAH ADA!");
            }
        }
        ASSERT(jumlahmember < MAX_MEMBER, "kapasitas member penuh");
        
        cout << "\n--- BUAT AKUN MEMBER ---" << endl;
        member baru;
        baru.id = (jumlahmember > 0) ? members[jumlahmember-1].id + 1 : 1;
        baru.username = username;
        baru.diskon_aktif = false;
        baru.nominal_diskon = 0;
        
        cout << "NAMA: ";
        getline(cin, baru.nama);
        ASSERT(!baru.nama.empty(), "nama tidak boleh kosong");
        
        tampilkaninfopaket();
        cout << "PAKET(SUN/MOON/STAR): ";
        getline(cin, baru.namapaket);
        
        for (char &c : baru.namapaket) {
            c = toupper(c);
        }
        
        if (baru.namapaket != "SUN" && baru.namapaket != "MOON" && baru.namapaket != "STAR") {
            throw PADELEXCEPTION("PAKET TIDAK VALID!");
        }
        
        baru.detail.hari = inputharivalid(baru.namapaket);
        infopilihanjam(baru.namapaket);
        
        int pilihanJam;
        while (true) {
            cout << "JAM BERAPA (1-" << (baru.namapaket == "SUN" ? "10" : "7") << "): ";
            cin >> pilihanJam;
            cin.ignore();
            
            try {
                baru.detail.jamsesi = jamsesi(baru.namapaket, pilihanJam);
                if (baru.detail.jamsesi != "-") {
                    break;
                }
            } catch (const PADELEXCEPTION& e) {
                cout << "[!] " << e.what() << endl;
            }
        }

        while (true) {
            cout << "\nMAU JOIN BERAPA BULAN (1-12): ";
            cin >> baru.detail.bulanMember;
            cin.ignore();
            
            if (baru.detail.bulanMember >= 1 && baru.detail.bulanMember <= 12) {
                break;
            } else {
                cout << "[!] Durasi harus antara 1-12 bulan! Silakan input kembali." << endl;
            }
        }
        
        baru.detail.harga = hargapaket(baru.namapaket);
        members[jumlahmember] = baru;
        jumlahmember++;
        
        savememberstoCSV(members, jumlahmember);
        
        int totalPertemuan = hitungpertemuan(baru.detail.bulanMember);
        int total = hargapaket(baru.namapaket, baru.detail.bulanMember);
        
        MiloUtils::printSuccess("YEY AKUN MEMBER UDAH ADA!");
        cout << "      PAKET           : " << baru.namapaket << endl;
        cout << "      HARI            : " << baru.detail.hari << endl;
        cout << "      JAM SESI        : " << baru.detail.jamsesi << endl;
        cout << "      DURASI          : " << baru.detail.bulanMember << " bulan" << endl;
        cout << "      TOTAL PERTEMUAN : " << totalPertemuan << "x" << endl;
        cout << "      HARGA /SESI     : Rp " << baru.detail.harga << ".000" << endl;
        cout << "      TOTAL BAYAR     : Rp " << total << ".000" << endl;
        catatLog("member baru: " + baru.nama + " (ID: " + to_string(baru.id) + ")");
    } catch (const PADELEXCEPTION& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void menutampildata(member members[], int jumlahmember) {
    MiloUtils::clearScreen();
    if (jumlahmember == 0) {
        MiloUtils::printWarning("data member masih kosong!");
        return;
    }

    int pilihanUtama;
    bool exitMenu = false;

    while (!exitMenu) {
        try {
            MiloUtils::printHeader("MENU TAMPILAN DATA");
            cout << "1. SEARCH" << endl;
            cout << "2. SORT" << endl;
            cout << "3. KEMBALI" << endl;
            MiloUtils::printSeparator();
            cout << "PILIHAN: ";
            cin >> pilihanUtama;
            cin.ignore();
            ASSERT(pilihanUtama >= 1 && pilihanUtama <= 3, "PILIHAN TIDAK VALID");

            if (pilihanUtama == 1) {
                int pilihanSearch;
                bool exitSearch = false;
                while (!exitSearch) {
                    cout << "\n--- MENU SEARCH ---" << endl;
                    cout << "1. BERDASARKAN ID" << endl;
                    cout << "2. BERDASARKAN NAMA" << endl;
                    cout << "3. KEMBALI" << endl;
                    cout << "PILIHAN: ";
                    cin >> pilihanSearch;
                    cin.ignore();
                    ASSERT(pilihanSearch >= 1 && pilihanSearch <= 3, "PILIHAN TIDAK VALID");

                    if (pilihanSearch == 1) {
                        int cariID; cout << "masukkan ID: "; cin >> cariID; cin.ignore();
                        int idx = -1;
                        for(int i=0; i<jumlahmember; i++) if(members[i].id == cariID) { idx = i; break; }
                        if (idx != -1) {
                            int total = hargapaket(members[idx].namapaket, members[idx].detail.bulanMember);
                            int totalAkhir = members[idx].diskon_aktif ? total - (total * members[idx].nominal_diskon / 100) : total;
                            MiloUtils::printSuccess("DATA DITEMUKAN!");
                            cout << "  ID: " << members[idx].id << " | NAMA: " << members[idx].nama 
                                 << " | PAKET: " << members[idx].namapaket 
                                 << " | DISKON: " << (members[idx].diskon_aktif ? to_string(members[idx].nominal_diskon) + "%" : "-")
                                 << " | TOTAL: Rp " << totalAkhir << "K" << endl;
                        } else {
                            MiloUtils::printWarning("ID TIDAK DITEMUKAN.");
                        }
                    } else if (pilihanSearch == 2) {
                        string cariNama; cout << "masukkan Nama: "; getline(cin, cariNama);
                        int idx = linearsearchname(members, jumlahmember, cariNama);
                        if (idx != -1) {
                            int total = hargapaket(members[idx].namapaket, members[idx].detail.bulanMember);
                            int totalAkhir = members[idx].diskon_aktif ? total - (total * members[idx].nominal_diskon / 100) : total;
                            MiloUtils::printSuccess("DATA DITEMUKAN!");
                            cout << "  ID: " << members[idx].id << " | NAMA: " << members[idx].nama 
                                 << " | PAKET: " << members[idx].namapaket 
                                 << " | DISKON: " << (members[idx].diskon_aktif ? to_string(members[idx].nominal_diskon) + "%" : "-")
                                 << " | TOTAL: Rp " << totalAkhir << "K" << endl;
                        } else {
                            MiloUtils::printWarning("NAMA TIDAK DITEMUKAN.");
                        }
                    } else if (pilihanSearch == 3) {
                        exitSearch = true;
                    }
                    if (!exitSearch) MiloUtils::pause();
                }
            } else if (pilihanUtama == 2) {
                int pilihanSort;
                bool exitSort = false;
                while (!exitSort) {
                    cout << "\n--- MENU SORT ---" << endl;
                    cout << "1. NAMA" << endl;
                    cout << "2. ID" << endl;
                    cout << "3. HARGA" << endl;
                    cout << "4. KEMBALI" << endl;
                    cout << "PILIHAN: ";
                    cin >> pilihanSort;
                    cin.ignore();
                    ASSERT(pilihanSort >= 1 && pilihanSort <= 4, "PILIHAN TIDAK VALID");

                    if (pilihanSort >= 1 && pilihanSort <= 3) {
                        member temp[MAX_MEMBER];
                        for(int i=0; i<jumlahmember; i++) temp[i] = members[i];

                        if(pilihanSort == 1) sortnamadescending(temp, jumlahmember);
                        else if(pilihanSort == 2) sorthargadescending(temp, jumlahmember);
                        else if(pilihanSort == 3) sorthargaascending(temp, jumlahmember);

                        cout << "\n================================================================================" << endl;
                        cout << left << setw(4) << "ID" << setw(15) << "NAMA" << setw(8) << "PAKET" 
                             << setw(8) << "BULAN" << setw(10) << "DISKON" << setw(15) << "TOTAL" << endl;
                        cout << "--------------------------------------------------------------------------------" << endl;
                        for (int i = 0; i < jumlahmember; i++) {
                            int total = hargapaket(temp[i].namapaket, temp[i].detail.bulanMember);
                            int totalAkhir = temp[i].diskon_aktif ? total - (total * temp[i].nominal_diskon / 100) : total;
                            cout << left << setw(4) << temp[i].id << setw(15) << temp[i].nama 
                                 << setw(8) << temp[i].namapaket << setw(8) << temp[i].detail.bulanMember
                                 << setw(10) << (temp[i].diskon_aktif ? to_string(temp[i].nominal_diskon) + "%" : "-")
                                 << setw(15) << ("Rp " + to_string(totalAkhir) + "K") << endl;
                        }
                        cout << "================================================================================" << endl;
                    } else if (pilihanSort == 4) {
                        exitSort = true;
                    }
                    if (!exitSort) MiloUtils::pause();
                }
            } else if (pilihanUtama == 3) {
                exitMenu = true;
            }
        } catch (const PADELEXCEPTION& e) { MiloUtils::printError(e.what()); MiloUtils::pause(); }
        catch (const exception& e) { MiloUtils::printError("ERROR " + string(e.what())); MiloUtils::pause(); }
    }
}

void keloladiskonmembership(member members[], int jumlahmember) {
    MiloUtils::clearScreen();
    try {
        vector<int> eligible;
        for(int i=0; i<jumlahmember; i++) {
            if(members[i].detail.bulanMember >= 6) eligible.push_back(i);
        }
        
        if(eligible.empty()) {
            MiloUtils::printWarning("tidak ada member dengan durasi >= 6 bulan.");
            return;
        }
        
        cout << "\n=== DISKON MEMBERSHIP ===" << endl;
        cout << "[INFO] KETENTUAN DISKON:" << endl;
        cout << "1. 6-8 bulan --> diskon 15%" << endl;
        cout << "2. 9-12 bulan --> diskon 30%" << endl;
        cout << "\nDAFTAR MEMBER   :" << endl;
        cout << left << setw(4) << "ID" << setw(15) << "NAMA" << setw(8) << "BULAN" 
             << setw(15) << "TOTAL AWAL" << endl;
        cout << "------------------------------------------" << endl;
        for(int idx : eligible) {
            int total = hargapaket(members[idx].namapaket, members[idx].detail.bulanMember);
            cout << left << setw(4) << members[idx].id << setw(15) << members[idx].nama 
                 << setw(8) << members[idx].detail.bulanMember
                 << setw(15) << ("Rp " + to_string(total) + "K") << endl;
        }
        cout << "------------------------------------------" << endl;
        
        cout << "\nmasukkan ID member untuk diberi diskon --> 0 untuk batal: ";
        int idPilih; cin >> idPilih; cin.ignore();
        if(idPilih == 0) return;
        
        int foundIdx = -1;
        for(int i=0; i<jumlahmember; i++) {
            if(members[i].id == idPilih && members[i].detail.bulanMember >= 6) {
                foundIdx = i;
                break;
            }
        }
        
        ASSERT(foundIdx != -1, "ID tidak valid atau durasi < 6 bulan!");
        
        int diskon = hitungdiskon(members[foundIdx].detail.bulanMember);
        int total = hargapaket(members[foundIdx].namapaket, members[foundIdx].detail.bulanMember);
        int totalAkhir = total - (total * diskon / 100);
        
        cout << "\n--- KONFIRMASI ---" << endl;
        cout << "MEMBER: " << members[foundIdx].nama << " (ID: " << members[foundIdx].id << ")" << endl;
        cout << "DURASI: " << members[foundIdx].detail.bulanMember << " bulan" << endl;
        cout << "klik 1 untuk ACC, angka lain --> batal: ";
        
        int konfirmasi; cin >> konfirmasi; cin.ignore();
        
        if(konfirmasi == 1) {
            members[foundIdx].diskon_aktif = true;
            members[foundIdx].nominal_diskon = diskon;
            
            savememberstoCSV(members, jumlahmember);
            
            cout << "\n[OK] DISKON " << diskon << "% BERHASIL DITERAPKAN!" << endl;
            cout << "data akhir setelah diskon: Rp " << totalAkhir << ".000" << endl;
            catatLog("diskon " + to_string(diskon) + "% untuk member ID " + to_string(idPilih));
        } else {
            MiloUtils::printWarning("pemberian diskon dibatalkan");
        }
        
    } catch (const PADELEXCEPTION& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void readdatamember(member members[], int jumlahmember, string username) {
    try {
        bool ditemukan = false;
        
        for (int i = 0; i < jumlahmember; i++) {
            if (members[i].username == username) {
                clitable::Table tabel;
                
                tabel.addColumn(clitable::Column("DATA MEMBER", clitable::Column::LEFT_ALIGN, 
                                clitable::Column::LEFT_ALIGN, 1, 20, clitable::Column::RESIZABLE));
                tabel.addColumn(clitable::Column("INFORMASI", clitable::Column::LEFT_ALIGN, 
                                clitable::Column::LEFT_ALIGN, 1, 35, clitable::Column::RESIZABLE));
                
                tabel.addTitle("DATA MEMBER ANDA");
                
                int totalPertemuan = hitungpertemuan(members[i].detail.bulanMember);
                int total = hargapaket(members[i].namapaket, members[i].detail.bulanMember);
                int totalAkhir = members[i].diskon_aktif ? total - (total * members[i].nominal_diskon / 100) : total;
                
                string row1[2] = {"ID", to_string(members[i].id)};
                string row2[2] = {"NAMA", members[i].nama};
                string row3[2] = {"USERNAME", members[i].username};
                string row4[2] = {"PAKET", members[i].namapaket};
                string row5[2] = {"HARI", members[i].detail.hari};
                string row6[2] = {"JAM SESI", members[i].detail.jamsesi};
                string row7[2] = {"DURASI", to_string(members[i].detail.bulanMember) + " bulan"};
                string row8[2] = {"PERTEMUAN /bulan", to_string(PERTEMUAN_PER_BULAN) + "x"};
                string row9[2] = {"TOTAL PERTEMUAN", to_string(totalPertemuan) + "x"};
                string row10[2] = {"HARGA /sesi", "Rp " + to_string(members[i].detail.harga) + ".000"};
                
                if (members[i].diskon_aktif) {
                    string row11[2] = {"DISKON", to_string(members[i].nominal_diskon) + "%"};
                    string row12[2] = {"TOTAL BAYAR", "Rp " + to_string(totalAkhir) + ".000"};
                    tabel.addRow(row1);
                    tabel.addRow(row2);
                    tabel.addRow(row3);
                    tabel.addRow(row4);
                    tabel.addRow(row5);
                    tabel.addRow(row6);
                    tabel.addRow(row7);
                    tabel.addRow(row8);
                    tabel.addRow(row9);
                    tabel.addRow(row10);
                    tabel.addRow(row11);
                    tabel.addRow(row12);
                } else {
                    string row11[2] = {"TOTAL BAYAR", "Rp " + to_string(total) + ".000"};
                    tabel.addRow(row1);
                    tabel.addRow(row2);
                    tabel.addRow(row3);
                    tabel.addRow(row4);
                    tabel.addRow(row5);
                    tabel.addRow(row6);
                    tabel.addRow(row7);
                    tabel.addRow(row8);
                    tabel.addRow(row9);
                    tabel.addRow(row10);
                    tabel.addRow(row11);
                }
                
                cout << tabel.draw();
                ditemukan = true;
                break;
            }
        }
        
        if (!ditemukan) {
            cout << "\n[ERROR] BELUM JOIN MEMBER! silakan buat akun membership dulu.." << endl;
        }
        
    } catch (const PADELEXCEPTION& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}



void updatedatamember(member members[], int jumlahmember) {
    MiloUtils::clearScreen();
    try {
        ASSERT(jumlahmember > 0, "data member masih kosong");
        cout << "\nDAFTAR MEMBER:" << endl;
        for (int i = 0; i < jumlahmember; i++) {
            cout << "  [" << members[i].id << "] " << members[i].nama << " | PAKET: " << members[i].namapaket << endl;
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
            throw PADELEXCEPTION("PAKET TIDAK VALID!");
        }
        
        members[idx].detail.hari = inputharivalid(members[idx].namapaket);
        infopilihanjam(members[idx].namapaket);
        cout << "pilih jam sesi (1-" << (members[idx].namapaket == "SUN" ? "10" : "7") << "): ";
        int pilihanJam;
        cin >> pilihanJam;
        cin.ignore();
        members[idx].detail.jamsesi = jamsesi(members[idx].namapaket, pilihanJam);
        
        cout << "\nMAU JOIN BERAPA BULAN: ";
        cin >> members[idx].detail.bulanMember;
        cin.ignore();
        ASSERT(members[idx].detail.bulanMember > 0, "durasi harus lebih dari 0 bulan");
        
        members[idx].detail.harga = hargapaket(members[idx].namapaket);
        int totalPertemuan = hitungpertemuan(members[idx].detail.bulanMember);
        int total = hargapaket(members[idx].namapaket, members[idx].detail.bulanMember);
        int totalAkhir = members[idx].diskon_aktif ? total - (total * members[idx].nominal_diskon / 100) : total;
        
        savememberstoCSV(members, jumlahmember);
        
        MiloUtils::printSuccess("UPDATE BERHASIL!");
        cout << "   TOTAL PERTEMUAN : " << totalPertemuan << "x" << endl;
        if (members[idx].diskon_aktif) {
            cout << "   DISKON          : " << members[idx].nominal_diskon << "%" << endl;
            cout << "   TOTAL BAYAR     : Rp " << totalAkhir << ".000 (setelah diskon)" << endl;
        } else {
            cout << "   TOTAL BAYAR     : Rp " << total << ".000" << endl;
        }
        catatLog("update member ID " + to_string(members[idx].id));
    } catch (const PADELEXCEPTION& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void deletedatamember(member members[], int &jumlahmember) {
    MiloUtils::clearScreen();
    try {
        ASSERT(jumlahmember > 0, "data member masih kosong");
        cout << "\nDAFTAR MEMBER:" << endl;
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
        
        savememberstoCSV(members, jumlahmember);
        
        MiloUtils::printSuccess("MEMBER ID " + to_string(index) + " BERHASIL DIHAPUS!");
        catatLog("hapus member ID " + to_string(index));
    } catch (const PADELEXCEPTION& e) {
        MiloUtils::printError(e.what());
    } catch (const exception& e) {
        MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
    }
}

void menuadmin(member members[], int &jumlahmember, string userLogin) {
    MiloUtils::clearScreen();
    int pilihan; 
    bool menuberjalan = true;
    
    do {
        try {
            cout << endl;
            MiloUtils::printHeader("ADMIN MENU - " + userLogin);
            cout << "1. TAMPILKAN DATA" << endl;
            cout << "2. KELOLA DISKON MEMBERSHIP" << endl;
            cout << "3. UPDATE PAKET MEMBER" << endl;
            cout << "4. HAPUS MEMBER" << endl;
            cout << "5. LOG AKTIVITAS" << endl;
            cout << "6. LOGOUT" << endl;
            MiloUtils::printSeparator();
            cout << "PILIHAN (1-6): ";
            cin >> pilihan;
            cin.ignore();
            ASSERT(pilihan >= 1 && pilihan <= 6, "PILIHAN TIDAK VALID!");
            
            switch (pilihan) {
                case 1: menutampildata(members, jumlahmember); break;
                case 2: keloladiskonmembership(members, jumlahmember); break;
                case 3: updatedatamember(members, jumlahmember); break;
                case 4: deletedatamember(members, jumlahmember); break;
                case 5: tampilkanLog(); MiloUtils::pause(); break;
                case 6:
                    MiloUtils::printSuccess("BERHASIL LOGOUT!");
                    menuberjalan = false;
                    break;
            }
        } catch (const PADELEXCEPTION& e) {
            MiloUtils::printError(e.what());
        } catch (const exception& e) {
            MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
        }
    } while (menuberjalan);
}

void menumember(member members[], int &jumlahmember, string userLogin) {
    MiloUtils::clearScreen();
    int pilihan; 
    bool menuberjalan = true;
    
    do {
        try {
            cout << endl;
            MiloUtils::printHeader("MEMBER MENU - " + userLogin);
            cout << "1. LIHAT JADWAL & HARGA PAKET" << endl;
            cout << "2. BUAT AKUN MEMBERSHIP" << endl;
            cout << "3. LIHAT DATA SAYA" << endl;
            cout << "4. LOGOUT" << endl;
            MiloUtils::printSeparator();
            cout << "PILIHAN: ";
            cin >> pilihan;
            cin.ignore();
            ASSERT(pilihan >= 1 && pilihan <= 4, "PILIHAN TIDAK VALID!");
            
            switch (pilihan) {
                case 1: tampilkaninfopaket(); break;
                case 2: createdatamember(members, jumlahmember, userLogin); break;
                case 3: readdatamember(members, jumlahmember, userLogin); break;
                case 4:
                    MiloUtils::printSuccess("BERHASIL LOGOUT!");
                    menuberjalan = false;
                    break;
            }
        } catch (const PADELEXCEPTION& e) {
            MiloUtils::printError(e.what());
        } catch (const exception& e) {
            MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
        }
    } while (menuberjalan);
}

int main() {
    try {
        user users[MAX_user] = {
            {"tirizzzmen", "2025", "member"},
            {"admin", "milopadel", "admin"}
        };
        int jumlahuser = 2;
        member members[MAX_MEMBER];
        int jumlahmember = 0;
        string inputnama, inputpw; 
        string userLogin, roleLogin;
        
        loadmembersfromCSV(members, jumlahmember);
        if (jumlahmember > 0) {
            cout << "[INFO] " << jumlahmember << " data member berhasil dimuat dari " << FILENAME << endl;
        }
        
        while (true) {
            try {
                MiloUtils::clearScreen();
                MiloUtils::printHeader("WELCOME TO MILO PADEL");
                cout << "\n--- MENU UTAMA ---" << endl;
                cout << "1. REGISTRASI AKUN BARU" << endl;
                cout << "2. LOGIN" << endl;
                cout << "3. KELUAR PROGRAM" << endl;
                cout << "PILIHAN: ";
                int pilihanawal;
                cin >> pilihanawal;
                cin.ignore();
                ASSERT(pilihanawal >= 1 && pilihanawal <= 3, "PILIHAN TIDAK VALID");
                
                if (pilihanawal == 1) {
                    cout << "\n--- REGISTRASI AKUN ---" << endl;
                    cout << "USERNAME: ";
                    getline(cin, inputnama);
                    bool usernameada = false;
                    for (int i = 0; i < jumlahuser; i++) {
                        if (inputnama == users[i].username) {
                            usernameada = true;
                            break;
                        }
                    }
                    ASSERT(!usernameada, "username sudah digunakan!");
                    cout << "PASSWORD: ";
                    getline(cin, inputpw);
                    ASSERT(inputpw.length() >= 3, "password minimal 3 karakter!");
                    users[jumlahuser].username = inputnama;
                    users[jumlahuser].password = inputpw;
                    users[jumlahuser].role = "user";
                    jumlahuser++;
                    MiloUtils::printSuccess("REGISTRASI BERHASIL! SILAKAN LOGIN");
                } else if (pilihanawal == 2) {
                    cout << "\n--- LOGIN ---" << endl;
                    bool loginSukses = false;
                    for (int percobaan = 0; percobaan < 3; percobaan++) {
                        cout << "USERNAME: ";
                        getline(cin, inputnama);
                        cout << "PASSWORD: ";
                        getline(cin, inputpw);
                        bool usnbenar = false, pwbenar = false;
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
                        MiloUtils::printError("LOGIN GAGAL!");
                        if (!usnbenar) cout << "  username tidak ditemukan!" << endl;
                        else cout << "  password salah!" << endl;
                        ASSERT(percobaan < 2, "AKSES DITOLAK! GAGAL LOGIN 3x");
                        cout << "  sisa percobaan: " << (2 - percobaan) << endl;
                    }
                    ASSERT(loginSukses, "LOGIN GAGAL");
                    if (roleLogin == "admin") menuadmin(members, jumlahmember, userLogin);
                    else menumember(members, jumlahmember, userLogin);
                } else if (pilihanawal == 3) {
                    savememberstoCSV(members, jumlahmember);
                    MiloUtils::printSuccess("TERIMA KASIH MILOVERS! SEE YOU...");
                    break;
                }
            } catch (const PADELEXCEPTION& e) {
                MiloUtils::printError(e.what());
                MiloUtils::pause();
            } catch (const exception& e) {
                MiloUtils::printError("TERJADI KESALAHAN: " + string(e.what()));
                MiloUtils::pause();
            }
        }
    } catch (const exception& e) {
        MiloUtils::printError("FATAL ERROR: " + string(e.what()));
        return 1;
    }
    return 0;
}