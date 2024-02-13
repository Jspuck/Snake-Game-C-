#include <iostream>
#include <conio.h>
#include <windows.h>
#include <list>
#include <time.h>

using namespace std;

int screenWidth = 25;  // Width of the game screen
int screenHeight = 20; // Height of the game screen
int score = 0;

// Enum to represent different levels of difficulty
enum Difficulty { EASY, MEDIUM, HARD };
Difficulty difficulty;

// A structure to represent the position of objects (snake parts or food)
struct Position {
    int x, y;
    Position(int col, int row) : x(col), y(row) {}
    Position() : x(0), y(0) {}
};
bool operator!=(const Position &lhs, const Position &rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

list<Position> snake;  // List to hold positions of snake parts
list<Position> foods;  // List to hold positions of food

int direction = 0; // 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
bool gameOver = false;

// Initialize game settings
void InitializeGame() {
    snake.clear();
    snake.push_back(Position(12, 10));
    snake.push_back(Position(12, 11));
    snake.push_back(Position(12, 12));

    foods.clear();
    for (int i = 0; i < 5; i++) {
        Position newFood(rand() % screenWidth, rand() % screenHeight);
        foods.push_back(newFood);
    }
    direction = 0;
    gameOver = false;
    score = 0;
}

// Draw the game screen
void Draw() {
    system("cls");
    for (int i = 0; i < screenWidth; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            if (j == 0 || j == screenWidth - 1) cout << "#"; // Draw walls
            else {
                bool isSnakePart = false;
                for (Position p : snake) {
                    if (p.x == j && p.y == i) {
                        cout << "O"; // Draw snake part
                        isSnakePart = true;
                        break;
                    }
                }
                if (!isSnakePart) {
                    bool isFoodPart = false;
                    for (Position f : foods) {
                        if (f.x == j && f.y == i) {
                            cout << "F"; // Draw food
                            isFoodPart = true;
                            break;
                        }
                    }
                    if (!isFoodPart) cout << " ";
                }
            }
        }
        cout << endl;
    }

    for (int i = 0; i < screenWidth; i++) cout << "#";
    cout << "\nScore: " << score << endl;  // Display score
}

// Handle user input
void Input() {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
            case 'w':
                if (direction != 2) direction = 0;
                break;
            case 's':
                if (direction != 0) direction = 2;
                break;
            case 'a':
                if (direction != 1) direction = 3;
                break;
            case 'd':
                if (direction != 3) direction = 1;
                break;
        }
    }
}

// Game logic (movement, eating food, collisions)
void Logic() {
    Position nextPos = snake.front();

    switch (direction) {
        case 0:
            nextPos.y--; break;
        case 1:
            nextPos.x++; break;
        case 2:
            nextPos.y++; break;
        case 3:
            nextPos.x--; break;
    }

    // Check if snake eats food
    for (auto it = foods.begin(); it != foods.end();) {
        if (nextPos.x == it->x && nextPos.y == it->y) {
            snake.push_front(nextPos); // Grow snake
            Position newFood(rand() % screenWidth, rand() % screenHeight);
            foods.push_back(newFood); // Add new food
            it = foods.erase(it);     // Remove eaten food
            score += 100;   // Increase score
        } else {
            ++it;
        }
    }
    snake.push_front(nextPos);
    snake.pop_back();

    // Check for collisions
    for (Position p : snake) {
        if (p.x == nextPos.x && p.y == nextPos.y && p != snake.front()) {
            gameOver = true;
        }
    }

    // Check for wall collisions
    if (nextPos.x < 0 || nextPos.x >= screenWidth || nextPos.y < 0 || nextPos.y >= screenHeight) {
        gameOver = true;
    }
}

// Display difficulty menu
void Menu() {
    system("cls");
    cout << "Snake Game - Select Difficulty\n";
    cout << "==============================\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "Enter choice: ";
    
    char choice;
    cin >> choice;
    
    switch(choice) {
        case '1':
            difficulty = EASY;
            break;
        case '2':
            difficulty = MEDIUM;
            break;
        case '3':
            difficulty = HARD;
            break;
        default:
            difficulty = MEDIUM;
            break;
    }
}

int main() {
    Menu();             // Display the menu
    InitializeGame();   // Setup game state
    while (!gameOver) {
        Draw();         // Render game screen
        Input();        // Process user input
        Logic();        // Game mechanics

        // Adjust delay based on difficulty level
        int delay;
        switch (difficulty) {
            case EASY:
                delay = 150;  // Slower delay
                break;
            case MEDIUM:
                delay = 100;  // Moderate delay
                break;
            case HARD:
                delay = 50;   // Faster delay
                break;
        }
        Sleep(delay);
    }

    // Display game over message
    cout << "\nGame Over!" << endl;
    cout << "Final Score: " << score << endl;
    cout << "Your snakes length: " << snake.size() << endl;
    return 0;
}
