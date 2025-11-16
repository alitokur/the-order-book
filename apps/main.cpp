
#include "../include/order_book.h"

int main (int argc, char *argv[]) {

    OrderBook book;
    book.add_order(1, Side::Buy, 10, 100, 0, 0);
    book.add_order(2, Side::Buy, 10, 100, 0, 0);
    book.add_order(3, Side::Buy, 10, 100, 0, 0);
    book.add_order(4, Side::Sell, 40, 100, 0, 0);
    book.modify_order(4,5, 80, 400 , Side::Sell,  0, 0);
    book.display_orders();

    return 0;
}
