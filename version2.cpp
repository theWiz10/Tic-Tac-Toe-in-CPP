#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;

class Player {
private:
    string name;
    char symbol;
public:
    Player(const string& n, char s) {
        name = n;
        symbol = s;
    }

    string getName() const {
        return name;
    }

    char getSymbol() const {
        return symbol;
    }
};

class Board {
private:
    char cells[3][3];
public:
    Board() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cells[i][j] = ' ';
            }
        }
    }

    void display() const {
        cout << "   1   2   3" << endl;
        cout << "1  " << cells[0][0] << " | " << cells[0][1] << " | " << cells[0][2] << endl;
        cout << "  ---+---+---" << endl;
        cout << "2  " << cells[1][0] << " | " << cells[1][1] << " | " << cells[1][2] << endl;
        cout << "  ---+---+---" << endl;
        cout << "3  " << cells[2][0] << " | " << cells[2][1] << " | " << cells[2][2] << endl;
    }

    bool isFull() const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (cells[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }

    bool placeSymbol(int row, int col, char symbol) {
        if (row < 0 || row > 2 || col < 0 || col > 2 || cells[row][col] != ' ') {
            return false;
        }
        cells[row][col] = symbol;
        return true;
    }

    bool checkWin(char symbol) const {
        for (int i = 0; i < 3; i++) {
            if (cells[i][0] == symbol && cells[i][1] == symbol && cells[i][2] == symbol) {
                return true;
            }
            if (cells[0][i] == symbol && cells[1][i] == symbol && cells[2][i] == symbol) {
                return true;
            }
        }
        if (cells[0][0] == symbol && cells[1][1] == symbol && cells[2][2] == symbol) {
            return true;
        }
        if (cells[0][2] == symbol && cells[1][1] == symbol && cells[2][0] == symbol) {
            return true;
        }
        return false;
    }
};

void saveWinnerRecord(const Player& player) {
    ofstream file("winners.txt", ios::app);
    if (file.is_open()) {
        file << player.getName() << endl;
        file.close();
        cout << "Winner record saved." << endl;
    } else {
        cout << "Failed to open the file." << endl;
    }
}

pair<int, int> parseMove(const string& move) {
    try {
        size_t commaPos = move.find(',');
        if (commaPos == string::npos) {
            throw invalid_argument("Invalid input format. Use row,column (e.g., 1,2).");
        }
        int row = stoi(move.substr(0, commaPos));
        int col = stoi(move.substr(commaPos + 1));
        return make_pair(row, col);
    } catch (const exception& e) {
        throw invalid_argument("Invalid input format. Use row,column (e.g., 1,2).");
    }
}

int main() {
    Board board;
    string player1Name, player2Name;
    cout << "Enter Player 1 name: ";
    getline(cin, player1Name);
    cout << "Enter Player 2 name: ";
    getline(cin, player2Name);

    Player player1(player1Name, 'X');
    Player player2(player2Name, 'O');
    Player* currentPlayer = &player1;

    cout << "Welcome to Tic Tac Toe!" << endl;
    board.display();

    while (!board.isFull()) {
        try {
            string move;
            cout << "It is " << currentPlayer->getName() << "'s turn. Enter row and column to place symbol: ";
            getline(cin, move);
            pair<int, int> coordinates = parseMove(move);

            if (!board.placeSymbol(coordinates.first - 1, coordinates.second - 1, currentPlayer->getSymbol())) {
                cout << "Invalid move, try again!" << endl;
                continue;
            }
            board.display();
            if (board.checkWin(currentPlayer->getSymbol())) {
                cout << "Player " << currentPlayer->getName() << " wins!" << endl;
                saveWinnerRecord(*currentPlayer);
                return 0;
            }
            currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    cout << "It's a tie!" << endl;
    return 0;
} 
