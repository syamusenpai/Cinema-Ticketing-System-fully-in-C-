#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#include <sqlite3.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

// Function to set console text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void playMusic() {
    PlaySound(TEXT("C:\\Users\\Hisya\\Desktop\\UNI STUFF\\Megalovania.wav"), NULL, SND_ASYNC);
}

void displayTitle() {
    setColor(11); // Cyan text
    cout << "   /$$$$$$  /$$$$$$ /$$   /$$ /$$$$$$$$ /$$      /$$  /$$$$$$  /$$   /$$" << endl;
    cout << "  /$$__  $$|_  $$_/| $$$ | $$| $$_____/| $$$    /$$$ /$$__  $$| $$  / $$" << endl;
    cout << " | $$        | $$  | $$$$| $$| $$      | $$$$  /$$$$| $$  \\ $$|  $$/ $$/" << endl;
    cout << " | $$        | $$  | $$ $$ $$| $$$$$   | $$ $$/$$ $$| $$$$$$$$ \\  $$$$/ " << endl;
    cout << " | $$        | $$  | $$  $$$$| $$__/   | $$  $$$| $$| $$__  $$  >$$  $$ " << endl;
    cout << " | $$        | $$  | $$\\  $$$| $$      | $$\\  $ | $$| $$  | $$ /$$/\\  $$" << endl;
    cout << " |  $$$$$$/ /$$$$$$| $$ \\  $$| $$$$$$$$| $$ \\/  | $$| $$  | $$| $$  \\ $$" << endl;
    cout << "  \\______/ |______/|__/  \\__/|________/|__/     |__/|__/  |__/|__/  |__/" << endl;
    setColor(7); 
}

void executeSQL(sqlite3* db, const string& sql) {
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        setColor(12); 
        cerr << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        setColor(7); 
    }
}

void createTableIfNotExists(sqlite3* db) {
    string sql = "CREATE TABLE IF NOT EXISTS bookings ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT," 
                 "name TEXT NOT NULL," 
                 "age INTEGER NOT NULL," 
                 "movie TEXT NOT NULL," 
                 "genre TEXT NOT NULL," 
                 "rating TEXT NOT NULL," 
                 "time_slot TEXT NOT NULL," 
                 "booking_time TEXT NOT NULL"
                 ");";
    executeSQL(db, sql);
}

void displayBookings(sqlite3* db) {
    sqlite3_stmt* stmt;
    string sql = "SELECT * FROM bookings;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        setColor(10); 
        cout << "\n=== Cinemax Booking Records ===" << endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            setColor(14); 
            cout << "ID: " << sqlite3_column_int(stmt, 0) << " | Name: " << sqlite3_column_text(stmt, 1)
                 << " | Age: " << sqlite3_column_int(stmt, 2)
                 << " | Movie: " << sqlite3_column_text(stmt, 3)
                 << " | Genre: " << sqlite3_column_text(stmt, 4)
                 << " | Rating: " << sqlite3_column_text(stmt, 5)
                 << " | Time Slot: " << sqlite3_column_text(stmt, 6)
                 << " | Booking Time: " << sqlite3_column_text(stmt, 7) << endl;
        }
        setColor(7); 
        sqlite3_finalize(stmt);
    } else {
        setColor(12); 
        cerr << "Failed to fetch bookings: " << sqlite3_errmsg(db) << endl;
        setColor(7); 
    }
}

void deleteBooking(sqlite3* db) {
    int id;
    setColor(9); 
    cout << "Enter the ID of the booking to delete: ";
    setColor(7); 
    cin >> id;

    string sql = "DELETE FROM bookings WHERE id = " + to_string(id) + ";";
    executeSQL(db, sql);
    setColor(10); 
    cout << "Booking with ID " << id << " deleted successfully." << endl;
    setColor(7);
}

void updateBooking(sqlite3* db) {
    int id;
    string newName, newMovie, newGenre, newRating, newTimeSlot;
    int newAge;

    setColor(9); 
    cout << "Enter the ID of the booking to update: ";
    setColor(7); 
    cin >> id;
    cin.ignore();

    cout << "Enter new name: ";
    getline(cin, newName);
    cout << "Enter new age: ";
    cin >> newAge;
    cin.ignore();
    cout << "Enter new movie: ";
    getline(cin, newMovie);
    cout << "Enter new genre: ";
    getline(cin, newGenre);
    cout << "Enter new rating: ";
    getline(cin, newRating);
    cout << "Enter new time slot: ";
    getline(cin, newTimeSlot);

    string sql = "UPDATE bookings SET name = '" + newName + "', age = " + to_string(newAge) + ", movie = '" + newMovie + "', genre = '" + newGenre + "', rating = '" + newRating + "', time_slot = '" + newTimeSlot + "' WHERE id = " + to_string(id) + ";";
    executeSQL(db, sql);
    setColor(10); 
    cout << "Booking with ID " << id << " updated successfully." << endl;
    setColor(7); 
}

void printReceipt(const string& name, int age, const string& movie, const string& genre, const string& rating, const string& timeSlot) {
    setColor(11); 
    cout << "\n=== Booking Receipt ===" << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Movie: " << movie << endl;
    cout << "Genre: " << genre << endl;
    cout << "Rating: " << rating << endl;
    cout << "Time Slot: " << timeSlot << endl;
    cout << "=======================" << endl;
    setColor(7); 
}

void userMenu(sqlite3* db);
void adminMenu(sqlite3* db);

void mainMenu(sqlite3* db) {
    int userType;
    do {
        setColor(9); 
        cout << "\nWelcome to the Cinema Ticketing System!" << endl;
        setColor(7); 
        cout << "1. User" << endl;
        cout << "2. Admin" << endl;
        cout << "3. Exit" << endl;
        cout << "Select your role: ";
        cin >> userType;

        if (userType == 1) {
            userMenu(db);
        } else if (userType == 2) {
            adminMenu(db);
        } else if (userType != 3) {
            setColor(12); 
            cout << "Invalid selection. Please try again." << endl;
            setColor(7); 
        }
    } while (userType != 3);

    setColor(10); 
    cout << "\nThank you for using the Cinema Ticketing System. Goodbye!" << endl;
    setColor(7); 
}

void userMenu(sqlite3* db) {
    string name, selectedMovie, selectedGenre, movieRating, bookedTime;
    int age, genreChoice, movieChoice, timeChoice;

    string genres[] = {"1. Action", "2. Comedy", "3. Romance", "4. Horror", "5. Sci-Fi"};
    string actionMovies[] = {"1. Avatar 2 (PG-13)", "2. Spider-Man: No Way Home (PG)", "3. The Batman (R)"};
    string comedyMovies[] = {"1. Minions: The Rise of Gru (PG)", "2. Deadpool (NC-17)", "3. Jumanji (PG-13)"};
    string romanceMovies[] = {"1. Titanic (G)", "2. The Notebook (PG-13)", "3. La La Land (PG-13)"};
    string horrorMovies[] = {"1. The Conjuring (R)", "2. IT Chapter Two (R)", "3. A Quiet Place (PG-13)"};
    string sciFiMovies[] = {"1. Interstellar (PG-13)", "2. Inception (PG-13)", "3. Dune (PG-13)"};
    string timeSlots[] = {"1. 10:00 AM", "2. 1:00 PM", "3. 4:00 PM", "4. 7:00 PM", "5. 10:00 PM"};

    setColor(13); 
    cout << "\n=== Cinema Ticketing System ===" << endl;
    setColor(7); 
    cout << "Enter Your Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Your Age: ";
    cin >> age;

    while (true) {
        setColor(10); 
        cout << "\nAvailable Genres:" << endl;
        setColor(7); 
        for (const auto& genre : genres) {
            cout << genre << endl;
        }
        cout << "Select a genre (1-5): ";
        cin >> genreChoice;
        if (genreChoice >= 1 && genreChoice <= 5) {
            selectedGenre = genres[genreChoice - 1].substr(3);
            break;
        } else {
            setColor(12); 
            cout << "Invalid choice. Try again." << endl;
            setColor(7); 
        }
    }

    string* movies = nullptr;
    switch (genreChoice) {
        case 1: movies = actionMovies; break;
        case 2: movies = comedyMovies; break;
        case 3: movies = romanceMovies; break;
        case 4: movies = horrorMovies; break;
        case 5: movies = sciFiMovies; break;
    }

    while (true) {
        setColor(10); 
        cout << "\nAvailable Movies in " << selectedGenre << " genre:" << endl;
        setColor(7); 
        for (int i = 0; i < 3; i++) {
            cout << movies[i] << endl;
        }
        cout << "Select a movie (1-3): ";
        cin >> movieChoice;
        if (movieChoice >= 1 && movieChoice <= 3) {
            selectedMovie = movies[movieChoice - 1].substr(3, movies[movieChoice - 1].find('(') - 4);
            movieRating = movies[movieChoice - 1].substr(movies[movieChoice - 1].find('(') + 1, 5);
            break;
        } else {
            setColor(12); 
            cout << "Invalid choice. Try again." << endl;
            setColor(7); 
        }
    }

    while (true) {
        setColor(10); 
        cout << "\nAvailable Time Slots:" << endl;
        setColor(7); 
        for (const auto& slot : timeSlots) {
            cout << slot << endl;
        }
        cout << "Select a time slot (1-5): ";
        cin >> timeChoice;
        if (timeChoice >= 1 && timeChoice <= 5) {
            bookedTime = timeSlots[timeChoice - 1].substr(3);
            break;
        } else {
            setColor(12); 
            cout << "Invalid choice. Try again." << endl;
            setColor(7); 
        }
    }

    time_t now = time(0);
    string currentTime = ctime(&now);

    string sql = "INSERT INTO bookings (name, age, movie, genre, rating, time_slot, booking_time) VALUES ('" +
                 name + "', " + to_string(age) + ", '" + selectedMovie + "', '" + selectedGenre + "', '" +
                 movieRating + "', '" + bookedTime + "', '" + currentTime + "');";
    executeSQL(db, sql);

    printReceipt(name, age, selectedMovie, selectedGenre, movieRating, bookedTime);

    
    mainMenu(db);
}

void adminMenu(sqlite3* db) {
    int choice;
    do {
        setColor(13); 
        cout << "\n=== Admin Menu ===" << endl;
        setColor(7); 
        cout << "1. Show Booking Records" << endl;
        cout << "2. Edit Booking Record" << endl;
        cout << "3. Delete Booking Record" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayBookings(db);
                break;
            case 2:
                updateBooking(db);
                break;
            case 3:
                deleteBooking(db);
                break;
            case 4:
                mainMenu(db);
                return;
            default:
                setColor(12); 
                cout << "Invalid choice. Please try again." << endl;
                setColor(7); 
        }
    } while (true);
}

int main() {
    sqlite3* db;
    if (sqlite3_open("cinema.db", &db) != SQLITE_OK) {
        setColor(12); 
        cerr << "Failed to open database: " << sqlite3_errmsg(db) << endl;
        setColor(7); 
        return 1;
    }

    createTableIfNotExists(db);

    playMusic();
    displayTitle();
    mainMenu(db);

    sqlite3_close(db);
    return 0;
}
