#ifndef HELPERFUNCTIONS_CPP
#define HELPERFUNCTIONS_CPP

#include <iostream>
#include <cstdlib>
#include <string>
#include "helperFunctions.hpp"

int random(int minVal, int maxVal)
{
    if (minVal > maxVal) throw std::logic_error("Function random() called with min parameter greater than max parameter."); // enforce precondition

    // current method (may be changed later)
    int result;
    do {result = (rand() % (maxVal + 1));} // get a random number lower than the max using the default rand function
    while (result < minVal); // repeat until random value greater than min

    return result; // return
}

int random(int maxVal)
{
    return random(0, maxVal);
}

bool rollChance(int num, int denom)
{
    // num stands for numerator, denom stands for denominator
    // enforce preconditions
    if (num < 1 || denom < 1) throw std::logic_error("Function rollChance() called with parameter lower than allowed range.");
    if (denom < num) throw std::logic_error("Function rollChance() called with min parameter greater than max parameter");

    // get random value
    int result = random(1, denom);

    // match up with odds parameters, return accordingly
    if (result <= num) return true;
    return false;
}

bool rollChance(int denom)
{
    return rollChance(1, denom);
}

float percent(int p)
{
    if (p < 0) throw std::logic_error("Function percent() called with parameter outside of allowed range"); // enforce precondition
    return static_cast<float>(p / 100.0); // return decimal
}

void pressEnterToContinue()
{
    std::string s;
    getline(std::cin, s); // initialize a string and call getline on it, leaving the function once the user presses the ENTER key
    std::cout << '\n';
}

void pressEnterToContinue(std::string prompt)
{
    std::cout << prompt; // same as above except it displays a prompt first
    std::string s;
    getline(std::cin, s);
    std::cout << '\n;'
}

int intInput(std::string prompt, int minVal, int maxVal)
{
    if (maxVal < 0) maxVal = 0; // take care of negative values
    if (minVal > maxVal) throw std::logic_error("Function intInput() called with min parameter greater than max parameter."); // enforce precondition

    int input; // input variable
    do
    {
        try
        {
            // std::cin.clear();
            std::cout << prompt; // display prompt for input
            std::cin >> input; // take input

            // check if input isn't valid, throw execptions accordingly
            if (input < minVal) throw std::runtime_error("Entered value too low. Please try again.");
            if (input > maxVal) throw std::runtime_error("Entered value too high. Please try again.");
            if (std::cin.fail()) throw std::runtime_error("Please enter a valid integer value.");

            // if input makes it past execption checks, return valid value
            return input;
        }
        catch (std::runtime_error inputError) // for invalid input:
        {
            std::cerr << inputError.what() << '\n'; // display error message
            std::cin.clear(); // clear input buffer to prepare for second attempt to get valid input
            std::cin.ignore(INT_MAX, '\n');
        }
    } while (true); // if input was valid, function should've returned by this point. repeat input process until valid value read
}

std::string strInput(std::string prompt, int minLen, int maxLen)
{
    if (minLen > maxLen) throw std::logic_error("Function strInput() called with min parameter greater than max parameter."); // enforce precondition

    std::string input; // input string
    do
    {
        try
        {
            //std::cin.clear();
            std::cout << prompt; // display prompt for input
             //std::cin.ignore('\n');
            getline(std::cin, input); // take input

            // check if input isn't valid, throw execptions accordingly
            if (input.length() < minLen) throw std::runtime_error("Too short. Please try again.");
            if (input.length() > maxLen) throw std::runtime_error("Too long. Please try again.");

            // if input makes it past execption checks, return valid value
            return input;
        }
        catch (std::runtime_error inputError) // for invalid input:
        {
            std::cerr << inputError.what() << '\n'; // display error message
            std::cin.clear(); // clear input buffer to prepare for second attempt to get valid input
            std::cin.ignore(INT_MAX, '\n');
        }
    } while (true); // if input was valid, function should've returned by this point. repeat input process until valid value read
}

bool ynInput(std::string prompt, char yes, char no)
{
    char input; // input variable
    do
    {
        try
        {
            std::cout << prompt; // display prompt for input
            std::cin >> input; // take input

            // see if input is one of two valid responses
            if (tolower(input) == tolower(yes)) return true; // return accordingly
            if (tolower(input) == tolower(no)) return false;

            // if not valid, proceed to throwing exception
            throw std::runtime_error("Please select one of the two valid options.");
        }
        catch (std::runtime_error inputError) // for invalid input:
        {
            std::cerr << inputError.what() << '\n'; // display error message
            std::cin.clear(); // clear input buffer to prepare for second attempt to get valid input
            std::cin.ignore(INT_MAX, '\n');
        }
    } while (true); // if input was valid, function should've returned by this point. repeat input process until valid value read
}

#endif // HELPERFUNCTIONS_CPP
