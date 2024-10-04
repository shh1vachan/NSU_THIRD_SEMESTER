#ifndef BITARRAY_H
#define BITARRAY_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>


class BitArray 
{
private:
    int num_bits;  
    std::vector<unsigned long> data;  // Storage for the bits

public:
    BitArray();
    ~BitArray();
    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);

    // Member functions
    void swap(BitArray& b);
    BitArray& operator=(const BitArray& b);
    void resize(int num_bits, bool value = false);
    void clear();
    void push_back(bool bit);
    
    // Bitwise operators
    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;
    
    // Bit manipulation functions
    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();
    
    
    // Utility functions
    [[nodiscard]] bool any() const;
    [[nodiscard]] bool none() const;
    BitArray operator~() const;
    [[nodiscard]] int count() const;
    bool operator[](int i) const;
    [[nodiscard]] int size() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::string to_string() const;

    // Friend functions
    friend bool operator==(const BitArray &a, const BitArray &b);
    friend bool operator!=(const BitArray &a, const BitArray &b);
    friend BitArray operator&(const BitArray& b1, const BitArray& b2);
    friend BitArray operator|(const BitArray& b1, const BitArray& b2);
    friend BitArray operator^(const BitArray& b1, const BitArray& b2);

    // Iterator class for range-based for loops
    class Iterator 
{
    private:
        const BitArray* bit_array;  
        int index; 

    public:
        Iterator(const BitArray* ba, int idx);
        bool operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
    };

    // Methods for iterator support
    Iterator begin() const;
    Iterator end() const;
};

bool operator==(const BitArray &a, const BitArray &b);
bool operator!=(const BitArray &a, const BitArray &b);
BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

#endif 
