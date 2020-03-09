#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <cstring>
#include <ctype.h>
#include <cmath>
#include <conio.h>

#define white 0
#define black 1

using namespace std;

void SetColor(int ForgC)    // Changes text color
{
 WORD wColor;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                 //Mask out all but the background attribute, and add in the forgournd color
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
 }
 return;
}

void ClearConsoleToColors(int ForgC, int BackC) // Changes background color
{
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
               //Get the handle to the current output buffer...
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
                     //This is used to reset the carat/cursor to the top left.
 COORD coord = {0, 0};
                  //A return value... indicating how many chars were written
                    //   not used but we need to capture this since it will be
                      //   written anyway (passing NULL causes an access violation).
  DWORD count;

                               //This is a structure containing all of the console info
                      // it is used here to find the size of the console.
 CONSOLE_SCREEN_BUFFER_INFO csbi;
                 //Here we will set the current color
 SetConsoleTextAttribute(hStdOut, wColor);
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                          //This fills the buffer with a given character (in this case 32=space).
      FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

      FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
                          //This will set our cursor position for the next print statement.
      SetConsoleCursorPosition(hStdOut, coord);
 }
 return;
}

void textColor (int color) // 0 for white, 1 for black (text color)
{
    if (color == 0)
        SetColor(15);

    else if (color == 1)
        SetColor(0);
}

void displayHelp () // The game's help menu
{
    int selection = 0, pieceSel = 0;
    char temp[2];

    while (selection != 10)
    {
        cout << "\t\t\tWelcome to the game help menu!\n"
             << "\n1. Entering commands"
             << "\n2. The pieces"
             << "\n3. Objective of the game"
             << "\n4. Ending a game"
             << "\n10. Back to game"
             << "\n\nPlease make a selection : ";

        cin >> selection;

        system("cls");

        switch (selection)
        {
            case 1 : cout << "While playing the game, you can use the Algebraic Notation to make a move.\n\n"
                          << "For example, say your pawn is in row 7 and column b\n"
                          << "and you want to move it to row 8 without changing the column.\n\n"
                          << "You will type \"b7 b8\" without the quotes and press enter."
                          << "\n\nPress any key to continue.";

                     getch();
                     break;

            case 2 : while (pieceSel != 10)
                     {
                         cout << "1. Pawn\n2. Rook\n3. Bishop\n4. Knight\n5. King\n6. Queen\n10. Back to main help\n"
                              << "\nPlease make a selection : ";
                         cin >> pieceSel;

                         system("cls");

                         switch (pieceSel)
                         {
                             case 1 : cout << "The pawn is denoted by a P.\n\n"
                                           << "It can move one step forward as long as the path is clear.\n\n"
                                           << "If an enemy is present diagonally from it (one step forward\nand"
                                           << " one step left or right), the pawn can capture that piece.\n\n"
                                           << "If a pawn has not been moved yet, it can also move two steps forward.";
                                      break;

                             case 2 : cout << "The rook is denoted by an R.\n\n"
                                           << "It can move horizontally and vertically as long as the path is clear.";
                                      break;

                             case 3 : cout << "The bishop is denoted by a B.\n\n"
                                           << "It can move diagonally as long as the path is clear.";
                                      break;

                             case 4 : cout << "The knight is denoted by an H.\n\n"
                                           << "It has the trickiest movement of all the pieces.\n\n"
                                           << "It moves two steps horizontally, then one step vertically or vice-versa.\n\n"
                                           << "For a knight, the path it takes need not be clear.";
                                      break;

                             case 5 : cout << "The king is denoted by a K.\n\n"
                                           << "It can move one step in any direction.";
                                      break;

                             case 6 : cout << "The queen is denoted by a Q.\n\n"
                                           << "It combines the movements of a bishop and a rook.\n\n"
                                           << "It can move horizontally, vertically and diagonally.";
                                      break;
                         }

                         cout << "\n\nPress any key to continue.";
                         getch();
                         system("cls");
                     }
                     break;


            case 3 : cout << "The primary objective of the game is to capture the enemy's king.\n\n"
                          << "Along the way, you will want to capture the enemy's other pieces,\n"
                          << "to make it easier for you to win."
                          << "\n\nPress any key to continue.";

                     getch();
                     break;

            case 4 : cout << "The game ends when the enemy king is threatened (known as a check)\n"
                          << "and the enemy cannot bring it out of danger by any move.\nThis is known as a checkmate.\n\n"
                          << "The game also ends if the enemy king is not in danger,\nbut any move the enemy makes"
                          << "will bring his king into danger.\nThis is known as a stalemate, or a draw."
                          << "\n\nPress any key to continue.";

                     getch();
                     break;
        }

        system("cls");
    }

    cin.getline(temp, 1); //This reads the extra \n character left over in the input stream
}

bool isSameCase (char a, char b) // Checks whether a and b have the same case
{
    if (isupper(a) and isupper(b))
        return true;

    if (islower(a) and islower(b))
        return true;

    else
        return false;
}

bool isValidInput (char* input) // Checks whether the move input was of the correct format like "a2 a3"
{
    if (!isalpha(input[0]) or !isalpha(input[3]) or !isdigit(input[1]) or !isdigit(input[4]) or input[2] != ' ')
        return false;

    input[0] = tolower(input[0]); // To convert commands like "A2 A3" to "a2 a3"
    input[3] = tolower(input[3]);

    return true;
}

class chessboard        // Everything related to the chessboard like piece positions, display, whose turn is it etc.
{
    private:

        char board[8][9];   // Initialized in the constructor, stores the location of all pieces
        bool turn;          // Stores whose turn it is
        bool canCastleWhite, canCastleBlack; // Stores whether the players can castle

    public:

        chessboard();
        chessboard(char[8][9], int, bool, bool);
        void displayBoard();
        void movePiece(int, int, int, int);
        void changeTurn();
        int checkTurn();
        int isMoveValid(int, int, int, int);
        bool checkPieceMove(int, int, int, int, char);
        bool isThreatened(int, int, int);
};

chessboard::chessboard() //caps is black, lower is white, the default configuration
{
    char tempBoard[8][9] = { "rhbkqbhr",   //Starting layout, 0's stand for empty places
                             "pppppppp",
                             "00000000",
                             "00000000",
                             "00000000",
                             "00000000",
                             "PPPPPPPP",
                             "RHBKQBHR" };

    for (int i = 0; i < 8; ++i)
    {
        strcpy(board[i], tempBoard[i]);
    }

    turn = white;
    canCastleBlack = true;
    canCastleWhite = true;
}

//To start a game with custom options
chessboard::chessboard(char tempBoard[8][9], int curTurn, bool blackCastleStatus, bool whiteCastleStatus)
{
    for (int i = 0; i < 8; ++i)
    {
        strcpy(board[i], tempBoard[i]);
    }

    turn = curTurn;
    canCastleBlack = blackCastleStatus;
    canCastleWhite = whiteCastleStatus;
}

void chessboard::displayBoard() //Displays the board with the Algebraic notation table
{
    textColor(black);

    cout << "   a b c d e f g h\n\n";

    for (int i = 0; i < 8; ++i)
    {
        textColor(black);
        cout << i + 1 << "  ";

        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j] == '0') // if empty space
            {
                if ((i + j) % 2 == 0) // if i + j is even
                    textColor(white);

                else
                    textColor(black);

                cout << '_';
            }

            else
            {
                if (islower(board[i][j]))
                    textColor(white);

                else
                    textColor(black);

                cout << (char)toupper(board[i][j]);
            }

            cout << ' ';

        }

        cout << '\n';
    }
}

void chessboard::movePiece(int fx, int fy, int lx, int ly) // Moves a piece from (fx, fy) to (lx, ly)
{
    board[lx][ly] = board[fx][fy];
    board[fx][fy] = '0';
}

void chessboard::changeTurn() // Changes the current turn
{
    turn = 1 - turn;
}

int chessboard::checkTurn() // Returns whose turn it is
{
    return turn;
}

int chessboard::isMoveValid(int fx, int fy, int lx, int ly) // unfinished
{
    if (board[fx][fy] == '0')
        return 6;

    if (isupper(board[fx][fy]) and turn != black)
        return 1;

    if (islower(board[fx][fy]) and turn != white)
        return 1;

    if (lx > 7 or lx < 0 or ly > 7 or ly < 0) // Out of boundaries
        return 2;

    if (fx == lx and fy == ly) // No move
        return 3;

    if (isSameCase(board[fx][fy], board[lx][ly])) // can't capture own piece
        return 4;

    // Add condition here - can't expose king to threat unless no other option

    if (!checkPieceMove(fx, fy, lx, ly, board[fx][fy])) // Checks whether the piece rules are followed
        return 5;




    return 0;
}

// First, it checks whether the piece can move in the way described according to the piece rules.
// Second, it checks whether the path the piece moves is clear
bool chessboard::checkPieceMove(int fx, int fy, int lx, int ly, char piece)
{
    if (piece == 'p') // White Pawn
    {
        if (lx == fx + 2 and ly == fy and fx == 1 and board[lx][ly] == '0' and board[lx-1][ly] == '0') // Unmoved, moves down two spaces
            return true;

        if (lx == fx + 1 and ly == fy and board[lx][ly] == '0') // One space down
            return true;

        if (lx == fx + 1 and (ly == fy + 1 or ly == fy - 1) and isupper(board[lx][ly])) // Capture
            return true;

        else
            return false;
    }

    if (piece == 'P') // Black Pawn
    {
        if (lx == fx - 2 and ly == fy and fx == 6 and board[lx][ly] == '0' and board[lx+1][ly] == '0') // Unmoved, moves up two spaces
            return true;

        if (lx == fx - 1 and ly == fy and board[lx][ly] == '0') // One space up
            return true;

        if (lx == fx - 1 and (ly == fy + 1 or ly == fy - 1) and islower(board[lx][ly])) // Capture
            return true;

        else
            return false;
    }

    if (piece == 'r' or piece == 'R' or piece == 'q' or piece == 'Q') // Rook or queen
    {
        if (fx == lx and fy != ly)
        {
            if (fy < ly)
            {
                for (int i = fy + 1; i < ly; ++i)
                {
                    if (board[fx][i] != '0')
                        return false;
                }
            }

            else
            {
                for (int i = fy - 1; i > ly; --i)
                {
                    if (board[fx][i] != '0')
                        return false;
                }
            }

            return true;
        }

        else if (fy == ly and fx != lx)
        {
            if (fx < lx)
            {
                for (int i = fx + 1; i < lx; ++i)
                {
                    if (board[i][fy] != '0')
                        return false;
                }
            }

            else
            {
                for (int i = fx - 1; i > lx; --i)
                {
                    if (board[i][fy] != '0')
                        return false;
                }
            }

            return true;
        }

    }

    if (piece == 'b' or piece == 'B' or piece == 'q' or piece == 'Q') // Bishop or queen
    {
        if (fabs(fx - lx) == fabs(fy - ly))
        {
            if (fx < lx and fy < ly)
            {
                for (int i = fx + 1, j = fy + 1; i < lx and j < ly; ++i, ++j)
                {
                    if (board[i][j] != '0')
                        return false;
                }
            }

            else if (fx < lx and fy > ly)
            {
                for (int i = fx + 1, j = fy - 1; i < lx and j > ly; ++i, --j)
                {
                    if (board[i][j] != '0')
                        return false;
                }
            }

            else if (fx > lx and fy < ly)
            {
                for (int i = fx - 1, j = fy + 1; i > lx and j < ly; --i, ++j)
                {
                    if (board[i][j] != '0')
                        return false;
                }
            }

            else
            {
                for (int i = fx - 1, j = fy - 1; i > lx and j > ly; --i, --j)
                {
                    if (board[i][j] != '0')
                        return false;
                }
            }

            return true;
        }
    }

    if (piece == 'k' or piece == 'K') // King
    {
        for (int i = fx - 1; i <= fx + 1; ++i)
        {
            for (int j = fy - 1; j <= fy + 1; ++j)
            {
                if (lx == i and ly == j)
                {
                    return true;
                }
            }
        }
    }

    if (piece == 'h' or piece == 'H') // Knight
    {
        if (fabs(fx - lx) == 2 and fabs(fy - ly) == 1)
        {
            return true;
        }

        else if (fabs(fx - lx) == 1 and fabs(fy - ly) == 2)
        {
            return true;
        }
    }

    return false;

}

// Checks whether the position at board[x][y] is currently threatened by an enemy piece
bool chessboard::isThreatened (int x, int y, int enemyColor)
{
    char enemyPieceType = (enemyColor == 0) ? 'a' : 'A'; // 0 is white, 1 is black and lowercase is white, uppercase is black

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (isalpha(board[i][j]) and isSameCase(board[i][j], enemyPieceType) and isMoveValid(i, j, x, y))
                return true;
        }
    }

    return false;
}

int saveBoard (chessboard c1) // Saves the board to a file
{
    char gameName[16];
    int input;

    cout << "\nPlease give a name under which to save this game (max 15 letters) : ";
    cin.getline(gameName, 16);

    ofstream out;

    out.open(gameName, ios::binary);
    out.write((char*)&c1, sizeof(c1));

    cout << "\nGame saved successfully. Enter 0 to exit or 1 to continue with the game : ";
    cin >> input;

    return input;
}

void play(chessboard c1) // The main function which controls the game, can also be used to continue an unfinished game
{
    char inputMove[6];
    int fx, fy, lx, ly, errorMessage;

    while (true)
    {
        system("cls");

        c1.displayBoard();

        if (c1.checkTurn() == white)
            cout << "\nIt is white's turn.";

        else
            cout << "\nIt is black's turn.";

        cout << "\n\nPlease enter your move or type either exit, help or save : ";
        cin.getline(inputMove, 6);

        if (!strcmp(inputMove, "exit"))
            break;

        else if (!strcmp(inputMove, "help"))
        {
            system("cls");
            displayHelp();

            continue;
        }

        else if (!strcmp(inputMove, "save"))
        {
            int temp = saveBoard(c1);

            if (temp == 0)
                break;

            else
                continue;
        }


        else if (!isValidInput(inputMove))
        {
            cout << "\nPlease input in the following format : a2 a3.\n\nPress any key to continue.";

            getch();

            continue;
        }

        fy = inputMove[0] - 'a';
        fx = inputMove[1] - '1';
        ly = inputMove[3] - 'a';
        lx = inputMove[4] - '1';

        errorMessage = c1.isMoveValid(fx, fy, lx, ly);

        if (c1.isMoveValid(fx, fy, lx, ly) != 0)
        {
            switch (errorMessage)
            {
                case 1 :    cout << "\nIt is not your turn!";
                            break;
                case 2 :    cout << "\nYou can't move a piece out of the board!";
                            break;
                case 3 :    cout << "\nNo laziness! You must move your piece somewhere.";
                            break;
                case 4 :    cout << "\nYou have to defeat the other guy!";
                            break;
                case 5 :    cout << "\nYou can't move your piece that way or the path isn't clear.";
                            break;
                case 6 :    cout << "\nThat place is empty.";
                            break;
            }


            cout << "\n\nPress any key to continue.";

            getch();

            continue;
        }

        c1.movePiece(fx, fy, lx, ly);
        c1.changeTurn();
    }

    system ("cls");

    cout << "Thank you for playing!\n\n";
}

void loadGame() // Loads a previously saved chessboard from a file
{
    system("cls");

    char gameName[16], temp[2];
    chessboard c1;

    cin.getline(temp, 1);

    cout << "Enter the name under which you saved the game : ";
    cin.getline(gameName, 16);

    ifstream in;

    in.open(gameName, ios::binary);
    in.read((char*) &c1, sizeof(c1));

    play(c1);
}

void mainMenu() // The main menu of the game
{
    system("cls");

    int selection = 0;
    char temp[2];

    while (true)
    {
        cout << "\tWelcome to 2-player chess. Please select an option from below.\n\n"
             << "1. New Game\n2. Load Game\n3. Help Menu\n4. Exit\n\n";

        cin >> selection;

        if (selection == 1)
        {
            cin.getline(temp, 1); // To read \n from input stream
            chessboard c1;
            play(c1);
            break;
        }

        else if (selection == 2)
        {
            loadGame();
            break;
        }

        else if (selection == 3)
        {
            system("cls");
            displayHelp();
        }

        else
            break;
    }
}

int main()
{
    ClearConsoleToColors(0, 7); // light gray background

    mainMenu();

    return 0;
}
