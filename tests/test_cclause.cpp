#include "cclause.h"
#include "clauses.h"
#include <gtest/gtest.h>
#include "time_tabler.h"
#include "global.h"
#include <iostream>

class TestCClause : public ::testing::Test {
public:
	Lit lit[6];
	CClause clause1, clause2;
	TestCClause() {}
	void SetUp();
	void TearDown() {}
	void printClause(CClause);
};

void TestClauses::SetUp() {
	for(int i = 0; i < 6; i++) {
		lit[i] = Global::timeTabler->newLiteral(false);
	}
	clause1 = CClause(lit[0], lit[1], lit[3]);
	clause2 = CClause(~lit[0], lit[5], lit[3]);
}

void TestClauses::printClause(CClause input) {
	std::cout << "Start printing " << std::endl;
	std::vector<Lit> lits = input.getLits();
	for(int j = 0; j < lits.size(); j++) {
		if(sign(lits[j])) {
			std::cout<<"-";
		}
		std::cout << var(lits[j]) << " ";
	}
	std::cout << std::endl;
	std::cout << "Done" << std::endl;
}

// TODO - these are the tests for Clauses. Have to modify for CClause.

TEST_F(TestCClause, ORTestNormal) {
	CClause result = clauseG1 | clauseG2;
	std::vector<Lit> resultClauses = result.getLits();
	ASSERT_EQ(resultClauses.size(), 5);
	ASSERT_EQ(resultClauses[0].getLits().size(), 2);

	ASSERT_EQ(resultClauses[1].getLits().size(), 3);
	ASSERT_EQ(resultClauses[2].getLits().size(), 4);
	ASSERT_EQ(resultClauses[3].getLits().size(), 4);
	ASSERT_EQ(resultClauses[4].getLits().size(), 4);
	
	ASSERT_EQ(resultClauses[1].getLits()[0], ~auxLits[0]);
	ASSERT_EQ(resultClauses[2].getLits()[0], ~auxLits[0]);
	ASSERT_EQ(resultClauses[3].getLits()[0], ~auxLits[1]);
	ASSERT_EQ(resultClauses[4].getLits()[0], ~auxLits[1]);
	
	ASSERT_EQ(resultClauses[1].getLits()[1], lit[0]);
	ASSERT_EQ(resultClauses[1].getLits()[2], lit[1]);

	ASSERT_EQ(resultClauses[2].getLits()[1], lit[2]);
	ASSERT_EQ(resultClauses[2].getLits()[2], ~lit[3]);
	ASSERT_EQ(resultClauses[2].getLits()[3], lit[4]);

	ASSERT_EQ(resultClauses[3].getLits()[1], ~lit[0]);
	ASSERT_EQ(resultClauses[3].getLits()[2], lit[5]);
	ASSERT_EQ(resultClauses[3].getLits()[3], lit[3]);

	ASSERT_EQ(resultClauses[4].getLits()[1], lit[1]);
	ASSERT_EQ(resultClauses[4].getLits()[2], ~lit[2]);
	ASSERT_EQ(resultClauses[4].getLits()[3], ~lit[3]);

}

TEST_F(TestClauses, ORTestRHSEmpty) {
	Clauses clause;
	Clauses result = clauseG1 | clause;
	std::vector<CClause> resultClauses = result.getClauses();
	ASSERT_EQ(resultClauses.size(), 2);

	ASSERT_EQ(resultClauses[0].getLits()[0], lit[0]);
	ASSERT_EQ(resultClauses[0].getLits()[1], lit[1]);

	ASSERT_EQ(resultClauses[1].getLits()[0], lit[2]);
	ASSERT_EQ(resultClauses[1].getLits()[1], ~lit[3]);
	ASSERT_EQ(resultClauses[1].getLits()[2], lit[4]);
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

TEST_F(TestClauses, NOTTestNormal) {
	Clauses result = ~clauseG1;
	std::vector<CClause> resultClauses = result.getClauses();
	ASSERT_EQ(resultClauses.size(), 6);
	ASSERT_EQ(resultClauses[0].getLits().size(), 2);
	Lit auxLits[2];
	for(int i = 0; i < resultClauses[0].getLits().size(); i++) {
		auxLits[i] = resultClauses[0].getLits()[i];
	}
	for(int i = 1; i <=5; i++) {
		ASSERT_EQ(resultClauses[i].getLits().size(), 2);
	}

	
	ASSERT_EQ(resultClauses[1].getLits()[0], ~auxLits[0]);
	ASSERT_EQ(resultClauses[2].getLits()[0], ~auxLits[0]);
	ASSERT_EQ(resultClauses[3].getLits()[0], ~auxLits[1]);
	ASSERT_EQ(resultClauses[4].getLits()[0], ~auxLits[1]);
	ASSERT_EQ(resultClauses[5].getLits()[0], ~auxLits[1]);
	
	ASSERT_EQ(resultClauses[1].getLits()[1], ~lit[0]);
	ASSERT_EQ(resultClauses[2].getLits()[1], ~lit[1]);
	ASSERT_EQ(resultClauses[3].getLits()[1], ~lit[2]);
	ASSERT_EQ(resultClauses[4].getLits()[1], lit[3]);
	ASSERT_EQ(resultClauses[5].getLits()[1], ~lit[4]);
}

TEST_F(TestClauses, NOTTestEmpty) {
	Clauses clauses;
	Clauses result = ~clauses;
	std::vector<CClause> resultClauses = result.getClauses();
	ASSERT_EQ(resultClauses.size(), 0);
}

TEST_F(TestClauses, IMPLIESTestNormal) {
	/*printClause(clauseG1);
	printClause(clauseG2);*/
	Clauses result = clauseG1>>clauseG2;
	/*printClause(clauseG1);
	printClause(clauseG2);*/
	Clauses otherWay = (~clauseG1) | clauseG2;
	/*printClause(clauseG1);
	printClause(clauseG2);
	printClause(result);
	printClause(otherWay);*/
	ASSERT_EQ(result.getClauses().size(), otherWay.getClauses().size());
	for(int i = 0; i < result.getClauses().size(); i++) {
		ASSERT_EQ(result.getClauses()[i].getLits().size(), otherWay.getClauses()[i].getLits().size());
	}
	// Checked using print that values were ok. Auxiliary vars causing issues
}


