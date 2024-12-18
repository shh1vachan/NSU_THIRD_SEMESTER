#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "csv_parser.h"
#include "errors.h"


TEST(CSVParserTests, OpenFileSuccess) 
{
    std::ifstream file("test.csv");
    ASSERT_TRUE(file.is_open());
}


TEST(CSVParserTests, OpenFileFail) 
{
    std::ifstream file("non_existing_file.csv");
    ASSERT_FALSE(file.is_open());
}


TEST(CSVParserTests, CorrectFileProcessing) 
{
    std::ifstream file("test.csv");
    ASSERT_TRUE(file.is_open()) << "Failed to open test.csv";

    parser<size_t, std::string, int, std::string, double> csv(file, 0);

    std::vector<std::tuple<size_t, std::string, int, std::string, double>> expected
    {
        {1, "qawg", -1, "AB BA", 1.1},
        {2, "Ivan", -2, "CD DC", 2.2},
        {3, "Sharapov", -3, "\"Hello!\"", 3.3},
        {4, "1212", -4, "GH, HG", 4.4},
        {5, "ddddddddddddd ddddddddd", -5, "IJ JI", 5.5},
        {6, "Black", -6, "KL, LK", 6.6}
    };

    size_t idx = 0;
    for (const auto& row : csv) 
    {
        ASSERT_LT(idx, expected.size()) << "Index out of range.";
        EXPECT_EQ(row, expected[idx]) << "Mismatch at row " << idx + 1;
        idx++;
    }
    EXPECT_EQ(idx, expected.size()) << "Row count mismatch!";
}


TEST(CSVParserTests, IncorrectFileProcessing) 
{
    std::ifstream file("test_error.csv");
    parser<size_t, std::string, int, std::string, double> csv(file, 0);

    try 
    {
        for (auto row : csv) 
            SUCCEED();  
    }
    catch (const FailedToReadData& e) 
    {
        EXPECT_EQ(e.line_number(), 5);
        EXPECT_EQ(e.col_number(), 3);
    }
    catch (const NotEnoughData& e)
    {
        EXPECT_EQ(e.line_number(), 6);
    }
}


TEST(CSVParserTests, NotEnoughDataError) 
{
    std::ofstream incomplete("test_incomplete.csv");
    incomplete << "1;White;-1;AB BA\n";   //incomplete row
    incomplete.close();

    std::ifstream file("test_incomplete.csv");
    parser<size_t, std::string, int, std::string, double> csv(file, 0);

    EXPECT_THROW(
        {
        for (auto row : csv) 
            SUCCEED();  
    }, NotEnoughData);
}


TEST(CSVParserTests, LargeFileProcessing) 
{
    std::ofstream large_file("test_long.csv");
    for (size_t i = 0; i < 1100; ++i) 
        large_file << i + 1 << ";Example;-10;Data" << i << ";" << i * 0.1 << "\n";
    
    large_file.close();

    std::ifstream file("test_long.csv");
    parser<size_t, std::string, int, std::string, double> csv(file, 0);

    size_t row_count = 0;
    for (auto row : csv) 
        row_count++;

    EXPECT_GT(row_count, 12) << "Row count is too small!";
}


TEST(CSVParserTests, EmptyFile) 
{
    std::ofstream empty("empty.csv", std::ios::trunc);
    empty.close();

    std::ifstream file("empty.csv");
    ASSERT_TRUE(file.is_open()) << "Failed to open empty.csv";

    parser<size_t, std::string, int, std::string, double> csv(file, 0);

    size_t row_count = 0;
    for (auto row : csv) 
        row_count++;

    EXPECT_EQ(row_count, 0) << "File should be empty!";
}

