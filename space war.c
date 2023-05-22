#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TORPEDOES 10

typedef struct {
    int x, y;
    int direction;
    int fuel;
    int score;
    int torpedoes;
} Spaceship;

typedef struct {
    int x, y;
    int gravity;
} Singularity;

void drawGame(const Spaceship* spaceship, const Singularity* singularity) {
    clear();
    mvprintw(spaceship->y, spaceship->x, "A");
    mvprintw(singularity->y, singularity->x, "*");
    mvprintw(0, 0, "Fuel: %d", spaceship->fuel);
    mvprintw(1, 0, "Score: %d", spaceship->score);
    mvprintw(2, 0, "Torpedoes: %d", spaceship->torpedoes);
    refresh();
}

void processInput(int input, Spaceship* spaceship) {
    switch (input) {
        case KEY_UP:
            spaceship->fuel--;
            spaceship->y--;
            break;
        case KEY_DOWN:
            spaceship->fuel--;
            spaceship->y++;
            break;
        case KEY_LEFT:
            spaceship->direction -= 10;
            break;
        case KEY_RIGHT:
            spaceship->direction += 10;
            break;
        case 's':
            if (spaceship->torpedoes > 0) {
                spaceship->torpedoes--;
                spaceship->score += 10;
            }
            break;
    }
}

void updateGame(Spaceship* spaceship, const Singularity* singularity) {
    int dx = singularity->x - spaceship->x;
    int dy = singularity->y - spaceship->y;
    int distance = dx * dx + dy * dy;
    int gravity = singularity->gravity / distance;

    spaceship->fuel--;
    spaceship->y += gravity;

    if (spaceship->x <= 0 || spaceship->x >= COLS - 1 || spaceship->y <= 0 || spaceship->y >= LINES - 1) {
        endwin();
        printf("Game Over!\n");
        printf("Final Score: %d\n", spaceship->score);
        exit(0);
    }

    if (spaceship->x == singularity->x && spaceship->y == singularity->y) {
        endwin();
        printf("Game Over!\n");
        printf("Final Score: %d\n", spaceship->score);
        exit(0);
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    srand(time(NULL));

    Spaceship spaceship;
    spaceship.x = COLS / 2;
    spaceship.y = LINES / 2;
    spaceship.direction = 0;
    spaceship.fuel = 100;
    spaceship.score = 0;
    spaceship.torpedoes = MAX_TORPEDOES;

    Singularity singularity;
    singularity.x = rand() % COLS;
    singularity.y = rand() % LINES;
    singularity.gravity = 1;

    int input;

    while (1) {
        drawGame(&spaceship, &singularity);
        input = getch();
        processInput(input, &spaceship);
        updateGame(&spaceship, &singularity);
    }

    endwin();

    return 0;
}
