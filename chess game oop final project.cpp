#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <cctype>
using namespace std;

class Board; // forward declaration


// Base Piece Class

class Piece {
protected:
    int x, y;
    bool white;
    char symbol;
    bool hasMoved = false; 

public:
    Piece(int x, int y, bool white, char symbol)
        : x(x), y(y), white(white), symbol(symbol) {
    }
    virtual ~Piece() {}

    bool isWhite() const { return white; }
    char getSymbol() const { return symbol; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool moved() const { return hasMoved; }
    void setMoved(bool v) { hasMoved = v; }

    void moveTo(int nx, int ny) {
        x = nx; y = ny;
        hasMoved = true;
    }

    virtual bool isValidMove(Board& board, int nx, int ny) = 0;
    virtual string getName() const = 0;
};


// Board Class

class Board {
public:
    Piece* box[8][8];

    
    int lastDoublePawnX = -1, lastDoublePawnY = -1;
   
    int halfMoveClock = 0;
   
    int fullMove = 1;

    Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                box[i][j] = NULL;
    }

    void placePiece(Piece* p) {
        box[p->getX()][p->getY()] = p;
    }

    bool isEmpty(int x, int y) {
        return box[x][y] == NULL;
    }

    bool hasEnemy(int x, int y, bool white) {
        return !isEmpty(x, y) && box[x][y]->isWhite() != white;
    }

    void movePiece(int sx, int sy, int dx, int dy) {
        box[dx][dy] = box[sx][sy];
        box[sx][sy] = NULL;
        box[dx][dy]->moveTo(dx, dy);
    }

    void draw() {
#ifdef _WIN32
        system("cls"); 
#else
        system("clear");
#endif
        cout << "\n   a b c d e f g h\n";
        cout << "  -----------------\n";
        for (int i = 0; i < 8; i++) {
            cout << 8 - i << "| ";
            for (int j = 0; j < 8; j++) {
                if (box[i][j] == NULL)
                    cout << ". ";
                else
                    cout << box[i][j]->getSymbol() << " ";
            }
            cout << "|" << 8 - i << "\n";
        }
        cout << "  -----------------\n";
        cout << "   a b c d e f g h\n\n";
    }
};


// Derived Classes (Pieces)

class Pawn : public Piece {
public:
    Pawn(int x, int y, bool white)
        : Piece(x, y, white, white ? 'P' : 'p') {
    }

    bool isValidMove(Board& board, int nx, int ny) override {
        if (nx < 0 || nx > 7 || ny < 0 || ny > 7) return false;
        int dir = white ? -1 : 1;          
        int startRow = white ? 6 : 1;      

        
        if (ny == y && nx == x + dir && board.isEmpty(nx, ny)) {
            return true;
        }

        
        if (!moved() && x == startRow && ny == y && nx == x + 2 * dir) {
            if (board.isEmpty(x + dir, y) && board.isEmpty(nx, ny))
                return true;
        }

        
        if (nx == x + dir && (ny == y + 1 || ny == y - 1)) {
            if (board.hasEnemy(nx, ny, white))
                return true;

            
            if (board.lastDoublePawnX == x && board.lastDoublePawnY == ny) {
                Piece* enemy = board.box[board.lastDoublePawnX][board.lastDoublePawnY];
                if (enemy && enemy->getSymbol() == (white ? 'p' : 'P')) {
                    return true;
                }
            }
        }

        return false;
    }

    string getName() const override { return "Pawn"; }
};

class Rook : public Piece {
public:
    Rook(int x, int y, bool white)
        : Piece(x, y, white, white ? 'R' : 'r') {
    }

    bool isValidMove(Board& board, int nx, int ny) override {
        if (nx != x && ny != y) return false;

        int dx = (nx > x) ? 1 : (nx < x ? -1 : 0);
        int dy = (ny > y) ? 1 : (ny < y ? -1 : 0);

        int cx = x + dx, cy = y + dy;
        while (cx != nx || cy != ny) {
            if (!board.isEmpty(cx, cy)) return false;
            cx += dx; cy += dy;
        }

        return board.isEmpty(nx, ny) || board.hasEnemy(nx, ny, white);
    }

    string getName() const override { return "Rook"; }
};

class Knight : public Piece {
public:
    Knight(int x, int y, bool white)
        : Piece(x, y, white, white ? 'N' : 'n') {
    }

    bool isValidMove(Board& board, int nx, int ny) override {
        int dx = abs(nx - x), dy = abs(ny - y);
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
            return board.isEmpty(nx, ny) || board.hasEnemy(nx, ny, white);
        return false;
    }

    string getName() const override { return "Knight"; }
};

class Bishop : public Piece {
public:
    Bishop(int x, int y, bool white)
        : Piece(x, y, white, white ? 'B' : 'b') {
    }

    bool isValidMove(Board& board, int nx, int ny) override {
        if (abs(nx - x) != abs(ny - y)) return false;

        int dx = (nx > x) ? 1 : -1;
        int dy = (ny > y) ? 1 : -1;

        int cx = x + dx, cy = y + dy;
        while (cx != nx) {
            if (!board.isEmpty(cx, cy)) return false;
            cx += dx; cy += dy;
        }

        return board.isEmpty(nx, ny) || board.hasEnemy(nx, ny, white);
    }

    string getName() const override { return "Bishop"; }
};

class Queen : public Piece {
public:
    Queen(int x, int y, bool white)
        : Piece(x, y, white, white ? 'Q' : 'q') {
    }

    bool isValidMove(Board& board, int nx, int ny) override {
        Rook r(x, y, white);
        Bishop b(x, y, white);
        return r.isValidMove(board, nx, ny) || b.isValidMove(board, nx, ny);
    }

    string getName() const override { return "Queen"; }
};

class King : public Piece {
public:
    King(int x, int y, bool white)
        : Piece(x, y, white, white ? 'K' : 'k') {
    }

    bool isValidMove(Board& board, int nx, int ny) override {
        if (nx < 0 || nx > 7 || ny < 0 || ny > 7) return false;

        
        if (abs(nx - x) <= 1 && abs(ny - y) <= 1)
            return board.isEmpty(nx, ny) || board.hasEnemy(nx, ny, white);

        
        if (!moved() && nx == x) {
            
            if (ny == y + 2) {
                Piece* rook = board.box[x][7];
                if (rook && rook->getSymbol() == (white ? 'R' : 'r') && !rook->moved()) {
                    if (board.isEmpty(x, y + 1) && board.isEmpty(x, y + 2))
                        return true;
                }
            }
            
            if (ny == y - 2) {
                Piece* rook = board.box[x][0];
                if (rook && rook->getSymbol() == (white ? 'R' : 'r') && !rook->moved()) {
                    if (board.isEmpty(x, y - 1) && board.isEmpty(x, y - 2) && board.isEmpty(x, y - 3))
                        return true;
                }
            }
        }

        return false;
    }

    string getName() const override { return "King"; }
};


// Game Class (Check + Checkmate + Pawn Promotion + Castling + En Passant + Stalemate)

class Game {
private:
    Board board;
    bool whiteTurn = true;
    bool gameOver = false;
    string winner = "";

public:
    Game() {
        setupPieces();
    }

    void setupPieces() {
        
        for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) board.box[i][j] = NULL;

        for (int j = 0; j < 8; j++) {
            board.placePiece(new Pawn(6, j, true));
            board.placePiece(new Pawn(1, j, false));
        }

        board.placePiece(new Rook(7, 0, true));
        board.placePiece(new Rook(7, 7, true));
        board.placePiece(new Rook(0, 0, false));
        board.placePiece(new Rook(0, 7, false));

        board.placePiece(new Knight(7, 1, true));
        board.placePiece(new Knight(7, 6, true));
        board.placePiece(new Knight(0, 1, false));
        board.placePiece(new Knight(0, 6, false));

        board.placePiece(new Bishop(7, 2, true));
        board.placePiece(new Bishop(7, 5, true));
        board.placePiece(new Bishop(0, 2, false));
        board.placePiece(new Bishop(0, 5, false));

        board.placePiece(new Queen(7, 3, true));
        board.placePiece(new Queen(0, 3, false));

        board.placePiece(new King(7, 4, true));
        board.placePiece(new King(0, 4, false));

        
        board.lastDoublePawnX = -1; board.lastDoublePawnY = -1;
        board.halfMoveClock = 0; board.fullMove = 1;
    }

    bool inputToPos(string move, int& x, int& y) {
        if (move.size() != 2) return false;
        y = move[0] - 'a';
        x = 8 - (move[1] - '0');
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    Piece* findKing(bool white) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (board.box[i][j] &&
                    board.box[i][j]->getSymbol() == (white ? 'K' : 'k'))
                    return board.box[i][j];
        return nullptr;
    }

    bool isKingInCheck(bool white) {
        Piece* king = findKing(white);
        if (!king) return false; 

        int kx = king->getX(), ky = king->getY();

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (board.box[i][j] && board.box[i][j]->isWhite() != white)
                    if (board.box[i][j]->isValidMove(board, kx, ky))
                        return true;

        return false;
    }

    
    bool sideHasLegalMove(bool white) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece* p = board.box[i][j];
                if (!p || p->isWhite() != white) continue;

                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (!p->isValidMove(board, x, y)) continue;

                        
                        Piece* target = board.box[x][y];
                        int oldx = p->getX(), oldy = p->getY();

                       
                        bool enPassantCapture = false;
                        Piece* enPassantCaptured = NULL;
                        if (dynamic_cast<Pawn*>(p) && oldx == (white ? 3 : 4) && x == oldx + (white ? -1 : 1) && y != oldy && target == NULL) {
                            
                            if (board.lastDoublePawnX == oldx && board.lastDoublePawnY == y) {
                                enPassantCapture = true;
                                enPassantCaptured = board.box[board.lastDoublePawnX][board.lastDoublePawnY];
                            }
                        }

                        board.box[x][y] = p;
                        board.box[oldx][oldy] = NULL;
                        p->moveTo(x, y);
                        if (enPassantCapture) board.box[board.lastDoublePawnX][board.lastDoublePawnY] = NULL;

                        bool kingInCheck = isKingInCheck(white);

                        
                        p->moveTo(oldx, oldy);
                        board.box[oldx][oldy] = p;
                        board.box[x][y] = target;
                        if (enPassantCapture) board.box[board.lastDoublePawnX][board.lastDoublePawnY] = enPassantCaptured;

                        if (!kingInCheck) return true;
                    }
                }
            }
        }
        return false;
    }

    bool isCheckmate(bool white) {
        if (!isKingInCheck(white)) return false;
        return !sideHasLegalMove(white);
    }

    bool isStalemate(bool white) {
        if (isKingInCheck(white)) return false;
        return !sideHasLegalMove(white);
    }

    void promotePawn(int x, int y) {
        Piece* pawn = board.box[x][y];
        if (!pawn) return;
        bool isWhite = pawn->isWhite();

        delete pawn;
        board.box[x][y] = NULL;

        char choice;
        cout << "\n*** PAWN PROMOTION! ***\n";
        cout << "Choose a piece to promote to:\n";
        cout << "Q - Queen\n";
        cout << "R - Rook\n";
        cout << "B - Bishop\n";
        cout << "N - Knight\n";
        cout << "Enter your choice: ";
        cin >> choice;

        Piece* newPiece = nullptr;
        switch (toupper(choice)) {
        case 'Q':
            newPiece = new Queen(x, y, isWhite);
            break;
        case 'R':
            newPiece = new Rook(x, y, isWhite);
            break;
        case 'B':
            newPiece = new Bishop(x, y, isWhite);
            break;
        case 'N':
            newPiece = new Knight(x, y, isWhite);
            break;
        default:
            cout << "Invalid choice. Promoting to Queen by default.\n";
            newPiece = new Queen(x, y, isWhite);
        }

        board.placePiece(newPiece);
        cout << "Pawn promoted to " << newPiece->getName() << "!\n";
    }

    bool isPawnPromotion(Piece* p, int dx) {
        if (dynamic_cast<Pawn*>(p)) {
            return (p->isWhite() && dx == 0) || (!p->isWhite() && dx == 7);
        }
        return false;
    }

    void performCastlingIfAny(Piece* p, int sx, int sy, int dx, int dy) {
        King* k = dynamic_cast<King*>(p);
        if (!k) return;
        if (abs(dy - sy) == 2) {
         
            if (dy == 6) { 
                Piece* rook = board.box[sx][7];
                if (rook) {
                    board.movePiece(sx, 7, sx, 5);
                }
            }
            else if (dy == 2) {
                Piece* rook = board.box[sx][0];
                if (rook) {
                    board.movePiece(sx, 0, sx, 3);
                }
            }
        }
    }

    void start() {
        string from, to;
        gameOver = false;
        winner = "";

        while (!gameOver) {
            board.draw();

            if (!winner.empty()) {
                displayWinner();
                return;
            }

            cout << (whiteTurn ? "White" : "Black") << "'s turn\n";
            cout << "Enter move (e.g., e2 e4): ";
            cin >> from >> to;

            int sx, sy, dx, dy;

            if (!inputToPos(from, sx, sy) || !inputToPos(to, dx, dy)) {
                cout << "Invalid move format.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

            Piece* p = board.box[sx][sy];

            if (!p) {
                cout << "No piece at source.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

            if (p->isWhite() != whiteTurn) {
                cout << "Not your turn.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

            if (!p->isValidMove(board, dx, dy)) {
                cout << "Illegal move for that piece.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

           
            if (board.box[dx][dy] && board.box[dx][dy]->isWhite() == p->isWhite()) {
                cout << "Cannot capture your own piece.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

            
            Piece* target = board.box[dx][dy];
            int oldx = p->getX(), oldy = p->getY();

            
            bool enPassantCapture = false;
            Piece* enPassantCaptured = NULL;
            if (dynamic_cast<Pawn*>(p) && oldy != dy && target == NULL) {
               
                if (board.lastDoublePawnX == oldx && board.lastDoublePawnY == dy) {
                    enPassantCapture = true;
                    enPassantCaptured = board.box[board.lastDoublePawnX][board.lastDoublePawnY];
                }
            }

            board.box[dx][dy] = p;
            board.box[oldx][oldy] = NULL;
            p->moveTo(dx, dy);
            if (enPassantCapture) board.box[board.lastDoublePawnX][board.lastDoublePawnY] = NULL;

            bool leavesKingInCheck = isKingInCheck(whiteTurn);

           
            p->moveTo(oldx, oldy);
            board.box[oldx][oldy] = p;
            board.box[dx][dy] = target;
            if (enPassantCapture) board.box[board.lastDoublePawnX][board.lastDoublePawnY] = enPassantCaptured;

            if (leavesKingInCheck) {
                cout << "Illegal: That move would leave your king in check.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
            }

           
            bool wasPawnMove = (dynamic_cast<Pawn*>(p) != NULL);
            if (wasPawnMove || target) board.halfMoveClock = 0; else board.halfMoveClock++;

            
            int prevLastDoubleX = board.lastDoublePawnX, prevLastDoubleY = board.lastDoublePawnY;
            board.lastDoublePawnX = -1; board.lastDoublePawnY = -1; 

            
            if (enPassantCapture) {
                
                if (prevLastDoubleX >= 0 && prevLastDoubleY >= 0) {
                    delete board.box[prevLastDoubleX][prevLastDoubleY];
                    board.box[prevLastDoubleX][prevLastDoubleY] = NULL;
                }
            }

            
            if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
                if (abs(dx - sx) == 2) {
                    
                    board.lastDoublePawnX = dx;
                    board.lastDoublePawnY = dy;
                }
            }

            
            board.movePiece(sx, sy, dx, dy);

            
            performCastlingIfAny(board.box[dx][dy], sx, sy, dx, dy);

            
            if (target) delete target;

            
            if (isPawnPromotion(board.box[dx][dy], dx)) {
                promotePawn(dx, dy);
            }

            
            if (!whiteTurn) board.fullMove++;

            
            bool opponent = !whiteTurn;
            if (isKingInCheck(opponent)) {
                board.draw();
                cout << (opponent ? "White" : "Black") << " is in CHECK!\n";
                if (isCheckmate(opponent)) {
                    board.draw();
                    cout << (opponent ? "White" : "Black") << " is CHECKMATED!\n";
                    winner = whiteTurn ? "WHITE" : "BLACK";
                    gameOver = true;
                    continue;
                }
            }
            else {
                if (isStalemate(opponent)) {
                    board.draw();
                    cout << "STALEMATE! The game is a draw.\n";
                    winner = "DRAW (STALEMATE)";
                    gameOver = true;
                    continue;
                }
            }

            whiteTurn = !whiteTurn;
        }

        if (!winner.empty()) {
            displayWinner();
        }
    }

    void displayWinner() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << "=================================\n";
        cout << "          GAME OVER!\n";
        cout << "=================================\n\n";

        if (!winner.empty()) {
            if (winner.find("DRAW") != string::npos) {
                cout << "Result: " << winner << "\n\n";
            }
            else {
                cout << "🎉 CONGRATULATIONS! 🎉\n\n";
                cout << "🏆 " << winner << " WINS THE GAME! 🏆\n\n";
            }
        }

        cout << "Thank you for playing!\n\n";
        cout << "Press ENTER to return to main menu...";
        cin.ignore();
        cin.get();
    }
};


// MAIN MENU

void showInfo() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    cout << "=========================\n";
    cout << "       GAME INFO\n";
    cout << "=========================\n\n";
    cout << "• This is a 2-player Chess Game.\n";
    cout << "• White moves first.\n";
    cout << "• Moves are entered like 'e2 e4'.\n";
    cout << "• Game includes:\n";
    cout << "  ✔ Valid piece movements\n";
    cout << "  ✔ Check detection\n";
    cout << "  ✔ Checkmate detection\n";
    cout << "  ✔ Pawn promotion\n";
    cout << "  ✔ Castling (king/queen side)\n";
    cout << "  ✔ En passant\n";
    cout << "  ✔ Stalemate detection\n";
    cout << "• Note: 50-move rule and threefold repetition are not auto-enforced.\n\n";
    cout << "Press any key to return to main menu...";
    cin.ignore();
    cin.get();
}

void showMenu() {
    while (true) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        cout << "==============================\n";
        cout << "        CHESS GAME\n";
        cout << "==============================\n\n";
        cout << "Press ENTER to Start Game\n";
        cout << "Press I for Info\n";
        cout << "Press Q to Quit\n\n";

        string choice;
        getline(cin, choice);

        if (choice == "") {
            Game game;
            game.start();
        }
        else if (choice == "i" || choice == "I") {
            showInfo();
        }
        else if (choice == "q" || choice == "Q") {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            cout << "Exiting game...\n";
            exit(0);
        }
        else {
            cout << "Invalid input. Try again...\n";
        }
    }
}

int main() {
    showMenu();
    return 0;
}
