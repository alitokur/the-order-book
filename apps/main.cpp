
#include "../include/order_book.h"

int main (int argc, char *argv[]) {

    OrderBook book;
    book.add_order(1, Side::Buy, 1, 100, 0, 0);
    book.add_order(2, Side::Buy, 2, 200, 0, 0);
    book.add_order(3, Side::Sell, 3, 300, 0, 0);

    return 0;
}
