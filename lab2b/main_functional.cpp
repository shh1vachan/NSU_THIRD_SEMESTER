#include <gtest/gtest.h>
#include <fstream>
#include "game.h"
#include "file_manager.h"
#include "parser.h"


TEST(GameTests, InitializeWithFileAndError)
{
    std::ofstream file("test_universe.txt");
    file << "#Life 1.06\n";
    file << "#N Test Universe\n";
    file << "#R B3/S23\n";
    file << "0 1\n1 2\n2 0\n";
    file.close();

    parser cmd_parser;
    //argv without mode
    const char* argv_const[] = {"program", "-f", "test_universe.txt", "-i", "5", "-o", "output.txt", "-m"}; 
    char** argv = const_cast<char**>(argv_const);
    cmd_parser.parse(8, argv); 

    EXPECT_FALSE(cmd_parser.parse(8, argv));

    file_manager manager;
    Game game(3);

    ASSERT_TRUE(game.prepare_game(cmd_parser, manager));
    EXPECT_EQ(manager.field_name, "Test Universe");
    EXPECT_EQ(manager.rule, "B3/S23");
}


TEST(GameTests, InitializeWithFile)
{
    std::ofstream file("test_universe.txt");
    file << "#Life 1.06\n";
    file << "#N Test Universe\n";
    file << "#R B3/S23\n";
    file << "0 1\n1 2\n2 0\n";
    file.close();

    parser cmd_parser;
    const char* argv_const[] = {"program", "-f", "test_universe.txt", "-i", "5", "-o", "output.txt", "-m", "offline"};
    char** argv = const_cast<char**>(argv_const);
    ASSERT_TRUE(cmd_parser.parse(9, argv));

    file_manager manager;
    Game game(3);

    ASSERT_TRUE(game.prepare_game(cmd_parser, manager));
    EXPECT_EQ(manager.field_name, "Test Universe");
    EXPECT_EQ(manager.rule, "B3/S23");
}


TEST(GameTests, RandomUniverseGeneration)
{
    Game game(5);
    game.randomized_universe();

    int alive_count = 0;
    for (int x = 0; x < 5; ++x)
    {
        for (int y = 0; y < 5; ++y)
        {
            if (game.game_field[x][y].is_alive())
            {
                alive_count++;
            }
        }
    }
    EXPECT_GT(alive_count, 0);
}


TEST(GameTests, RunIteration)
{
    Game game(10);
    //standard format
    std::vector<int> neighbors_to_born;
    std::vector<int> neighbors_to_survive;
    parser::parse_rule("B3/S23", neighbors_to_born, neighbors_to_survive);

    //make rules
    game.neighbors_to_born = neighbors_to_born;
    game.neighbors_to_survive = neighbors_to_survive;

    //clock figure - cycle length 2
    game.game_field[3][2].set_current_state(true);
    game.game_field[3][3].set_current_state(true);
    game.game_field[2][4].set_current_state(true);
    game.game_field[4][4].set_current_state(true);
    game.game_field[4][5].set_current_state(true);
    game.game_field[5][3].set_current_state(true);

    game.run_iteration();

    EXPECT_TRUE(game.game_field[4][2].is_alive());
    EXPECT_TRUE(game.game_field[2][3].is_alive());
    EXPECT_TRUE(game.game_field[3][3].is_alive());
    EXPECT_TRUE(game.game_field[4][4].is_alive());
    EXPECT_TRUE(game.game_field[5][4].is_alive());
    EXPECT_TRUE(game.game_field[3][5].is_alive());
}


TEST(GameTests, CommandTick)
{
    parser cmd_parser;
    const char* argv_const[] = {"program", "-m", "online"};
    char** argv = const_cast<char**>(argv_const);
    cmd_parser.parse(3, argv);

    file_manager manager;
    Game game(3);
    game.prepare_game(cmd_parser, manager);

    std::istringstream tick_command("tick 3");
    EXPECT_TRUE(game.execute_command("tick 3"));
}


TEST(GameTests, CommandDump)
{
    parser cmd_parser;
    const char* argv_const[] = {"program", "-f", "test_universe.txt", "-m", "online"};
    char** argv = const_cast<char**>(argv_const);
    cmd_parser.parse(5, argv);

    file_manager manager;
    Game game(3);
    game.prepare_game(cmd_parser, manager);

    std::string filename = "output_test.txt";
    game.save_to_file(filename);

    std::ifstream saved_file(filename);
    ASSERT_TRUE(saved_file.is_open());
    std::string line;
    std::getline(saved_file, line);
    EXPECT_EQ(line, "#Life 1.06");
    saved_file.close();
}


TEST(GameTests, InvalidRuleFormat)
{
    std::ofstream file("test_invalid_rule.txt");
    file << "#Life 1.06\n";
    file << "#N Invalid Universe\n";
    file << "#R B9/S23\n";  //9 neighbors are impossible
    file << "0 1\n1 2\n2 0\n";
    file.close();

    parser cmd_parser;
    const char* argv_const[] = {"program", "-f", "test_invalid_rule.txt", "-i", "3", "-o", "output.txt", "-m", "offline"};
    char** argv = const_cast<char**>(argv_const);
    ASSERT_TRUE(cmd_parser.parse(9, argv));

    file_manager manager;
    Game game(3);
    EXPECT_FALSE(game.prepare_game(cmd_parser, manager));
}


TEST(GameTests, EmptyInputFile)
{
    std::ofstream file("test_empty.txt");
    file.close();

    parser cmd_parser;
    const char* argv_const[] = {"program", "-f", "test_empty.txt", "-i", "3", "-o", "output.txt", "-m", "offline"};
    char** argv = const_cast<char**>(argv_const);
    ASSERT_TRUE(cmd_parser.parse(9, argv));

    file_manager manager;
    Game game(3);
    EXPECT_FALSE(game.prepare_game(cmd_parser, manager));
}
