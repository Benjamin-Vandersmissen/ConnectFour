#include <iostream>
#include "Board.h"
#include "GameWindow.h"
int main() {
//    Board b;
//    std::cout << b << std::endl;
//    while(!b.isFull()) {
//        bool a = b.AImove(0);
//        std::cout << b << std::endl;
//        if (a) {
//            std::cout << "De AI wint" << std::endl;
//            break;
//        }
//        int x, y;
//        std::cin >> x;
//        b.setRow(x, 1, y);
//        std::cout << b << std::endl;
//        if (b.connectFour(x, y)) {
//            std::cout << "De Speler wint" << std::endl;
//            break;
//        }
//    }
    GameWindow window(1000,1000);
    window.add(new Piece(20, 20, 0));
    window.show();
    return Fl::run();
}