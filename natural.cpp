#include "calculator.hpp"

natural::natural(int v) {
    while (v > 0) {
        digits.push_back(v % 10);
        v /= 10;
    }
}

/** @returns converted number by particular base stored in a vector of naturals **/
std::vector<natural> natural::convert_to_base(const natural &base) const {
    natural copy = *this, temp, rest;
    std::vector<natural> result;

    while (copy != 0) {
        temp = copy / base;
        rest = copy - (temp * base);
        result.push_back(rest.digits.empty() ? natural(0) : rest);
        copy = temp;
    }
    return result;
}

natural natural::digit_count(natural base) const {
    return natural(convert_to_base(base).size());
}

natural natural::digit_sum(natural base) const {
    natural sum;
    for (const natural &n: convert_to_base(base))
        sum += n;

    return sum;
}

/** arithmetic functions **/

natural natural::power(natural exponent) const {
    if (exponent == 0) return natural(1);
    if (exponent == 1) return *this;

    natural result = natural(1), base = *this;

    while (exponent > 0) {
        if (exponent.digits[0] & 1)
            result *= base;
        base *= base;
        exponent /= 2;
    }
    return result;
}

natural natural::operator+(const natural &n) const {
    natural result = *this;
    size_t rest = 0, bigger_num = std::max(result, n).digits.size();

    for (size_t i = 0; i < bigger_num || rest; ++i) {
        if (i == result.digits.size())
            result.digits.push_back(0);

        result.digits[i] += (i < n.digits.size() ? n.digits[i] : 0) + rest;
        rest = result.digits[i] >= 10;
        if (rest != 0)
            result.digits[i] -= 10;
    }
    return result;
}

natural natural::operator-(const natural &n) const {
    if (n > *this) throw std::out_of_range("Negative result");

    natural result = *this;
    size_t rest = 0;

    for (size_t i = 0; i < n.digits.size() || rest; ++i) {
        int diff = int(result.digits[i] - (rest + (i < n.digits.size() ? n.digits[i] : 0)));
        rest = diff < 0;
        if (rest != 0) diff += 10;
        result.digits[i] = diff;
    }
    delete_front_zeroes(result);
    return result;
}

natural natural::operator*(const natural &n) const {
    if (*this == 0 || n == 0) return natural(0);
    if (*this == 1) return n;
    if (n == 1) return *this;

    natural result;
    result.digits = std::vector<uint8_t>(digits.size() + n.digits.size(), 0);

    size_t product;

    for (size_t i = 0; i < digits.size(); ++i)
        for (size_t j = 0, rest = 0; j < n.digits.size() || rest; ++j) {
            if (j < n.digits.size()) {
                product = result.digits[i + j] + this->digits[i] * n.digits[j] + rest;
            } else {
                product = result.digits[i + j] + rest;
            }
            result.digits[i + j] = int(product % 10);
            rest = int(product / 10);
        }
    delete_front_zeroes(result);
    return result;
}

/** removes unnecessary zeros at the beginning of the number **/
void natural::delete_front_zeroes(natural &n) {
    for (size_t i = 0, N = digits.size(); i < N; ++i)
        n.digits.pop_back();
}

/**
 * long division, gradually dividing Natural numbers using divide_by_subtraction
 * each block_result is the digit added to the quotient
 */
natural natural::operator/(const natural &n) const {
    if (n > *this) return natural(0);
    if (n == *this) return natural(1);
    if (n == 1) return *this;
    if (n == 0) throw std::logic_error("Division by zero");

    natural block, block_result, rest, result;
    size_t count = n.digits.size() - 1, idx;
    rest.digits = std::vector(this->digits.end() - count, this->digits.end());

    while (count < this->digits.size()) {
        idx = this->digits.size() - 1 - count;
        rest.digits.insert(rest.digits.begin(), 1, this->digits[idx]);
        block.digits = rest.digits;
        count++;

        while (block < n) {
            result.digits.insert(result.digits.begin(), 1, 0);
            if (count < this->digits.size()) {
                idx = this->digits.size() - count - 1;
                block.digits.insert(block.digits.begin(), 1, this->digits[idx]);
                count++;
            } else { break; }
        }
        if (block == n) {
            result.digits.insert(result.digits.begin(), 1, 1);
            rest = 0;
        } else if (block > n) {
            delete_front_zeroes(block);

            std::tie(block_result, rest) = divide_by_subtraction(block, n);

            if (block_result.digits.empty()) {
                result.digits.insert(result.digits.begin(), 1, 0);
            } else {
                result.digits.insert(result.digits.begin(), block_result.digits.begin(), block_result.digits.end());
            }
        }
    }
    delete_front_zeroes(result);
    return result;
}

/**
 * division by gradually subtracting divisor from dividend
 * @return quotient and remainder of division
 **/
std::tuple<natural, natural> natural::divide_by_subtraction(const natural &dividend, const natural &divisor) {
    natural quotient = natural(1), remainder, copy = divisor;
    while (copy < dividend) {
        ++quotient;
        copy += divisor;
    }
    if (copy > dividend) {
        --quotient;
        remainder = dividend - (copy - divisor);
    }
    return std::make_tuple(quotient, remainder);
}

natural natural::operator%(const natural &n) const {
    if (n == 0) throw std::logic_error("Division with zero");
    if (n == 1 || n == *this) return natural(0);
    if (n > *this) return natural(*this);

    natural result = *this - ((*this / n) * n);
    delete_front_zeroes(result);
    return result;
}

/** natural:int arithmetics **/

natural natural::operator+(const int &n) const {
    return *this + natural(n);
}

natural natural::operator-(const int &n) const {
    return *this - natural(n);
}

natural natural::operator*(const int &n) const {
    return *this * natural(n);
}

natural natural::operator/(const int &n) const {
    return *this / natural(n);
}

natural natural::operator%(const int &n) const {
    return *this % natural(n);
}

/** increment & decrement **/

natural &natural::operator++() {
    auto copy(*this);
    copy += 1;
    *this = copy;
    return *this;
}

natural &natural::operator--() {
    *this = *this - natural(1);
    return *this;
}

/** assignments **/

natural &natural::operator=(const int &n) {
    *this = natural(n);
    return *this;
}

natural &natural::operator=(const natural &) = default;

natural &natural::operator=(natural &&) noexcept = default;

/** natural:natural arithmetic assignments **/

natural &natural::operator+=(const natural &n) {
    *this = *this + n;
    return *this;
}

natural &natural::operator-=(const natural &n) {
    *this = *this - n;
    return *this;
}

natural &natural::operator*=(const natural &n) {
    *this = *this * n;
    return *this;
}

natural &natural::operator/=(const natural &n) {
    *this = *this / n;
    return *this;
}

natural &natural::operator%=(const natural &n) {
    *this = *this / n;
    return *this;
}

/** natural:int arithmetic assignments **/

natural &natural::operator+=(const int &n) {
    *this = *this + natural(n);
    return *this;
}

natural &natural::operator-=(const int &n) {
    *this = *this - natural(n);
    return *this;
}

natural &natural::operator*=(const int &n) {
    *this = *this * natural(n);
    return *this;
}

natural &natural::operator/=(const int &n) {
    *this = *this / natural(n);
    return *this;
}

natural &natural::operator%=(const int &n) {
    *this = *this / natural(n);
    return *this;
}

/** natural:natural comparisons **/

bool natural::operator==(const natural &n) const {
    return this->digits == n.digits;
}

bool natural::operator!=(const natural &n) const {
    return !operator==(n);
}

bool natural::operator>=(const natural &n) const {
    return !operator<(n);
}

bool natural::operator<=(const natural &n) const {
    return !operator>(n);
}

bool natural::operator>(const natural &n) const {
    if (digits.size() != n.digits.size())
        return digits.size() > n.digits.size();

    for (int i = static_cast<int> (digits.size() - 1); i >= 0; --i) {
        if (digits[i] != n.digits[i])
            return digits[i] > n.digits[i];
    }
    return false;
}

bool natural::operator<(const natural &n) const {
    return !operator>(n) && !operator==(n);
}

/** natural:int comparisons **/

bool natural::operator==(const int &n) const {
    return *this == natural(n);
}

bool natural::operator!=(const int &n) const {
    return *this != natural(n);
}

bool natural::operator>(const int &n) const {
    return *this > natural(n);
}

bool natural::operator<(const int &n) const {
    return *this < natural(n);
}

bool natural::operator>=(const int &n) const {
    return *this >= natural(n);
}

bool natural::operator<=(const int &n) const {
    return *this <= natural(n);
}

natural natural::operator^(const natural &n) const {
    auto num1 = convert_to_base(natural(2));
    auto num2 = n.convert_to_base(natural(2));
    align_with_zeroes(num1, num2);

    natural result;
    result.digits = std::vector<uint8_t>(num1.size(), 0);

    for (size_t i = 0; i < num1.size(); ++i) {
        size_t x1 = num1[i].digits.empty() ? 0 : num1[i].digits[0];
        size_t x2 = num2[i].digits.empty() ? 0 : num2[i].digits[0];
        result.digits[i] = x1 != x2;
    }
    return binary_to_decimal(result);
}

natural natural::operator&(const natural &n) const {
    auto num1 = convert_to_base(natural(2));
    auto num2 = n.convert_to_base(natural(2));
    align_with_zeroes(num1, num2);

    natural result;
    result.digits = std::vector<uint8_t>(num1.size(), 0);

    for (size_t i = 0; i < num1.size(); ++i) {
        size_t x1 = num1[i].digits.empty() ? 0 : num1[i].digits[0];
        size_t x2 = num2[i].digits.empty() ? 0 : num2[i].digits[0];
        result.digits[i] = x1 & x2;
    }

    return binary_to_decimal(result);
}

natural natural::operator|(const natural &n) const {
    auto num1 = convert_to_base(natural(2));
    auto num2 = n.convert_to_base(natural(2));
    align_with_zeroes(num1, num2);

    natural result;
    result.digits = std::vector<uint8_t>(num1.size(), 0);

    for (size_t i = 0; i < num1.size(); ++i) {
        size_t x1 = num1[i].digits.empty() ? 0 : num1[i].digits[0];
        size_t x2 = num2[i].digits.empty() ? 0 : num2[i].digits[0];
        result.digits[i] = x1 | x2;
    }
    return binary_to_decimal(result);
}

void natural::align_with_zeroes(std::vector<natural> &num1, std::vector<natural> &num2) const {
    while (num1.size() != num2.size()) {
        if (num1.size() < num2.size()) num1.emplace_back(0);
        if (num1.size() > num2.size()) num2.emplace_back(0);
    }
}

natural natural::binary_to_decimal(const natural &num) const {
    natural dec_value(0), base(1);

    for (unsigned char digit : num.digits) {
        if (digit) dec_value += base;
        base = base * 2;
    }
    return dec_value;
}

int main() {
    natural a( 4 );
    natural b( 2 );
    natural zero( 0 );
    natural one( 1 );
    natural vv = ( a & b );
    assert(vv == zero);
}
