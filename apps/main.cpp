
#include "../include/order_book.h"

int main (int argc, char *argv[]) {

    OrderBook book;
    book.add_order(1, Side::Buy, 1, 100, 0, 0);
    book.add_order(2, Side::Buy, 2, 200, 0, 0);
    book.add_order(3, Side::Sell, 3, 300, 0, 0);
    book.add_order(5, Side::Sell, 29, 400, 0, 0);
    book.add_order(6, Side::Sell, 29, 500, 0, 0);
    book.add_order(7, Side::Sell, 55, 600, 0, 0);
    book.add_order(8, Side::Sell, 3, 240, 0, 0);
    book.add_order(9, Side::Sell, 29, 250, 0, 0);
    book.add_order(10, Side::Buy, 9, 900, 0, 0);
    book.add_order(11, Side::Buy, 21, 700, 0, 0);
    book.add_order(12, Side::Buy, 13, 10, 0, 0);
    book.add_order(13, Side::Buy, 34, 920, 0, 0);
    book.add_order(14, Side::Buy, 12, 290, 0, 0);
    book.add_order(15, Side::Buy, 12, 290, 0, 0);
    book.add_order(16, Side::Buy, 12, 290, 0, 0);
    
    book.display_orders();
    
    book.cancel_order(1);
    book.cancel_order(2);
    book.cancel_order(3);
    book.cancel_order(5);
    book.cancel_order(6);
    book.cancel_order(7);
    book.cancel_order(8);
    book.cancel_order(9);
    book.cancel_order(10);
    book.cancel_order(11);
    book.cancel_order(12);
    book.cancel_order(13);
    book.cancel_order(14);
    book.display_orders();
    book.cancel_order(15);
    book.display_orders();

    return 0;
}
