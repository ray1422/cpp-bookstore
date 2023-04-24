#include <iostream>

#include "bookstore.h"
int main() {
    using namespace std;
    auto store_1 = Bookstore("store_1", 1);
    auto store_2 = Bookstore("store_2", 2);
    auto store_3_ptr = new Bookstore("store_3", 3);
    {
        SalableBook book_1(Book("isbn_1", "title_1", "author_1", "content_1"));
        SalableBook book_2(Book("isbn_2", "title_2", "author_2", "content_2"));
        store_3_ptr->add_book(SalableBook(Book("r_isbn_1", "title_1", "author_1", "content_1")), 1);
        store_3_ptr->add_book(std::move(book_2));
    }
    auto book_5 = SalableBook(Book("isbn_5", "title_5", "author_5", "content_5"));
    delete store_3_ptr;
    auto new_bookstore = Bookstore("new_bookstore", 4).add_book(std::move(book_5), 1);
}