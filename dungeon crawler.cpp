// Dungeon crwaler von Vindows1
#include <iostream> // Fuer Ein- und Ausgabe
#include <string> // Fuer Zeichenketten
#include <fstream> // Fuer Dateizugriff
#include <exception> // Fuer std::exception
#include <sstream> // Fuer std::ostringstream
#include <algorithm> // Fuer std::transform
#include <vector> // Fuer Vektoren
#include <windows.h> // Fuer Farben (nur unter Windows)
#include <cstdlib> // Fuer rand()
#include <ctime> // Fuer srand()
#include <conio.h> // Fuer _getch()

// ANSI-Farbcodes
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"  
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define ORANGE  "\033[38;5;214m" // Orange
#define WHITE   "\033[37m"
#define BLACK   "\033[30m"
#define BOLD    "\033[1m"
#define BRIGHT_RED   "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BlUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN  "\033[96m"
#define BRIGHT_WHITE "\033[97m"
#define RESET   "\033[0m"

const int MAP_HEIGHT = 10; // Maximale Kartengroesse
const int MAP_WIDTH = 10; // Maximale Kartenbreite
const int maxPlayerCount = 1; // Maximale Anzahl an Spielern

// Struktur fuer Gegenstaende
struct Item {
    std::string name;
    std::string type; // "Weapon", "Armor", "Potion"
    int atkBonus; // Bonus auf Angriff
    int defBonus; // Bonus auf Verteidigung
    int hpHeal;   // Heilung (fuer Traenke)
	float critChance; // Kritische Trefferchance (fuer Artefakte)
    bool isEquipped; // Ob der Gegenstand ausgeruestet ist
};

// Struktur fuer Spielerstatistiken
struct Playerstats {
	int id; // Spieler-ID (kann verwendet werden, um mehrere Spieler zu unterscheiden)
	int currentLevelNumber; // Aktuelles Level des Spielers, beginnt bei 1
	int level; // Spielerlevel pro levle +1 atk +1 def +10 +10 hp
	int ep; // Aktuelle Erfahrungspunkte
	int maxEp; // Maximale Erfahrungspunkte bis zu naechstem Levelaufstieg
	std::string name; // Name des Spielers
	int hp; // Aktuelle HP
    int maxHp; // Maximale HP
	int atk; // Angriffswert
	int def; // Verteidigungswert
	float critChance; // Kritische Trefferchance
	std::vector<Item> inventory; // Inventar des Spielers
	int playerX; // Aktuelle X-Position des Spielers auf der Karte
	int playerY; // Aktuelle Y-Position des Spielers auf der Karte
};

// Struktur fuer Monsterstatistiken
struct Monsterstats {
	int id; // Monster-ID (kann verwendet werden, um mehrere Monster zu unterscheiden)
	int ep; // Erfahrungspunkte, die das Monster gibt
	int level; // Monsterlevel
	std::string name; // Name des Monsters
	int hp; // Aktuelle HP
	int atk; // Angriffswert
	int def; // Verteidigungswert
    float critChance;
};

// Funktionsdeklarationen
bool loadLevel(const std::string& filename, std::string map[MAP_HEIGHT][MAP_WIDTH]); // Laedt das Level aus einer Datei
bool loadMonsters(const std::string& filename, std::vector<Monsterstats>& monsterList, char separator = ','); // Laedt die Monster aus einer CSV-Datei
void setConsoleColor(int color); // Setzt die Konsolenfarbe (nur unter Windows)
void game(std::string gameMap[MAP_HEIGHT][MAP_WIDTH], Playerstats Player[], std::vector<Monsterstats>& monster); // Hauptspiel-Logik
void startmenu(Playerstats Player[], std::string gameMap[MAP_HEIGHT][MAP_WIDTH], std::vector<Monsterstats>& monster); // Startmenue des Spiels
void playerdraw(std::string gameMap[MAP_HEIGHT][MAP_WIDTH]); // Zeichnet den Spieler auf der Karte
void mapdraw(std::string gameMap[MAP_HEIGHT][MAP_WIDTH]); // Zeichnet die Karte
void nextlevel(std::string gameMap[MAP_HEIGHT][MAP_WIDTH], Playerstats Player[], std::vector<Monsterstats>& monster); // Wechselt zum naechsten Level
void playermove(std::string gameMap[MAP_HEIGHT][MAP_WIDTH], int& playerX, int& playerY, char direction, Playerstats Player[], std::vector<Monsterstats>& monster); // Bewegt den Spieler auf der Karte
void monsterkampf(Playerstats Player[], std::vector<Monsterstats>& monster); // Fuehrt einen Kampf gegen ein Monster durch
void Trap(Playerstats Player[], std::string gameMap[MAP_HEIGHT][MAP_WIDTH], int x, int y); // Behandelt das Betreten einer Falle
void showInventory(Playerstats Player[]); // Zeigt das Inventar des Spielers an
void generateItem(Playerstats Player[], std::string gameMap[MAP_HEIGHT][MAP_WIDTH], int x, int y); // Generiert einen Gegenstand in der Kiste
void displayPlayerStats(Playerstats Player[]);
void End_game(); // Beendet das Spiel

int monsteranzahl = 100; // Anzahl der Monster im Spiel
const int MAX_INVENTORY_SIZE = 10;

int main() {
    // Seed fuer Zufallszahlen
    srand(time(0));

	std::string gameMap[MAP_HEIGHT][MAP_WIDTH]; // Karte initialisieren
	std::vector<Monsterstats> monster; // Monsterliste initialisieren
    std::cout << "Lade Spieldaten...\n\n"; 

    //  Level aus "Level1.txt" laden
    std::cout << "Versuche, Level aus 'Level1.txt' zu laden...\n";
    if (loadLevel("Level1.txt", gameMap)) {
        std::cout << "-> Erfolg!\n";
    }
    else {
        std::cerr << "-> FEHLER beim Laden des Levels. Programm wird beendet.\n";
        return 1; // Fehlercode
    }

    // Monster aus "monsters.csv" laden
    std::cout << "Versuche, Monster aus 'monsters.csv' zu laden...\n";
    if (loadMonsters("monsters.csv", monster)) {
        std::cout << "-> Erfolg! " << monster.size() << " Monster geladen.\n";
    }
    else {
        std::cerr << "-> FEHLER beim Laden der Monster. Programm wird beendet.\n";
        return 1; // Fehlercode
    }
	Sleep(1000); // Kurze Pause fuer Spannung    
	system("cls"); // Bildschirm loeschen (nur unter Windows)
    // Spieler initialisieren
    Playerstats Player[1];
	Player[0].id = 0; // Spieler-ID
    Player[0].name = "Held*in";
	Player[0].hp = 100; // Start-HP
	Player[0].level = 1; // Startlevel
	Player[0].currentLevelNumber = 1; // Startlevel
	Player[0].ep = 0; // Start-EP
	Player[0].maxEp = 100; // Maximale EP
	Player[0].maxHp = 100; // Maximale HP
	Player[0].atk = 10; // Angriffswert
	Player[0].def = 3; // Verteidigungswert
	Player[0].critChance = 0.1f; // Kritische Trefferchance (10%)
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (gameMap[i][j] == "S") {// startposition des Spielers
                Player[0].playerX = i; // Startposition des Spielers (x-Koordinate)
                Player[0].playerY = j; // Startposition des Spielers (y-Koordinate)
            }
        }
    }

    // Startmenue anzeigen
    startmenu(Player, gameMap, monster);
    return 0;
}

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); 
}

void game(std::string gameMap[MAP_HEIGHT][MAP_WIDTH], Playerstats Player[], std::vector<Monsterstats>& monster) {
    // Karte anzeigen
	system("cls"); // Bildschirm loeschen (nur unter Windows)
    playerdraw(gameMap);
    char input;
    while (true) {
        std::cout << "Aktueller Raum: " << Player[0].currentLevelNumber << "\n";
        displayPlayerStats(Player);
        std::cout << "Bewege den Spieler (WASD zum Bewegen, I fuer Inventar, Q zum Beenden): ";
        input = _getch();

        if (input == 'q' || input == 'Q') {
            std::cout << "Spiel beendet. Danke fuers Spielen!\n";
			End_game(); // Beendet das Spiel
            break; // Beenden des Spiels
        }
        else if (input == 'i' || input == 'I') {
            system("cls");
            showInventory(Player);
            system("cls"); // Bildschirm loeschen (nur unter Windows)
            mapdraw(gameMap);
            continue;
        }
        std::cout << "\033[H";
		std::cout << "\033[J"; // Bildschirm loeschen (nur unter Windows)
        playermove(gameMap, Player[0].playerX, Player[0].playerY, input, Player, monster); // Spieler bewegen
        mapdraw(gameMap); // Karte neu zeichnen
    }
}

void startmenu(Playerstats Player[], std::string gameMap[MAP_HEIGHT][MAP_WIDTH], std::vector<Monsterstats>& monster) {
    std::cout << BRIGHT_RED << "Willkommen im Dungeon Crawler!                            " << RED << R"(              _          _)" << RESET << std::endl;
    std::cout << "Erkunde die Tiefen des Dungeons und finde den Schatz!     " << RED << R"(             _/|    _   |\_)" << RESET << std::endl;
    std::cout << "                                                          " << RED << R"(           _/_ |   _|\\ | _\)" << RESET << std::endl;
    std::cout << "---------------------------------------------------------" << RED << R"(         _/_/| /  /   \|\ |\_\_)" << RESET << std::endl;
    std::cout << "Steuerung:        |   Player: " << Player[0].name << "		         " << RED << R"(       _/_/  |/  /  _  \/\|  \_\_ )" << RESET << std::endl;
    std::cout << "W - Nach oben     |   HP: " << Player[0].hp << "/" << Player[0].maxHp << "                        " << RED << R"(     _/_/    ||  | | \o/ ||    \_\_)" << RESET << std::endl;
    std::cout << "A - Nach links    |   EP: " << Player[0].ep << "/" << Player[0].maxEp << "                          " << RED << R"(    /_/  | | |\  | \_ V  /| | |  \_\)" << RESET << std::endl;
    std::cout << "S - Nach unten    |   ATK: " << Player[0].atk << "                             " << RED << R"(   //    ||| | \_/   \__/ | |||    \\)" << RESET << std::endl;
    std::cout << "D - Nach rechts   |   DEF: " << Player[0].def << "                             " << RED << R"(  // __| ||\  \          /  /|| |__ \\)" << RESET << std::endl;
    std::cout << "I - Inventar      |   Crit:  " << Player[0].critChance << "                         " << RED << R"( //_/ \|||| \/\\        //\/ ||||/ \_\\)" << RESET << std::endl;
    std::cout << "Q - Beenden                                               " << RED << R"(///    \\\\/  /        \   \////    \\\)" << RESET << std::endl;
    std::cout << "                                                          " << RED << R"(|/      \/    |    |    |     \/      \|)" << RESET << std::endl;
    std::cout << "Druecke 'S', um zu starten, oder 'Q', um zu beenden.       " << RED << R"(             /_|  | |_  \)" << RESET << std::endl;
    std::cout << "Druecke 'i' fuer Inventory                                 " << RED << R"(             ///_| |_||\_ \)" << RESET << std::endl;
    std::cout << "                                                          " << RED << R"(             |//||/||\/||\| )" << RESET << std::endl;
    std::cout << "                                                          " << RED << R"(              / \/|||/||/\/)" << RESET << std::endl;
    std::cout << "                                                          " << RED << R"(                /|/\| \/)" << RESET << std::endl;
    std::cout << "                                                          " << RED << R"(                \/  | )" << RESET << std::endl;
    char choice;
    std::cin >> choice;

    if (choice == 'S' || choice == 's') {
        system("cls");
        game(gameMap, Player, monster); // Korrigierte Reihenfolge
    }
    else if (choice == 'I' || choice == 'i') {
        system("cls");
        showInventory(Player); // Inventar anzeigen
        system("cls"); // Bildschirm loeschen (nur unter Windows)
        mapdraw(gameMap); // Karte neu zeichnen
        startmenu(Player, gameMap, monster); // Zurueck zum Startmen
    }
    else if (choice == 'Q' || choice == 'q') {
        End_game(); // Beendet das Spiel
    }
    else {
        std::cout << "Ungueltige Eingabe. Bitte versuche es erneut.\n";
        startmenu(Player, gameMap, monster);
    }
}

void playerdraw(std::string gameMap[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (gameMap[i][j] == "S") {// startposition des Spielers
                gameMap[i][j] = "P"; // Spielerposition
            }
        }
	}
    mapdraw(gameMap);
}

void mapdraw(std::string gameMap[MAP_HEIGHT][MAP_WIDTH]) {
    char wand = static_cast<char>(219);

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            std::string feld = gameMap[i][j];
            std::transform(feld.begin(), feld.end(), feld.begin(), ::toupper);

            if (feld == "W") {
                if (j == 0 || j == 9 || i == 0 || i == 9) {
                    setConsoleColor(8); // Dunkelgrau fuer Randwaende
                    if (i == 0)
                        std::cout << wand << wand;
                    else
                        std::cout << wand << wand;
                }
                else {
                    setConsoleColor(15); // Weiss fuer innere Waende
                    std::cout << wand << wand;
                }
            }
            else if (feld == "E") {
                setConsoleColor(10); // Gruen fuer Eingang/Ausgang
                std::cout << "E ";
            }
            else if (feld == "T") {
                setConsoleColor(11); // Blau fuer Kiste
                std::cout << "T ";
            }
            else if (feld == "M") {
                setConsoleColor(12); // Rot fuer Monster
                std::cout << "M ";
            }
            else if (feld == "P") {
                setConsoleColor(14); // Gelb fuer den Spieler
                std::cout << "P ";
            }
            else if (feld == " ") {
                setConsoleColor(7); // Grau fuer leere Felder
                std::cout << ". ";
            }
        }
        setConsoleColor(7); // Farben zuruecksetzen
        std::cout << "\n";
    }
}

void nextlevel(std::string gameMap[MAP_HEIGHT][MAP_WIDTH], Playerstats Player[], std::vector<Monsterstats>& monster)
{
	// map switche
	std::cout << YELLOW << "Du hast den Ausgang gefunden! Du gehst zum naechsten Level...\n" << RESET;
	Sleep(2000); // Kurze Pause fuer Spannung
	system("cls"); // Bildschirm loeschen (nur unter Windows)
	// immer die naechste Karte laden
    Player[0].currentLevelNumber++; // Level erhoehen
    std::ostringstream filename;
    filename << "level" << Player[0].currentLevelNumber << ".txt";
    if (!loadLevel(filename.str(), gameMap)) {
        std::cout << BOLD << GREEN << "Glueckwunsch! Du hast alle Level abgeschlossen!" << RESET << std::endl;
        exit(0); // Oder zeige einen Sieg-Bildschirm
    }
	// Spielerposition zuruecksetzen
	game(gameMap, Player, monster); // Starte das Spiel mit der neuen Karte
}

void playermove(std::string gameMap[MAP_HEIGHT][MAP_WIDTH], int& playerX, int& playerY, char direction, Playerstats Player[], std::vector<Monsterstats>& monster) {
    int newX = playerX, newY = playerY;

    switch (tolower(direction)) {
    case 'w': newX--; break;
    case 's': newX++; break;
    case 'a': newY--; break;
    case 'd': newY++; break;
    default:
        std::cout << "Ungueltige Eingabe. Verwende W, A, S, D, I oder Q.\n";
        return;
    }

    // Pruefen, ob die neue Position innerhalb der Kartengrenzen liegt
    if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10) {

        // Inhalt des Zielfeldes speichern
        std::string targetTile = gameMap[newX][newY];

        // Gegen eine Wand laufen
        if (targetTile == "W") {
            std::cout << "Du kannst nicht durch Waende gehen!\n";
            return; // Bewegung abbrechen
        }

        // Spielerbewegung auf der Karte durchfuehren (visuelle Aktualisierung)
        gameMap[playerX][playerY] = " "; // Alte Position leeren
        playerX = newX;
        playerY = newY;
        gameMap[playerX][playerY] = "P"; // Spieler auf die neue Position setzen

        // Interaktion basierend auf dem urspruenglichen Inhalt des Feldes starten
        if (targetTile == "M") {
            mapdraw(gameMap); // Karte neu zeichnen, um die neue Spielerposition zu zeigen
            std::cout << BRIGHT_RED << "Du bist auf ein Monster gestossen! Du musst kaempfen.\n" << RESET;
            monsterkampf(Player, monster);
            // Pruefung, ob der Spieler den Kampf ueberlebt hat
            if (Player[0].hp <= 0) {
                // Die monsterkampf-Funktion setzt bei Niederlage die HP, beendet das Spiel aber nicht.
                // Das Spielende wird hier ausgeloest.
                std::cout << RED << "Du bist tot! Spiel beendet.\n" << RESET;
                End_game(); // Beendet das Spiel
                exit(0);
            }
        }
        else if (targetTile == "T") {
            std::cout << YELLOW << "Du hast eine Kiste gefunden!\n" << RESET;
            generateItem(Player, gameMap, newX, newY);
        }
        else if (targetTile == "E") {
            nextlevel(gameMap, Player, monster);
        }
        else if (targetTile == " ") {
            // Trap 
			int chance = rand() % 100; // Zufallszahl zwischen 0 und 99
			if (chance == rand() % 100) { // 1% Chance, in eine Trap zu treten
				Trap(Player, gameMap, newX, newY);
                gameMap[playerX][playerY] = "P"; // Spieler auf die neue Position setzen
			}
		}
		else {
			gameMap[newX][newY] = " "; // Leeres Feld
        }
    }
    else {
        std::cout << "Du kannst die Karte nicht verlassen!\n";
    }
}

void monsterkampf(Playerstats Player[], std::vector<Monsterstats>& monster) {
	// Monsterkampf-Logik
    //  Eine Liste aller Monster erstellen, die dem aktuellen Level entsprechen
    std::vector<Monsterstats> possibleMonsters;
    // vector leeren
	possibleMonsters.clear();
	// Fuege Monster hinzu, die dem aktuellen Level des Spielers entsprechen
    for (const auto& m : monster) {
        if (m.level == Player[0].currentLevelNumber|| m.level == Player[0].currentLevelNumber+1) {
            possibleMonsters.push_back(m);
        }
    }

    //  Pruefen, ob es ueberhaupt passende Monster gibt
    if (possibleMonsters.empty()) {
        std::cout << "Keine passenden Monster fuer dieses Level gefunden!" << std::endl;
        // Optional: ein zufaelliges Monster aus allen verfuegbaren nehmen
        if (monster.empty()) return;
        possibleMonsters = monster; // Nimm irgendeins, wenn keins passt
    }

    //  Ein zufaelliges Monster aus der Liste der passenden Monster auswaehlen
    int i = rand() % possibleMonsters.size();
    Monsterstats& currentMonster = possibleMonsters[i];

    std::cout << "---------------------------------------------------------\n";
    std::cout << YELLOW << "Du hast einen " << currentMonster.name << " Level " << currentMonster.level << " getroffen!Der Kampf beginnt...\n" << RESET;
    Sleep(1000); // Kurze Pause fuer Spannung

    int playerHp = Player[0].hp;
    int playerAtk = Player[0].atk;
    int playerDef = Player[0].def;

    int monsterHp = currentMonster.hp;
    int monsterAtk = currentMonster.atk;
    int monsterDef = currentMonster.def;

    while (playerHp > 0 && monsterHp > 0) {
        std::cout << BLUE << "Du greifst das Monster an!\n" << RESET;
        Sleep(1000); // Kurze Pause fuer Spannung
        // Kritischer Trefferchance
        int currentPlayerAtk = playerAtk; // Speichere den urspruenglichen Angriffswert
        if (static_cast<float>(rand()) / RAND_MAX < Player[0].critChance) {
            std::cout << BRIGHT_RED << "Kritischer Treffer!\n" << RESET;
            currentPlayerAtk *= 2; // Verdopple den Angriff bei kritischem Treffer
        }
        int damage = currentPlayerAtk - monsterDef;
        if (damage < 0) damage = 0;
        monsterHp -= damage;
		if (monsterHp < 0) monsterHp = 0; // Verhindere negative HP
        std::cout << "Du verursachst " << RED << damage << RESET << " Schaden. Monster HP: " << WHITE << monsterHp << RESET << "\n";
        Sleep(1000); // Kurze Pause fuer Spannung

        if (monsterHp <= 0) {
            std::cout << YELLOW << "Das Monster ist besiegt!\n" << RESET;
            break;
        }

        std::cout << ORANGE << "Das Monster greift dich an!\n" << RESET;
        Sleep(1000); // Kurze Pause fuer Spannung
		// Kritischer Trefferchance des Monsters
		int currentMonsterAtk = monsterAtk; // Speichere den urspruenglichen Angriffswert
        if (static_cast<float>(rand()) / RAND_MAX < currentMonster.critChance) {
            std::cout << BRIGHT_MAGENTA << "Das Monster trifft kritisch!\n" << RESET;
            currentMonsterAtk *= 2;
        }
		// Schberechnen
		damage = currentMonsterAtk - playerDef;
        if (damage < 0) damage = 0;
        playerHp -= damage;

        std::cout << "Das Monster verursacht " << RED << damage << RESET << " Schaden. Deine HP: " << GREEN << playerHp << RESET << "\n";
        Sleep(1000); // Kurze Pause fuer Spannung

        if (playerHp <= 0) {
            std::cout << RED << "Du bist besiegt! Game Over.\n" << RESET;
            Player[0].hp = 0; // Setze die HP des Spielers auf 0
            End_game(); // Beendet das Spiel
            return;
        }
    }

    Player[0].hp = playerHp; // Setze die HP des Spielers auf den verbleibenden Wert
    if (playerHp > 0) {
        std::cout << YELLOW << "Der Kampf ist vorbei. Du hast das Monster besiegt!\n" << RESET;
        Player[0].ep += currentMonster.ep;
        std::cout << "Du hast " << GREEN << currentMonster.ep << RESET << " EP erhalten. Aktuelle EP: " << Player[0].ep << "/" << Player[0].maxEp << "\n";
		// EP-Pruefung
        while(Player[0].ep >= Player[0].maxEp) {
            if (Player[0].ep >= Player[0].maxEp) {
                Player[0].level++;
                Player[0].atk += 1; // Erhoehe Angriffswert
                Player[0].def += 1; // Erhoehe Verteidigungswert
                Player[0].maxHp += 10; // Erhoehe maximale HP
                Player[0].hp = Player[0].maxHp; // Setze aktuelle HP auf maximale HP
                Player[0].ep = Player[0].ep - Player[0].maxEp; // Setze EP zurueck
				Player[0].maxEp += 50; // Erhoehe maximale EP fuer naechstes Level
                std::cout << "Du bist auf Level " << Player[0].level << " aufgestiegen! ATK: " << Player[0].atk << ", DEF: " << Player[0].def << ", Max HP: " << Player[0].maxHp << "\n";
            }
        }
		Sleep(1000); // Kurze Pause fuer Spannung
		system("cls"); // Bildschirm loeschen (nur unter Windows)
    }
}

void Trap(Playerstats Player[], std::string gameMap[MAP_HEIGHT][MAP_WIDTH], int x, int y)
{	// Spieler tritt in eine Falle
    const int Damage_low = 20;
    const int Damage_high = 50;

    if (Player[0].currentLevelNumber < 5) {
        // Nur in den ersten 4 Leveln
        std::cout << BRIGHT_RED << "Du bist in eine Falle getreten! Du verlierst 20 HP.\n" << RESET;
        Player[0].hp -= Damage_low; // Spieler verliert 20 HP
        Sleep(1000);
    }
	if (Player[0].currentLevelNumber >= 5) {
		// Ab Level 5 verliert der Spieler 50 HP
		std::cout << BRIGHT_RED << "Du bist in eine Falle getreten! Du verlierst 50 HP.\n" << RESET;
		Player[0].hp -= Damage_high; // Spieler verliert 50 HP
        Sleep(1000);

	}
	if (Player[0].hp <= 0) {
		std::cout << RED << "Du bist tot! Spiel beendet.\n" << RESET;
		Player[0].hp = 0; // Setze die HP des Spielers auf 0
        End_game(); // Beendet das Spiel
		exit(0); // Beende das Spiel
	}
	else {
		std::cout << "Deine aktuelle HP: " << GREEN << Player[0].hp << RESET << "\n";
		gameMap[x][y] = " "; // Leere das Feld, auf dem die Falle war
	}
}

void generateItem(Playerstats Player[], std::string gameMap[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    if (Player[0].inventory.size() >= MAX_INVENTORY_SIZE) {
        std::cout << "Dein Inventar ist voll! \n";
        std::cout << "Du kannst keine weiteren Gegenstaende aufnehmen.\n";
        std::cout << "Druecke eine Taste, um fortzufahren.\n";
        return;
    }

    Item newItem;
    int itemType = rand() % 4; // 0: Waffe, 1: Ruestung, 2: Heiltrank, 3: Artefakt
    int playerLevel = Player[0].currentLevelNumber;

    switch (itemType) {
    case 0: // Waffe
        newItem.name = "Schwert";
        newItem.type = "Weapon";
        if (playerLevel <= 3) newItem.atkBonus = rand() % 7 + 2; // 2-8
        else if (playerLevel <= 7) newItem.atkBonus = rand() % 6 + 10; // 10-15
        else newItem.atkBonus = rand() % 11 + 15; // 15-25
        newItem.defBonus = 0;
        newItem.hpHeal = 0;
        newItem.critChance = 0.0f;
        break;
    case 1: // Ruestung
        newItem.name = "Schild";
        newItem.type = "Armor";
        newItem.atkBonus = 0;
        if (playerLevel <= 3) newItem.defBonus = rand() % 7 + 2; // 2-8
        else if (playerLevel <= 7) newItem.defBonus = rand() % 6 + 10; // 10-15
        else newItem.defBonus = rand() % 11 + 15; // 15-25
        newItem.hpHeal = 0;
        newItem.critChance = 0.0f;
        break;
    case 2: // Heiltrank
        newItem.name = "Heiltrank";
        newItem.type = "Potion";
        newItem.atkBonus = 0;
        newItem.defBonus = 0;
        if (playerLevel <= 3) newItem.hpHeal = rand() % 11 + 20; // 20-30
        else if (playerLevel <= 7) newItem.hpHeal = rand() % 21 + 40; // 40-60
        else newItem.hpHeal = rand() % 21 + 80; // 80-100
        newItem.critChance = 0.0f;
        break;
    case 3: // Artefakt
        newItem.name = "Amulett";
        newItem.type = "Artefakt";
        newItem.atkBonus = 0;
        newItem.defBonus = 0;
        newItem.hpHeal = 0;
        newItem.critChance = static_cast<float>(rand() % 5 + 1) / 100.0f; // 1-5%
        break;
    }
    newItem.isEquipped = false;

    Player[0].inventory.push_back(newItem);
    std::cout << "Du hast einen " << newItem.name << " gefunden!\n";
}

void End_game()
{
	std::cout << BRIGHT_RED << "Spiel beendet. Danke fuers Spielen!\n" << RESET;
	std::cout << R"(
 ____              _           __ _   _            ____        _      _
|  _ \  __ _ _ __ | | _____   / _(_) (_)_ __ ___  / ___| _ __ (_) ___| | ___ _ __
| | | |/ _` | '_ \| |/ / _ \ | |_| | | | '__/ __| \___ \| '_ \| |/ _ \ |/ _ \ '_ \
| |_| | (_| | | | |   <  __/ |  _| |_| | |  \__ \  ___) | |_) | |  __/ |  __/ | | |
|____/ \__,_|_| |_|_|\_\___| |_|  \__,_|_|  |___/ |____/| .__/|_|\___|_|\___|_| |_|
														|_|
)";
	exit(0);   
}

void showInventory(Playerstats Player[]) {
    if (Player[0].inventory.empty()) {
        std::cout << "Dein Inventar ist leer.\n";
        std::cout << "Druecke eine Taste, um fortzufahren.\n";
        std::cin.ignore();
        std::cin.get();
        return;
    }

    while (true) {
        std::cout << "Inventar:\n";
        for (size_t i = 0; i < Player[0].inventory.size(); ++i) {
            Item& item = Player[0].inventory[i];
            std::cout << i + 1 << ": " << item.name << " (" << item.type << ")";
            if (item.isEquipped) std::cout << " [AUSGERUESTET]";
            std::cout << "\n";
            if (item.type == "Weapon") std::cout << "  ATK Bonus: " << item.atkBonus << "\n";
            else if (item.type == "Armor") std::cout << "  DEF Bonus: " << item.defBonus << "\n";
            else if (item.type == "Potion") std::cout << "  Heilung: " << item.hpHeal << "\n";
            else if (item.type == "Artefakt") std::cout << "  Kritische Trefferchance: " << item.critChance * 100 << "%\n";
        }

        std::cout << "Waehle einen Gegenstand zum Ausruesten/Verwenden (1-" << Player[0].inventory.size() << "), loeschen (11), oder 0 zum Verlassen: ";
        int choice;
        std::cin >> choice;

        if (choice == 0) break;

        if ((choice < 1 || choice > static_cast<int>(Player[0].inventory.size())) && choice != 11) {
            std::cout << "Ungueltige Auswahl.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        if (choice == 11) {
            std::cout << "Gegenstand loeschen...\n";
            std::cout << "Waehle einen Gegenstand zum loeschen (1-" << Player[0].inventory.size() << "): ";
            int deleteChoice;
            std::cin >> deleteChoice;
            if (deleteChoice < 1 || deleteChoice > static_cast<int>(Player[0].inventory.size())) {
                std::cout << "Ungueltige Auswahl.\n";
                std::cin.ignore();
                std::cin.get();
                continue;
            }
            Player[0].inventory.erase(Player[0].inventory.begin() + (deleteChoice - 1));
            std::cout << "Gegenstand geloescht.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        Item& item = Player[0].inventory[choice - 1];
        if (item.type == "Potion") {
            Player[0].hp = min(Player[0].hp + item.hpHeal, Player[0].maxHp);
            std::cout << "Du hast einen " << item.name << " verwendet. HP: " << Player[0].hp << "/" << Player[0].maxHp << "\n";
            Player[0].inventory.erase(Player[0].inventory.begin() + (choice - 1));
        }
        else {
            if (item.isEquipped) {
                item.isEquipped = false;
                if (item.type == "Weapon") Player[0].atk -= item.atkBonus;
                else if (item.type == "Armor") Player[0].def -= item.defBonus;
                else if (item.type == "Artefakt") Player[0].critChance -= item.critChance;
                std::cout << item.name << " wurde abgelegt.\n";
            }
            else {
                for (auto& other : Player[0].inventory) {
                    if (other.type == item.type && other.isEquipped) {
                        other.isEquipped = false;
                        if (other.type == "Weapon") Player[0].atk -= other.atkBonus;
                        else if (other.type == "Armor") Player[0].def -= other.defBonus;
                        else if (other.type == "Artefakt") Player[0].critChance -= other.critChance;
                    }
                }
                item.isEquipped = true;
                if (item.type == "Weapon") Player[0].atk += item.atkBonus;
                else if (item.type == "Armor") Player[0].def += item.defBonus;
                else if (item.type == "Artefakt") Player[0].critChance += item.critChance;
                std::cout << item.name << " wurde ausgeruestet.\n";
            }
        }

        std::cout << "Druecke eine Taste, um fortzufahren.\n";
        std::cin.ignore();
        std::cin.get();
    }
}

bool loadLevel(const std::string& filename, std::string map[MAP_HEIGHT][MAP_WIDTH]) {
    std::ifstream datei(filename);
    if (!datei.is_open()) {
        return false;
    }

    std::string zeile;
    int currentRow = 0;
    while (std::getline(datei, zeile) && currentRow < MAP_HEIGHT) {
        for (int col = 0; col < MAP_WIDTH && col < zeile.length(); ++col) {
            map[currentRow][col] = zeile[col];
        }
        currentRow++;
    }
    datei.close();
    return true;
}

bool loadMonsters(const std::string& filename, std::vector<Monsterstats>& monsterList, char separator) {
    std::ifstream datei(filename);
    if (!datei.is_open()) {
        std::cerr << "Fehler: Konnte die Monster-Datei nicht oeffnen: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::getline(datei, line); // Erste Zeile (Header) ueberspringen, um flexibel zu bleiben

    while (std::getline(datei, line)) {
        // Leere Zeilen ueberspringen
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string field;
        Monsterstats newMonster;

        try {
            // Spalte 1: id (int)
            std::getline(ss, field, separator);
            newMonster.id = std::stoi(field);

            // Spalte 2: ep (int)
            std::getline(ss, field, separator);
            newMonster.ep = std::stoi(field);

            // Spalte 3: level (int)
            std::getline(ss, field, separator);
            newMonster.level = std::stoi(field);

            // Spalte 4: name (string)
            std::getline(ss, field, separator);
            newMonster.name = field;

            // Spalte 5: hp (int)
            std::getline(ss, field, separator);
            newMonster.hp = std::stoi(field);

            // Spalte 6: atk (int)
            std::getline(ss, field, separator);
            newMonster.atk = std::stoi(field);

            // Spalte 7: def (int)
            std::getline(ss, field, separator);
            newMonster.def = std::stoi(field);

            // Spalte 8: critChance (float) - WICHTIG: std::stof verwenden!
            std::getline(ss, field, separator);
            newMonster.critChance = std::stof(field);

            // Das erfolgreich gelesene Monster zur Liste hinzufuegen
            monsterList.push_back(newMonster);
        }
        catch (const std::exception& e) {
            // Wenn beim Konvertieren einer Zeile ein Fehler auftritt (z.B. falsches Format),
            // wird eine Nachricht ausgegeben und diese Zeile uebersprungen.
            std::cerr << "Warnung: Ueberspringe fehlerhafte Zeile in " << filename << ": " << line << " (" << e.what() << ")" << std::endl;
            continue;
        }
    }
    datei.close();
    return true;
}

void displayPlayerStats(Playerstats Player[]) {
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";
    std::cout << "Level: " << Player[0].level << "| ";
    std::cout << "Spieler: " << Player[0].name << " | HP: " << Player[0].hp << "/" << Player[0].maxHp
        << " | EP: " << Player[0].ep << "/" << Player[0].maxEp
        << " | ATK: " << Player[0].atk << " | DEF: " << Player[0].def
        << " | Kritische Trefferchance: " << Player[0].critChance * 100 << "%\n";
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";
}