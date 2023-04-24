#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <string>

class Book;
class SalableBook;
class Bookstore;

#include "book.h"

template <class T>
bool operator<(const std::weak_ptr<const Bookstore>& lhs,
               const std::weak_ptr<const Bookstore>& rhs) {
    return lhs.lock() < rhs.lock();
}

class Bookstore : std::enable_shared_from_this<Bookstore> {
   private:
    static void move_bookstore_in_book(SalableBook& book,
                                       std::weak_ptr<const Bookstore> old_bookstore,
                                       std::weak_ptr<const Bookstore> new_bookstore);

   public:
    virtual ~Bookstore();
    std::string name;
    std::uint32_t uuid;
    // move constructor
    Bookstore(Bookstore&&);
    // move assignment
    Bookstore& operator=(Bookstore&&);
    Bookstore(std::string name, std::uint32_t uuid);

    Bookstore& add_book(SalableBook&& book, unsigned int num = 1) &;
    Bookstore& add_book(std::shared_ptr<SalableBook>, unsigned int num = 1) &;

    Bookstore& add_book(SalableBook&& book, unsigned int num = 1) &&;
    Bookstore& add_book(std::shared_ptr<SalableBook>, unsigned int num = 1) &&;

    friend bool operator<(const Bookstore& lhs, const Bookstore& rhs);
    std::map<std::shared_ptr<SalableBook>, unsigned int> books;
};