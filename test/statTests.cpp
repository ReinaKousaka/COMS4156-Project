#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stat.h"

#define EXPECT_ZERO(stmt) EXPECT_EQ(stmt, 0)

/**
Table: players
    player_id, username
*/
class StatTest:public::testing::Test {
 protected:
    void SetUp() override {
        // clear existing data
        EXPECT_ZERO(db.deleteData("DELETE FROM players;"));
        EXPECT_ZERO(db.deleteData("DELETE FROM player_stats;"));
        EXPECT_ZERO(db.deleteData("DELETE FROM game_list;"));
        EXPECT_ZERO(db.deleteData("DELETE FROM achievements;"));
        // insert test data
        EXPECT_ZERO(db.insertData(
            "INSERT INTO players(player_id, username)"
            "VALUES('player1', 'username1'),"
            "('player2', 'username2'),"
            "('player3', 'username4'),"
            "('player4', 'username4');"));
        EXPECT_ZERO(db.insertData(
            "INSERT INTO player_stats(player_id, username, game_type, "
            "total_wins, total_losses, most_won, most_lost, total_money) "
            "VALUES('player1', 'username1', 'RPS', 1, 1, 1, -1, 0),"
            "('player1', 'username1', 'BlackJack', 1, 1, 1, -2, 0),"
            "('player2', 'username2', 'RPS', 1, 1, 1, 0, 0),"
            "('player3', 'username3', 'BlackJack', 1, 0, 1, -1, 0),"
            "('player4', 'username4', 'BlackJack', 0, 1, 1, -1, 0),"
            "('player101', 'username101', 'BJTest', 1, 1, 1, -1, 5),"
            "('player102', 'username101', 'BJTest', 2, 2, 1, -2, 5),"
            "('player103', 'username101', 'BJTest', 3, 3, 1, -3, 5),"
            "('player104', 'username101', 'BJTest', 4, 4, 1, -4, 5),"
            "('player105', 'username101', 'BJTest', 5, 5, 1, -5, 5),"
            "('player108', 'username101', 'BJTest', 1, 5, 1, -5, 5),"
            "('player109', 'username101', 'BJTest', 500, 8, 1, -9, 7),"
            "('player106', 'username101', 'BJTest', 100, 6, 100, -1, 5),"
            "('player107', 'username101', 'BJTest', 106, 7, 200, -1, 5);"));
        EXPECT_ZERO(db.insertData(
            "INSERT INTO game_list(game_id, game_type, username, "
            "player_id, result, earning) "
            "VALUES(100, 'RPS', 'username1', 'player1', '', '1'),"
            "(100, 'RPS', 'username2', 'player2', '', '-1'),"
            "(110, 'RPS', 'username1', 'player1', '', '-1'),"
            "(110, 'RPS', 'username2', 'player2', '', '-1'),"
            "(200, 'BlackJack', 'username1', 'player1', '', '-1'),"
            "(200, 'BlackJack', 'username3', 'player3', '', '-1'),"
            "(200, 'BlackJack', 'username4', 'player4', '', '2');"));
    }
    Database db = Database("dummy5.db");
};


TEST_F(StatTest, testGetNumTotalUsers) {
    EXPECT_EQ(getNumTotalUsers(db), 4);
}

TEST_F(StatTest, testGetNumGames) {
    EXPECT_EQ(getNumGames(db, ""), 3);
    EXPECT_EQ(getNumGames(db, "RPS"), 2);
    EXPECT_EQ(getNumGames(db, "BlackJack"), 1);
}

TEST_F(StatTest, testGetTotalPlayersForGame) {
    EXPECT_EQ(getTotalPlayersForGame(db, "RPS"), 2);
    EXPECT_EQ(getTotalPlayersForGame(db, "BlackJack"), 3);
    EXPECT_EQ(getTotalPlayersForGame(db, "doesNotExists"), 0);
}

TEST_F(StatTest, testMedianValue) {
    std::string command = "SELECT total_wins, player_id FROM player_stats"
      " WHERE username = 'username101' ORDER BY total_wins ASC;";

    float median = medianValue(pulledIntDataVector(db, command));
    EXPECT_EQ(median, 4.0);
}

TEST_F(StatTest, testBoxWhiskerPoints) {
    std::string command = "SELECT total_wins, player_id FROM player_stats"
      " WHERE username = 'username101' ORDER BY total_wins ASC;";

    std::array<float, 4> percentiles = percentileValues(\
      pulledIntDataVector(db, command));

    EXPECT_EQ(percentiles.at(0), 1.5);
    EXPECT_EQ(percentiles.at(1), 4.0);
    EXPECT_EQ(percentiles.at(2), 103.0);
    EXPECT_EQ(percentiles.at(3), 101.5);
}

// TEST_F(StatTest, testGetGreatestPlayerByWins) {
// ASSERT_THAT(getGreatestPlayerByWins(db), testing::ElementsAre("player1"));
// }
