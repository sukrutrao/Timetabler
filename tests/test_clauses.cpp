#include <gtest/gtest.h>
#include <iostream>
#include "cclause.h"
#include "clauses.h"
#include "global.h"
#include "time_tabler.h"

class TestClauses : public ::testing::Test {
 public:
  Lit lit[6];
  Clauses clauseG1, clauseG2;
  TestClauses() {}
  void SetUp();
  void TearDown() {}
  void printClause(Clauses);
};

void TestClauses::SetUp() {
  Timetabler *timetabler = new Timetabler();
  for (int i = 0; i < 6; i++) {
    lit[i] = timetabler->newLiteral(false);
  }
  std::vector<CClause> clauses1;
  std::vector<CClause> clauses2;
  CClause temp;
  temp.clear();
  temp.addLits(lit[0], lit[1]);
  clauses1.push_back(temp);
  temp.clear();
  temp.addLits(lit[2], ~lit[3], lit[4]);
  clauses1.push_back(temp);
  clauseG1 = Clauses(clauses1);
  temp.clear();
  temp.addLits(~lit[0], lit[5], lit[3]);
  clauses2.push_back(temp);
  temp.clear();
  temp.addLits(lit[1], ~lit[2], ~lit[3]);
  clauses2.push_back(temp);
  clauseG2 = Clauses(clauses2);
}

void TestClauses::printClause(Clauses input) {
  std::cout << "Start printing " << std::endl;
  std::vector<CClause> clauses = input.getClauses();
  for (int i = 0; i < clauses.size(); i++) {
    std::vector<Lit> lits = clauses[i].getLits();
    for (int j = 0; j < lits.size(); j++) {
      if (sign(lits[j])) {
        std::cout << "-";
      }
      std::cout << var(lits[j]) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "Done" << std::endl;
}

// Disabled because this need Timetabler and it fails (TODO)
/*TEST_F(TestClauses, ORTestNormal) {
    Clauses result = clauseG1 | clauseG2;

    std::vector<CClause> resultClauses = result.getClauses();
    ASSERT_EQ(resultClauses.size(), 4);

    ASSERT_EQ(resultClauses[0].getLits().size(), 2);
    ASSERT_EQ(resultClauses[1].getLits().size(), 4);
    ASSERT_EQ(resultClauses[2].getLits().size(), 2);
    ASSERT_EQ(resultClauses[3].getLits().size(), 2);

    ASSERT_EQ(resultClauses[0].getLits()[0], lit[0]);
    ASSERT_EQ(resultClauses[0].getLits()[1], ~lit[0]);

    ASSERT_EQ(resultClauses[1].getLits()[0], lit[0]);
    ASSERT_EQ(resultClauses[1].getLits()[1], lit[1]);
    ASSERT_EQ(resultClauses[1].getLits()[2], ~lit[2]);
    ASSERT_EQ(resultClauses[1].getLits()[3], ~lit[3]);

    ASSERT_EQ(resultClauses[2].getLits()[0], lit[3]);
    ASSERT_EQ(resultClauses[2].getLits()[1], ~lit[3]);

    ASSERT_EQ(resultClauses[3].getLits()[0], lit[2]);
    ASSERT_EQ(resultClauses[3].getLits()[1], ~lit[2]);
}*/

TEST_F(TestClauses, ORTestRHSEmpty) {
  Clauses clause;
  Clauses result = clauseG1 | clause;

  std::vector<CClause> resultClauses = result.getClauses();
  ASSERT_EQ(resultClauses.size(), 0);
}

TEST_F(TestClauses, ANDTestNormal) {
  Clauses result = clauseG1 & clauseG2;
  std::vector<CClause> resultClauses = result.getClauses();
  ASSERT_EQ(resultClauses.size(), 4);

  ASSERT_EQ(resultClauses[0].getLits()[0], lit[0]);
  ASSERT_EQ(resultClauses[0].getLits()[1], lit[1]);

  ASSERT_EQ(resultClauses[1].getLits()[0], lit[2]);
  ASSERT_EQ(resultClauses[1].getLits()[1], ~lit[3]);
  ASSERT_EQ(resultClauses[1].getLits()[2], lit[4]);

  ASSERT_EQ(resultClauses[2].getLits()[0], ~lit[0]);
  ASSERT_EQ(resultClauses[2].getLits()[1], lit[5]);
  ASSERT_EQ(resultClauses[2].getLits()[2], lit[3]);

  ASSERT_EQ(resultClauses[3].getLits()[0], lit[1]);
  ASSERT_EQ(resultClauses[3].getLits()[1], ~lit[2]);
  ASSERT_EQ(resultClauses[3].getLits()[2], ~lit[3]);
}

TEST_F(TestClauses, ANDTestRHSEmpty) {
  Clauses clause;
  Clauses result = clauseG1 & clause;
  std::vector<CClause> resultClauses = result.getClauses();
  ASSERT_EQ(resultClauses.size(), 2);

  ASSERT_EQ(resultClauses[0].getLits()[0], lit[0]);
  ASSERT_EQ(resultClauses[0].getLits()[1], lit[1]);

  ASSERT_EQ(resultClauses[1].getLits()[0], lit[2]);
  ASSERT_EQ(resultClauses[1].getLits()[1], ~lit[3]);
  ASSERT_EQ(resultClauses[1].getLits()[2], lit[4]);
}

// Disabled because this need Timetabler and it fails (TODO)
/*TEST_F(TestClauses, NOTTestNormal) {
    Clauses result = ~clauseG1;
    std::vector<CClause> resultClauses = result.getClauses();
    ASSERT_EQ(resultClauses.size(), 6);
    for (int i = 0; i <= 5; i++) {
        ASSERT_EQ(resultClauses[i].getLits().size(), 2);
    }

    ASSERT_EQ(resultClauses[0].getLits()[0], ~lit[0]);
    ASSERT_EQ(resultClauses[1].getLits()[0], ~lit[0]);
    ASSERT_EQ(resultClauses[2].getLits()[0], ~lit[0]);
    ASSERT_EQ(resultClauses[3].getLits()[0], ~lit[1]);
    ASSERT_EQ(resultClauses[4].getLits()[0], ~lit[1]);
    ASSERT_EQ(resultClauses[5].getLits()[0], ~lit[1]);

    ASSERT_EQ(resultClauses[0].getLits()[1], ~lit[2]);
    ASSERT_EQ(resultClauses[1].getLits()[1], lit[3]);
    ASSERT_EQ(resultClauses[2].getLits()[1], ~lit[4]);
    ASSERT_EQ(resultClauses[3].getLits()[1], ~lit[2]);
    ASSERT_EQ(resultClauses[4].getLits()[1], lit[3]);
    ASSERT_EQ(resultClauses[5].getLits()[1], ~lit[4]);
}*/

TEST_F(TestClauses, NOTTestEmpty) {
  Clauses clauses;
  Clauses result = ~clauses;
  std::vector<CClause> resultClauses = result.getClauses();
  ASSERT_EQ(resultClauses.size(), 1);
  std::vector<Lit> lits = resultClauses[0].getLits();
  ASSERT_EQ(lits.size(), 0);
}

TEST_F(TestClauses, IMPLIESTestNormal) {
  Clauses result = clauseG1 >> clauseG2;
  Clauses otherWay = (~clauseG1) | clauseG2;
  ASSERT_EQ(result.getClauses().size(), otherWay.getClauses().size());
  for (int i = 0; i < result.getClauses().size(); i++) {
    ASSERT_EQ(result.getClauses()[i].getLits().size(),
              otherWay.getClauses()[i].getLits().size());
  }
}
