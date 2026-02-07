#include <iostream>
#include <ctime>
#include <cstring>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

int score = 0;
string playerName;

void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printjadval(int jadval[4][4]){
    cout<<"#------#------#------#------#\n";
    for(int i=0;i<4;i++){
        cout<<"|";
        for(int j=0;j<4;j++){
            if(jadval[i][j]==0){
                cout<<"      |";
            } else {
                int val = jadval[i][j];

                if(val == 2) setColor(9);
                else if(val == 4) setColor(12);
                else if(val == 8) setColor(10);
                else if(val == 16) setColor(14);
                else if(val == 32) setColor(11);
                else if(val == 64) setColor(13);
                else setColor(15);

                printf(" %4d ", val);
                setColor(7);
                cout<<"|";
            }
        }
        cout<<"\n#------#------#------#------#\n";
    }
}

void moveleft(int a[4]){
    int chap[4]={0,0,0,0};
    int index=0;
    for(int i=0;i<4;i++)
        if(a[i]!=0)
            chap[index++]=a[i];

    for(int i=0;i<3;i++){
        if(chap[i]!=0 && chap[i]==chap[i+1]){
            chap[i]*=2;
            score += chap[i];
            chap[i+1]=0;
        }
    }

    int b[4]={0,0,0,0};
    int index2=0;
    for(int i=0;i<4;i++)
        if(chap[i]!=0)
            b[index2++]=chap[i];

    for(int i=0;i<4;i++)
        a[i]=b[i];
}

void moveright(int a[4]){
    int reversed[4];
    for(int i=0;i<4;i++)
        reversed[i]=a[3-i];
    moveleft(reversed);
    for(int i=0;i<4;i++)
        a[i]=reversed[3-i];
}

void transpose(int jadval[4][4]){
    for(int i=0;i<4;i++)
        for(int j=i+1;j<4;j++)
            swap(jadval[i][j], jadval[j][i]);
}

void moverightjadval(int jadval[4][4]){
    for(int i=0;i<4;i++)
        moveright(jadval[i]);
}

void moveleftjadval(int jadval[4][4]){
    for(int i=0;i<4;i++)
        moveleft(jadval[i]);
}

void moveupjadval(int jadval[4][4]){
    transpose(jadval);
    moveleftjadval(jadval);
    transpose(jadval);
}

void movedownjadval(int jadval[4][4]){
    transpose(jadval);
    moverightjadval(jadval);
    transpose(jadval);
}

void addrandomnumber(int jadval[4][4]){
    int count=0;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(jadval[i][j]==0)
                count++;

    if(count==0) return;

    int r=rand()%count;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(jadval[i][j]==0){
                if(r==0){
                    jadval[i][j]=(rand()%10<9)?2:4;
                    return;
                }
                r--;
            }
        }
    }
}

bool checklose(int jadval[4][4]){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(jadval[i][j]==0)
                return false;

    for(int i=0;i<4;i++)
        for(int j=0;j<3;j++)
            if(jadval[i][j]==jadval[i][j+1])
                return false;

    for(int j=0;j<4;j++)
        for(int i=0;i<3;i++)
            if(jadval[i][j]==jadval[i+1][j])
                return false;

    return true;
}

bool checkwin(int jadval[4][4]){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(jadval[i][j]==2048)
                return true;
    return false;
}

int main(){
    srand(time(0));

    cout<<"Enter player name: ";
    getline(cin, playerName);

    int jadval[4][4]={0};
    addrandomnumber(jadval);
    addrandomnumber(jadval);

    while(true){
        system("cls");
        printjadval(jadval);
        cout<<"Score: "<<score<<endl;
        cout<<"Use keys W/A/S/D to move. Press Ctrl+C to exit.\n";

        char c=_getch();

        int oldjadval[4][4];
        memcpy(oldjadval,jadval,sizeof(jadval));

        if(c=='w'||c=='W')
            moveupjadval(jadval);
        else if(c=='a'||c=='A')
            moveleftjadval(jadval);
        else if(c=='d'||c=='D')
            moverightjadval(jadval);
        else if(c=='s'||c=='S')
            movedownjadval(jadval);
        else
            continue;

        bool changed=false;
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                if(oldjadval[i][j]!=jadval[i][j])
                    changed=true;

        if(changed)
            addrandomnumber(jadval);

        if(checkwin(jadval)){
            system("cls");
            printjadval(jadval);
            cout<<"YOU WIN\n";
            break;
        }

        if(checklose(jadval)){
            system("cls");
            printjadval(jadval);
            cout<<"YOU LOST\n";
            break;
        }
    }

    ofstream file("scores.txt", ios::app);
    if(file.is_open()){
        file << playerName << " " << score << endl;
        file.close();
    }

    return 0;
}