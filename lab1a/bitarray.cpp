#include "bitarray.h"

static constexpr int BITS_PER_LONG = sizeof(unsigned long) * 8;

// Default constructor
BitArray::BitArray() : num_bits(0) {}

// Destructor
BitArray::~BitArray() {}

BitArray::BitArray(int num_bits, unsigned long value) : num_bits(num_bits)
{
    data.resize((num_bits + BITS_PER_LONG - 1) / BITS_PER_LONG, 0);
    if (num_bits > 0 && !data.empty())
        data[0] = value;
}


// Copy constructor
BitArray::BitArray(const BitArray& b) : num_bits(b.num_bits), data(b.data) {}

// Swap method
void BitArray::swap(BitArray& b) {
    std::swap(num_bits, b.num_bits);
    std::swap(data, b.data);
}

// Assignment operator
BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        BitArray temp(b);
        swap(temp);
    }
    return *this;
}

void BitArray::resize(int new_size, bool value)
{
    if (new_size < 0)
        throw std::invalid_argument("New size must be non-negative");

    std::vector<unsigned long> new_data((new_size + BITS_PER_LONG - 1) / BITS_PER_LONG, value ? ~0UL : 0);
    int copy_bits = std::min(num_bits, new_size);
    for (int i = 0; i < copy_bits; ++i)
        if ((*this)[i])
            new_data[i / BITS_PER_LONG] |= (1UL << (i % BITS_PER_LONG));

    data = std::move(new_data);
    num_bits = new_size;
}

// Clear the bit array
void BitArray::clear() {
    data.clear();
    num_bits = 0;
}

// Add a bit to the end
void BitArray::push_back(bool bit) {
    resize(num_bits + 1, bit);
}

// Bitwise AND assignment
BitArray& BitArray::operator&=(const BitArray& b) {
    if (num_bits != b.num_bits) throw std::invalid_argument("Sizes must be equal");
    
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] &= b.data[i];
    }
    
    return *this;
}

// Bitwise OR assignment
BitArray& BitArray::operator|=(const BitArray& b) {
    if (num_bits != b.num_bits) throw std::invalid_argument("Sizes must be equal");
    
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] |= b.data[i];
    }
    
    return *this;
}

// Bitwise XOR assignment
BitArray& BitArray::operator^=(const BitArray& b) {
    if (num_bits != b.num_bits) throw std::invalid_argument("Sizes must be equal");
    
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= b.data[i];
    }
    
    return *this;
}

// Left shift assignment
BitArray& BitArray::operator<<=(int n) {
    std::rotate(data.rbegin(), data.rbegin() + n / 64, data.rend());
    for (int i = 0; i < n % 64; ++i) {
        for (size_t j = 0; j < data.size(); ++j) {
            data[j] = (data[j] << 1) | (j + 1 < data.size() ? (data[j + 1] >> 63) : 0);
        }
    }
    return *this;
}

// Right shift assignment
BitArray& BitArray::operator>>=(int n) {
    std::rotate(data.begin(), data.begin() + n / 64, data.end());
    for (int i = 0; i < n % 64; ++i) {
        for (size_t j = data.size(); j > 0; --j) {
            data[j - 1] = (data[j - 1] >> 1) | (j > 1 ? (data[j - 2] << 63) : 0);
        }
    }
    return *this;
}

// Left shift (const version)
BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

// Right shift (const version)
BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

// Set a specific bit
BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= num_bits) throw std::out_of_range("Index out of bounds");
    
    if (val) {
        data[n / 64] |= (1UL << (n % 64));
    } else {
        data[n / 64] &= ~(1UL << (n % 64));
    }
    
    return *this;
}

// Set all bits
BitArray& BitArray::set() {
    std::fill(data.begin(), data.end(), ~0UL);
    return *this;
}

// Reset a specific bit
BitArray& BitArray::reset(int n) {
    return set(n, false);
}

// Reset all bits
BitArray& BitArray::reset() {
    std::fill(data.begin(), data.end(), 0);
    return *this;
}

// Check if any bit is set
bool BitArray::any() const {
    for (unsigned long word : data) {
        if (word != 0) return true;
    }
    return false;
}

// Check if no bits are set
bool BitArray::none() const {
    return !any();
}

// Bitwise NOT
BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = ~data[i];
    }
    return result;
}

// Count the number of set bits
int BitArray::count() const {
    int total = 0;
    for (unsigned long word : data) {
        total += __builtin_popcountl(word);
    }
    return total;
}

// Access a specific bit
bool BitArray::operator[](int i) const {
    if (i < 0 || i >= num_bits) throw std::out_of_range("Index out of bounds");
    return (data[i / 64] >> (i % 64)) & 1;
}

// Return the number of bits
int BitArray::size() const {
    return num_bits;
}

// Check if the bit array is empty
bool BitArray::empty() const {
    return num_bits == 0;
}

std::string BitArray::to_string() const
{
    std::string result;
    for (int i = num_bits - 1; i >= 0; --i)
        result += (*this)[i] ? '1' : '0';
    return result;
}

// Equality operator
bool operator==(const BitArray& a, const BitArray& b) {
    return a.num_bits == b.num_bits && a.data == b.data;
}

// Inequality operator
bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

// Bitwise AND
BitArray operator&(const BitArray& b1, const BitArray& b2) {
    if (b1.num_bits != b2.num_bits) throw std::invalid_argument("Sizes must be equal");
    
    BitArray result(b1);
    result &= b2;
    return result;
}

// Bitwise OR
BitArray operator|(const BitArray& b1, const BitArray& b2) {
    if (b1.num_bits != b2.num_bits) throw std::invalid_argument("Sizes must be equal");
    
    BitArray result(b1);
    result |= b2;
    return result;
}

// Bitwise XOR
BitArray operator^(const BitArray& b1, const BitArray& b2) {
    if (b1.num_bits != b2.num_bits) throw std::invalid_argument("Sizes must be equal");
    
    BitArray result(b1);
    result ^= b2;
    return result;
}

// Iterator constructor
BitArray::Iterator::Iterator(const BitArray* ba, int idx) : bit_array(ba), index(idx) {}

// Dereference operator
bool BitArray::Iterator::operator*() const {
    return (*bit_array)[index];
}

// Pre-increment operator
BitArray::Iterator& BitArray::Iterator::operator++() {
    ++index;
    return *this;
}

// Inequality comparison operator
bool BitArray::Iterator::operator!=(const Iterator& other) const {
    return index != other.index;
}

// Equality comparison operator
bool BitArray::Iterator::operator==(const Iterator& other) const {
    return index == other.index;
}

// Begin iterator
BitArray::Iterator BitArray::begin() const {
    return Iterator(this, 0);
}

// End iterator
BitArray::Iterator BitArray::end() const {
    return Iterator(this, num_bits);
}