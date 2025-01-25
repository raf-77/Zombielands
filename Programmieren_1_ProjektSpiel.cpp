#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mmsystem.h>
#include <conio.h>
#include <vector>

#pragma comment(lib, "winmm.lib")

using namespace std;

// ClearConsole Funktion
void clearConsole() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

// Funktion um Text Farbe zu ändern 
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void displayMenu(int selectedOption) {
    clearConsole();
    SetConsoleOutputCP(CP_UTF8);
    setConsoleColor(12);
    cout << R"(▒███████▒ ▒█████   ███▄ ▄███▓ ▄▄▄▄    ██▓▓█████  ██▓    ▄▄▄       ███▄    █ ▓█████▄   ██████ 
▒ ▒ ▒ ▄▀░▒██▒  ██▒▓██▒▀█▀ ██▒▓█████▄ ▓██▒▓█   ▀ ▓██▒   ▒████▄     ██ ▀█   █ ▒██▀ ██▌▒██    ▒ 
░ ▒ ▄▀▒░ ▒██░  ██▒▓██    ▓██░▒██▒ ▄██▒██▒▒███   ▒██░   ▒██  ▀█▄  ▓██  ▀█ ██▒░██   █▌░ ▓██▄   
  ▄▀▒   ░▒██   ██░▒██    ▒██ ▒██░█▀  ░██░▒▓█  ▄ ▒██░   ░██▄▄▄▄██ ▓██▒  ▐▌██▒░▓█▄   ▌  ▒   ██▒
▒███████▒░ ████▓▒░▒██▒   ░██▒░▓█  ▀█▓░██░░▒████▒░██████▒▓█   ▓██▒▒██░   ▓██░░▒████▓ ▒██████▒▒
░▒▒ ▓░▒░▒░ ▒░▒░▒░ ░ ▒░   ░  ░░▒▓███▀▒░▓  ░░ ▒░ ░░ ▒░▓  ░▒▒   ▓▒█░░ ▒░   ▒ ▒  ▒▒▓  ▒ ▒ ▒▓▒ ▒ ░
░░▒ ▒ ░ ▒  ░ ▒ ▒░ ░  ░      ░▒░▒   ░  ▒ ░ ░ ░  ░░ ░ ▒  ░ ▒   ▒▒ ░░ ░░   ░ ▒░ ░ ▒  ▒ ░ ░▒  ░ ░
░ ░ ░ ░ ░░ ░ ░ ▒  ░      ░    ░    ░  ▒ ░   ░     ░ ░    ░   ▒      ░   ░ ░  ░ ░  ░ ░  ░  ░  
  ░ ░        ░ ░         ░    ░       ░     ░  ░    ░  ░     ░  ░         ░    ░          ░  
░                                  ░                                         ░               

)" << endl;
    setConsoleColor(15);

    string options[] = {
        "1. Spiel starten",
        "2. ReadMe",
    };

    for (int i = 0; i < 2; ++i) {
        if (i == selectedOption) {
            setConsoleColor(10); 
            cout << " > " << options[i] << endl;
            setConsoleColor(15); 
        }
        else {
            cout << "   " << options[i] << endl;
        }
    }
}

int navigateMenu() {
    int selectedOption = 0; // Default erste option
    const int numOptions = 2;

    while (true) {
        displayMenu(selectedOption);

        int key = _getch(); // Wait for key press

        // Pfeiltasten codierung
        if (key == 224) { // erstes arrowkey signal
            key = _getch();
            if (key == 72) { // hoch
                selectedOption = (selectedOption - 1) % numOptions;
            }
            else if (key == 80) { // runter
                selectedOption = (selectedOption + 1) % numOptions;
            }
        }
        else if (key == 13) { // Enter 
            return selectedOption; // Select ist mehr ein "zurück"
        }
    }
}

// ReadMe Menüpunkt
void ReadMe() {
    clearConsole();
    cout << "Programmieren1 Test Spiel!" << endl;
    cout << "Das Ziel ist Ressourcen zu sammeln, und Gebiete erfolgreich von Zombies zu säubern." << endl;
    cout << "\nGameloop:\n";
    cout << "1- Baue eine Farm, um Nahrung zu erhalten.\n";
    cout << "2- Sammle Schrott, um Scouts auszubilden.\n";
    cout << "3- Bilde Scouts aus, um Zombies zu töten und neue Gebiete zu erobern.\n";
    cout << "4- Je mehr Gebiete du besitzt desto mehr Recourcen generierst du pro Runde.\n";
    cout << "\n Versuche keinen Ungültigen Input zu tätigen da das Spiel dann abstürzt." << endl;
    cout << "Press Enter, um zum Hauptmenü zurückzukehren." << endl;
    cin.ignore();
    cin.get();
}

// Spieler Stats
struct Player {
    int food = 100;
    int scraps = 50;
    int population = 10;
    int army = 0;
    int territories = 1;
};
// Displaystats
void displayStatus(const Player& player) {
    cout << "\n >>>> Aktueller Status <<<<" << endl;
    cout << "Nahrung: " << player.food << endl;
    cout << "Schrott: " << player.scraps << endl;
    cout << "Bevölkerung: " << player.population << endl;
    cout << "Scouts: " << player.army << endl;
    cout << "Gebiete: " << player.territories << endl;
    cout << "===========================\n" << endl;
}
bool checkWinCondition(const Player& player) {
    return player.territories >= 9;
}

void buildFarm(Player& player) {
    if (player.scraps >= 25) {
        player.scraps -= 25;
        player.food += 50;
        setConsoleColor(14);
        cout << "Du hast eine Farm gebaut! Nahrung erhöht sich um 50." << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));

    }
    else {
        setConsoleColor(12);
        cout << "Nicht genug Schrott, um eine Farm zu bauen." << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
void buildMine(Player& player) {
    if (player.food >= 30) {
        player.food -= 30;
        player.scraps += 40;
        setConsoleColor(14);
        cout << "Du hast beim Sammeln 40 Schrott gefunden." << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else {
        setConsoleColor(12);
        cout << "Nicht genug Nahrung, um Schrott zu sammeln." << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void trainArmy(Player& player) {
    if (player.food >= 50 && player.population >= 5) {
        player.food -= 50;
        player.population -= 5;
        player.army += 10;
        setConsoleColor(14);
        cout << "Du hast 10 Scouts ausgebildet!" << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else {
        setConsoleColor(12);
        cout << "Nicht genug Nahrung oder Bevölkerung, um Scouts auszubilden." << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void conquerTerritory(Player& player) {
    int enemyStrength = rand() % 45 + 10;
    cout << "Es befinden sich " << enemyStrength << "Zombies im Gebiet." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (player.army > enemyStrength) {
        player.territories++;
        player.army -= enemyStrength / 2;
        setConsoleColor(10),
        cout << "Du hast das Gebiet erfolgreich von Zombies gereinigt! " << enemyStrength / 2 << " Scouts mussten ihre Hirne Opfern." << endl;
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else {
        setConsoleColor(12);
        cout << "Deine Scouts wurden von einer großen Horde Zombies gefressen. " << endl;
        player.territories--;
        player.army = max(0, player.army - enemyStrength);
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }
    // Win-Condition prüfen
    if (checkWinCondition(player)) {
        setConsoleColor(11);
        cout << "\n Du hast 9 Gebiete erobert und Somit erfolgreich die gesamte Stadt befreit!\n";
        setConsoleColor(15);
        std::this_thread::sleep_for(std::chrono::seconds(6));
        exit(0); // Beendet das Programm
    }
}

void displayImage1() {
    clearConsole();
    SetConsoleOutputCP(CP_UTF8);
    std::cout << R"(░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░▒▒▓▓▓▓▓▓▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░▒▒▓▒░░░░▓▓▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░▓▒▒▒░░▒▒▓█░▓█▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓██▓▓▒▒▒░░░░░░░░░░░░░░░░
░░░░░░░░░░▒▓▓▓▒▓▓▓▒▓█▒░░░▒▒▒▒▒▒▓▒▓█▓████████▓▓▓▒░░░░░░░░░░░░
░░░░░░░░▒▓▓▓▓▓▓▓▓▓▒▒░▒▒▒▒▒▒▓▓█▓██████████████▓██▓▓▒░░░░░░░░░
░░░░░░▒▓▓▓▒▒▒▒▒░░▒▒▒▒▒▒▒▓▓▓████████████████████▓▓▓█▓▒░░░░░░░
░░░░▒▒▓▓▒▒▒▒▒░▒▒▒▒▒▒▓▓█████▓▓█▓▓▓▓▓▓▓▓▓▓▓█████████▓▓█▒▒░░░░░
░░░░▓▓▒▒▒░▒▒▒▓▒▒▓▓███████▓▓▓▓▒▒▒▒▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓████▓█▒▒░░░░
░░░░▒▓░▓▒▒█▒▓██████████▓▓▒▒▒▒▒▒▓▒▓▓▒▓▓▓▓▓▓▓▓▓▓███▓███▓▓▓░░░░
░░░░▓▓▒▓█▓███████████▓▓▒▒░░▒▒▒▒▓▓▓▒▓▓█████████████████▓▓░░░░
░░░░▒█████████████▓▓▓▒▒▒▒▒▒▒▓▓▓▓██▓█████████████████▓█▒▓░░░░
░░░░░▒▓██████████▓▓▒▒░▒▒▒▓▓▓▓██████████████████████▓█▓█░░░░░
░░░░░░▒▓▓██████▓▓▒▒▒▒▒▒▓▓▓█████████████████████████▓▓██░░░░░
░░░░░░░██▒▓███▓▓▒▒▒▒▓▓▓██████████████████▓███████▓▓████░░░░░
░░░░░░░▓▓██▓▓▓██▓▓▓▓▓██████████████▓▓█████████▓▓▓█████▓░░░░░
░░░░░░░▒▓▓▓▓▓█▓▓▓▓▓██████████████████████▓▓▓▓███▓█████▓░░░░░
░░░░░░░░█▓▓▒░░▒▓███▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓▒▓▓█████▓▒░░░░
░░░░░░░░█▓▓▒░░▒▓▓▓▓█████████████████████▓▓▓▓▓▓▒▓▓█████▓▒░░░░
░░░░░░░░▓█▓▒░░▒▓▓▓▓▓▓▓▓███████████▓▓▓▓▓▓▓▓▓▓▓▓▒▓▓████▓▒▒░░░░
░░░░░░░░▒▓█▓▒░▒▒▓▓▓▓▓▓▓█▓█████████▓▓▓▓▓▓▓▓▓▓▓▓▒▓▓███▓▒▒░░░░░
░░░░░░░░░▒▓█▓░▒▒▓▓▓▓▓▓▓███████████▓▓▓▓▓▓▓▓▓▓▓▓▒▓▓██▓▒▒░░░░░░
░░░░░░░░░░░▒▓█▓▓▓▓▓▓▓▓▓███████████▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▒▒░░░░░░░░
░░░░░░░░░░░░░░▒▓█████▓▓███████████▓▓▓▓▓▓▓▓████▓▒▒░░░░░░░░░░░
░░░░░░░░░░░░░░░░░▒▒▓▓████████████████████▓▓▒▒░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▓▓▓▓▓▓▓▓▒▒▒░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
)";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    clearConsole();
}

void displayImage2() {
    clearConsole();
    SetConsoleOutputCP(CP_UTF8);
    std::cout << R"(████████████████████████████████████████████████████████████
██████████████████████████▓▓▒▒▒▒░░░░▒▒▓▓████████████████████
█████████████████████▓▒░░░░░░░░░░░░░░░░░░░▒▓████████████████
█████████████████▓▒░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▓████████████
██████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓██████████
████████████▓▒▓▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█████████
██████████▓▒▓█▓█▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓███████
█████████▓▓██▓▒▒▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓██████
████████▒███░░░░░░░▒▒░░░▒░░░▒░░░░░░░░░░░░░░░░░░░░░░░░░██████
███████▒▓█▒▓▓░░▒▓▓░░░░░░▓▓▒░░▒░░░▒▒▒▒▒▒░░░░▒░░░░▒░░▒░░██████
██████▒▓█░▒▓░░████▓░░░▓░▓█▓▒░░░░▒▒▒▒░░░░░░░░░▒▒▒░░▒▒▒░██████
████▓▓▒████▓▒█████░░░░█▒█▒▓▒░░░░░░▒▓▓▓▓▓░░░░▓░▒░░░░░░░▒█████
█████▓▒██████▓▓▓▒░▒░░░░░▓▓▒░░░▒▒▒▒▒▓█████░░░█░░░░███▒░░░▓███
██████▓█████▒░░▒▓█▓▒█▓░░▓▒░░░▒░░░░░░░▒███▒░░█░█▒▒░░░▒▓▓░░▓██
██████▒██████▓░███▓░░▒░░▓░░░▒░░▓▓▒▒░░░▓██▒░░▒░▒▓▒▓▓▓▒░█▒░▒██
██████▒▓▓▒▒▒░░░░▓▒░░░░░▓░▒░░▒░░▓▓▓▓░░░▓█▓░░▒░░░░▓█▓▓▒░▓░░███
█████▒░░▓▓▓▒▒░░▒▒░░░░▓░█░▒▒░▒▒░░░░░░░▒█▓░▒░░░██▓▓░▒░▒░░░████
████▓░▓█▓█▒▓█▓░░░░▓▓█▓▓▓▒▓░▒▒░▒▒▒▒▒▒▒░░▒▒░░░▓▒▓█▒▒░░░░░▒████
█████░▓█░▓░██▓▒░░░░▒▒▒▓▒░░░░░░░░░░░░░░░▒▒░░▓█▓░█░░░░░░░░▓███
██████▒▒▒░▒▒▒▒▒█░░▓▓▒▒▒░░░░░░░░░░░░░░░░▒▓░░██▓░█▓▒░▒▓▒░░████
████████▓▓▒░░░░▓▒▓▓░█▒░██▒▒▒▒▒▒▒▒▓▒░░▒░▓▒░░▓▓░▒▒▒▒░▓░▒▒█████
███████▓█▓█▓░░░▒▒▒█▒█▒░██▒░░▓▓██▒░░▓▒▒▒░░░░░░▓░░░▒▒██▓▒█████
███████▓█▒█▓▒▓▒▒▓▒█▒█▓░█▒▓░░████░░░░▒░░░░░░░░▓░▒░░▒██▓▓█████
█████████████▒▓▒▓██▓▒▓▒▓▒░░█████░▒░▒▒░░░░░░░░▒░▒░▒▒█████████
█████████████▓▒███▓▒▓▒▒░░░▓▒▒▓██▓░░▓░▒░░▓░▓░░▒░▓░▓▓█▓███████
██████████████▓██▒▒░█▓▒░░░█▒░░░▓█▒▒▒▒▓▒▒▒▒▓░░▓░▓▓███░███████
████████████████▒▓▒░░▒░░░▒▓▒▓░░░▓▓░░▒██▓████████████████████
████████████████▒█░▒░░░░░▒▒░▒▓░░▒▒░░▒██▓▒▓██████████████████
████████████████▓▒░▓▒░░░░░▓░░▒▒░░▓░░▒███▒░▒█████████████████
█████████████████░░▓█▓░░░░░▒▓░▒▒▒▒▒░▒▓███▓▒▒████████████████
████████████████▓░░█▓▓▒▒░░░░▒░▓░▒░░▓░▒░▓░▓▓█████████████████
████████████████▓░░███░▓▓▒░░░░░▒▓░▒▒▒▒▒▒▒▒██████████████████
████████████████▓░▒███▒░▒█▓░░░░░░░░░░░░░░▓██████████████████
)";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    clearConsole();
}


void displayImage3() {
    clearConsole();
    SetConsoleOutputCP(CP_UTF8);
    std::cout << R"(░░░░░░░░░░░░░░░░░░░░░▒▓▓▓███▓▓▓▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░▒███████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░█▓▒▒▒▒▒▒▓██░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░████████████▓▓▒░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░▓███████████▓▒▓███▓▓▓▒░░░░▒▒░░░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░▓█████████▓░░░▒▓██████▓▓▓███▒░░░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░░▓███████▓░░░░░░░░▒▓▓▓█████████▓▒░░░░░░
░░░░░░░░░░░░░░░░░░░░░░▓███████▒░░░░░░░░░░░░░▒██████████▓░░░░
░░░░░░░░░░░░░░░░░░░░░▒█████████░░░░░░░░░░░░░░░░▓█████████▓░░
░░░░░░░░░░░░░░░░░▒▓▓▓░███████████▓▓░░░░░░▒░░░▒▒▓███████████▒
░░░░░░░░░░░░░░░▒▓░░▓▒░▒██████▓▓▓▓▒▒▓░▓▒▓█▓▓██▓██████████████
░░░░░░░░░░▒░░▒▒▒▓▒▓█▓▓▓▓▓██▓░▒████▒▒▓██████████████████████░
░░░░░▒▓▒░▒▓████▓▓▓███▓█████▓████▒█▓▓████████████████████▓░░░
░░░▒█████▒▒▓███▓████▓▓▓█████████▓██████████████████▓▓▒░░░░░░
░▒█████████▓▒▓███████████████████████████████▓▓▒▒░░░░░░░░░░░
░▓▒▓██████████▒▒█████████████████████████▓▒░░░░░░░░░░░░░░░░░
░░░▒▓███████████▓▓██████████████████████▒░░░░░░░░░░░░░░░░░░░
░░▒█████████████████▓▓█████████████████▒░░░░░░░░░░░░░░░░░░░░
░▒██████████▓▒▓███████▒▓████████████▓▒▓▓▓▓░░░░░░░░░░░░░░░░░░
▒█▓▒▒▓▓█▓▒▒▓▓▒░░░░▒▓█████▓█████████████████░░░░░░░░░░░░░░░░░
██▓██▓██▓██████▓▓▓██▓▓████▓▒▓██████████████░░░░░░░░░░░░░░░░░
███████████████████████▓████▓▓█████████████░░░░░░░░░░░░░░░░░
▓█████████▓████████████████████████████████░░░░░░░░░░░░░░░░░
░░▒▒▓▓▒▒░░░███████████▓████████████████████░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░▓████████▓███████████████████▒░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░█████▓███████████████████▒░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░▓███████████████████████████░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░█████████████████████████████▒▒▓░░░░░░░░░░░░░░
░░░░░░░░░░░░░▓███████████████████████████▒▓█▓░░░░░░░░░░░░░░░
░░░░░░░░░░░░░████████████████████████████░░▒▓▒░░░░░░░░░░░░░░
░░░░░░░░░░░░░▒█████████████████████████▓░░░░░▒▓▓▒░░░░░░░░░░░
░░░░░░░░░░░░░▒██████████████████████████░░░░░░░▒▓▓░░░░░░░░░░
░░░░░░░░░░░░░▓██████████████████████████░░░░░░░░░░░░░░░░░░░░
)";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    clearConsole();
}

void displayImage4() {
    clearConsole();
    SetConsoleOutputCP(CP_UTF8);
    std::cout << R"(                                                            
                               .                            
                           +*. -.                           
                           +@@%*:                           
                           *@@@%-                           
                           *@@@@-                           
                           *@@@@=                           
                           *@@@@=                           
                           *@@@@=                           
             .:==+*+++*+***%@@@@%#####%%%%%##**-            
              -%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@=             
             -###%%@@@@@@@@@@@@@@@@@@@@%%@%##*=--.          
                   ....... *@@@@*                           
                           *@@@@*                           
                           *@@@@*                           
                           *@@@@*                           
                           +@@@@*                           
                           +@@@@#                           
                           +@@@@#                           
                           +@@@@#                           
                           =@@@@%                           
                           =@@@@%                           
                           -@@@@@                           
                           -@@@@@                           
                           :@@@@*                           
                           :@@@@*                           
                            %%@@*                           
                            + :#+                           
                            .                               
                                                            
)";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    clearConsole();
}


void displayImage5() {
    clearConsole();
    SetConsoleOutputCP(CP_UTF8);
    std::cout << R"()";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    clearConsole();
}


void randomEvent(Player& player) {
    int event = rand() % 4;

    switch (event) {
        clearConsole(); 
        SetConsoleOutputCP(CP_UTF8);
        
   case 0:
       for (int i = 0; i < 4; ++i) {
           displayImage2();

       }
       setConsoleColor(12);
        cout << "Eine Horde Zombies hat deine Stadt angegriffen, du hast 10 scouts verloren"
            ; player.army = max(0, player.army - 10);
        setConsoleColor(15);
        break;
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    case 1:
        //Flash Image
        for (int i = 0; i < 4; ++i) {
            displayImage3();

        }
        setConsoleColor(10);
            cout << "Deine Scouts fanden ein Gebiet in dem keine Zombies waren, Du gewinnst 1 Gebiet"
            ; player.territories  += 1;
        setConsoleColor(15);
        break;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    case 2:
        for (int i = 0; i < 4; ++i) {
            displayImage1();

        }
        setConsoleColor(10);
        cout << "Es wurde eine Garage voll mit Dosenfutter gefunden, das bringt + 30 Nahrung"
            ; player.food = max(0, player.food + 30);
        setConsoleColor(15);
        break;
        std::this_thread::sleep_for(std::chrono::seconds(4));
    case 3:
        for (int i = 0; i < 4; ++i) {
            displayImage4();

        }
        setConsoleColor(12);
        cout << "2 deiner Bewohner wurden krank und erlagen an ihren leiden"
            ; player.population -= 5;
        setConsoleColor(15);
        break;
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }
}

void nextTurn(Player& player) {
    player.food += 10 * player.territories;
    player.scraps += 5 * player.territories;
    player.population += 3;
    setConsoleColor(3);
    cout << "Die Runde ist vorbei. Deine Ressourcen wurden aktualisiert." << endl;
    setConsoleColor(15);
    randomEvent(player);
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


vector<string> asciiArt = {
    "                  :-==+****+==-:                  ",
    "             .=*%@@@@@@@@@@@@@@@@@#*=.            ",
    "           =%@@@@@@@@@@@@@@@@@@@@@@@@@@+:         ",
    "         =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#.       ",
    "        *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.      ",
    "       *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#      ",
    "       @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#      ",
    "       @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*      ",
    "       *@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.      ",
    "       .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*       ",
    "        %@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.       ",
    "      .%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#       ",
    "      %@@@@#-::::-=+*@@@@@@@@@@*+======+%@@%      ",
    "     .@@@+:           -@@@@@@#           =@@=     ",
    "     =@%.              #@@@@@+            =@%     ",
    "     *@@+             =@@@@%@*           :+@@     ",
    "     @@@@- .        :%@%*%@#@%=.     :-= +@@@.    ",
    "    =@@**%#*::.  .-#@@@:-@%@@#*%#%*###+*#@@@@-    ",
    "     #@@%=@@@@#%%@%@@@%#- . .#@#.+@@@@@@@@@@=     ",
    "      -#%@@@@@@++=::+@@.  =   *@= +@@@@@%%#-      ",
    "         ..  ..-**::+@*       =@%%**-.            ",
    "                 =#@@@@*:-#+:+@@@@*               ",
    "                 -=***@%*@@@@@@@=@-*              ",
    "               .+=#@+@@@%%@@@@@@*@@=              ",
    "                .-@@@@@@@@@@@@@@@*#.              "
};

// Funktion um ascii und loading zu displayen
void displayLoadingScreen() {
    clearConsole();

    // ascii art zentral
    int startX = 20; 
    int startY = 1;  
    for (size_t i = 0; i < asciiArt.size(); i++) {
        setCursorPosition(startX, startY + i);
        cout << asciiArt[i];
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


int main() {
    srand(time(0)); // Initialisiere Zufallsgenerator

    // Startet Background musik
    PlaySound(TEXT("Silent8-Bit.wav"), NULL, SND_FILENAME | SND_ASYNC);

    // Simulierter Loading screen bei start
    displayLoadingScreen();

    // Clear console and proceed mit den game
    clearConsole();

    while (true) {
        int choice = navigateMenu(); // Get user's selection

        if (choice == 0) {
            clearConsole();
            break; // Exit menü und startet das spiel
        }
        else if (choice == 1) {
            ReadMe();
        }
    }

    Player player;
    bool running = true;

    while (running) {
        clearConsole(); // Konsole bereinigen
        SetConsoleOutputCP(CP_UTF8);
        cout << R"( ▄▄·  ▄ .▄            .▄▄ · ▄▄▄ .     ▄▄·  ▄▄▄· ▄▄▄  ▄▄▄ .·▄▄▄▄• ▄▌▄▄▌  
▐█ ▌▪██▪▐█▪     ▪     ▐█ ▀. ▀▄.▀·    ▐█ ▌▪▐█ ▀█ ▀▄ █·▀▄.▀·▐▄▄·█▪██▌██•  
██ ▄▄██▀▐█ ▄█▀▄  ▄█▀▄ ▄▀▀▀█▄▐▀▀▪▄    ██ ▄▄▄█▀▀█ ▐▀▀▄ ▐▀▀▪▄██▪ █▌▐█▌██▪  
▐███▌██▌▐▀▐█▌.▐▌▐█▌.▐▌▐█▄▪▐█▐█▄▄▌    ▐███▌▐█ ▪▐▌▐█•█▌▐█▄▄▌██▌.▐█▄█▌▐█▌▐▌
·▀▀▀ ▀▀▀ · ▀█▄▀▪ ▀█▄▀▪ ▀▀▀▀  ▀▀▀     ·▀▀▀  ▀  ▀ .▀  ▀ ▀▀▀ ▀▀▀  ▀▀▀ .▀▀▀ 

)" << endl;
        displayStatus(player);

        cout << "Wähle eine Aktion:" << endl;
        cout << "1. Farm bauen (-25 Schrott, +50 Nahrung)" << endl;
        cout << "2. Schrott sammeln (-30 Nahrung, +40 Schrott)" << endl;
        cout << "3. Soldaten ausbilden (-50 Nahrung, -5 Bevölkerung, +10 Armee)" << endl;
        cout << "4. Stadt erkunden (benötigt Scouts)" << endl;
        cout << "5. Nächste Runde" << endl;
        cout << "6. Beenden" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            buildFarm(player);
            break;
        case 2:
            buildMine(player);
            break;
        case 3:
            trainArmy(player);
            break;
        case 4:
            conquerTerritory(player);
            break;
        case 5:
            nextTurn(player);
            break;
        case 6:
            running = false;
            cout << "Spiel beendet. Danke fürs Testen!" << endl;
            break;
        default:
            cout << "Ungültige Auswahl. Bitte versuche es erneut." << endl;
        }
    }

    return 0;
}
