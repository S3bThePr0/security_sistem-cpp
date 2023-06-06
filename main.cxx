#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;

struct User {
    string username;
    string password;
};

const int MAX_USERS = 10;
User users[MAX_USERS];
int numUsers = 0;

void loadUsers() {
    ifstream file("users.txt");
    string line;
    int i = 0;

    while (getline(file, line) && i < MAX_USERS) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            users[i].username = line.substr(0, pos);
            users[i].password = line.substr(pos + 1);
            i++;
        }
    }

    file.close();
}

void saveUser(string username, string password) {
    ofstream file("users.txt", ios::app);
    file << username << "," << password << endl;
    file.close();
}

bool authenticateUser(string username, string password) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].username == username && users[i].password == password) {
            return true;
        } 
    }

    return false;
}

void deleteAccount() {
    string username, password;
    cout << "Inserisci il tuo username: ";
    cin >> username;
    cout << "Inserisci la tua password: ";
    cin >> password;

    int index = -1;
    for (int i = 0; i < numUsers; i++) {
        if (users[i].username == username && users[i].password == password) {
            index = i;
            break;
        } 
    }

    if (index == -1) {
        cout << "Credenziali errate." << endl;
        return;
    }

    for (int i = index; i < numUsers - 1; i++) {
        users[i] = users[i + 1];
    }
    numUsers--;

    ofstream file("users.txt", ofstream::trunc); // sovrascrive il file
    for (int j = 0; j < numUsers; j++) {
        file << users[j].username << "," << users[j].password << endl;
    }
    file.close();

    cout << "Account eliminato con successo." << endl;
}

void accessLog(string username) {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&current_time);
    ofstream file("access_log.txt", ios::app);
    file << username << "," << time_str << endl;
    file.close();
}

bool resetPassword(string username, string newPassword) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].username == username) {
            users[i].password = newPassword;
            ofstream file("users.txt", ofstream::trunc);
            for (int j = 0; j < MAX_USERS; j++) {
                if (!users[j].username.empty() && !users[j].password.empty()) {
                    file << users[j].username << "," << users[j].password << endl;
                }
            }
            file.close();
            return true;
        }
    }
    return false;
}

int main() {
    loadUsers();
    int choice;
    string username, newPassword;

    cout << "1. Login\n2. Reset Password\n3. Crea un nuovo utente\n4. Elimina un utente\n5. Esci\n" << endl;
    cout << "Scelta: ";
    cin >> choice;

    switch (choice) {
        case 1: {
            int attempts = 3;

            while (attempts > 0) {
                cout << "Inserisci il tuo username: ";
                cin >> username;
                cout << "Inserisci la tua password: ";
                cin >> newPassword;

                if (authenticateUser(username, newPassword)) {
                    cout << "Accesso consentito!" << endl;
                    cout << "Accesso effettuato da:" << username << endl;
                    accessLog(username);
                    break;
                } else {
                    attempts--;
                    cout << "Credenziali errate. Tentativi rimasti: " << attempts << endl;
                }
            }

            if (attempts == 0) {
                cout << "Accesso negato. Troppi tentativi falliti." << endl;
            }
            break;
        }
        case 2: {
            cout << "Inserisci il tuo username: ";
            cin >> username;
            cout << "Inserisci la tua nuova password: ";
            cin >> newPassword;

            if (resetPassword(username, newPassword)) {
                cout << "Password cambiata con successo!" << endl;
            } else {
                cout << "Username non trovato." << endl;
            }
            break;
        }
        case 3: {
            string username, password;
            cout << "Inserisci il nuovo username: ";
            cin >> username;
            cout << "Inserisci la nuova password: ";
            cin >> password;
            
            saveUser(username, password);
            cout << "Utente creato con successo!" << endl;
            break;
        }
        case 4: {
            deleteAccount();
            break;
        }
        case 5: {
            cout << "Grazie di aver usato il nostro servizio!" << endl;
            break;
        }
        default:
            cout << "Scelta non valida, riprova\n";
            break;
    }

    return 0;
}
