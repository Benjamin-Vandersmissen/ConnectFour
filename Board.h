//
// Created by Benjamin on 22/09/2017.
//

#ifndef CONNECTFOUR_BOARD_H
#define CONNECTFOUR_BOARD_H
#include <vector>
#include <stdexcept>
#include <iostream>
#include <random>
#include <numeric>

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 7;
const int BOARD_NRCOLORS = 2;

class Board {
private:
    std::vector<std::vector<int> > board;
public:
    Board();

    int operator()(int x, int y);

    bool isRowFree(int x);

    bool isRowFree(int x, int& y);

    bool setRow(int x, int color);

    bool setRow(int x, int color, int& y);

    bool connectFour(int x, int y);

    bool connectI(int x, int y, int i);

    bool connectI(int x, int y, int i, int color);

    bool willConnectI(int x, int y, int j, int color);

    int countPossibleI(int x, int y, int i, int color);

    int AIChance(int x, int y, int color);

    bool AImove(int color);

    bool isFull();
};

double random_range(int low, int high);

std::ostream& operator<<(std::ostream& stream, Board& b);

#endif //CONNECTFOUR_BOARD_H
