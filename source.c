#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
    #include <sys/ioctl.h>
#endif

#define WIDTH 60
#define HEIGHT 30

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    STOP
};
enum Direction dir;
struct termios old_props;

void setup();
void clear_screen();
void draw();
void input();
void set_terminal_attributes();
void reset_terminal_attributes();
int input_available();
void game_play();
void load_highscore();
void save_highscore();


int score,high_score,tail_length;
int fruit_x,fruit_y;
int head_x,head_y;
int tail_x[100];
int tail_y[100];

int main() {
    srand(time(NULL));  
    set_terminal_attributes();
    setup();
    while(1){
        draw();
        input();
        game_play();
        int sleep_time= 500000/(score != 0 ? score : 1);
        usleep(sleep_time);
    }
    save_highscore();
    printf("\nGame Over! Final Score: %d\n", score);
    return 0;
}

void load_highscore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        if (fscanf(file, "%d", &high_score) != 1) {
            high_score = 0; 
        }
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
    if(input_available()){
        char ch = getchar();
        switch (ch)
        {
        case 'w': 
            if(dir != DOWN){
                dir=UP;
            }
            break;
        case 'a':
            if(dir != RIGHT){
                dir=LEFT;
            }
            break;
        case 's':
            if(dir != UP){
                dir=DOWN;
            }
            break;        
        case 'd': 
            if(dir != LEFT){
                dir=RIGHT;
            }
            break;
        case 'x': exit(0);
            break;    
        }
    }
}

void game_play(){
    for(int i= tail_length-1;i>0;i--){
        tail_x[i]=tail_x[i-1];
        tail_y[i]=tail_y[i-1];
    }
    tail_x[0]=head_x;
    tail_y[0]=head_y;
    switch(dir){
        case UP: head_y--;
               break;
        case DOWN: head_y++;
               break;
        case LEFT: head_x--;
               break;
        case RIGHT: head_x++;
               break;
        case STOP:
               break;                            
    }
    if(head_x<0){
        head_x=WIDTH-1;
    } else if(head_x>=WIDTH){
        head_x=0;
    }
    if(head_y<0){
        head_y=HEIGHT-1;
    } else if(head_y>=HEIGHT){
        head_y=0;
    }
    for(int i=0;i<tail_length;i++){
        if(tail_x[i]==head_x && tail_y[i] == head_y){
            printf("\nGAME OVER !!!\n");
            exit(0);
        }
    }
    if(head_x==fruit_x&&head_y==fruit_y){
        score+=2;
        tail_length++;
        fruit_x= rand() % WIDTH;
        fruit_y= rand() % HEIGHT;
     }
    if (score > high_score) {
        high_score = score;
    }
}

int input_available(){
    struct timeval tv = {0L,0L};
    fd_set fds;
    FD_SET(0,&fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void setup(){
    load_highscore();
    head_x=WIDTH/2;
    head_y=HEIGHT/2;
    fruit_x= rand() % WIDTH;
    fruit_y= rand() % HEIGHT;
    dir = STOP;
    score=0;
    tail_length=0;
}

void clear_screen(){
      #ifdef _Win32
      system("cls");
    #else 
      system("clear");
    #endif  
}
void draw(){
clear_screen();
    
   
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
void set_terminal_attributes(){
    tcgetattr(STDIN_FILENO, &old_props);
    atexit(reset_terminal_attributes);
    struct termios new_props=old_props;
    new_props.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);
}
void reset_terminal_attributes(){
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
}
