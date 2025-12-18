#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define SLEEP_MS(x) Sleep(x)
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
    #include <sys/ioctl.h>
    #define SLEEP_MS(x) usleep((x) * 1000)
#endif


#define WIDTH 60
#define HEIGHT 30
#define MAX_TAIL 2000 

enum Direction { UP, DOWN, RIGHT, LEFT, STOP };
enum Direction dir;


int score, high_score, tail_length;
int fruit_x, fruit_y;
int head_x, head_y;
int tail_x[MAX_TAIL];
int tail_y[MAX_TAIL];
int game_running = 1;


void setup();
void draw();
void input();
void logic();
void load_highscore();
void save_highscore();
void hide_cursor();
void reset_cursor_pos();
int kbhit_custom();
char getch_custom();
void enable_raw_mode();
void disable_raw_mode();

int main() {
    srand(time(NULL));
    setup();
    
    
    enable_raw_mode();
    hide_cursor();

    while(game_running){
        draw();
        input();
        logic();
        
        
        int base_speed = 100; 
        int speed_deduction = score / 5; 
        int current_speed = base_speed - speed_deduction;
        if (current_speed < 40) current_speed = 40; 

        SLEEP_MS(current_speed);
    }

    disable_raw_mode();
    save_highscore();
    printf("\nGame Over! Final Score: %d\n", score);
    return 0;
}

void setup(){
    load_highscore();
    head_x = WIDTH / 2;
    head_y = HEIGHT / 2;
    fruit_x = rand() % WIDTH;
    fruit_y = rand() % HEIGHT;
    dir = STOP;
    score = 0;
    tail_length = 0;
}

void load_highscore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        if (fscanf(file, "%d", &high_score) != 1) high_score = 0;
        fclose(file);
    } else {
        high_score = 0;
    }
}

void save_highscore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", high_score);
        fclose(file);
    }
}

void input(){
    if(kbhit_custom()){
        char ch = getch_custom();
        switch (ch) {
            case 'w': if(dir != DOWN) dir = UP; break;
            case 'a': if(dir != RIGHT) dir = LEFT; break;
            case 's': if(dir != UP) dir = DOWN; break;        
            case 'd': if(dir != LEFT) dir = RIGHT; break;
            case 'x': game_running = 0; break;    
        }
    }
}

void logic(){
    
    for(int i = tail_length - 1; i > 0; i--){
        tail_x[i] = tail_x[i-1];
        tail_y[i] = tail_y[i-1];
    }
    tail_x[0] = head_x;
    tail_y[0] = head_y;

    
    switch(dir){
        case UP: head_y--; break;
        case DOWN: head_y++; break;
        case LEFT: head_x--; break;
        case RIGHT: head_x++; break;
        case STOP: break;                            
    }

   
    if(head_x < 0) head_x = WIDTH - 1;
    else if(head_x >= WIDTH) head_x = 0;
    
    if(head_y < 0) head_y = HEIGHT - 1;
    else if(head_y >= HEIGHT) head_y = 0;

   
    for(int i = 0; i < tail_length; i++){
        if(tail_x[i] == head_x && tail_y[i] == head_y){
            game_running = 0;
        }
    }

    
    if(head_x == fruit_x && head_y == fruit_y){
        score += 10;
        if(tail_length < MAX_TAIL) tail_length++;
        fruit_x = rand() % WIDTH;
        fruit_y = rand() % HEIGHT;
     }

    if (score > high_score) high_score = score;
}

void draw(){
    reset_cursor_pos(); 
    
    
    printf("\n");
    printf("     \033[1;36m   ______ \033[0m  \033[1;32m  _____ ______ _____  _____  ______ _   _ _______\033[0m\n");
    printf("     \033[1;36m  / _____|\033[0m  \033[1;32m / ____|  ____|  __ \\|  __ \\|  ____| \\ | |__   __|\033[0m\n");
    printf("     \033[1;36m | |      \033[0m  \033[1;32m| (___ | |__  | |__) | |__) | |__  |  \\| |  | |   \033[0m\n");
    printf("     \033[1;36m | |      \033[0m  \033[1;32m \\___ \\|  __| |  _  /|  ___/|  __| | . ` |  | |   \033[0m\n");
    printf("     \033[1;36m | |______\033[0m  \033[1;32m ____) | |____| | \\ \\| |    | |____| |\\  |  | |   \033[0m\n");
    printf("     \033[1;36m  \\______|\033[0m  \033[1;32m|_____/|______|_|  \\_\\_|    |______|_| \\_|  |_|   \033[0m\n");
    printf("\n");

    
    printf("  \033[1;37m+------------------------------------------------------------+\033[0m\n");
    printf("  | \033[1;33mSCORE: %-10d\033[0m \033[1;36mHIGH SCORE: %-10d\033[0m                |\n", score, high_score);
    printf("  \033[1;37m+------------------------------------------------------------+\033[0m\n");

   
    for(int i = 0; i < HEIGHT; i++){
        printf("  \033[1;37m|\033[0m"); 
        for(int j = 0; j < WIDTH; j++){
            if(i == head_y && j == head_x){
                printf("\033[1;32mO\033[0m"); 
            } else if(i == fruit_y && j == fruit_x){
                printf("\033[1;31m@\033[0m"); 
            } else {
                int tail_found = 0;
                for(int k = 0; k < tail_length; k++){
                    if(tail_x[k] == j && tail_y[k] == i){
                        printf("\033[0;32mo\033[0m"); 
                        tail_found = 1;
                        break;
                    }
                }
                if(!tail_found) printf(" ");
            }
        }
        printf("\033[1;37m|\033[0m\n"); 
    }
    printf("  \033[1;37m+------------------------------------------------------------+\033[0m\n");
    printf("   Controls: W,A,S,D to Move | X to Quit\n");
}


void hide_cursor() {
    #ifdef _WIN32
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    #else
        printf("\e[?25l");
    #endif
}

void reset_cursor_pos() {
    #ifdef _WIN32
        HANDLE hOut;
        COORD Position;
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        Position.X = 0;
        Position.Y = 0;
        SetConsoleCursorPosition(hOut, Position);
    #else
        printf("\033[H");
    #endif
}

int kbhit_custom() {
    #ifdef _WIN32
        return _kbhit();
    #else
        struct timeval tv = { 0L, 0L };
        fd_set fds;
        FD_SET(0, &fds);
        return select(1, &fds, NULL, NULL, &tv);
    #endif
}

char getch_custom() {
    #ifdef _WIN32
        return _getch();
    #else
        return getchar();
    #endif
}

#ifndef _WIN32
struct termios orig_termios;
#endif

void enable_raw_mode() {
    #ifndef _WIN32
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    #endif
}

void disable_raw_mode() {
    #ifndef _WIN32
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
        printf("\e[?25h");
    #endif
}