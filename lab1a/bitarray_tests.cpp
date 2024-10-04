#include "bitarray.h"
#include "gtest/gtest.h"
#include <stdexcept>

// Test default constructor
TEST(BitArrayTest, DefaultConstructor) {
    BitArray ba;
    ASSERT_EQ(ba.size(), 0);
    ASSERT_TRUE(ba.empty());
}

TEST(BitArrayTest, ConstructorWithValue)
{
    BitArray bitArray(10, 5);
    ASSERT_EQ(bitArray.size(), 10);
    ASSERT_EQ(bitArray.to_string().substr(7), "101");
}


TEST(BitArrayTest, CopyConstructor) {
    BitArray ba(64, 1);
    BitArray ba_copy(ba);
    ASSERT_EQ(ba_copy.size(), ba.size());
    for (int i = 0; i < ba_copy.size(); ++i) {
        ASSERT_EQ(ba_copy[i], ba[i]);  // Все значения должны совпадать
    }
}


TEST(BitArrayTest, AssignmentOperator) {
    BitArray ba1(64, 1);
    BitArray ba2;
    ba2 = ba1;
    ASSERT_EQ(ba2.size(), ba1.size());
    for (int i = 0; i < ba2.size(); ++i) {
        ASSERT_EQ(ba2[i], ba1[i]);  // Проверяем, что копия корректна
    }
}
TEST(BitArrayTest, Resize) {
    BitArray ba(64, 1); // Инициализируем 64 бита с первым битом равным 1
    ba.resize(128, false); // Увеличиваем размер до 128, все новые биты должны быть 0

    ASSERT_EQ(ba.size(), 128); // Проверяем новый размер

    // Проверяем первые 64 бита
    for (int i = 0; i < 64; ++i) {
        if (i == 0) {
            ASSERT_EQ(ba[i], true); // Первый бит должен быть 1
        } else {
            ASSERT_EQ(ba[i], false); // Остальные 63 бита должны быть 0
        }
    }

    // Проверяем новые биты
    for (int i = 64; i < 128; ++i) {
        ASSERT_EQ(ba[i], false);  // Новые биты должны быть 0
    }
}


// Test clear method
TEST(BitArrayTest, Clear) {
    BitArray ba(64, 1);
    ba.clear();
    ASSERT_EQ(ba.size(), 0);
    ASSERT_TRUE(ba.empty());
}

TEST(BitArrayTest, PushBack) {
    BitArray ba(64, false);
    ba.push_back(true);
    ASSERT_EQ(ba.size(), 65);
    ASSERT_EQ(ba[64], true);  // Проверяем последний добавленный бит
}


// Test operator&= (bitwise AND)
TEST(BitArrayTest, BitwiseAndAssignment) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 0);
    ba1 &= ba2;
    ASSERT_EQ(ba1.count(), 0);
    ASSERT_THROW(ba1 &= BitArray(128), std::invalid_argument);
}

TEST(BitArrayTest, BitwiseOrAssignment) {
    BitArray ba1(64, 0);
    BitArray ba2(64, 1);
    ba1 |= ba2;
    for (int i = 0; i < 64; ++i) {
        if (i == 0)
            ASSERT_EQ(ba1[i], true);
        else
            ASSERT_EQ(ba1[i], false);  // Операция | должна установить все биты в 1
    }
}


// Test operator^= (bitwise XOR)
TEST(BitArrayTest, BitwiseXorAssignment) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 1);
    ba1 ^= ba2;
    ASSERT_EQ(ba1.count(), 0);
    ASSERT_THROW(ba1 ^= BitArray(128), std::invalid_argument);
}

// Test left shift assignment operator
TEST(BitArrayTest, LeftShiftAssignment) {
    BitArray ba(64, 1);
    ba <<= 1;
    ASSERT_EQ(ba.size(), 64);
    ASSERT_EQ(ba.count(), 1);
    ASSERT_TRUE(ba[1]);
}

// Test right shift assignment operator
TEST(BitArrayTest, RightShiftAssignment) {
    BitArray ba(64, 1);
    ba >>= 1;
    ASSERT_EQ(ba.size(), 64);
    ASSERT_EQ(ba.count(), 0);
}

// Test operator<< (const version)
TEST(BitArrayTest, LeftShift) {
    BitArray ba(64, 1);
    BitArray shifted = ba << 1;
    ASSERT_EQ(shifted.count(), 1);
    ASSERT_TRUE(shifted[1]);
}

// Test operator>> (const version)
TEST(BitArrayTest, RightShift) {
    BitArray ba(64, 1);
    BitArray shifted = ba >> 1;
    ASSERT_EQ(shifted.count(), 0);
}

// Test set method (specific bit)
TEST(BitArrayTest, SetBit) {
    BitArray ba(64);
    ba.set(0, true);
    ASSERT_TRUE(ba[0]);
    ASSERT_THROW(ba.set(100), std::out_of_range);
}

// Test set method (all bits)
TEST(BitArrayTest, SetAllBits) {
    BitArray ba(64);
    ba.set();
    ASSERT_EQ(ba.count(), 64);
}

// Test reset method (specific bit)
TEST(BitArrayTest, ResetBit) {
    BitArray ba(64, 1);
    ba.reset(0);
    ASSERT_FALSE(ba[0]);
    ASSERT_THROW(ba.reset(100), std::out_of_range);
}

// Test reset method (all bits)
TEST(BitArrayTest, ResetAllBits) {
    BitArray ba(64, 1);
    ba.reset();
    ASSERT_EQ(ba.count(), 0);
}

// Test any method
TEST(BitArrayTest, Any) {
    BitArray ba(64, 1);
    ASSERT_TRUE(ba.any());
    ba.reset();
    ASSERT_FALSE(ba.any());
}

// Test none method
TEST(BitArrayTest, None) {
    BitArray ba(64, 1);
    ASSERT_FALSE(ba.none());
    ba.reset();
    ASSERT_TRUE(ba.none());
}

// Test operator~ (bitwise NOT)
TEST(BitArrayTest, BitwiseNot) {
    BitArray ba(64, 1);
    BitArray not_ba = ~ba;
    ASSERT_EQ(not_ba.count(), 63);
}

TEST(BitArrayTest, Count) {
    BitArray ba(64, 10);
    ASSERT_EQ(ba.count(), 2);  // Все биты установлены в 1, должно быть 64
    ba.reset(1);
    ASSERT_EQ(ba.count(), 1);  // Один бит сброшен, теперь должно быть 63
}


// Test operator[] (access specific bit)
TEST(BitArrayTest, AccessSpecificBit) {
    BitArray ba(64, 1);
    ASSERT_TRUE(ba[0]);
    ASSERT_FALSE(ba[63]);
    ASSERT_THROW(ba[100], std::out_of_range);
}

// Test size method
TEST(BitArrayTest, Size) {
    BitArray ba(64);
    ASSERT_EQ(ba.size(), 64);
}

// Test empty method
TEST(BitArrayTest, Empty) {
    BitArray ba;
    ASSERT_TRUE(ba.empty());
}

TEST(BitArrayTest, ToString) {
    BitArray ba(8, 0b10101010);
    ASSERT_EQ(ba.to_string(), "10101010");  // Проверка строкового представления битов
}


// Test equality operator
TEST(BitArrayTest, EqualityOperator) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 1);
    ASSERT_TRUE(ba1 == ba2);
}

// Test inequality operator
TEST(BitArrayTest, InequalityOperator) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 0);
    ASSERT_TRUE(ba1 != ba2);
}

// Test operator& (bitwise AND)
TEST(BitArrayTest, BitwiseAnd) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 0);
    BitArray result = ba1 & ba2;
    ASSERT_EQ(result.count(), 0);
}

TEST(BitArrayTest, BitwiseOr) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 5);
    BitArray ba3 = ba1 | ba2;
    for (int i = 0; i < 64; ++i) {
        if (i == 0 or i == 2)
            ASSERT_EQ(ba3[i], true);  // Проверка результата побитовой операции |
        else
            ASSERT_EQ(ba3[i], false);
    }
}


// Test operator^ (bitwise XOR)
TEST(BitArrayTest, BitwiseXor) {
    BitArray ba1(64, 1);
    BitArray ba2(64, 1);
    BitArray result = ba1 ^ ba2;
    ASSERT_EQ(result.count(), 0);
}

TEST(BitArrayTest, Iterator) {
    BitArray ba(64, 1); // Инициализируем 64 бита с первым битом равным 1
    int count = 0;
    for (auto it = ba.begin(); it != ba.end(); ++it) {
        // Проверяем, что только первый бит равен true
        if (count == 0) {
            ASSERT_EQ(*it, true);  // Первый бит должен быть true
        } else {
            ASSERT_EQ(*it, false); // Остальные биты должны быть false
        }
        ++count;
    }
    ASSERT_EQ(count, 64);  // Должно быть 64 итерации
}



// Test iterator equality and inequality
TEST(BitArrayTest, IteratorEqualityInequality) {
    BitArray ba(64, 1);
    auto it1 = ba.begin();
    auto it2 = ba.end();
    ASSERT_NE(it1, it2);  // Проверка, что итераторы не равны
    ++it1;  // Инкрементируем it1
    auto it3 = ba.begin();
    ++it3;  // Инкрементируем другой итератор для сравнения
    ASSERT_EQ(it1, it3);  // Теперь проверяем равенство итераторов
}