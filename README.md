# chess-game

Overview:
This project is a console-based Chess Game implemented in C++ using Object-Oriented Programming principles. It simulates a complete chess environment where two players can play by entering moves in standard chess notation.
The system validates all moves according to official chess rules and ensures fair gameplay by preventing illegal moves.

Features:

Two-player chess game
Move input using standard notation (e.g., e2 e4)
Complete validation of chess rules
Detection of check, checkmate, and stalemate
Support for special moves:
Castling
Pawn Promotion
En Passant
Illegal move prevention with feedback
Console-based board display with labeled rows and columns

Game Flow:
The game starts with a main menu
The chessboard initializes with standard piece positions
Players take turns (White starts first)
Each move is validated based on:
Piece movement rules
Turn correctness
King safety (no self-check)
Valid moves update the board

System checks for:
Check
Checkmate
Stalemate
Game ends when checkmate or stalemate occurs

->OOP Concepts Used
Encapsulation:
Piece attributes are kept private and accessed through methods.
Inheritance:
All chess pieces inherit from a base Piece class.
Polymorphism:
Each piece overrides the isValidMove() function according to its movement rules.
Abstraction:
Game logic interacts with pieces without knowing internal implementations.
Composition:
The Board contains piece objects
The Game manages the board

Implementation Details:
Accurate movement rules for all pieces
Prevention of illegal moves
Check and checkmate detection
Stalemate handling
Pawn promotion with user choice
Castling logic implementation
En passant capture support
Console refresh for better readability
Winner announcement after game ends

Technologies Used:
C++
Object-Oriented Programming

Conclusion:
This project demonstrates the application of Object-Oriented Programming concepts in building a fully functional chess game. It highlights modular design, rule validation, and implementation of complex game mechanics.
