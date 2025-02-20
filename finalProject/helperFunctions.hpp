#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

#include <string>

/// non-gameplay-related functions utilised by the rest of the program to help with low-level tasks

int random(int minVal, int maxVal);
// pre: valid int values greater than or equal to 0 for minVal and maxVal, maxVal greater than or equal to minVal
// post: return random integer between minVal and maxVal

int random(int maxVal);
// pre: valid int value greater than or equal to 0 for maxVal
// post: return random integer between 0 and maxVal

bool rollChance(int num, int denom);
// pre: valid int values greater than or equal to 1 or num and denom, denom is greater than or equal to num
// post: randomly return true at the odds of denom:num, returning false otherwise (random chance)

bool rollChance(int denom);
// pre: valid int value greater than 1 for denom
// post: randomly return true at the odds of denom:1, returning false otherwise

float percent(int p);
// pre: valid int value greater than 0
// post: returns int as a decimal percentage

void pressEnterToContinue(std::string prompt);
// pre: valid string parameter
// post: displays prompt, pauses program until user presses ENTER

void pressEnterToContinue();
// pre: N/A
// post: pauses program until user presses ENTER

int intInput(std::string prompt, int minVal, int maxVal);
// pre: valid string for prompt, valid int values for minVal and maxVal, maxVal greater than or equal to minVal
// post: display prompt, take and validate integer input between minVal and maxVal until valid value read, return result

std::string strInput(std::string prompt, int minLen, int maxLen);
// pre: valid string for prompt, valid int values greater than or equal to 0 for minLen and maxLen, maxLen greater than or equal to minLen
// post: display prompt, take and validate string input until reading string of length between minLen and maxLen, return result;

bool ynInput(std::string prompt, char yes, char no);
// pre: valid string for prompt, valid char values for yes and no
// post: display prompt, take and validate char input until reading input equal to yes or no parameter, return true if yes, false if no

#endif // HELPERFUNCTIONS_HPP
