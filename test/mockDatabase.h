#ifndef TEST_MOCKDATABASE_H
#define TEST_MOCKDATABASE_H

#include <string>
#include <vector>
#include <set>
#include "gmock/gmock.h"
#include "src/sqliteDB/sql.h"

crow::SimpleApp app;

class MockDatabase : public Database {
 public:
  explicit MockDatabase() {

  }
  int insertMockData(std::string field, std::string data);
  int selectData(std::string command);
  int updateData(std::string command);
  int totalMockRows(std::string field, std::string data);
  sqlite3_stmt* makeStatement(std::string command);
  int getMax(std::string table_name, std::string col_name);
  int getIntValue(std::string command);
  std::string getTextValue(std::string command);
 
 private:
  std::set<std::string> hosts;
  std::set<std::string> tokens;
};

#endif