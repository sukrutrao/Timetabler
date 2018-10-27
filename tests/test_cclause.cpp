#include <gtest/gtest.h>
#include <iostream>
#include "cclause.h"
#include "clauses.h"
#include "global.h"
#include "time_tabler.h"

class TestCClause : public ::testing::Test {
 public:
  Lit lit[6];
  CClause clause1, clause2;
  TestCClause() {}
  void SetUp();
  void TearDown() {}
  void printClause(CClause);
};

void TestCClause::SetUp() {
  Timetabler *timetabler = new Timetabler();
  for (int i = 0; i < 6; i++) {
    lit[i] = timetabler->newLiteral(false);
  }
  clause1.addLits(lit[0]);
  clause1.addLits(lit[1]);
  clause1.addLits(lit[3]);
  clause2.addLits(~lit[0]);
  clause2.addLits(lit[5]);
  clause2.addLits(lit[3]);
}

void TestCClause::printClause(CClause input) {
  std::cout << "Start printing " << std::endl;
  std::vector<Lit> lits = input.getLits();
  for (unsigned j = 0; j < lits.size(); j++) {
    if (sign(lits[j])) {
      std::cout << "-";
    }
    std::cout << var(lits[j]) << " ";
  }
  std::cout << std::endl;
  std::cout << "Done" << std::endl;
}

TEST_F(TestCClause, ORTestNormal) {
  CClause result = clause1 | clause2;
  std::vector<Lit> resultLits = result.getLits();

  ASSERT_EQ(resultLits.size(), 2);

  ASSERT_EQ(resultLits[0], lit[0]);
  ASSERT_EQ(resultLits[1], ~lit[0]);
}

TEST_F(TestCClause, ORTestRHSEmpty) {
  CClause clause;
  CClause result = clause1 | clause;
  std::vector<Lit> resultLits = result.getLits();

  ASSERT_EQ(resultLits.size(), 3);

  ASSERT_EQ(resultLits[0], lit[0]);
  ASSERT_EQ(resultLits[1], lit[1]);
  ASSERT_EQ(resultLits[2], lit[3]);
}

TEST_F(TestCClause, ANDTestNormal) {
  std::vector<CClause> result = clause1 & clause2;

  ASSERT_EQ(result.size(), 2);

  ASSERT_EQ(result[0].getLits(), clause1.getLits());
  ASSERT_EQ(result[1].getLits(), clause2.getLits());
}

TEST_F(TestCClause, ANDTestRHSEmpty) {
  CClause clause;
  std::vector<CClause> result = clause1 & clause;

  ASSERT_EQ(result.size(), 2);

  ASSERT_EQ(result[0].getLits(), clause1.getLits());
  ASSERT_EQ(result[1].getLits(), clause.getLits());
}

TEST_F(TestCClause, NOTTestNormal) {
  std::vector<CClause> result = ~clause1;

  ASSERT_EQ(result.size(), 3);

  ASSERT_EQ(result[0].getLits().size(), 1);
  ASSERT_EQ(result[1].getLits().size(), 1);
  ASSERT_EQ(result[2].getLits().size(), 1);

  ASSERT_EQ(result[0].getLits()[0], ~lit[0]);
  ASSERT_EQ(result[1].getLits()[0], ~lit[1]);
  ASSERT_EQ(result[2].getLits()[0], ~lit[3]);
}

TEST_F(TestCClause, NOTTestEmpty) {
  CClause clauses;
  std::vector<CClause> result = ~clauses;

  ASSERT_EQ(result.size(), 0);
}