//
// Created by Benjamin on 22/09/2017.
//

#include "Board.h"

Board::Board() {
    board = std::vector<std::vector<int>>(WIDTH, std::vector<int>(HEIGHT, -1));
}

int Board::operator()(int x, int y) {
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        throw std::out_of_range{"Board::operator()"};
    return board[x][y];
}

bool Board::isRowFree(int x) {
    int temp;
    return isRowFree(x, temp);
}

bool Board::isRowFree(int x, int &y) {
    if (x < 0 || x >= WIDTH)
        throw std::out_of_range{"Board::isRowFree"};
    y = 0;
    while(y < HEIGHT && (*this)(x,y) != -1){
        y++;
    }
    return y != HEIGHT;
}

bool Board::setRow(int x, int color) {
    int temp;
    return setRow(x, color, temp);
}

bool Board::setRow(int x, int color, int &y) {
    if (isRowFree(x, y)){
        board[x][y] = color;
        return true;
    }
    return false;
}

bool Board::connectFour(int x, int y) {
    return connectI(x, y, 4);
}

bool Board::connectI(int x, int y, int j) {
    int color = (*this)(x,y);
    return connectI(x, y, j, color);
}

bool Board::connectI(int x, int y, int j, int color) {
    bool success = false;

    int sequence = 0;
    //horizontal
    for(int i = std::max(0, x - (j-1)); i < std::min(WIDTH-1, x + j); i++){
        if ((*this)(i, y) == color){
            sequence ++;
        }
        else{
            sequence = 0;
            if (WIDTH - i < j)
                break;
        }
        if (sequence == j)
            success = true;
    }
    sequence = 0;
    //vertical
    for(int i = std::max(0, y - (j-1)); i < std::min(HEIGHT-1, y + j); i++){
        if ((*this)(x, i) == color){
            sequence ++;
        }
        else{
            sequence = 0;
            if (HEIGHT - i < j)
                break;
        }
        if (sequence == j)
            success = true;
    }
    sequence = 0;
    //diagonal, going upward
    for(int i = -std::min(std::min(x, j), std::min(y, j)); x+i < WIDTH && y+i < HEIGHT; i++){
        if ((*this)(x+i, y+i) == color){
            sequence ++;
        }
        else{
            sequence = 0;
            if (WIDTH-(x+i) < j || HEIGHT-(y+i) < j)
                break;
        }
        if (sequence == j)
            success = true;
    }
    sequence = 0;
    //diagonal going downward
    for(int i = -std::min(std::min(x,j), std::min(HEIGHT-y-1, j)); x+i < WIDTH && y-i >= 0; i++){
        if ((*this)(x+i,y-i) == color){
            sequence++;
        }
        else{
            sequence = 0;
            if (WIDTH-(x+i) < j || (y-i) < j)
                break;
        }
        if (sequence == j)
            success = true;
    }
    return success;
}

int Board::AIChance(int x, int y, int color) {
    int chance = 0;

    std::vector<int> colors = std::vector<int>(NRCOLORS);
    std::iota(colors.begin(), colors.end(), 0); //fill the vector with sequential numbers starting from zero
    for(int tempcolor : colors) {
        if (color == tempcolor)
            continue;
        if (willConnectI(x, y, 4, tempcolor)) {
            /**
             * de ander kan 4 op een rij halen door op deze plek iets te zetten
             * */
            chance += 300;
        } else if (willConnectI(x, y, 3, tempcolor)) {
            /**
             * de ander kan 3 op een rij halen door op deze plek iets te zetten, maar geen 4 op een rij
             * controleer of door hier te plaatsen de ander niet 4 op een rij kan halen op de positie erboven
             * */
            chance += 60 * (y < HEIGHT - 1 ? !willConnectI(x, y + 1, 4, tempcolor) : 1);
        } else if (willConnectI(x, y, 2, tempcolor)) {
            /**
             * de ander kan 2 op een rij halen door op deze plek iets te zetten, maar geen 3 of 4 op een rij
             * controleer of door hier te plaatsen de ander niet 4 op een rij kan halen op de positie erboven
             * */
            chance += 20 * (y < HEIGHT - 1 ? !willConnectI(x, y + 1, 4, tempcolor) : 1);
        }
    }
    chance = chance / 2 + chance%2; //afronden naar boven;
    if (willConnectI(x, y, 4, color)){
        /**
         * de AI kan 4 op een rij halen door op deze plek iets te zetten
         * */
        chance += 400;
    }
    else if (willConnectI(x, y, 3, color)){
        /**
         * de AI kan 3 op een rij halen door op deze plek iets te zetten, maar geen directe 4 op een rij
         * per mogelijkheid op 4 op een rij verhoogt de kans, maar de kans verlaagt als er geen mogelijkheid is
         * controleer of door hier te plaatsen de ander niet 4 op een rij kan halen op de positie erboven
         * */
        int tempchance  = 80 + (countPossibleI(x, y, 4, color) -1) * 20;
        chance += tempchance * (y < HEIGHT-1 ? !willConnectI(x, y+1, 4, !color) : 1);
    }
    else if (willConnectI(x, y, 2, color)){
        /**
         * de AI kan 2 op een rij halen door op deze plek iets te zetten, maar geen directe 3 of 4 op een rij
         * per mogelijkheid op 4 op een rij verhoogt de kans, maar de kans verlaagt als er geen mogelijkheid is
         * controleer of door hier te plaatsen de ander niet 4 op een rij kan halen op de positie erboven
         * */
        int tempchance  = 40 + (countPossibleI(x, y, 4, color) -1) * 10;
        chance += tempchance * (y < HEIGHT-1 ? !willConnectI(x, y+1, 4, !color) : 1);
    }
    else{
        /**
         * de AI kan hier iets zetten
         * per mogelijkheid op 4 op een rij verhoogt de kans, maar de kans verlaagt als er geen mogelijkheid is
         * controleer of door hier te plaatsen de ander niet 4 op een rij kan halen op de positie erboven
         * */
        int tempchance  = 10 + (countPossibleI(x, y, 4, color) -1) * 5;
        chance += tempchance * (y < HEIGHT-1 ? !willConnectI(x, y+1, 4, !color) : 1);
    }

//    chance += 100 * connectI(x, y, 4, !color);
//    chance += 80 * countPossibleI(x,y,4,color)*(connectI(x, y, 3, color) && !connectI(x, y, 4, color));
//    chance += 60 * (connectI(x, y, 3, !color) && !connectI(x, y, 4, !color));
//    chance += 40 * countPossibleI(x,y,4,color)*(connectI(x, y, 2, color) && !connectI(x, y, 3, color) && !connectI(x, y, 4, color));
//    chance += 20 * (connectI(x, y, 2, !color) && !connectI(x, y, 3, !color) && !connectI(x, y, 4, !color));
//    chance += 10 * countPossibleI(x,y,4,color)*(connectI(x, y, 1, color) && !connectI(x, y, 2, color) && !connectI(x, y, 3, color) && !connectI(x, y, 4 ,color));

    chance *= random_range(90,110)/100.0;
    return chance;
}

bool Board::AImove(int color) {
    std::vector<std::pair<int,int>> chances(WIDTH, {0,-1});
    int maxChance = -1;
    int pos = 0;
    for(int x = 0; x < WIDTH; x++){
        if(isRowFree(x, chances[x].first)){
            chances[x].second = AIChance(x, chances[x].first, color);
        }
        if (chances[x].second >= maxChance){
            maxChance = chances[x].second;
            pos = x;
        }
        std::cout << chances[x].second << ' ';
    }
    std::cout << std::endl;
    setRow(pos, color);
    return (connectFour(pos, chances[pos].first));
}

int Board::countPossibleI(int x, int y, int j, int color) {
    int possibilities = (x <= WIDTH-j) + (x >= j-1) + (y <= HEIGHT - j) + (y >= j-1) +
            (x <= WIDTH - j && y <= HEIGHT - j) + (x <= WIDTH - j && y >= j-1) + (x >= j-1 && y <= HEIGHT - j) +
            (x >= j-1 && y >= j-1);
    return possibilities;
}

bool Board::willConnectI(int x, int y, int j, int color) {
    int temp = board[x][y];
    board[x][y] = color;
    bool retValue = connectI(x, y, j, color);
    board[x][y] = temp;
    return retValue;
}

bool Board::isFull() {
    bool retvalue = true;
    for(int i = 0; i < WIDTH; i++){
        retvalue = retvalue && (*this)(i, HEIGHT-1) != -1;
    }
    return retvalue;
}

std::ostream &operator<<(std::ostream &stream, Board &b) {
    for(int y = HEIGHT-1; y >= 0; y--){
        stream << '|';
        for(int x = 0; x < WIDTH; x++){
            if (b(x,y) == -1){
                stream << ' ';
            }
            else{
                stream << b(x,y);
            }
            stream << '|';
        }
        stream << std::endl;
    }
    return stream;
}

double random_range(int low, int high) {
    return low + rand() %(high-low);
}
