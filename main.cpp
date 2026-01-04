#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KNoir    (40);
const unsigned KRouge   (41);
const unsigned KVert    (42);
const unsigned KJaune   (43);
const unsigned KBleu    (44);
const unsigned KMAgenta (45);
const unsigned KCyan    (46);
vector<unsigned> couleurs ={KNoir,KRouge,KVert,KJaune,KBleu,KMAgenta,KCyan};
const unsigned int KNbCandies = 6;
const unsigned Kimpossible = 0;
int coup = 0;
int score = 0;
bool infinite = false;

int test = 1;
typedef vector <unsigned> line; // un type représentant une ligne de la grille
typedef vector <line> mat; // un type représentant la grille

struct maposition {
    unsigned abs;
    unsigned ord;
}; // une position dans la girlle


void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

int getEntier(){
    while(true){
        string str;
        getline(cin,str);
        int intRenvoye;
        if(not str.empty() && (isdigit(str[0]) || (isdigit(str[0]) and isdigit(str[1])))){
            intRenvoye=stoi(str);
            return intRenvoye;
        }else{
            cout<<"la valeur n'est pas valide"<<endl;
        }
    }
}

char getChar(){
    while(true){
        string str;
        getline(cin,str);
        char charRenvoye;
        if(not str.empty() && not isdigit(str[0])){
            charRenvoye = str[0];
            return charRenvoye;
        }else{
            cout<<"la valeur n'est pas valide"<<endl;
        }
    }
}

void initGrid(mat &grid, size_t matSize) {
    grid.resize(matSize);
    for (size_t x = 0; x < matSize; ++x) {
        grid[x].resize(matSize);
    }
    for (size_t x = 0; x < matSize; ++x) {
        for (size_t y = 0; y < matSize; ++y) {
            grid[x][y] = rand() % KNbCandies + 1;
        }
    }
}

void  displayGrid (const mat & grid , maposition & pos){
    clearScreen ();
    couleur(KReset);
    cout<<"O/A";
    for (size_t x = 1; x <= grid.size(); ++x) {
        if(x < 10){
            cout<<"  "<<x<<" ";
        }else{
            cout<<" "<<x<<" ";
        }
    }
    cout<<endl<<endl;
    for (size_t x = 0; x < grid.size(); ++x) {
        if(x+1 < 10){
            cout<<x+1<<"  ";
        }else{
            cout<<x+1<<" ";
        }
        for (size_t y = 0; y < grid[x].size(); ++y) {
            if(grid[x][y] <= 0 or grid[x][y] > KNbCandies){
                if(pos.ord==x and pos.abs==y){
                    cout<<" [/]";
                }else{
                cout<<"  / ";
                }
            }else{
                cout<<" ";
                couleur (couleurs[grid[x][y]]);
                if(pos.ord==x and pos.abs==y){
                   cout<<">"<<grid[x][y]<<"<";
                }else{
                cout<<" "<<grid[x][y]<<" ";
                }
                couleur (KReset);
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

void makeAMove (mat & grid, const maposition & pos, const char & direction){

    if (pos.abs < grid.size() && pos.ord < grid[pos.abs].size()){
        ++coup;
        if(tolower(direction) == 's' && pos.ord < grid.size() - 1){
            swap(grid[pos.ord][pos.abs],grid[pos.ord + 1][pos.abs]);
        }else if(tolower(direction) == 'z' && pos.ord > 0){
            swap(grid[pos.ord][pos.abs],grid[pos.ord - 1][pos.abs]);
        }else if(tolower(direction) == 'a' && pos.abs > 0){
            swap(grid[pos.ord][pos.abs],grid[pos.ord][pos.abs - 1]);
        }else if(tolower(direction) == 'e' && pos.abs < grid.size()-1){
            swap(grid[pos.ord][pos.abs],grid[pos.ord][pos.abs + 1]);
        }else{
            cout<<"key error"<<endl;
            this_thread::sleep_for(chrono::seconds(2));
        }
    }else{
        cout<<"mauvais indice"<<endl;
        this_thread::sleep_for(chrono::seconds(2));
    }
}

bool atLeastThreeInAColumn(const mat &grid, maposition &pos, unsigned &howMany) {
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid.size(); ++y) {
            howMany = 1;
            size_t posEltVerifie = x +1;
            while (posEltVerifie < grid.size()) {
                if(grid[x][y] != grid[posEltVerifie][y] or grid[x][y] == 0) break;
                ++howMany;
                ++posEltVerifie;
            }
            if(howMany >= 3){
                pos.abs = y;
                pos.ord = x;
                return true;
            }
        }
    }
    return false;
}

bool atLeastThreeInARow(const mat &grid, maposition &pos, unsigned &howMany) {
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid.size(); ++y) {
            howMany = 1;
            size_t posEltVerifie = y +1;
            while (posEltVerifie < grid.size()) {
                if(grid[x][y] != grid[x][posEltVerifie] or grid[x][y] == 0) break;
                ++howMany;
                ++posEltVerifie;
            }
            if(howMany >= 3){
                pos.abs = y;
                pos.ord = x;
                return true;
            }
        }
    }
    return false;
}

void removalInColumn (mat & grid, const maposition & pos, unsigned  howMany){
    size_t y;
    for(size_t i = 0; i < howMany; ++i){
        y = pos.ord;
        grid[y][pos.abs] = Kimpossible;
        while(true){
            if(y + 1 >= grid.size() or grid[y + 1][pos.abs] == 0) break;
            swap(grid[y][pos.abs],grid[y+1][pos.abs]);
            ++y;
        }
        ++score;
    }
}

void removalInRow (mat & grid, const maposition & pos, unsigned  howMany){
    for(unsigned int i = 0; i < howMany; ++i){
        removalInColumn (grid, maposition {pos.abs + i,pos.ord}, 1);
    }
}

void displayAMessage(const string & texte,const string & title,size_t size){
    if(size == 0)size = texte.size();
    cout<<"#="<<title;
    for(size_t i = 0; i < size-title.size()-1;++i){
        cout<<"=";
    }
    cout<<"#"<<endl<<"|";
    cout<<texte<<"|"<<endl;
    cout<<"#";
    for(size_t i = 0; i < size;++i){
        cout<<"=";
    }
    cout<<"#"<<endl;
}

void completeGrid(mat & grid){
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid.size(); ++y) {
            if(grid[x][y]==0){
            grid[x][y] = rand() % KNbCandies + 1;
            }
        }
    }
}


int main(){
    srand(time(0));

    mat grid;
    size_t taille = 0;
    maposition pos;

    pos.abs=0;
    pos.ord=0;

    unsigned Mode;
    int Times = 0;
    unsigned howMany;
    char moveInstr;

    displayAMessage("\n|\tfait par\n|\tJimmy LEFEVBRE et Lilian CHEVREMONT G2A1\n|\n|\tbonne partie\t\tsae 1.01\n","CANDYcrous",47);
    this_thread::sleep_for(chrono::seconds(2));

    while (true) {
        clearScreen();
        displayAMessage("\n|\tChoisissez votre difficultee\n|\t1 - FACILE\n|\t2 - NORMAL\n|\t3 - DIFFICILE\t\n|\t4 - pour un rappel des regles\t\n","Menu",39);
        Mode=getEntier();
        if(Mode == 4){
            displayAMessage("Les regles sont simples;\n\t-Tout d'abord vous devez choisir votre mode de jeux\n\t-Ensuite vous verrez une grille de chiffres de couleur\n\t-Ceux-ci disparaissent lorsque vous en alignez 3 ou plus\n\t-Lorsque vous les faites disparaitre, vous gagnez des points\n\t-Votre objectif est d'obtenir le score le plus haut possible, bonne partie!\n","CANDYcrous",40);
            this_thread::sleep_for(chrono::seconds(3));
        }else if(Mode == 1){
            taille = 6;
            Times = (taille*taille)/2;
            break;
        }else if(Mode == 2){
            taille = 12;
            Times = (taille*taille)/4;
            break;
        }else if(Mode == 3){
            taille = 24;
            Times = (taille*taille)/6;
            break;
        }
    }

    while (true) {
        clearScreen();
        displayAMessage("\n|\tChoisissez votre mode de jeux\n|\t1 - NORMAL\n|\t2 - INFINI\n|\n","Menu",39);
        Mode=getEntier();
        if(Mode == 1){
            break;
        }else if(Mode == 2){
            infinite = true;
            break;
        }
    }

    initGrid(grid,taille);
    const int KmaxTimes = Times;

    while(KmaxTimes > coup){
        displayGrid(grid,pos);

        displayAMessage("coup : "+to_string(coup)+"/"+to_string(KmaxTimes)+"   score:"+to_string(score),"",0);

        displayAMessage("Veuillez selectionner une abscisse","",0);
        pos.abs=getEntier();
        --pos.abs;
        displayGrid(grid,pos);

        displayAMessage("coup : "+to_string(coup)+"/"+to_string(KmaxTimes)+"   score:"+to_string(score),"",0);
        displayAMessage("Veuillez selectionner une ordonnee","",0);
        pos.ord=getEntier();
        --pos.ord;
        displayGrid(grid,pos);

        displayAMessage("coup : "+to_string(coup)+"/"+to_string(KmaxTimes)+"   score:"+to_string(score),"",0);
        displayAMessage("Veuillez selectionner un mouvement via les touches a(gauche),e(droite),z(haut),s(bas),/(pour arreter)","",0);
        moveInstr = getChar();
        if(moveInstr == '/'){
            break;
        }

        makeAMove(grid,pos,moveInstr);

        while(atLeastThreeInARow(grid,pos,howMany) or atLeastThreeInAColumn(grid,pos,howMany)){
            if(atLeastThreeInARow(grid,pos,howMany)){
                removalInRow(grid,pos,howMany);
            }
            else if(atLeastThreeInAColumn(grid,pos,howMany)){
                removalInColumn(grid,pos,howMany);
            }
        }
        if(infinite){
            completeGrid(grid);
        }
    }
    displayAMessage("\n|\tVous avez fini votre partie avec "+to_string(coup)+" coups ce qui vous fait un score de "+to_string(score)+"\t|\n|\n|\t (n'hesitez pas a tester d'autres difficultees)\n","!FINI!",0);
    return 0;
}
