#include <stdio.h>


#define FIELD_WIDTH (10)
#define FIELD_HEIGHT (20)

void DrawScreen(){
    for(int y=0;y<FIELD_HEIGHT;y++){
        printf("□");
    }
}

int main(){
    DrawScreen();
    return 0;
}