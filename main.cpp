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
const unsigned int KNbCandies = 6;
const unsigned Kimpossible = 0;
int coup = 0;
int score = 0;


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


void initGrid(mat &grid, size_t matSize) {
    srand(time(0));
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

/*
void  displayGrid (const mat & grid){
    clearScreen ();
    couleur(KReset);
    vector<unsigned> couleurs ={KNoir,KRouge,KVert,KJaune,KBleu,KMAgenta,KCyan};
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid[x].size(); ++y) {
            if(grid[x][y] <= 0 or grid[x][y] > KNbCandies){
                cout<<"    ";
            }else{
                cout<<"";
                couleur (couleurs[grid[x][y]]);
                cout<<" "<<grid[x][y]<<" ";
                couleur (KReset);
            }
        }
        cout<<endl;
    }
}
*/

void  displayGrid (const mat & grid , maposition & pos){
    clearScreen ();
    couleur(KReset);
    vector<unsigned> couleurs ={KNoir,KRouge,KVert,KJaune,KBleu,KMAgenta,KCyan};
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid[x].size(); ++y) {
            if(grid[x][y] <= 0 or grid[x][y] > KNbCandies){
                cout<<"  / ";
            }else{
                cout<<" ";
                couleur (couleurs[grid[x][y]]);
                if(pos.ord==x and pos.abs==y){
                   cout<<"["<<grid[x][y]<<"]";
                }else{
                cout<<" "<<grid[x][y]<<" ";
                }
                couleur (KReset);
            }
        }
        cout<<endl;
    }
}

void makeAMove (mat & grid, const maposition & pos, const char & direction){

    if (pos.abs >= 0 && pos.abs < grid.size() && pos.ord >= 0 && pos.ord < grid[pos.abs].size()){

        if(tolower(direction) == 's' && pos.ord < grid.size() - 1){ ++coup; swap(grid[pos.ord][pos.abs],grid[pos.ord + 1][pos.abs]);

        }else if(tolower(direction) == 'z' && pos.ord > 0){ ++coup; swap(grid[pos.ord][pos.abs],grid[pos.ord - 1][pos.abs]);

        }else if(tolower(direction) == 'a' && pos.abs > 0){ ++coup; swap(grid[pos.ord][pos.abs],grid[pos.ord][pos.abs - 1]);

        }else if(tolower(direction) == 'e' && pos.abs < grid.size()-1){ ++coup; swap(grid[pos.ord][pos.abs],grid[pos.ord][pos.abs + 1]);

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

void menu(const string & texte){
    cout<<"+";
    for(size_t i = 0; i < texte.size();++i){
        cout<<"-";
    }
    cout<<"+"<<endl<<"|";
    cout<<texte<<"|"<<endl;
    cout<<"+";
    for(size_t i = 0; i < texte.size();++i){
        cout<<"-";
    }
    cout<<"+"<<endl;
}

int main(){
    menu("    CANDYCROUS  \n|\n|  fait par \n|  Jimmy LEFEVBRE et Lilian CHEVREMONT \n|\n| bonne partie     sae 1.01");
    this_thread::sleep_for(chrono::seconds(2));

    //création de la grille
    mat grid;
    size_t taille = 0;

    while(taille < 3){
        clearScreen ();
        menu("veuillez choisir une taille");
        cin >>taille;
        if(taille < 3){
            cout<<"taille trop petite"<<endl;
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    initGrid(grid,taille);
    const int KmaxTimes = (taille*taille)/3;

    maposition pos;
    unsigned howMany;
    char moveInstr;

    while(KmaxTimes >= coup){

        displayGrid(grid,pos);

        menu("coup : "+to_string(coup)+"/"+to_string(KmaxTimes)+"   score:"+to_string(score));

        menu("veuillez selectionner une abscisse");
        cin >>pos.abs;

        menu("veuillez selectionner une ordonnée");
        cin >>pos.ord;

        pos.ord -= 1;
        pos.abs -= 1;

        menu("veuillez selectionner un mouvement via les touches a(droite),e(gauche),z(haut),s(bas)");
        cin >>moveInstr;

        makeAMove(grid,pos,moveInstr);

        if(atLeastThreeInARow(grid,pos,howMany)) removalInRow(grid,pos,howMany);
        if(atLeastThreeInAColumn(grid,pos,howMany)) removalInColumn(grid,pos,howMany);

    }
    return 0;
}
