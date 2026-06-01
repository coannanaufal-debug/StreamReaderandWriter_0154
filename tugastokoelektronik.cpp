#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <stdexcept>

using namespace std;

// 1. STRUKTUR KELAS TOKOELEKTRONIK (OOP & ENKAPSULASI)
class TokoElektronik {
private:
    array<string, 3> etalase; // Array private kapasitas 3

public:
    // Constructor otomatis mengisi data awal
    TokoElektronik() {
        etalase[0] = "Laptop"; etalase[1] = "Smartphone"; etalase[2] = "SmartTV";
    }

    // 2. MEKANISME EXCEPTION HANDLING
    string ambilProduk(size_t nomorRak) {
        try {
            return etalase.at(nomorRak); // Wajib menggunakan .at()
        } catch (const out_of_range& e) {
            // Lempar kembali pesan error kustom sesuai instruksi
            throw runtime_error("Gagal Mengambil Barang : Rak nomor [" + to_string(nomorRak) + "] kosong atau tidak tersedia!");
        }
    }
};

// 3. FITUR MANAJEMEN GUDANG (FILE I/O & CRUD)
void tampilkanGudang() {
    ifstream file("gudang.txt");
    string baris;
    cout << "\n--- DAFTAR BARANG DI GUDANG ---\n";
    if (!file) { cout << "[Gudang kosong / file belum dibuat]\n"; return; }
    while (getline(file, baris)) { cout << baris << endl; }
    file.close();
}

void tambahBarang() {
    ofstream file("gudang.txt", ios::app);
    string id, nama, harga;
    cout << "ID: "; cin >> id; cin.ignore();
    cout << "Nama: "; getline(cin, nama);
    cout << "Harga: "; cin >> harga;
    file << id << " | " << nama << " | Rp." << harga << "\n";
    file.close();
    cout << ">> Barang berhasil ditambahkan!\n";
}

void updateAtauDeleteBarang(bool isDelete) {
    ifstream file("gudang.txt");
    ofstream temp("temp.txt");
    string targetID, baris;
    cout << "Masukkan ID Barang target: "; cin >> targetID;

    bool ditemukan = false;
    while (getline(file, baris)) {
        // Cari ID di awal baris
        if (baris.rfind(targetID, 0) == 0) { 
            ditemukan = true;
            if (isDelete) continue; // Jika delete, baris ini dilewati (dihapus)
            
            // Jika update, minta input baru
            string nama, harga; cin.ignore();
            cout << "Nama Baru: "; getline(cin, nama);
            cout << "Harga Baru: "; cin >> harga;
            baris = targetID + " | " + nama + " | Rp." + harga;
        }
        temp << baris << "\n";
    }
    file.close(); temp.close();
    remove("gudang.txt"); rename("temp.txt", "gudang.txt");
    
    if (ditemukan) cout << ">> Operasi Berhasil!\n";
    else cout << ">> ID tidak ditemukan!\n";
}

// 4. PENGUJIAN PADA FUNGSI MAIN() & SIMULASI ETALASE
void simulasiEtalase(TokoElektronik& toko) {
    cout << "\n--- SIMULASI EXCEPTION HANDLING ---\n";
    // Skenario 1: Rak indeks 1 (Sukses)
    try {
        cout << "[Skenario 1 - Rak 1]: " << toko.ambilProduk(1) << " -> BERHASIL\n";
    } catch (const runtime_error& e) { cout << e.what() << endl; }

    // Skenario 2: Rak indeks 5 (Gagal & Tangkap Pesan Kustom)
    try {
        cout << "[Skenario 2 - Rak 5]: " << toko.ambilProduk(5) << endl;
    } catch (const runtime_error& e) {
        cout << "[Skenario 2 - Error Ditangkap]: " << e.what() << "\n";
    }
}

int main() {
    TokoElektronik gibranJaya;
    int pilihan;

    do {
        tampilkanGudang(); // Otomatis tampil saat menu dibuka
        cout << "\n=== TOKO GIBRAN JAYA ===\n";
        cout << "1. Tambah Barang (Create)\n2. Update Barang (Update)\n3. Hapus Barang (Delete)\n";
        cout << "4. Simulasi Etalase (Exception)\n5. Keluar\nPilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahBarang(); break;
            case 2: updateAtauDeleteBarang(false); break; // Update
            case 3: updateAtauDeleteBarang(true); break;  // Delete
            case 4: simulasiEtalase(gibranJaya); break;
            case 5: cout << "Keluar program.\n"; break;
            default: cout << "Pilihan salah!\n";
        }
    } while (pilihan != 5);

    return 0;
}