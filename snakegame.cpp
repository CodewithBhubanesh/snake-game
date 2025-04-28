#include <bits/stdc++.h> 
#include <conio.h> 
#include <windows.h> 
 
using namespace std; 
 
#define MAX_LENGTH 1000 
 
// Directions 
const char DIR_UP = 'U'; 
const char DIR_DOWN = 'D'; 
const char DIR_LEFT = 'L'; 
const char DIR_RIGHT = 'R'; 
 
int consoleWidth, consoleHeight; 
 
// Initialize screen dimensions 
void initScreen() 
{ 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    CONSOLE_SCREEN_BUFFER_INFO csbi; 
    GetConsoleScreenBufferInfo(hConsole, &csbi); 
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; 
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1; 
} 
 
// Represents a point in the game 
struct Point 
{ 
    int x, y; 
    Point() {} 
    Point(int xCoord, int yCoord) : x(xCoord), y(yCoord) {} 
}; 
 
// Snake class 
class Snake 
{ 
    int length; 
    char direction; 
 
public: 
    Point body[MAX_LENGTH]; 
 
    Snake(int startX, int startY) 
    { 
        length = 1; 
        body[0] = Point(startX, startY); 
        direction = DIR_RIGHT; // Default direction 
    } 
 
    int getLength() { return length; } 
 
    void ChangeDirection(char newDirection) 
    { 
        if (newDirection == DIR_UP && direction != DIR_DOWN) 
            direction = newDirection; 
        else if (newDirection == DIR_DOWN && direction != DIR_UP) 
            direction = newDirection; 
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT) 
            direction = newDirection; 
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT) 
            direction = newDirection; 
    } 
 
    bool move(Point food) 
    { 
        // Move the snake's body 
        for (int i = length - 1; i > 0; i--) 
        { 
            body[i] = body[i - 1]; 
        } 
 
        // Update the head's position based on direction 
        switch (direction) 
        { 
        case DIR_UP: 
            body[0].y--; 
            break; 
        case DIR_DOWN: 
            body[0].y++; 
            break; 
        case DIR_LEFT: 
            body[0].x--; 
            break; 
        case DIR_RIGHT: 
            body[0].x++; 
            break; 
        } 
 
        // Check for self-collision 
        for (int i = 1; i < length; i++) 
        { 
            if (body[0].x == body[i].x && body[0].y == body[i].y) 
                return false; 
        } 
 
        // Check if the snake eats the food 
        if (body[0].x == food.x && body[0].y == food.y) 
        { 
            body[length] = body[length - 1]; 
            length++; 
        } 
 
        // Check for boundary collision 
        if (body[0].x < 0 || body[0].x >= consoleWidth || body[0].y < 0 || body[0].y >= 
consoleHeight) 
        { 
            return false; 
        } 
 
        return true; 
    } 
}; 
 
// Board class 
class Board 
{ 
    Snake *snake; 
    Point food; 
    int score; 
 
    const char SNAKE_BODY = 'O'; 
    const char FOOD = '*'; 
 
public: 
    Board() 
    { 
        snake = new Snake(consoleWidth / 2, consoleHeight / 2); 
        score = 0; 
        spawnFood(); 
    } 
 
    ~Board() { delete snake; } 
 
    int getScore() { return score; } 
 
    void spawnFood() 
    { 
        while (true) 
        { 
            int x = rand() % consoleWidth; 
            int y = rand() % consoleHeight; 
 
            // Ensure food does not overlap with the snake 
            bool overlap = false; 
            for (int i = 0; i < snake->getLength(); i++) 
            { 
                if (snake->body[i].x == x && snake->body[i].y == y) 
                { 
                    overlap = true; 
                    break; 
                } 
            } 
 
            if (!overlap) 
            { 
                food = Point(x, y); 
                break; 
            } 
        } 
    } 
 
    void gotoxy(int x, int y) 
    { 
        COORD coord; 
        coord.X = x; 
        coord.Y = y; 
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
    } 
 
    void draw() 
    { 
        system("cls"); 
 
        // Draw the snake 
        for (int i = 0; i < snake->getLength(); i++) 
        { 
            gotoxy(snake->body[i].x, snake->body[i].y); 
            cout << SNAKE_BODY; 
        } 
 
        // Draw the food 
        gotoxy(food.x, food.y); 
        cout << FOOD; 
 
        // Display the score 
        gotoxy(0, 0); 
        cout << "Score: " << score; 
    } 
 
    bool update() 
    { 
        bool alive = snake->move(food); 
        if (!alive) 
            return false; 
 
        if (snake->body[0].x == food.x && snake->body[0].y == food.y) 
        { 
            score++; 
            spawnFood(); 
        } 
 
        return true; 
    } 
 
    void getInput() 
    { 
        if (_kbhit()) 
        { 
            char key = _getch(); 
            switch (key) 
            { 
            case 'w': 
                snake->ChangeDirection(DIR_UP); 
                break; 
            case 'a': 
                snake->ChangeDirection(DIR_LEFT); 
                break; 
            case 's': 
                snake->ChangeDirection(DIR_DOWN); 
                break; 
            case 'd': 
                snake->ChangeDirection(DIR_RIGHT); 
                break; 
            } 
        } 
    } 
}; 
 
int main() 
{ 
    srand(time(0)); 
    initScreen(); 
 
    Board board; 
 
    while (board.update()) 
    { 
        board.getInput(); 
        board.draw(); 
        Sleep(140); 
    } 
 
    system("cls"); 
    cout << "Game Over!" << endl; 
    cout << "Final Score: " << board.getScore() << endl; 
 
    return 0; 
}