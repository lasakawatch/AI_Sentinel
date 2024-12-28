#include <iostream>
#include <string>
#include <iomanip> // Untuk pengaturan output
using namespace std;

// --------------------------------------------------
// Struktur Data
// --------------------------------------------------

// Struktur data untuk Aktor (Child)
struct Actor {
    string id;          // ID unik Aktor
    string name;        // Nama Aktor
    double discount;    // Potongan member dalam %
    Actor* next;        // Pointer ke Aktor berikutnya
};

// Struktur data untuk Film (Parent)
struct Film {
    string id;                  // ID unik Film
    string title;               // Judul Film
    int capacity;               // Kapasitas maksimal
    int participantCount;       // Jumlah peserta saat ini
    double cost;                // Biaya perjalanan dalam juta
    string pic;                 // PIC (Person in Charge)
    Film* next;                 // Pointer ke Film berikutnya
    struct Relation* relations; // Pointer ke daftar relasi
};

// Struktur data untuk Relasi antara Film dan Aktor
struct Relation {
    Actor* actor;       // Pointer ke Aktor yang terlibat
    Relation* next;     // Pointer ke Relasi berikutnya
};

// --------------------------------------------------
// Head Pointers untuk Daftar Film dan Aktor
// --------------------------------------------------
Film* headFilm = nullptr;
Actor* headActor = nullptr;

// --------------------------------------------------
// a. Insert Element Parent : 5
// --------------------------------------------------
void addFilm(string id, string title, int capacity, double cost, string pic) {
    // Membuat Film baru
    Film* newFilm = new Film{id, title, capacity, 0, cost, pic, nullptr, nullptr};
    // Menambahkan ke awal daftar Film
    newFilm->next = headFilm;
    headFilm = newFilm;
    cout << "Film \"" << title << "\" berhasil ditambahkan.\n";
}

// --------------------------------------------------
// b. Insert Element Child : 5
// --------------------------------------------------
void addActor(string id, string name, double discount) {
    // Membuat Aktor baru
    Actor* newActor = new Actor{id, name, discount, nullptr};
    // Menambahkan ke awal daftar Aktor
    newActor->next = headActor;
    headActor = newActor;
    cout << "Aktor \"" << name << "\" berhasil ditambahkan.\n";
}

// --------------------------------------------------
// c. Insert Element Relation : 5
// --------------------------------------------------
void addRelation(string filmId, string actorId) {
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
        // Mengecek apakah relasi sudah ada
        Relation* rel = film->relations;
        while (rel) {
            if (rel->actor == actor) {
                cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << actor->name << "\" sudah ada.\n";
                return;
            }
            rel = rel->next;
        }

        // Membuat Relasi baru
        Relation* newRel = new Relation{actor, film->relations};
        film->relations = newRel;

        // Menambah jumlah peserta film jika kapasitas masih tersedia
        if (film->participantCount < film->capacity) {
            film->participantCount++;
            cout << "Relasi berhasil ditambahkan dan jumlah peserta film \"" << film->title << "\" ditambah.\n";
        } else {
            cout << "Relasi berhasil ditambahkan, tetapi kapasitas film \"" << film->title << "\" sudah penuh.\n";
        }
    } else {
        cout << "Film atau Aktor tidak ditemukan.\n";
    }
}

// --------------------------------------------------
// d. Delete Element Parent : 5
// --------------------------------------------------
void deleteFilm(string id) {
    Film** current = &headFilm;
    while (*current && (*current)->id != id) {
        current = &((*current)->next);
    }

    if (*current) {
        Film* toDelete = *current;
        *current = (*current)->next;

        // Menghapus semua relasi yang terkait dengan Film ini
        Relation* rel = toDelete->relations;
        while (rel) {
            Relation* temp = rel;
            rel = rel->next;
            delete temp;
        }

        delete toDelete;
        cout << "Film dengan ID \"" << id << "\" berhasil dihapus.\n";
    } else {
        cout << "Film dengan ID \"" << id << "\" tidak ditemukan.\n";
    }
}

// --------------------------------------------------
// e. Delete Element Child : 5
// --------------------------------------------------
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

// --------------------------------------------------
// f. Delete Element Relation : 5
// --------------------------------------------------
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

// --------------------------------------------------
// g. Find Element Parent : 5
// --------------------------------------------------
Film* findFilmById(string id) {
    Film* current = headFilm;
    while (current && current->id != id) {
        current = current->next;
    }
    return current;
}

// --------------------------------------------------
// h. Find Element Child : 5
// --------------------------------------------------
Actor* findActorById(string id) {
    Actor* current = headActor;
    while (current && current->id != id) {
        current = current->next;
    }
    return current;
}

// --------------------------------------------------
// i. Find Element Relation : 5
// --------------------------------------------------
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

// --------------------------------------------------
// j. Show All Data di List Parent : 5
// --------------------------------------------------
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
             << ", Biaya: " << fixed << setprecision(2) << current->cost << " juta"
             << ", PIC: " << current->pic << "\n";
        current = current->next;
    }
}

// --------------------------------------------------
// k. Show All Data di List Child : 5
// --------------------------------------------------
void showAllActors() {
    Actor* current = headActor;
    if (!current) {
        cout << "\nDaftar Aktor kosong.\n";
        return;
    }

    cout << "\n===== Daftar Aktor =====\n";
    while (current) {
        cout << "- " << current->name << " (ID: " << current->id << "), Potongan: " 
             << fixed << setprecision(2) << current->discount << "%\n";
        current = current->next;
    }
}

// --------------------------------------------------
// l. Show Data Child dari Parent Tertentu : 5
// --------------------------------------------------
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
            cout << "- " << rel->actor->name << " (ID: " << rel->actor->id << "), Potongan: " 
                 << fixed << setprecision(2) << rel->actor->discount << "%\n";
            rel = rel->next;
        }
    } else {
        cout << "Film dengan ID \"" << filmId << "\" tidak ditemukan.\n";
    }
}

// --------------------------------------------------
// m. Show Setiap Data Parent Beserta Data Child yang Berelasi dengannya : 5
// --------------------------------------------------
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
            cout << "  - Aktor: " << rel->actor->name << " (ID: " << rel->actor->id << "), Potongan: " 
                 << fixed << setprecision(2) << rel->actor->discount << "%\n";
            rel = rel->next;
        }
        film = film->next;
    }
}

// --------------------------------------------------
// n. Show Data Child Beserta Data Parent yang Masing-masing Child Miliki : 10
// --------------------------------------------------
void showAllActorsWithFilms() {
    Actor* actor = headActor;
    if (!actor) {
        cout << "\nDaftar Aktor kosong.\n";
        return;
    }

    cout << "\n===== Semua Aktor dan Film yang Berelasi =====\n";
    while (actor) {
        cout << "Aktor: " << actor->name << " (ID: " << actor->id << ")\n";
        bool hasRelation = false;
        Film* film = headFilm;
        while (film) {
            Relation* rel = film->relations;
            while (rel) {
                if (rel->actor == actor) {
                    cout << "  - Film: " << film->title << " (ID: " << film->id << "), Biaya: " 
                         << fixed << setprecision(2) << film->cost << " juta\n";
                    hasRelation = true;
                    break; // Agar tidak mencetak film yang sama berkali-kali jika ada relasi ganda
                }
                rel = rel->next;
            }
            film = film->next;
        }
        if (!hasRelation) {
            cout << "  - Tidak ada relasi.\n";
        }
        actor = actor->next;
    }
}

// --------------------------------------------------
// o. Show Data Parent yang Berelasi dengan Child Tertentu : 5
// --------------------------------------------------
void showFilmsByActor(string actorId) {
    Actor* actor = findActorById(actorId);
    if (!actor) {
        cout << "Aktor dengan ID \"" << actorId << "\" tidak ditemukan.\n";
        return;
    }

    bool found = false;
    cout << "\n===== Film yang Dikutip oleh Aktor \"" << actor->name << "\" =====\n";
    Film* film = headFilm;
    while (film) {
        Relation* rel = film->relations;
        while (rel) {
            if (rel->actor == actor) {
                cout << "- " << film->title << " (ID: " << film->id << "), Biaya: " 
                     << fixed << setprecision(2) << film->cost << " juta\n";
                found = true;
                break; // Agar tidak mencetak film yang sama berkali-kali jika ada relasi ganda
            }
            rel = rel->next;
        }
        film = film->next;
    }

    if (!found) {
        cout << "Tidak ada film yang terkait dengan aktor tersebut.\n";
    }
}

// --------------------------------------------------
// p. Count Relation dari Setiap Element Parent : 5
// --------------------------------------------------
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

// --------------------------------------------------
// q. Count Relation yang Dimiliki oleh Child Tertentu : 5
// --------------------------------------------------
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

// --------------------------------------------------
// r. Count Element Child yang Tidak Memiliki Relasi : 5
// --------------------------------------------------
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

// --------------------------------------------------
// s. Edit Relasi / Mengganti Child dari Parent Tertentu : 5
// --------------------------------------------------
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
                temp = temp->next;
            }
            if (exists) {
                cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << newActor->name << "\" sudah ada.\n";
                return;
            }

            // Mengubah Aktor dalam relasi
            rel->actor = newActor;
            cout << "Relasi berhasil diubah dari Aktor \"" << oldActor->name << "\" menjadi Aktor \"" << newActor->name << "\".\n";
            return;
        }
        rel = rel->next;
    }
    cout << "Relasi antara Film \"" << film->title << "\" dan Aktor \"" << oldActor->name << "\" tidak ditemukan.\n";
}

// --------------------------------------------------
// Detail Functions
// --------------------------------------------------

// 15. Show Detail Film : 5
void showFilmDetails(string filmId) {
    Film* film = findFilmById(filmId);
    if (film) {
        cout << "\n===== Detail Film =====\n";
        cout << "ID: " << film->id << "\n";
        cout << "Title: " << film->title << "\n";
        cout << "Kapasitas Maksimal: " << film->capacity << "\n";
        cout << "Jumlah Peserta: " << film->participantCount << "\n";
        cout << "Biaya Perjalanan: " << fixed << setprecision(2) << film->cost << " juta\n";
        cout << "PIC: " << film->pic << "\n";
    } else {
        cout << "Film dengan ID \"" << filmId << "\" tidak ditemukan.\n";
    }
}

// 16. Show Detail Aktor : 5
void showActorDetails(string actorId) {
    Actor* actor = findActorById(actorId);
    if (actor) {
        cout << "\n===== Detail Aktor =====\n";
        cout << "ID: " << actor->id << "\n";
        cout << "Name: " << actor->name << "\n";
        cout << "Potongan Member: " << fixed << setprecision(2) << actor->discount << "%\n";
    } else {
        cout << "Aktor dengan ID \"" << actorId << "\" tidak ditemukan.\n";
    }
}

// --------------------------------------------------
// Main Function
// --------------------------------------------------
int main() {
    // Judul Program
    cout << "============================================\n";
    cout << "         Selamat Datang di Sistem Data Film    \n";
    cout << "============================================\n";

    int choice;
    string filmId, filmTitle, actorId, actorName, pic;
    int capacity;
    double cost, discount;
    string relationFilmId, relationActorId;
    string editFilmId, oldActorId_input, newActorId_input;

    // Menambahkan beberapa Film
    addFilm("F1", "Inception", 5, 10.0, "Febryanti");
    addFilm("F2", "Interstellar", 5, 12.0, "Febryanti");
    addFilm("F3", "The Dark Knight", 5, 11.0, "Febryanti");
    addFilm("F4", "Dunkirk", 5, 9.0, "Febryanti");
    addFilm("F5", "Tenet", 5, 13.0, "Febryanti");

    // Menambahkan beberapa Aktor
    addActor("A1", "Leonardo DiCaprio", 10.0);
    addActor("A2", "Matthew McConaughey", 5.0);
    addActor("A3", "Christian Bale", 12.0);
    addActor("A4", "Joseph Gordon-Levitt", 7.0);
    addActor("A5", "Elliot Page", 8.0);
    addActor("A6", "Tom Hardy", 9.0);
    addActor("A7", "Anne Hathaway", 6.0);
    addActor("A8", "Michael Caine", 4.0);
    addActor("A9", "Cillian Murphy", 5.0);
    addActor("A10", "Ken Watanabe", 7.0);

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
        cout << "\n===== Menu =====\n";
        cout << "1. Tambah Film : \n";
        cout << "2. Tambah Aktor : \n";
        cout << "3. Tambah Relasi : \n";
        cout << "4. Hapus Film : \n";
        cout << "5. Hapus Aktor : \n";
        cout << "6. Tampilkan Semua Film : \n";
        cout << "7. Tampilkan Semua Aktor : \n";
        cout << "8. Tampilkan Aktor berdasarkan Film : \n";
        cout << "9. Tampilkan Film berdasarkan Aktor : \n";
        cout << "10. Tampilkan Semua Relasi : \n";
        cout << "11. Hitung Relasi per Film : \n";
        cout << "12. Hitung Relasi per Aktor : \n";
        cout << "13. Hitung Aktor tanpa Relasi : \n";
        cout << "14. Edit Relasi : \n";
        cout << "15. Tampilkan Detail Film : \n";
        cout << "16. Tampilkan Detail Aktor : \n";
        cout << "17. Keluar\n";
        cout << "Pilih opsi (1-17): ";
        cin >> choice;

        switch (choice) {
            case 1:
                // a. Insert Element Parent : 5
                cout << "\n--- Insert Element Parent ---\n";
                cout << "Masukkan ID Film: ";
                cin >> filmId;
                cout << "Masukkan Title Film: ";
                cin.ignore();
                getline(cin, filmTitle);
                cout << "Masukkan Kapasitas Maksimal: ";
                cin >> capacity;
                cout << "Masukkan Biaya Perjalanan (dalam juta): ";
                cin >> cost;
                cout << "Masukkan PIC: ";
                cin.ignore();
                getline(cin, pic);
                addFilm(filmId, filmTitle, capacity, cost, pic);
                break;
            case 2:
                // b. Insert Element Child : 5
                cout << "\n--- Insert Element Child ---\n";
                cout << "Masukkan ID Aktor: ";
                cin >> actorId;
                cout << "Masukkan Nama Aktor: ";
                cin.ignore();
                getline(cin, actorName);
                cout << "Masukkan Potongan Member (%): ";
                cin >> discount;
                addActor(actorId, actorName, discount);
                break;
            case 3:
                // c. Insert Element Relation : 5
                cout << "\n--- Insert Element Relation ---\n";
                cout << "Masukkan ID Film: ";
                cin >> relationFilmId;
                cout << "Masukkan ID Aktor: ";
                cin >> relationActorId;
                addRelation(relationFilmId, relationActorId);
                break;
            case 4:
                // d. Delete Element Parent : 5
                cout << "\n--- Delete Element Parent ---\n";
                cout << "Masukkan ID Film yang akan dihapus: ";
                cin >> filmId;
                deleteFilm(filmId);
                break;
            case 5:
                // e. Delete Element Child : 5
                cout << "\n--- Delete Element Child ---\n";
                cout << "Masukkan ID Aktor yang akan dihapus: ";
                cin >> actorId;
                deleteActor(actorId);
                break;
            case 6:
                // f. Show All Data di List Parent : 5
                showAllFilms();
                break;
            case 7:
                // g. Show All Data di List Child : 5
                showAllActors();
                break;
            case 8:
                // h. Show Data Child berdasarkan Film : 5
                cout << "\n--- Tampilkan Aktor berdasarkan Film ---\n";
                cout << "Masukkan ID Film: ";
                cin >> filmId;
                showActorsByFilm(filmId);
                break;
            case 9:
                // i. Show Data Film berdasarkan Aktor : 5
                cout << "\n--- Tampilkan Film berdasarkan Aktor ---\n";
                cout << "Masukkan ID Aktor: ";
                cin >> actorId;
                showFilmsByActor(actorId);
                break;
            case 10:
                // j. Show Semua Relasi : 5
                showAllFilmsWithActors();
                break;
            case 11:
                // k. Count Relasi per Film : 5
                countRelationsPerFilm();
                break;
            case 12:
                // l. Count Relasi per Aktor : 5
                countRelationsPerActor();
                break;
            case 13:
                // m. Count Aktor tanpa Relasi : 5
                countActorsWithoutRelations();
                break;
            case 14:
                // n. Edit Relasi : 5
                cout << "\n--- Edit Relasi ---\n";
                cout << "Masukkan ID Film: ";
                cin >> editFilmId;
                cout << "Masukkan ID Aktor Lama: ";
                cin >> oldActorId_input;
                cout << "Masukkan ID Aktor Baru: ";
                cin >> newActorId_input;
                editRelation(editFilmId, oldActorId_input, newActorId_input);
                break;
            case 15:
                // o. Show Detail Film : 5
                cout << "\n--- Tampilkan Detail Film ---\n";
                cout << "Masukkan ID Film: ";
                cin >> filmId;
                showFilmDetails(filmId);
                break;
            case 16:
                // p. Show Detail Aktor : 5
                cout << "\n--- Tampilkan Detail Aktor ---\n";
                cout << "Masukkan ID Aktor: ";
                cin >> actorId;
                showActorDetails(actorId);
                break;
            case 17:
                // q. Keluar
                cout << "\nTerima kasih telah menggunakan sistem ini. Sampai jumpa!\n";
                return 0;
            default:
                cout << "\nOpsi tidak valid. Silakan coba lagi.\n";
        }
    }

    return 0;
}
