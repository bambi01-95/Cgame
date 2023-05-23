//参考動画　https://www.youtube.com/watch?v=BJs29RicyPw&t=418s
//hbhit   https://hotnews8.net/programming/tricky-code/c-code03
//乱数　　　https://monozukuri-c.com/langc-funclist-rand/
//memset  https://bituse.info/c_func/58
//system  https://stackoverflow.com/questions/228617/how-do-i-clear-the-console-in-both-windows-and-linux-using-c


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// #include <conio.h>
// #include <curses.h>
//
#include <termios.h> //tcgetattr
#include <unistd.h>
#include <fcntl.h>
int kbhit(void);
//

#define FIELD_WIDTH (10)
#define FIELD_HEIGHT (20)

#define SHAPE_WIDTH_MAX (4)
#define SHAPE_HEIGHT_MAX (4)

int LEVEL = 1;
int INTERVAL  = 1000000;
int SCORE  = 10;
int BONUS1 = 1;
int BONUS2 = 1;

enum {
    SHAPE_I,
    SHAPE_O,
    SHAPE_S,
    SHAPE_Z,
    SHAPE_J,
    SHAPE_L,
    SHAPE_T,
    SHAPE_MAX
};


typedef struct {
    int width,height;
    int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
}SHAPE;


typedef struct{
    int x,y;
    SHAPE shape;
}MINO;


SHAPE shapes[SHAPE_MAX] = {
    // SHAPE_I
    {
        // int width,height;
        4,4,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    // SHAPE_O
    {
        // int width,height;
        2,2,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,1},
            {1,1}
        }
    },
    // SHAPE_S
    {
        // int width,height;
        3,3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,1,1},
            {1,1,0},
            {0,0,0}
        }
    },
    // SHAPE_Z
    {
        // int width,height;
        3,3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,1,0},
            {0,1,1},
            {0,0,0}
        }
    },
    // SHAPE_J
    {
        // int width,height;
        3,3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,0,0},
            {1,1,1},
            {0,0,0}
        }
    },
    // SHAPE_L
    {
        // int width,height;
        3,3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,0,1},
            {1,1,1},
            {0,0,0}
        }
    },
    // SHAPE_T
    {
        // int width,height;
        3,3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,1,0},
            {1,1,1},
            {0,0,0}
        }
    }
};


int  field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];

MINO mino;
MINO next_mino;

typedef int bool;

bool MinoIntegersectField(){
    for(int y=0;y<mino.shape.height;y++){
        for(int x=0; x<mino.shape.width;x++){
            if(mino.shape.pattern[y][x]){
                    if((mino.y+y<0)||(mino.y+y>=FIELD_HEIGHT) || (mino.x+x<0)||(mino.x+x>=FIELD_WIDTH))//左右下の壁の境界を認知
                        return 1;
                    if(field[mino.y+y][mino.x+x])
                        return 1;
            }
        }
    }
    return 0;
}

void DrawScreen(){
    memcpy(screen,field,sizeof field);//screenにfieldをsizeof fieldの大きさでコピーする

    for(int y=0;y<mino.shape.height;y++){
        for(int x=0; x<mino.shape.width;x++){
            if(mino.shape.pattern[y][x])
                screen[mino.y+y][mino.x+x] |= 1;
        }
    }

    system("clear");


    for(int y=0;y<FIELD_HEIGHT;y++){
        printf("|");
        for(int x=0;x<FIELD_WIDTH;x++){
            printf("%s",screen[y][x]?"⬜︎":"  ");
        }
        printf("|");
        putchar('\n');
    }
    for(int x=0;x<FIELD_WIDTH+1;x++){
        printf("--");
    }
    putchar('\n');
    printf("<NEXT BLOCK>\n\n");
    for(int y=0;y<SHAPE_HEIGHT_MAX;y++){
        printf("    ");
        for(int x=0;x<SHAPE_WIDTH_MAX;x++){
            printf("%s",next_mino.shape.pattern[y][x]?"⬜︎":"  ");
        }
        putchar('\n');
    }
    printf("<LEVEL:%5d>\n",LEVEL);
    printf("<SCORE:%5d>\n",SCORE);
}


void InitMino(){
    mino.shape = next_mino.shape;
    next_mino.shape  = shapes[rand() % SHAPE_MAX];// shapes array の中の形を一つランダムで生成（コピー）する
    mino.x = (FIELD_WIDTH - mino.shape.width) / 2;//ブロックを真ん中に設定する
    mino.y = 0;//yはゼロに設定する。
    for(int y=0;y<SHAPE_HEIGHT_MAX;y++){
        for(int x=0;x<FIELD_WIDTH;x++){
            if(mino.shape.pattern[y][x]==1)
                if(mino.shape.pattern[y][x]==field[y][x]){
                    system("clear");
                    printf("\n\n+++++ GAME OVER +++++\n");
                    printf("--- LEVEL%d ---\n\n",LEVEL);
                    printf("YOUR SCORE:%5d\n",SCORE);
                    exit(1);
                }
        }
    }
}

void Init(){
    memset(field,0,sizeof field);// array/tableを0でsizeof分セットする。

    InitMino();

    DrawScreen();
}

int main(){
    printf("\n\n\n<<<<< WELCOM TO TETRIS >>>>>\n");
    printf("\n- INPUT YOUR LEVEL 1~9: ");
    int c = getchar();
    LEVEL = c-'0';
    INTERVAL = INTERVAL / LEVEL;
    next_mino.shape  = shapes[rand() % SHAPE_MAX];
    srand((unsigned int)time(NULL));//時間によって乱数を変える
    Init();
    
    clock_t lastClock = clock();

    while(1){
        clock_t nowClock = clock();


        if(nowClock>=lastClock+INTERVAL){
            lastClock = nowClock;

            MINO lastMino = mino; //ひとつ前のminoを暗記
            mino.y++;               //下に自動で下げる

            if(MinoIntegersectField()){
                mino = lastMino;
                for(int y=0;y<mino.shape.height;y++)
                        for(int x=0; x<mino.shape.width;x++)
                            if(mino.shape.pattern[y][x])
                                field[mino.y+y][mino.x+x] |= 1;//固定されたブロックを保存

                for(int y=0;y<FIELD_HEIGHT;y++){
                    int completed = 1;
                    for(int x=0; x<FIELD_WIDTH;x++)
                        if(!field[y][x]){//一つでも０があったら空白がある。
                            completed = 0;
                            break;
                        }
                    if(completed){
                        if(BONUS2<2)
                            SCORE = SCORE + (10*BONUS2*BONUS1);
                        else
                            SCORE += (10*BONUS1);
                        BONUS2+=1;
                        for(int x=0;x<FIELD_WIDTH;x++)//埋まっている行を全て消す
                            field[y][x] = 0;
                        for(int y2=y;y2>1;y2--){//yより上の行を下に持ってくる。
                            for(int x=0;x<FIELD_WIDTH;x++){
                                field[y2][x] = field[y2 - 1][x];
                                field[y2 -1][x] = 0;
                            }
                        }
                    }
                }
                if(BONUS2>1){
                    BONUS1 +=1;
                }
                else
                    BONUS1 = 1;
                BONUS2 = 1;
                InitMino();
            }
            
            DrawScreen();
        }


        if(kbhit()){
            MINO lastMino = mino;
            switch(getchar()){
                case 'w':
                    break;
                case 's':{
                    mino.y++;
                    break;
                }
                case 'a':{
                    mino.x--;
                    break;
                }
                case 'd':{
                    mino.x++;
                    break;
                }
                case 'c':{
                    exit(1);
                }
                default:{
                    MINO newMino = mino;

                    for(int y=0;y<mino.shape.height;y++){
                        for(int x=0; x<mino.shape.width;x++){
                            newMino.shape.pattern[mino.shape.width-1-x][y] = mino.shape.pattern[y][x];
                        }
                    }
                    mino = newMino;
                }
                break;
                
            }
            if(MinoIntegersectField())
                mino = lastMino;
            
            DrawScreen();
        }
    }
    return 0;
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}