#include<bits/stdc++.h> //header file care include toate bibliotecile standard
#include <stdlib.h> //pentru culoare

using namespace std;

#define INCEPATOR 1
#define MEDIU 2
#define AVANSAT 3
#define MAXSIDE 25
#define MAXMINES 99
#define MOVESIZE 526 // (25 * 25 - 99)

int SIDE ; // lungimea tablei de joc
int NRBOMBE ; // numarul de bombe de pe tabla de joc
int STEGULETE ; //numarul de stegulete disponibile

char liber=220; //patratele libere vor avea ca simbol un patratel alb in matricea afisata in consola
char stegulet=120; //steguletele vor fi marcate printr-o casuta

//verifica daca linia si coloana unei celule date
//sunt corecte (nu depasesc limita)
bool valid(int lin, int col)
{
    return (lin>=0 && lin<SIDE && col>=0 && col<SIDE);
}

// verifica daca o celula contine o bomba
bool este_bomba(int lin, int col, char a[][MAXSIDE])
{
    /* 1 - bomba
       0 - celula sigura
    */
    return (a[lin][col]=='*');
}

//functie pentru mutare
void mutare(int *x, int *y)
{
    printf("(lin, col): ");
    scanf("%d %d", x, y);
}

//afisarea tablei de joc
void afisare(char a[][MAXSIDE])
{
    printf("    ");
    for (int i=0; i<SIDE; i++)
        printf ("%d ",i);
    printf("\n\n");
    for (int i=0; i<SIDE; i++)
    {
        if (i<=9) printf ("%d   ", i);
        else printf ("%d  ", i);
        for (int j=0; j<SIDE; j++)
            printf ("%c ",a[i][j]);
        printf("\n");
    }
    return;
}

//functie care sa numere cate bombe se afla printre vecinii unei celule de pe tabla a[][]
int nr_bombe_vecine(int lin, int col, int bombe[][2], char a[][MAXSIDE])
{
    int nr=0;

    /*
             N.V   N   N.E
               \   |   /
                \  |  /
             V---Celula--E
                 / | \
                /  |  \
             S.V   S   S.E


        N  -->  Nord        (lin-1, col)
        N.E-->  Nord-est    (lin-1, col+1)
        E  -->  Est         (lin,   col+1)
        S.E-->  Sud-est     (lin+1, col+1)
        S  -->  Sud         (lin+1, col)
        S.V-->  Sud-vest    (lin+1, col-1)
        V  -->  Vest        (lin,   col-1)
        N.V-->  Nord-vest   (lin-1, col-1)

    */

    //---------(1) celula din N----------

    if (valid(lin-1,col)) //daca celula din N este valida
    {
        if (este_bomba(lin-1,col,a)) //daca este bomba
            nr++;
    }
    //si analoagele:

    //--------- (2) celula din N-E----------

    if (valid(lin-1,col+1)) //daca celula din N-E este valida
    {
        if (este_bomba(lin-1,col+1,a)) //daca este bomba
            nr++;
    }

    //--------- (3) celula din E----------

    if (valid(lin,col+1)) //daca celula din E este valida
    {
        if (este_bomba(lin,col+1,a)) //daca este bomba
            nr++;
    }

    //--------- (4) celula din S-E----------

    if (valid(lin+1,col+1)) //daca celula din S-E este valida
    {
        if (este_bomba(lin+1,col+1,a)) //daca este bomba
            nr++;
    }

    //---------(5) celula din S----------

    if (valid(lin+1,col)) //daca celula din S este valida
    {
        if (este_bomba(lin+1,col,a)) //daca este bomba
            nr++;
    }

    //--------- (6) celula din S-V----------

    if (valid(lin+1,col-1)) //daca celula din S-V este valida
    {
        if (este_bomba(lin+1,col-1,a)) //daca este bomba
            nr++;
    }

    //--------- (7) celula din V----------
    if (valid(lin,col-1)) //daca celula din V este valida
    {
        if (este_bomba(lin,col-1,a)) //daca este bomba
            nr++;
    }

    //--------- (8) celula din N-V----------
    if (valid(lin-1,col-1)) //daca celula din N-V este valida
    {
        if (este_bomba(lin-1,col-1,a)) //daca este bomba
            nr++;
    }

    return (nr);
}

// functie recursiva pt a juca minesweeper
bool playMinesweeper(char myBoard[][MAXSIDE], char realBoard[][MAXSIDE], int bombe[][2], int lin, int col, int *mutari_ramase)
{
    /*
    realBoard - tabla de joc generata de functia random(), pe care trebuie s-o descoperim
    myBoard - tabla de joc afisata in consola
    */

    // Base Case of Recursion
    if (myBoard[lin][col]!=liber)
        return (false);

    // la deschiderea unei celule cu bomba, jucatorul pierde

    if (realBoard[lin][col]=='*')
    {
        myBoard[lin][col]='*';
        for (int i=0; i<NRBOMBE; i++)
            myBoard[bombe[i][0]][bombe[i][1]]='*';
        afisare(myBoard); //se afiseaza tabla actuala de joc
        printf ("\nAti pierdut!\n");
        return (true);
    }
    else
     {
        // Calculati si tipariti numarul de bombe adiacente de pe tabla

        int nr=nr_bombe_vecine(lin,col,bombe,realBoard);
        *mutari_ramase--;
        myBoard[lin][col]=nr+'0';
        if (!nr)
        {
        /*
             N.V   N   N.E
               \   |   /
                \  |  /
             V---Celula--E
                 / | \
                /  |  \
             S.V   S   S.E


        N  -->  Nord        (lin-1, col)
        N.E-->  Nord-est    (lin-1, col+1)
        E  -->  Est         (lin,   col+1)
        S.E-->  Sud-est     (lin+1, col+1)
        S  -->  Sud         (lin+1, col)
        S.V-->  Sud-vest    (lin+1, col-1)
        V  -->  Vest        (lin,   col-1)
        N.V-->  Nord-vest   (lin-1, col-1)

        */

            //-----------(1) celula din N ------------

            if (valid(lin-1,col)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin-1,col,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin-1,col,mutari_ramase);
            }
            //si analoagele

            //-----------(2) celula din N-E ------------

            if (valid(lin-1,col+1)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin-1,col+1,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin-1,col+1,mutari_ramase);
            }

            //-----------(3) celula din E ------------

            if (valid(lin,col+1)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin,col+1,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin,col+1,mutari_ramase);
            }

            //-----------(4) celula din S-E ------------

            if (valid(lin+1,col+1)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin+1,col+1,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin+1,col+1,mutari_ramase);
            }

            //-----------(5) celula din S ------------

            if (valid(lin+1,col)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin+1,col,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin+1,col,mutari_ramase);
            }

            //-----------(6) celula din S-V ------------

            if (valid(lin+1,col-1)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin+1,col-1,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin+1,col-1,mutari_ramase);
            }

            //-----------(7) celula din V ------------

            if (valid(lin,col-1)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin,col-1,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin,col-1,mutari_ramase);
            }

            //-----------(8) celula din N-V ------------

            if (valid(lin-1,col-1)) // verifica mai intai daca celula este valida
            {
                   if (este_bomba(lin-1,col-1,realBoard)==0)
                        playMinesweeper(myBoard,realBoard,bombe,lin-1,col-1,mutari_ramase);
            }

        return 0;
    }
}
}


//functie pentru plasarea aleatorie a bombelor

void plantare_bombe(int bombe[][2], char realBoard[][MAXSIDE])
{
    bool marcare[MAXSIDE*MAXSIDE];

    // vectorul marcare[] va fi initializat cu 0
    memset(marcare,false,sizeof(marcare));

    // Continua pana cand au fost create (si plasate) toate bombele
    for (int i=0; i<NRBOMBE;)
     {
        int random=rand()%(SIDE*SIDE);
        int x=random/SIDE;
        int y=random%SIDE;

        // Daca celula (x,y) este goala, se plaseaza o bomba
        if (marcare[random]==0)
        {
            // linia bombei
            bombe[i][0]=x;
            // coloana bombei
            bombe[i][1]=y;

            // plaseaza bomba
            realBoard[bombe[i][0]][bombe[i][1]]='*';
            marcare[random]=1;
            i++;
        }
    }
    return;
}

void plantare_stegulet(int x, int y, char myBoard[][MAXSIDE], int &tip)
{
    /* variabila tip:
            1 - se adauga un stegulet
            0 - se sterge un stegulet
    */
    if (myBoard[x][y]==liber)
        myBoard[x][y]=stegulet, tip=1;
    else if (myBoard[x][y]==stegulet)
        myBoard[x][y]=liber, tip=0;
}

// functie pentru initiliazarea jocului
void initializare(char realBoard[][MAXSIDE], char myBoard[][MAXSIDE])
{
    srand(time (NULL));
    // initial, nicio celula nu are bomba
    for (int i=0; i<SIDE; i++)
        for (int j=0; j<SIDE; j++)
            myBoard[i][j]=realBoard[i][j]=liber;
    /*char liber=220; //patratele libere vor avea ca simbol un patratel
    plin in matricea afisata in consola*/
}

// functie care sa triseze, tiparind tabla (si descoperind pozitiile bombelor)
void reveal(char realBoard[][MAXSIDE])
{
    cout<<"Pozitiile bombelor -\n";
    afisare(realBoard);
}

// functie care sa mute bomba de pe pozitia (lin,col) pe o pozitie vacanta
void inlocuieste_bomba(int lin, int col, char a[][MAXSIDE])
{
    int ok=0;
    for (int i=0; i<SIDE && ok==0; i++)
    {
        for (int j=0; j<SIDE && ok==0; j++)
            {
                // Gaseste prima celula care nu contine o mina si plaseaza o bomba acolo
                if (a[i][j] != '*')
                {
                    a[i][j] = '*';
                    a[lin][col] = liber;
                    ok=1;
                }
            }
    }
}

// joaca Minesweeper
void play_Minesweeper()
{
    // Consideram ca jocul nu este incheiat
    bool gameOver=0;

    // Tabla propriu-zisa si tabla afisata in consola

    char realBoard[MAXSIDE][MAXSIDE], myBoard[MAXSIDE][MAXSIDE];
    int mutari_ramase=SIDE*SIDE-NRBOMBE, x, y;
    int bombe[MAXMINES][2]; // coordonatele bombelor, de forma (x,y)

    initializare(realBoard, myBoard);

    // plantarea bombelor in mod aleatoriu

    plantare_bombe(bombe,realBoard);

    //Jocul continua atata timp cat nu s-a deschis nicio celula care sa contina o bomba
    int mutare_curenta=0, steaguri_plantate=0;
    while(gameOver==0)
     {
        printf ("\tTabla curenta : \n\n");
        afisare(myBoard);
        printf ("\n\tDoriti sa plantati/stergeti un stegulet?\n(1 - DA, 0 - NU)\n");
        int ok;
        cin>>ok;
        mutare(&x,&y);
        system("CLS");
        if (!ok) //daca nu se doreste plasarea unui stegulet in casuta(x,y)
        {
            // Pentru a fi siguri ca prima mutare este sigura (daca este prima mutare din joc)
            if (mutare_curenta==0)
            {
                // Daca prima celula aleasa contine o bomba, se indeparteaza bomba de pe acea pozitie si se plaseaza pe urmatoarea pozitie valida
                if (este_bomba(x,y,realBoard))
                    inlocuieste_bomba(x,y,realBoard);
            }
            mutare_curenta++;
            gameOver=playMinesweeper(myBoard,realBoard,bombe,x,y,&mutari_ramase);

            if ((gameOver==0) && (mutari_ramase == 0)) //daca jocul nu a fost pierdut pana aici si s-a terminat numarul mutarilor
            {
                cout<<"Ati castigat! Felicitari!"; //ai castigat
                gameOver=1;
            }
        }
        else
        {
            int tip;
            /* tip = 1 --> se adauga un stegulet
               tip = 0 --> se scoate un stegulet
            */
            plantare_stegulet(x,y,myBoard, tip);
            if (tip==1) steaguri_plantate++;
            else steaguri_plantate--;
        }
    }
    return;
}

// Functie care sa permita alegerea dificultatii jocului
// Dificultatea unui nivel consta in dimensiunile tablei de joc pentru nivelul respectiv

void alege_dificultate()
{
    /*NIVEL:
    INCEPATOR -->   9 * 9 = 81      celule si 10 bombe
    MEDIU -->       16 * 16 = 256   celule si 40 de bombe
    AVANSAT -->     24 * 24 = 576   celule si 99 de bombe
    */
    int level;
    cout<<"\tIntroduceti nivelul: \n\n";
    cout<<"Tastati 1 pentru INCEPATOR (9*9 celule si 10 bombe)\n";
    cout<<"Tastati 2 pentru MEDIU (16*16 celule si 40 bombe)\n";
    cout<<"Tastati 3 pentru AVANSAT (24*24 celule si 99 bombe)\n\n";
    // asigurati-va ca nivelul ales este corect
    int introducereNivel=0;
    do
    {
        introducereNivel++;
        if (introducereNivel==1)
            cout<<"Nivelul ales: ";
        else cout<<"Nivelul ales este gresit! Introduceti din nou: ";
        cin>>level;
        cout<<'\n';
    }
    while (level<=0 || level>3);
    switch(level) //in functie de nivel, seteaza dimensiunile tablei de joc
    {
    case 1:
        SIDE=9;
        NRBOMBE=10;
        break;
    case 2:
        SIDE=16;
        NRBOMBE=40;
        break;
    case 3:
        SIDE=24;
        NRBOMBE=99;
        break;
    }
    /*
    if (level==INCEPATOR) // egal cu 1
    {
        SIDE=9;
        BOMBE=10;
    }

    if (level==MEDIU) //egal cu 2
    {
        SIDE=16;
        BOMBE=40;
    }

    if (level==AVANSAT) //egal cu 3
    {
        SIDE=24;
        BOMBE=99;
    }*/
}

int main()
{
    /*
        Alegeti nivelul:

    INCEPATOR -->   9 * 9 = 81      celule si 10 bombe
    MEDIU -->       16 * 16 = 256   celule si 40 de bombe
    AVANSAT -->     24 * 24 = 576   celule si 99 de bombe

    */

    //4E - galben pe fundal rosu
    //4F - alb pe fundal rosu
    //7E - galben pe fundal gri
    //7F - gri pe fundal alb

    system("COLOR 4F");
    cout<<"\t\t\t  JOCUL MINESWEEPER\n\t\t\tPROIECT  DE  ATESTAT\n\n";
    alege_dificultate();
    play_Minesweeper();
    return 0;
}
