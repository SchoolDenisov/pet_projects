#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20
#define PADDLE_HEIGHT 5
#define PADDLE_WIDTH 1
#define BALL 'O'
#define PLAYER1 '|'
#define PLAYER2 '|'

int ballX, ballY, ballDirX, ballDirY;
int player1Y, player2Y;
int player1Score = 0, player2Score = 0;

void init() {
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;
    ballDirX = 1;  // направление по оси X
    ballDirY = 1;  // направление по оси Y
    player1Y = HEIGHT / 2 - PADDLE_HEIGHT / 2; // начало положения первого игрока
    player2Y = HEIGHT / 2 - PADDLE_HEIGHT / 2; // начало положения второго игрока
}

void draw() {
    clear(); // очищаем экран

    // рисуем верхнюю границу
    for (int i = 0; i < WIDTH + 2; i++)
        addch('-');
    addch('\n');

    // рисуем игровую область
    for (int y = 0; y < HEIGHT; y++) {
        addch('|'); // левая граница

        for (int x = 0; x < WIDTH; x++) {
            if (x == ballX && y == ballY)
                addch(BALL); // мяч
            else if (x == 0 && y >= player1Y && y < player1Y + PADDLE_HEIGHT)
                addch(PLAYER1); // ракетка первого игрока
            else if (x == WIDTH - 1 && y >= player2Y && y < player2Y + PADDLE_HEIGHT)
                addch(PLAYER2); // ракетка второго игрока
            else
                addch(' '); // пустое пространство
        }

        addch('|'); // правая граница
        addch('\n');
    }

    // рисуем нижнюю границу
    for (int i = 0; i < WIDTH + 2; i++)
        addch('-');
    addch('\n');

    // показываем счет
    printw("Player 1: %d   Player 2: %d\n", player1Score, player2Score);
}

void update() {
    // обновляем позицию мяча
    ballX += ballDirX;
    ballY += ballDirY;

    // проверяем столкновение с верхней и нижней границами
    if (ballY <= 0 || ballY >= HEIGHT - 1) {
        ballDirY = -ballDirY; // изменяем направление по Y
    }

    // проверка на столкновение с ракетками
    if (ballX == 0 && ballY >= player1Y && ballY < player1Y + PADDLE_HEIGHT) {
        ballDirX = -ballDirX;
    } else if (ballX == WIDTH - 1 && ballY >= player2Y && ballY < player2Y + PADDLE_HEIGHT) {
        ballDirX = -ballDirX;
    }

    // проверяем, вышел ли мяч за пределы
    if (ballX < 0) {
        player2Score++;
        init(); // инициализируем заново
    } else if (ballX >= WIDTH) {
        player1Score++;
        init(); // инициализируем заново
    }
}

void control() {
    int ch = getch();
    
    switch (ch) {
        case 's':
            if (player1Y + PADDLE_HEIGHT < HEIGHT)
                player1Y++;
            break;
        case 'x':
            if (player1Y > 0)
                player1Y--;
            break;
        case 'o':
            if (player2Y + PADDLE_HEIGHT < HEIGHT)
                player2Y++;
            break;
        case 'l':
            if (player2Y > 0)
                player2Y--;
            break;
    }
}

int main() {
    // Инициализируем ncurses
    initscr();
    cbreak();          // Включаем режим быстрого ввода
    noecho();          // Не показывать нажатые клавиши
    keypad(stdscr, TRUE); // Включаем работу с функциональными клавишами
    nodelay(stdscr, TRUE); // Не блокировать ввод

    init();
    while (player1Score < 3 && player2Score < 3) {
        draw();
        control();
        update();
        usleep(80000); // замедление обновления для управления скоростью игры
    }

    // Завершаем работу с ncurses
    clear();
    printw("Game Over! Player %d wins!\n", player1Score == 21 ? 1 : 2);
    refresh();
    getch(); // Ждем ввода перед выходом
    endwin();
    return 0;
}