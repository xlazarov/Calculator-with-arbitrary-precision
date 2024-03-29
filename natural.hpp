#include <cassert>
#include <stdexcept>
#include <cstdint>
#include <tuple>
#include <vector>

/* Implementation of a class that represents
 * arbitrary-size natural numbers (including 0). In addition to the
 * methods prescribed below, the class supports the following:
 *
 *  • arithmetic operators ‹+›, ‹-›, ‹*›, ‹/› and ‹%› (the last two
 *    implementing division and reminder),
 *  • all relational operators,
 *  • bitwise operators ‹^› (xor), ‹&› (and) and ‹|› (or).
 *
 */

class natural {

    std::vector<uint8_t> digits;

    std::vector<natural> convert_to_base(const natural &base) const;
    static std::tuple<natural, natural> divide_by_subtraction(const natural &dividend, const natural &divisor);
    static void delete_front_zeroes(natural &n);

public:
    explicit natural(int v = 0);
    natural(const natural &) = default;

    natural power(natural exponent) const;

    natural digit_count(natural base) const;
    natural digit_sum(natural base) const;

    natural operator+(const natural &n) const;
    natural operator-(const natural &n) const;
    natural operator*(const natural &n) const;
    natural operator/(const natural &n) const;
    natural operator%(const natural &n) const;

    natural operator^(const natural &n) const;
    natural operator&(const natural &n) const;
    natural operator|(const natural &n) const;

    natural operator+(const int &n) const;
    natural operator-(const int &n) const;
    natural operator*(const int &n) const;
    natural operator/(const int &n) const;
    natural operator%(const int &n) const;

    natural &operator+=(const natural &n);
    natural &operator-=(const natural &n);
    natural &operator*=(const natural &n);
    natural &operator/=(const natural &n);
    natural &operator%=(const natural &n);

    natural &operator++();
    natural &operator--();

    natural &operator+=(const int &n);
    natural &operator-=(const int &n);
    natural &operator*=(const int &n);
    natural &operator/=(const int &n);
    natural &operator%=(const int &n);

    bool operator==(const natural &n) const;
    bool operator!=(const natural &n) const;
    bool operator<(const natural &n) const;
    bool operator>(const natural &n) const;
    bool operator<=(const natural &n) const;
    bool operator>=(const natural &n) const;

    natural &operator=(const natural &);
    natural &operator=(const int &n);
    natural &operator=(natural &&) noexcept;

    bool operator==(const int &n) const;
    bool operator!=(const int &n) const;
    bool operator<(const int &n) const;
    bool operator>(const int &n) const;
    bool operator<=(const int &n) const;
    bool operator>=(const int &n) const;

    void align_with_zeroes(std::vector<natural> &num1, std::vector<natural> &num2) const;
    natural binary_to_decimal(const natural& num) const;

};
