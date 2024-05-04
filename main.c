#include <stdio.h>
#include <windows.h>
#include <conio.h> 
#include <time.h>

int jump = 0; // Flag to indicate if the dinosaur is jumping
int t = 0;    // Vertical position of the dinosaur
int speed = 40; // Speed of the game, used in delay
int score = 0; // Variable to store the score

// Function to set cursor position
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to introduce delay
void delay(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

// Function to display game setup
void getup() {
    system("cls");
    gotoxy(10, 2);
    printf("                  Press Space to Jump and X to exit");
    gotoxy(63, 2);
    printf("SCORE:   %d", score);
    gotoxy(1, 25);
    for (int x = 0; x < 80; x++)
        printf("_");
}

// Function to draw the dinosaur
void ds(int jump) {
    static const int min_height = 0;
    static const int max_height = 15; // Maximum jump height

    // Update vertical position based on jump state
    if (jump == 0)
        t = min_height;
    else if (jump == 2) {
        t = t - 1; // Descending
    }
    else {
        t = t + 1; // Ascending
    }

    // Limit the vertical position within the boundaries
    t = (t < min_height) ? min_height : t;
    t = (t > max_height) ? max_height : t;

    // Clear previous dinosaur position
    for (int i = min_height; i <= max_height; i++) {
        gotoxy(7, 17 - i);
        printf("                     \n");
    }

    // Clear the area below the dinosaur
    for (int i = 0; i < 6; i++) {
        gotoxy(7, 23 - t - i);
        printf("                    \n");
    }

    // Print dinosaur at adjusted position
    gotoxy(7, 18 - t);
    printf("            __ \n");
    gotoxy(7, 19 - t);
    printf("           / _)\n");
    gotoxy(7, 20 - t);
    printf("    .----/ /  \n");
    gotoxy(7, 21 - t);
    printf(" __/       /   \n");
    gotoxy(7, 22 - t);
    printf("|__.|_|-|_|     \n");
    if (jump != 0) {
        printf("                \n");
    }

    delay(speed); // Delay to control the speed of animation
}

// Function to draw obstacles and handle collisions
void obj() {
    static int x = 0;
    char ch;
    int gameover = 0;

    // Define bounding boxes for the dinosaur and the obstacle
    int dinoLeft = 7; // Left edge of dinosaur bounding box
    int dinoRight = 17; // Right edge of dinosaur bounding box
    int dinoTop = 18 - t; // Top edge of dinosaur bounding box
    int dinoBottom = 23 - t; // Bottom edge of dinosaur bounding box
    int obstacleLeft = 74 - x; // Left edge of obstacle bounding box
    int obstacleRight = 77 - x; // Right edge of obstacle bounding box
    int obstacleTop = 20; // Top edge of obstacle bounding box
    int obstacleBottom = 23; // Bottom edge of obstacle bounding box

    // Improved collision detection for precise touch
    if (obstacleRight >= dinoLeft && obstacleLeft <= dinoRight && dinoBottom >= obstacleTop && dinoTop <= obstacleBottom) {
        gameover = 1;
        gotoxy(40, 15);
        printf("Game Over!\n");
        gotoxy(40, 17);
        printf("Your Score: %d\n", score);
        gotoxy(10, 14);
        printf("Press space key to replay!");
        while (1) {
            if (kbhit()) {
                ch = getch();
                if (ch == ' ') {
                    system("cls");
                    score = 0; // Reset score
                    x = 0;
                    t = 0;
                    jump = 0;
                    getup();
                    return;
                }
            }
        }
    }

    // Draw the obstacle
    gotoxy(74 - x, 20);
    printf("# ");
    gotoxy(74 - x, 21);
    printf("# ");
    gotoxy(74 - x, 22);
    printf("# ");
    gotoxy(74 - x, 23);
    printf("# ");
    x++;

    // If the obstacle has passed, increment the score
    if (x == 73) {
        x = 0;
        score++; // Increment score when obstacle passes

        // Display the updated score
        gotoxy(70, 2);
        printf("       "); // Clear previous score
        gotoxy(70, 2);
        printf("%d", score);

        if (speed > 25) speed--; // Increase speed gradually
    }
}

int main() {
    system("mode con: lines=29 cols=82");
    char ch;
    getup(); // Display game setup
    ds(0); // Draw the dinosaur
    obj(); // Draw the obstacle

    // Display start message and wait for space key
    gotoxy(30, 10);
    printf("Press space key to start the game...");

    while (1) {
        if (kbhit()) {
            ch = getch();
            if (ch == ' ')
                break;
        }
    }

    getup(); // Clear setup message
    while (1) {
        while (!kbhit()) {
            ds(0); // Draw dinosaur
            obj(); // Draw obstacle
        }
        ch = getch();
        if (ch == ' ') {
            // Perform jump animation
            for (int i = 0; i < 15; i++) { // Increased jump iterations for a longer jump
                jump = 1;
                ds(1);
                obj();
            }
            // Perform descent animation
            for (int i = 0; i < 15; i++) { // Increased descent iterations
                jump = 2;
                ds(2);
                obj();
            }
        } else if (ch == 'x' || ch == 'X') {
            return 0; // Exit the game if 'X' is pressed
        }
    }
    return 0;
}
