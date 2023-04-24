#include "bookstore.h"

bool operator<(const Bookstore& lhs, const Bookstore& rhs) { return lhs.uuid < rhs.uuid; }

Bookstore::~Bookstore() {
    for (auto& book : books) {
        book.first->remove_from_bookstore(weak_from_this());
    }
}
Bookstore::Bookstore(std::string name, std::uint32_t uuid) : name(name), uuid(uuid), books() {}
Bookstore::Bookstore(Bookstore&& rhs)
    : name(std::move(rhs.name)), uuid(rhs.uuid), books(std::move(rhs.books)) {
    for (auto& [book_ptr, num] : books) {
        Bookstore::move_bookstore_in_book(*book_ptr, rhs.weak_from_this(), weak_from_this());
    }
}

Bookstore& Bookstore::operator=(Bookstore&& rhs) {
    name = std::move(rhs.name);
    uuid = rhs.uuid;
    books = std::move(rhs.books);
    rhs.books.clear();
    for (auto& [book_ptr, num] : books) {
        Bookstore::move_bookstore_in_book(*book_ptr, rhs.weak_from_this(), weak_from_this());
    }
    return *this;
}

void Bookstore::move_bookstore_in_book(SalableBook& book,
                                       std::weak_ptr<const Bookstore> old_bookstore,
                                       std::weak_ptr<const Bookstore> new_bookstore) {
    // update the (smart) pointer points to the bookstore in the book
    auto nh = book.num_in_bookstores.extract(old_bookstore);
    nh.key() = new_bookstore;
    book.num_in_bookstores.insert(std::move(nh));
}

Bookstore& Bookstore::add_book(SalableBook&& book, unsigned int num) & {
    auto book_ptr = std::make_shared<SalableBook>(std::move(book));
    return add_book(book_ptr, num);
}

Bookstore& Bookstore::add_book(std::shared_ptr<SalableBook> book, unsigned int num) & {
    books[book] += num;
    book->increase_num_in_bookstore(weak_from_this());
    return *this;
}

Bookstore&& Bookstore::add_book(SalableBook&& book, unsigned int num) && {
    auto book_ptr = std::make_shared<SalableBook>(std::move(book));
    return std::move(add_book(book_ptr, num));
}

Bookstore&& Bookstore::add_book(std::shared_ptr<SalableBook> book, unsigned int num) && {
    return std::move(add_book(book, num));
}