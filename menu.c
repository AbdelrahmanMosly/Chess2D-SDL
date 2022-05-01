#include <stdio.h>
#include <stdlib.h>

void chooseMode();

//-----------main menu----------//
void selectFromMainMenu(){
    int n=0;
    while (n!=1 && n!=2){
        printf("\nEnter your choice:");
        scanf("%d",&n);
    }
    switch(n){
        case 1:
            chooseMode();
            break;

        case 2:
            exit(0);
            break;

        default:
            break;

    }
}
void loadMainMenu(){

    system("cls");
    printf("1.Choose Mode");
    printf("\n2.Exit\n");
    selectFromMainMenu();
}



void chooseTime(){

    system("cls");
    int minutes,seconds,increment;
    printf("Enter Time minutes then seconds (space seperated):");
    scanf("%d %d",&minutes,&seconds);
    while(seconds>=60 || minutes<0 || seconds <0 || (minutes==0&&seconds==0)){
        printf("\nInvalid Formate");
        printf("\nEnter Time minutes then seconds (space seperated):");
        scanf("%d %d",&minutes,&seconds);
    }
    printf("the time you entered %d:%02d",minutes,seconds);
    printf("\n enter increment if you want(0 for no increment):");
    scanf("%d",&increment);
    while(increment<0){
        printf("\n enter increment if you want(0 for no increment)~cant be negative~:");
        scanf("%d",&increment);
    }
}
void selectFromChooseMode(){
    int n=0;
    while (n!=1 && n!=2 && n!=3){
        printf("\nEnter your choice:");
        scanf("%d",&n);
    }
    switch(n){
    case 1:
        //go to the normal mode
        break;

    case 2:
        //go to choose time menu
        chooseTime();
        break;
    case 3:
        loadMainMenu();
    default:
        break;

    }
}
void chooseMode(){
    system("cls");
    printf("1.Normal");
    printf("\n2.Time");
    printf("\n3.back\n");
    selectFromChooseMode();
}
