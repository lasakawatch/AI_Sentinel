#include <iostream>
#include <string>
#include <iomanip> // Untuk pengaturan output
using namespace std;
// Struktur Data
// --------------------------------------------------
// Struktur data untuk Aktor (Child)
struct Actor {
    string id;          // ID unik Aktor
    string name;        // Nama Aktor
    Actor* next;        // Pointer ke Aktor berikutnya
};
// Struktur data untuk Film (Parent)
struct Film {
    string id;                  // ID unik Film
    string title;               // Judul Film
    int capacity;               // Kapasitas maksimal
    int participantCount;       // Jumlah peserta saat ini
    double ticketPrice;         // Harga tiket
    string pic;                 // PIC (Person in Charge)
    Film* next;                 // Pointer ke Film berikutnya
    struct Relation* relations; // Pointer ke daftar relasi
};
// Struktur data untuk Relasi antara Film dan Aktor
struct Relation {
    Actor* actor;       // Pointer ke Aktor yang terlibat
    Relation* next;     // Pointer ke Relasi berikutnya
};
// Head Pointers untuk Daftar Film dan Aktor
Film* headFilm = nullptr;
Actor* headActor = nullptr;

// a. Insert Element Parent : 5 ===============================================
void addFilm(string id, string title, int capacity, double ticketPrice, string pic) {
    // Membuat Film baru
    Film* newFilm = new Film{id, title, capacity, 0, ticketPrice, pic, nullptr, nullptr};
    // Menambahkan ke awal daftar Film
    newFilm->next = headFilm; headFilm = newFilm;
    cout << "Film \"" << title << "\" berhasil ditambahkan.\n";}

// b. Insert Element Child : 5 ====================================================
void addActor(string id, string name) {
    // Membuat Aktor baru
    Actor* newActor = new Actor{id, name, nullptr};
    newActor->next = headActor; headActor = newActor;
    cout << "Aktor \"" << name << "\" berhasil ditambahkan.\n";}

// c. Insert Element Relation : 5 =================================================
void addRelation(string filmId, string actorId) {
    // Mencari Film berdasarkan ID
    Film* film = headFilm;
    while (film && film->id != filmId)
    {film = film->next;}
    // Mencari Aktor berdasarkan ID
    Actor* actor = headActor;
    while (actor && actor->id != actorId)
    {actor = actor->next;}

    if (film && actor) {// Mengecek apakah relasi sudah ada
    Relation* rel = film->relations;
    while (rel) {if (rel->actor == actor)
    { cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << actor->name << "\" sudah ada.\n";
    return;} rel = rel->next;}
        // Membuat Relasi baru
        Relation* newRel = new Relation{actor, film->relations};
        film->relations = newRel;
        // Menambah jumlah peserta film jika kapasitas masih tersedia
        if (film->participantCount < film->capacity) {
            film->participantCount++;
        cout << "Relasi berhasil ditambahkan dan jumlah peserta film \"" << film->title << "\" ditambah.\n";
        } else {cout << "Relasi berhasil ditambahkan, tetapi kapasitas film \"" << film->title << "\" sudah penuh.\n";}
        } else {cout << "Film atau Aktor tidak ditemukan.\n";}
}

// d. Delete Element Parent : 5 ====================================================
void deleteFilm(string id) {
    Film** current = &headFilm;
    while (*current && (*current)->id != id) {
    current = &((*current)->next);}

    if (*current) {
    Film* toDelete = *current;
    *current = (*current)->next;
     // Menghapus semua relasi yang terkait dengan Film ini
        Relation* rel = toDelete->relations;
        while (rel) { Relation* temp = rel;
        rel = rel->next; delete temp;
        }
        delete toDelete;
    cout << "Film dengan ID \"" << id << "\" berhasil dihapus.\n";
    } else {
    cout << "Film dengan ID \"" << id << "\" tidak ditemukan.\n";}
}
// e. Delete Element Child : 5 =====================================================
void deleteActor(string id) {
    Actor** current = &headActor;
    while (*current && (*current)->id != id) {
        current = &((*current)->next);
    }
    if (*current) {
        Actor* toDelete = *current;
        *current = (*current)->next;
        // Menghapus Aktor dari semua relasi yang ada
        Film* film = headFilm;
        while (film) {
            Relation** rel = &(film->relations);
            while (*rel) {
                if ((*rel)->actor == toDelete) {
                    Relation* temp = *rel;
                    *rel = (*rel)->next;
                    delete temp;
                    // Mengurangi jumlah peserta film
                    if (film->participantCount > 0) film->participantCount--;
                } else {
                    rel = &((*rel)->next);
                }
            }
            film = film->next;
        }
        delete toDelete;
        cout << "Aktor dengan ID \"" << id << "\" berhasil dihapus.\n";
    } else {
        cout << "Aktor dengan ID \"" << id << "\" tidak ditemukan.\n";
    }
}
// f. Delete Element Relation : 5 =================================================
void deleteRelation(string filmId, string actorId) {
    // Mencari Film berdasarkan ID
    Film* film = headFilm;
    while (film && film->id != filmId) {
        film = film->next;
    }

    // Mencari Aktor berdasarkan ID
    Actor* actor = headActor;
    while (actor && actor->id != actorId) {
        actor = actor->next;
    }

    if (film && actor) {
        // Mencari dan menghapus Relasi
        Relation** relPtr = &(film->relations);
        while (*relPtr) {
            if ((*relPtr)->actor == actor) {
                Relation* toDelete = *relPtr;
                *relPtr = (*relPtr)->next;
                delete toDelete;
                // Mengurangi jumlah peserta film
                if (film->participantCount > 0) film->participantCount--;
                cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << actor->name << "\" berhasil dihapus.\n";
                return;
            }
            relPtr = &((*relPtr)->next);
        }

        cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << actor->name << "\" tidak ditemukan.\n";
    } else {
        cout << "Film atau Aktor tidak ditemukan.\n";
    }
}
// g. Find Element Parent : 5 =================================================
Film* findFilmById(string id) {
    Film* current = headFilm;
    while (current && current->id != id) {
        current = current->next;
    }
    return current;
}
// h. Find Element Child : 5 =================================================
Actor* findActorById(string id) {
    Actor* current = headActor;
    while (current && current->id != id) {
        current = current->next;
    }
    return current;
}
// i. Find Element Relation : 5 =================================================
bool findRelation(string filmId, string actorId) {
    Film* film = findFilmById(filmId);
    Actor* actor = findActorById(actorId);
    if (film && actor) {
        Relation* rel = film->relations;
        while (rel) {
            if (rel->actor == actor) {
                return true;
            }
            rel = rel->next;
        }
    }
    return false;
}
// j. Show All Data di List Parent : 5  ================================================
void showAllFilms() {
    Film* current = headFilm;
    if (!current) {
        cout << "\nDaftar Film kosong.\n";
        return;
    }

    cout << "\n===== Daftar Film =====\n";
    while (current) {
        cout << "- " << current->title << " (ID: " << current->id << ")\n";
        cout << "  Kapasitas: " << current->capacity
             << ", Peserta: " << current->participantCount
             << ", Harga Tiket: Rp " << fixed << setprecision(2) << current->ticketPrice
             << ", PIC: " << current->pic << "\n";
        current = current->next;
    }
}
// k. Show All Data di List Child :5  ================================================
void showAllActors() {
    Actor* current = headActor;
    if (!current) {
        cout << "\nDaftar Aktor kosong.\n";
        return;
    }

    cout << "\n===== Daftar Aktor =====\n";
    while (current) {
        cout << "- " << current->name << " (ID: " << current->id << ")\n";
        current = current->next;
    }
}
// l. Show Data Child dari Parent Tertentu : 5  ===============================================
void showActorsByFilm(string filmId) {
    Film* film = findFilmById(filmId);
    if (film) {
        cout << "\n===== Aktor dalam Film \"" << film->title << "\" =====\n";
        if (!film->relations) {
            cout << "Tidak ada aktor yang terlibat dalam film ini.\n";
            return;
        }
        Relation* rel = film->relations;
        while (rel) {
            cout << "- " << rel->actor->name << " (ID: " << rel->actor->id << ")\n";
            rel = rel->next;
        }
    } else {
        cout << "Film dengan ID \"" << filmId << "\" tidak ditemukan.\n";
    }
}
// m. Show Setiap Data Parent Beserta Data Child yang Berelasi dengannya : 5 ==============================================
void showAllFilmsWithActors() {
    Film* film = headFilm;
    if (!film) {
        cout << "\nDaftar Film kosong.\n";
        return;
    }
    cout << "\n===== Semua Film dan Aktor yang Berelasi =====\n";
    while (film) {
        cout << "Film: " << film->title << " (ID: " << film->id << ")\n";
        Relation* rel = film->relations;
        if (!rel) {
            cout << "  - Tidak ada relasi.\n";
        }
        while (rel) {
            cout << "  - Aktor: " << rel->actor->name << " (ID: " << rel->actor->id << ")\n";
            rel = rel->next;
        }
        film = film->next;
    }
}
// n. Show Data Child Beserta Data Parent yang Masing-masing Child Miliki : 10 =====================================
void showAllActorsWithFilms() {
    Actor* actor = headActor;
    if (!actor) { cout << "\nDaftar Aktor kosong.\n";return;
    }
    cout << "\n===== Semua Aktor dan Film yang Berelasi =====\n";
    while (actor) { cout << "Aktor: " << actor->name << " (ID: " << actor->id << ")\n";
        bool hasRelation = false;
        Film* film = headFilm;
        while (film) {
        Relation* rel = film->relations;
        while (rel) { if (rel->actor == actor) {
                    cout << "  - Film: " << film->title << " (ID: " << film->id << "), Harga Tiket: Rp "
                    << fixed << setprecision(2) << film->ticketPrice << "\n";
                    hasRelation = true; break;
                }
            rel = rel->next;
            }film = film->next;
        }if (!hasRelation) { cout << "  - Tidak ada relasi.\n";}
        actor = actor->next;}
}
// o. Show Data Parent yang Berelasi dengan Child Tertentu : 5 ============================================
void showFilmsByActor(string actorId) {
    Actor* actor = findActorById(actorId);
    if (!actor) { cout << "Aktor dengan ID \"" << actorId << "\" tidak ditemukan.\n";return;
    }
    bool found = false;
    cout << "\n===== Film yang Dikutip oleh Aktor \"" << actor->name << "\" =====\n";
    Film* film = headFilm; while (film) { Relation* rel = film->relations; // Mencari relasi antara film dan aktor
        while (rel) {
        if (rel->actor == actor) {
                cout << "- " << film->title << " (ID: " << film->id << "), Harga Tiket: Rp "
                 << fixed << setprecision(2) << film->ticketPrice << "\n";
                found = true; break; }
            rel = rel->next;}
            film = film->next;
    }
    if (!found) {cout << "Tidak ada film yang terkait dengan aktor tersebut.\n"; }
}
// p. Count Relation dari Setiap Element Parent : 5 ===============================================
void countRelationsPerFilm() {
    Film* film = headFilm;
    if (!film) {
        cout << "\nTidak ada Film untuk dihitung relasinya.\n";
        return;
    }

    cout << "\n===== Jumlah Relasi per Film =====\n";
    while (film) {
        int count = 0;
        Relation* rel = film->relations;
        while (rel) {
            count++;
            rel = rel->next;
        }
        cout << "- " << film->title << " (ID: " << film->id << "): " << count << " relasi\n";
        film = film->next;
    }
}
// q. Count Relation yang Dimiliki oleh Child Tertentu : 5 ==============================================
void countRelationsPerActor() {
    Actor* actor = headActor;
    if (!actor) {
        cout << "\nTidak ada Aktor untuk dihitung relasinya.\n";
        return;
    }
    cout << "\n===== Jumlah Relasi per Aktor =====\n";
    while (actor) {
        int count = 0;
        Film* film = headFilm;
        while (film) {
            Relation* rel = film->relations;
            while (rel) {
                if (rel->actor == actor) {
                    count++;
                    break; // Agar tidak menghitung relasi ganda untuk film yang sama
                }
                rel = rel->next;
            }
            film = film->next;
        }
        cout << "- " << actor->name << " (ID: " << actor->id << "): " << count << " relasi\n";
        actor = actor->next;
    }
}
// r. Count Element Child yang Tidak Memiliki Relasi : 5 ===============================================
void countActorsWithoutRelations() {
    Actor* actor = headActor;
    if (!actor) {
        cout << "\nTidak ada Aktor untuk dihitung.\n";
        return;
    }
    int count = 0;
    while (actor) {
        bool hasRelation = false;
        Film* film = headFilm;
        while (film && !hasRelation) {
            Relation* rel = film->relations;
            while (rel) {
                if (rel->actor == actor) {
                    hasRelation = true;
                    break;
                }
                rel = rel->next;
            }
            film = film->next;
        }
        if (!hasRelation) count++;
        actor = actor->next;
    }
    cout << "\n===== Jumlah Aktor tanpa Relasi =====\n";
    cout << "Jumlah Aktor tanpa relasi: " << count << "\n";
}
// s. Edit Relasi / Mengganti Child dari Parent Tertentu : 5 ==============================================
void editRelation(string filmId, string oldActorId, string newActorId) {
    // Mencari Film
    Film* film = findFilmById(filmId);
    if (!film) {
        cout << "Film dengan ID \"" << filmId << "\" tidak ditemukan.\n";
        return;
    }
    // Mencari Aktor lama dan baru
    Actor* oldActor = findActorById(oldActorId);
    Actor* newActor = findActorById(newActorId);

    if (!oldActor || !newActor) {
        cout << "Aktor lama atau baru tidak ditemukan.\n";
        return;
    }
    // Mencari Relasi yang ingin diubah
    Relation* rel = film->relations;
    while (rel) {
        if (rel->actor == oldActor) {
            // Mengecek apakah relasi baru sudah ada
            Relation* temp = film->relations;
            bool exists = false;
            while (temp) {
                if (temp->actor == newActor) {
                    exists = true;
                    break;
                }
                temp = temp->next; }
            if (exists) {
                cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << newActor->name << "\" sudah ada.\n";
                return; }
            // Mengubah Aktor dalam relasi
            rel->actor = newActor;
            cout << "Relasi berhasil diubah dari Aktor \"" << oldActor->name << "\" menjadi Aktor \"" << newActor->name << "\".\n";
            return;}
        rel = rel->next;}
    cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << oldActor->name << "\" tidak ditemukan.\n";
}
// Detail Functions  =====================================
// 15. Show Detail Film : 5
void showFilmDetails(string filmId) {
    Film* film = findFilmById(filmId);
    if (film) {
        cout << "\n===== Detail Film =====\n";
        cout << "ID: " << film->id << "\n";
        cout << "Title: " << film->title << "\n";
        cout << "Kapasitas Maksimal: " << film->capacity << "\n";
        cout << "Jumlah Peserta: " << film->participantCount << "\n";
        cout << "Harga Tiket: Rp " << fixed << setprecision(2) << film->ticketPrice << "\n";
        cout << "PIC: " << film->pic << "\n";
    } else {
        cout << "Film dengan ID \"" << filmId << "\" tidak ditemukan.\n";}
}
// 16. Show Detail Aktor : 5
void showActorDetails(string actorId) {
    Actor* actor = findActorById(actorId);
    if (actor) {
        cout << "\n===== Detail Aktor =====\n";
        cout << "ID: " << actor->id << "\n";
        cout << "Name: " << actor->name << "\n";
    } else {cout << "Aktor dengan ID \"" << actorId << "\" tidak ditemukan.\n";}
}
// Main Function  =====================================
int main() {
    // Judul Program
    cout << "============================================\n";
    cout << "         Selamat Datang di Sistem Data Film    \n";
    cout << "============================================\n";

    int choice;
    string filmId, filmTitle, actorId, actorName, pic;
    int capacity;
    double ticketPrice;
    string relationFilmId, relationActorId;
    string editFilmId, oldActorId_input, newActorId_input;
    // Menambahkan beberapa Film
    addFilm("F1", "Inception", 5, 75000, "Febryanti");
    addFilm("F2", "Interstellar", 5, 80000, "Febryanti");
    addFilm("F3", "The Dark Knight", 5, 90000, "Febryanti");
    addFilm("F4", "Dunkirk", 5, 60000, "Febryanti");
    addFilm("F5", "Tenet", 5, 95000, "Febryanti");
    // Menambahkan beberapa Aktor
    addActor("A1", "Leonardo DiCaprio");
    addActor("A2", "Matthew McConaughey");
    addActor("A3", "Christian Bale");
    addActor("A4", "Joseph Gordon-Levitt");
    addActor("A5", "Elliot Page");
    addActor("A6", "Tom Hardy");
    addActor("A7", "Anne Hathaway");
    addActor("A8", "Michael Caine");
    addActor("A9", "Cillian Murphy");
    addActor("A10", "Ken Watanabe");
    // Menambahkan beberapa Relasi
    addRelation("F1", "A1"); // Inception - Leonardo DiCaprio
    addRelation("F1", "A4"); // Inception - Joseph Gordon-Levitt
    addRelation("F2", "A2"); // Interstellar - Matthew McConaughey
    addRelation("F3", "A3"); // The Dark Knight - Christian Bale
    addRelation("F3", "A6"); // The Dark Knight - Tom Hardy
    addRelation("F4", "A2"); // Dunkirk - Matthew McConaughey
    addRelation("F4", "A8"); // Dunkirk - Michael Caine
    addRelation("F5", "A1"); // Tenet - Leonardo DiCaprio
    addRelation("F5", "A10"); // Tenet - Ken Watanabe

    while (true) {
        cout << "\n===== Menu Utama =====\n";
        cout << "1. Tambah Data\n";
        cout << "2. Hapus Data\n";
        cout << "3. Tambah Relasi\n";
        cout << "4. Hapus Relasi\n";
        cout << "5. Tampilkan Data\n";
        cout << "6. Hitung Data\n";
        cout << "7. Edit Relasi\n";
        cout << "8. Detail Data\n";
        cout << "9. Keluar\n";
        cout << "Pilih opsi (1-9): ";
        cin >> choice;
        cin.ignore(); // Membersihkan newline dari input sebelumnya

        switch (choice) {
            case 1: { int subChoice;
                cout << "\n===== Tambah Data =====\n";
                cout << "1. Tambah Film\n";
                cout << "2. Tambah Aktor\n";
                cout << "Pilih opsi (1-2): ";
                cin >> subChoice;
                cin.ignore();
                if (subChoice == 1) { cout << "\n--- Tambah Film ---\n";
                    cout << "Masukkan ID Film: ";getline(cin, filmId);
                    cout << "Masukkan Title Film: ";getline(cin, filmTitle);
                    cout << "Masukkan Kapasitas Maksimal: ";cin >> capacity;
                    cout << "Masukkan Harga Tiket (50000-100000): ";cin >> ticketPrice;
                    cin.ignore();
                    cout << "Masukkan PIC: ";getline(cin, pic);
                    addFilm(filmId, filmTitle, capacity, ticketPrice, pic);
                } else if (subChoice == 2) {
                    cout << "\n--- Tambah Aktor ---\n";
                    cout << "Masukkan ID Aktor: "; getline(cin, actorId);
                    cout << "Masukkan Nama Aktor: ";getline(cin, actorName);
                    addActor(actorId, actorName);
                } else {
                    cout << "Opsi tidak valid.\n";
                }
                break;
            }
            case 2: { // Hapus Data
                int subChoice;
                cout << "\n===== Hapus Data =====\n";
                cout << "1. Hapus Film\n";
                cout << "2. Hapus Aktor\n";
                cout << "Pilih opsi (1-2): ";
                cin >> subChoice;
                cin.ignore();
                if (subChoice == 1) {
                    cout << "\n--- Hapus Film ---\n";
                    cout << "Masukkan ID Film yang akan dihapus: ";
                    getline(cin, filmId);
                    deleteFilm(filmId);
                } else if (subChoice == 2) {
                    cout << "\n--- Hapus Aktor ---\n";
                    cout << "Masukkan ID Aktor yang akan dihapus: ";
                    getline(cin, actorId);
                    deleteActor(actorId);
                } else {
                    cout << "Opsi tidak valid.\n";
                }
                break;
            }
            case 3: // Tambah Relasi
                cout << "\n--- Tambah Relasi ---\n";
                cout << "Masukkan ID Film: ";
                getline(cin, relationFilmId);
                cout << "Masukkan ID Aktor: ";
                getline(cin, relationActorId);
                addRelation(relationFilmId, relationActorId);
                break;
            case 4: // Hapus Relasi
                cout << "\n--- Hapus Relasi ---\n";
                cout << "Masukkan ID Film: ";
                getline(cin, filmId);
                cout << "Masukkan ID Aktor: ";
                getline(cin, actorId);
                deleteRelation(filmId, actorId);
                break;
            case 5: { // Tampilkan Data
                int subChoice;
                cout << "\n===== Tampilkan Data =====\n";
                cout << "1. Tampilkan Semua Film\n";
                cout << "2. Tampilkan Semua Aktor\n";
                cout << "3. Tampilkan Aktor berdasarkan Film\n";
                cout << "4. Tampilkan Film berdasarkan Aktor\n";
                cout << "5. Tampilkan Semua Relasi\n";
                cout << "Pilih opsi (1-5): ";
                cin >> subChoice;
                cin.ignore();
                if (subChoice == 1) {
                    showAllFilms();
                } else if (subChoice == 2) {
                    showAllActors();
                } else if (subChoice == 3) {
                    cout << "\n--- Tampilkan Aktor berdasarkan Film ---\n";
                    cout << "Masukkan ID Film: ";
                    getline(cin, filmId);
                    showActorsByFilm(filmId);
                } else if (subChoice == 4) {
                    cout << "\n--- Tampilkan Film berdasarkan Aktor ---\n";
                    cout << "Masukkan ID Aktor: ";
                    getline(cin, actorId);
                    showFilmsByActor(actorId);
                } else if (subChoice == 5) {
                    showAllFilmsWithActors();
                } else {
                    cout << "Opsi tidak valid.\n";
                }
                break;
            }
            case 6: { // Hitung Data
                int subChoice;
                cout << "\n===== Hitung Data =====\n";
                cout << "1. Hitung Relasi per Film\n";
                cout << "2. Hitung Relasi per Aktor\n";
                cout << "3. Hitung Aktor tanpa Relasi\n";
                cout << "Pilih opsi (1-3): ";
                cin >> subChoice;
                cin.ignore();
                if (subChoice == 1) {
                    countRelationsPerFilm();
                } else if (subChoice == 2) {
                    countRelationsPerActor();
                } else if (subChoice == 3) {
                    countActorsWithoutRelations();
                } else {
                    cout << "Opsi tidak valid.\n";
                }
                break;
            }
            case 7: // Edit Relasi
                cout << "\n--- Edit Relasi ---\n";
                cout << "Masukkan ID Film: ";
                getline(cin, editFilmId);
                cout << "Masukkan ID Aktor Lama: ";
                getline(cin, oldActorId_input);
                cout << "Masukkan ID Aktor Baru: ";
                getline(cin, newActorId_input);
                editRelation(editFilmId, oldActorId_input, newActorId_input);
                break;
            case 8: { // Detail Data
                int subChoice;
                cout << "\n===== Detail Data =====\n";
                cout << "1. Tampilkan Detail Film\n";
                cout << "2. Tampilkan Detail Aktor\n";
                cout << "Pilih opsi (1-2): ";
                cin >> subChoice;
                cin.ignore();
                if (subChoice == 1) {
                    cout << "\n--- Tampilkan Detail Film ---\n";
                    cout << "Masukkan ID Film: ";
                    getline(cin, filmId);
                    showFilmDetails(filmId);
                } else if (subChoice == 2) {
                    cout << "\n--- Tampilkan Detail Aktor ---\n";
                    cout << "Masukkan ID Aktor: ";
                    getline(cin, actorId);
                    showActorDetails(actorId);
                } else {
                    cout << "Opsi tidak valid.\n";
                }
                break;
            }
            case 9: // Keluar
                cout << "\nTerima kasih telah menggunakan sistem ini. Sampai jumpa!\n";
                return 0;
            default:
                cout << "\nOpsi tidak valid. Silakan coba lagi.\n";
        }
    }

    return 0;
}
