#include "book.h"

#include <iostream>
using std::string;
Book::~Book() { std::cout << "Book '" << isbn_ << "' is destroyed" << std::endl; }
Book::Book(string isbn, string title, string author, string content)
    : isbn_(isbn),
      title_(title),
      author_(author),
      content_ptr_(std::make_shared<string>(content)) {}

Book::Book(Book&& book)
    : enable_shared_from_this(),
      title_(std::move(book.title_)),
      author_(std::move(book.author_)),
      content_ptr_(std::move(book.content_ptr_)) {
    isbn_ = std::move(book.isbn_);
    using std::cout;
    using std::endl;
    cout << "book " << isbn_ << " moved constructed" << endl;
}

Book& Book::operator=(Book&& book) {
    isbn_ = std::move(book.isbn_);
    title_ = std::move(book.title_);
    author_ = std::move(book.author_);
    content_ptr_ = std::move(book.content_ptr_);
    return *this;
}

Book::Book(const Book& book)
    : enable_shared_from_this(),
      isbn_(book.isbn_),
      title_(book.title_),
      author_(book.author_),
      content_ptr_(std::make_shared<string>(book.content())) {
    using std::cout;
    using std::endl;
    cout << "book " << isbn_ << " copied constructed" << endl;
}

Book& Book::setContent(string&& s) {
    *content_ptr_ = s;
    return *this;
}

Book& Book::setContent(string s) {
    *content_ptr_ = std::move(s);
    return *this;
}

// copy constructor for SalableBook
SalableBook::SalableBook(const SalableBook& rhs)
    : Book(rhs), num_in_bookstores(rhs.num_in_bookstores) {}

// move constructor for SalableBook
SalableBook::SalableBook(SalableBook&& rhs)
    : Book(std::move(rhs)), num_in_bookstores(rhs.num_in_bookstores) {
        using namespace std;
        cout << "SalableBook " << isbn() << " moved constructed" << endl;
    }

// move assignment for SalableBook
SalableBook& SalableBook::operator=(SalableBook&& rhs) {
    num_in_bookstores = std::move(rhs.num_in_bookstores);
    std::cout << "SalableBook " << rhs.isbn() << " moving" << std::endl;
    Book::operator=(std::move(rhs));
    std::cout << "SalableBook '" << isbn() << "' moved assigned" << std::endl;
    return *this;
}

SalableBook::SalableBook(const Book& book) : Book(book) {}
SalableBook::SalableBook(Book&& book) : Book(std::move(book)) {}
SalableBook& SalableBook::remove_from_bookstore(std::weak_ptr<const Bookstore> bookstore) {
    num_in_bookstores.erase(bookstore);
    return *this;
}

SalableBook& SalableBook::increase_num_in_bookstore(std::weak_ptr<const Bookstore> bookstore) {
    num_in_bookstores[bookstore]++;
    return *this;
}
SalableBook& SalableBook::set_num_in_bookstore(std::weak_ptr<const Bookstore> bookstore,
                                               unsigned int num) {
    num_in_bookstores[bookstore] = num;
    return *this;
}
SalableBook& SalableBook::decrease_num_in_bookstore(std::weak_ptr<const Bookstore> bookstore) {
    auto ptr = bookstore.lock();
    if (ptr == nullptr) {
        num_in_bookstores.erase(bookstore);
        return *this;
    }
    num_in_bookstores[bookstore]--;
    if (num_in_bookstores[bookstore] == 0) {
        num_in_bookstores.erase(bookstore);
    }
    return *this;
}
std::shared_ptr<SalableBook> SalableBook::shared_from_this() {
    return std::dynamic_pointer_cast<SalableBook>(Book::shared_from_this());
}
std::weak_ptr<SalableBook> SalableBook::weak_from_this() {
    return std::dynamic_pointer_cast<SalableBook>(Book::shared_from_this());
}
const string& Book::isbn() const { return isbn_; }
const string& Book::title() const { return title_; }
const string& Book::author() const { return author_; }
const string& Book::content() const { return *content_ptr_; }
bool operator<(const Book& lhs, const Book& rhs) { return lhs.isbn_ < rhs.isbn_; }