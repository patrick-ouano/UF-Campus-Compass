// #include <catch2/catch_test_macros.hpp>
// #include <string>
// #include <vector>
// #include <sstream>
// #include <iostream>
// #include <algorithm>
// #include "CampusCompass.h"
//
// using namespace std;
//
// // Test Case 1: tests at least five incorrect commands
// TEST_CASE("Test 1: Incorrect Commands", "[flag]") {
//     // redirects a stringstream to cout - https://stackoverflow.com/questions/4191089/how-to-unit-test-function-writing-to-stdout-stdcout
//     ostringstream oss;
//     streambuf* rdbuf = std::cout.rdbuf();
//     cout.rdbuf(oss.rdbuf());
//
//     CampusCompass cc;
//     cc.ParseCSV("data/edges.csv", "data/classes.csv");
//
//     // invalid Name (contains numbers)
//     cc.ParseCommand("insert \"Pa3r1ck\" 12345678 1 1 COP3530");
//
//     // invalid ID (too short)
//     cc.ParseCommand("insert \"Patrick\" 123 1 1 COP3530");
//
//     // invalid Class Count (mismatch between N and provided classes)
//     cc.ParseCommand("insert \"Patrick\" 12345678 1 2 COP3530");
//
//     // invalid Class Code (bad format)
//     cc.ParseCommand("insert \"Patrick\" 12345678 1 1 badcode");
//
//     // unknown Command
//     cc.ParseCommand("fly 12345678");
//
//     cout.rdbuf(rdbuf);
//     REQUIRE(oss.str() == "unsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\n");
// }
//
// // Test Case 2: tests at least three edge cases
// TEST_CASE("Test 2: Edge Cases", "[edge_cases]") {
//     ostringstream oss;
//     streambuf* rdbuf = std::cout.rdbuf();
//     cout.rdbuf(oss.rdbuf());
//
//     CampusCompass cc;
//     cc.ParseCSV("data/edges.csv", "data/classes.csv");
//
//     // removing a student that doesn't exist
//     cc.ParseCommand("remove 67676767");
//
//     // dropping a class a student doesn't have
//     cc.insert("Patrick Ouano", 12345678, 1, {"COP3530"});
//     cc.dropClass(12345678, "MAC2311");
//
//     // checking edge status for non-existent location
//     cc.checkEdgeStatus(1, 9999);
//
//     cout.rdbuf(rdbuf);
//     REQUIRE(oss.str() == "unsuccessful\nsuccessful\nunsuccessful\nDNE\n");
// }
//
// // Test Case 3: tests dropClass, removeClass, remove, and replaceClass
// TEST_CASE("Test 3: Command Functionality", "[commands]") {
//     ostringstream oss;
//     streambuf* rdbuf = std::cout.rdbuf();
//     cout.rdbuf(oss.rdbuf());
//
//     CampusCompass cc;
//     cc.ParseCSV("data/edges.csv", "data/classes.csv");
//
//     cc.insert("Patrick Ouano", 67676767, 1, {"COP3530", "MAC2311"});
//
//     // replaces COP3530 with CDA3101
//     cc.replaceClass(67676767, "COP3530", "CDA3101");
//
//     // drops MAC2311
//     cc.dropClass(67676767, "MAC2311");
//
//     // removes CDA3101 from all students (student now has 0 classes, so they're auto-removed)
//     cc.removeClass("CDA3101");
//
//     cout.rdbuf(rdbuf);
//     REQUIRE(oss.str() == "successful\nsuccessful\nsuccessful\n1\n");
// }
//
// // Test Case 4: tests printShortestEdges
// TEST_CASE("Test 4: Shortest Path Dynamic Change", "[graph_logic]") {
//     ostringstream oss;
//     streambuf* rdbuf = std::cout.rdbuf();
//     cout.rdbuf(oss.rdbuf());
//
//     CampusCompass cc;
//     cc.ParseCSV("data/edges.csv", "data/classes.csv");
//
//     // Scenario:
//     // Patrick OUano lives at 43 (Malachowsky Hall), IDS2935 is at Location 7 (Computer Science & Engineering), Edge 43 -> 7 exists with weight 2.
//
//     // inserts student
//     cc.insert("Patrick Ouano", 87654321, 43, {"IDS2935"});
//
//     // checks initial path
//     cc.printShortestEdges(87654321);
//
//     // closes the direct edge (43 -> 7) and the indirect edge (43 -> 6)
//     cc.toggleEdgesClosure({43, 7, 43, 6});
//
//     // checks path again (should be -1/unreachable)
//     cc.printShortestEdges(87654321);
//
//     cout.rdbuf(rdbuf);
//     REQUIRE(oss.str() == "successful\n"
//                       "Name: Patrick Ouano\n"
//                       "IDS2935 | Total Time: 2\n"
//                       "successful\n"
//                       "Name: Patrick Ouano\n"
//                       "IDS2935 | Total Time: -1\n");
// }