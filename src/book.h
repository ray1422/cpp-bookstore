#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
class Book;
class SalableBook;
class Bookstore;

#include "bookstore.h"

class Book : public std::enable_shared_from_this<Book> {
   public:
    const std::string& isbn() const;
    const std::string& title() const;
    const std::string& author() const;
    const std::string& content() const;
    Book(std::string isbn, std::string title, std::string author, std::string content);

    explicit Book(const Book&);
    // move constructor
    explicit Book(Book&&);
    // move assignment
    Book& operator=(Book&&);

    Book& setContent(std::string&& s);
    Book& setContent(std::string s);
    virtual ~Book();
    friend bool operator<(const Book& lhs, const Book& rhs);

   private:
    std::string isbn_;
    std::string title_;
    std::string author_;
    std::shared_ptr<std::string> content_ptr_;
};

class SalableBook : public Book {
   public:
    ~SalableBook() = default;
    // copy constructor
    SalableBook(const SalableBook& book);
    // copy assignment
    SalableBook& operator=(const SalableBook& book);
    // move constructor
    SalableBook(SalableBook&& book);
    // move assignment
    SalableBook& operator=(SalableBook&& book);

    explicit SalableBook(Book&& book);
    explicit SalableBook(const Book& book);
    SalableBook& remove_from_bookstore(std::weak_ptr<const Bookstore> bookstore);
    SalableBook& set_num_in_bookstore(std::weak_ptr<const Bookstore> bookstore, unsigned int num);
    SalableBook& increase_num_in_bookstore(std::weak_ptr<const Bookstore> bookstore);
    SalableBook& decrease_num_in_bookstore(std::weak_ptr<const Bookstore> bookstore);
    std::shared_ptr<SalableBook> shared_from_this();
    std::weak_ptr<SalableBook> weak_from_this();

   private:
    std::map<std::weak_ptr<const Bookstore>, unsigned int, std::owner_less<>> num_in_bookstores;
    friend class Bookstore;
};