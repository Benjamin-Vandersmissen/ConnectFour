//
// Created by Benjamin on 23/09/2017.
//

#include "GameWindow.h"

void Piece::timeoutCB(void *v) {
    Piece& piece = *(Piece*)v;
    static int coords[2] = {0,0};
    switch (piece.state){
        case PIECE_STATES::preparingtofall:{
            /**
             * Bereken de rij en kolom voor het stuk
             * */
            int x = piece.x()-piece.w()/2;
            int d = (x-BOARD_X-PIECE_SEPERATION)/(PIECE_WIDTH+PIECE_SEPERATION);
            int e = (x-BOARD_X-PIECE_SEPERATION) % (PIECE_WIDTH+PIECE_SEPERATION);
            std::cout << e << std::endl;
            d = (x-BOARD_X-(d+1)*PIECE_SEPERATION)/PIECE_WIDTH;
            if (e > 0)
                d++;
            coords[0] = d;
            board.isRowFree(coords[0], coords[1]);
            coords[1] = BOARD_HEIGHT-coords[1]-1;
            piece.state = PIECE_STATES::falling;
        }

        case PIECE_STATES::falling :{
            /**
             * Controleer of het stuk nog kan vallen:
             * Zo wel : herhaal callback
             * Zo niet : update spelbord en verwijder instance
             * */
            if (piece.moveTo(BOARD_X + PIECE_SEPERATION + coords[0]*(PIECE_WIDTH+PIECE_SEPERATION), piece.y(), 5)){
                if (piece.moveTo(piece.x(), BOARD_Y+PIECE_SEPERATION + coords[1]*(PIECE_HEIGHT+PIECE_SEPERATION), 20)){
                    //right place
                    board.setRow(coords[0], piece.color);
                    std::cout << board << std::endl;
                    piece.window()->redraw();
                    CURRENT_COLOR = (CURRENT_COLOR+1)%BOARD_NRCOLORS;
                    piece.window()->add(new Piece(20,20, CURRENT_COLOR));
                    Fl::delete_widget(&piece);
                    break;
                }
            }
            Fl::repeat_timeout(0.05, Piece::timeoutCB, v);
            break;
        }
        case PIECE_STATES::idle :{
            /**
             * Controleer of het stuk op de beginplaats staat:
             * Zo wel  : doe niets
             * Zo niet : beweeg richting de originele plek
             * */
            if(piece.moveTo(piece.origX, piece.origY, 5))
                return;
            Fl::repeat_timeout(0.05, Piece::timeoutCB, v);
        }
        default:{
            break;
        }
    }
}

Piece::Piece(int x, int y, int color) : Fl_Widget(x, y, PIECE_WIDTH, PIECE_HEIGHT), origX(x), origY(y), color(color){

}

int Piece::handle(int event) {
    static int offset[2] = {0,0};
    switch(event){
        case FL_PUSH:{
            offset[0] = Fl::event_x() - x();
            offset[1] = Fl::event_y() - y();
            if (state != PIECE_STATES::falling) {
                state = PIECE_STATES::dragged;
                return 1;
            }
            return 0;
        }
        case FL_DRAG:{
            this->position(Fl::event_x() - offset[0], Fl::event_y() - offset[1]);
            this->window()->redraw();
            return 1;
        }
        case FL_RELEASE:{
            int x;
            x = this->x()+w()/2;
            if (y() < BOARD_Y && (x >= BOARD_X + PIECE_SEPERATION && x <= BOARD_X + BOARD_WIDTH*(PIECE_WIDTH+PIECE_SEPERATION))){
                int d = (x-BOARD_X)/(PIECE_WIDTH+PIECE_SEPERATION);
                d = (x-BOARD_X-(d+1)*PIECE_SEPERATION)/PIECE_WIDTH;
                bool a = board.isRowFree(d);
                if (a)
                    state = PIECE_STATES::preparingtofall;
            }
            else {
                state = PIECE_STATES::idle;
            }
            Fl::add_timeout(0.05, this->timeoutCB, this);
            break;
        }
    }
    return 0;
}

void Piece::draw() {
    fl_rectf(x(), y(), w(), h(), PIECE_COLORS[color]);
}

bool Piece::moveTo(int X, int Y, int speed) {
    if (x() == X && y() == Y){
        return true;
    }
    double angle = atan2(x()- X, y()- Y);
    int dx, dy;
    dx = (std::abs(std::sin(angle)*speed) > std::abs(x()-X)) ? x()-X : std::sin(angle)*speed;
    dy = (std::abs(std::cos(angle)*speed) > std::abs(y()-Y)) ? y()-Y : std::cos(angle)*speed;
    position(x()-dx, y()-dy);
    window()->redraw();
    return false;
}

GameWindow::GameWindow(int W, int H, const char *l) : Fl_Double_Window(W, H, l) {}

void GameWindow::draw() {
    Fl_Window::draw();
    for(int i = 0; i <= BOARD_WIDTH; i++){
        fl_rectf(BOARD_X + i*(PIECE_WIDTH+PIECE_SEPERATION), BOARD_Y, PIECE_SEPERATION, (BOARD_HEIGHT)*(PIECE_HEIGHT+PIECE_SEPERATION)+PIECE_SEPERATION, FL_BLUE);
    }
    for(int i = 0; i <= BOARD_HEIGHT; i++) {
        fl_rectf(BOARD_X, BOARD_Y + i * (PIECE_HEIGHT + PIECE_SEPERATION), (BOARD_WIDTH) * (PIECE_HEIGHT + PIECE_SEPERATION), PIECE_SEPERATION, FL_BLUE);
    }
    for(int x = 0; x < BOARD_WIDTH; x++){
        for(int y = 0; y < BOARD_HEIGHT; y++){
            if (board(x,y) != -1)
                fl_rectf(BOARD_X+PIECE_SEPERATION + x*(PIECE_WIDTH+PIECE_SEPERATION), BOARD_Y+PIECE_SEPERATION + (BOARD_HEIGHT-y-1)*(PIECE_HEIGHT+PIECE_SEPERATION), PIECE_WIDTH, PIECE_HEIGHT, PIECE_COLORS[board(x,y)]);
        }
    }
}
