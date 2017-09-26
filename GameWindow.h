//
// Created by Benjamin on 23/09/2017.
//

#ifndef CONNECTFOUR_WINDOW_H
#define CONNECTFOUR_WINDOW_H

#include "Board.h"
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/fl_draw.H"

static Board board;

enum PIECE_STATES{
    idle,
    dragged,
    preparingtofall,
    falling
};

const int PIECE_WIDTH = 64;
const int PIECE_HEIGHT = 64;
const int PIECE_SEPERATION = 16;
const int BOARD_X = 100;
const int BOARD_Y = 100;
static int CURRENT_COLOR = 0;

const Fl_Color PIECE_COLORS[BOARD_NRCOLORS] = {FL_RED, FL_YELLOW};

class Piece : public Fl_Widget{
private:
    int color;
    int state = PIECE_STATES::idle;

    static void timeoutCB(void *v);

    int origX, origY;

public:
    Piece(int x, int y, int color);

    int handle(int event);

    void draw();

    bool moveTo(int X, int Y, int speed);
};

class GameWindow  : public Fl_Double_Window{
public:
    GameWindow(int W, int H, const char *l = 0);

    void draw();
};


#endif //CONNECTFOUR_WINDOW_H
