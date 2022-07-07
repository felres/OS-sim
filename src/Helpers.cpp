/* *****************************************************************************
 * 
 * General purpose helper script.
 * 
 */

#ifndef HELPERS_CPP
#define HELPERS_CPP

#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <ios>

/* MACROS */
#define SIZE 256
#define SHIFT 7


/**
 * ANSI COLORS
 * Sourced from:
 * https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
 * https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
 */
// general
#define RESET               "\033[0m"
#define BOLD                "\u001b[1m"
#define FAINT               "\u001b[2m"
#define UNDERLINE           "\u001b[4m"
#define REVERSED            "\u001b[7m"
// colors
#define BLACK               "\033[30m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define YELLOW              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define WHITE               "\033[37m"
// background colors
#define BACK_BLACK          "\u001b[40m"
#define BACK_RED            "\u001b[41m"
#define BACK_GREEN          "\u001b[42m"
#define BACK_YELLOW         "\u001b[43m"
#define BACK_BLUE           "\u001b[44m"
#define BACK_MAGENTA        "\u001b[45m"
#define BACK_CYAN           "\u001b[46m"
#define BACK_WHITE          "\u001b[47m"

static int printYesNo(bool v)
{
    if(v)
        std::cout << RESET << GREEN << "Yes";
    else
        std::cout << RESET << RED << "No";

    std::cout << RESET;
    return 0;
}

/**
 * Recieves text and shift and does ceasar encryption
 * Sourced: geeksforgeeks.org/caesar-cipher-in-cryptography
 *
 * @param text to shift
 * @param shift amount
 * @return new text
 */
static std::string ceasarShift(std::string text, int s)
{
    std::string result = "";
    // traverse text
    for (int i=0;i<text.length();i++)
    {
        if (isupper(text[i]))
            result += char(int(text[i]+s-65)%26 +65);
        else
            result += char(int(text[i]+s-97)%26 +97);
    }
    return result;
}

/**
 * Returns true if string is a number.
 *
 * @param str to check.
 * @return true or false.
 */
static bool isNumber(std::string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (isdigit(s[i]) == 0)
              return false;
    }
    return true;
}

/**
 * Prints a vector.
 */
static int printVec(std::vector<std::string> vec)
{
    std::cout << RESET << BOLD;
    std::cout << "Printing vector of size "<<vec.size()<<":\n";
    for(int i = 0; i < vec.size(); i++)
    {
        std::cout << RESET;
        std::cout << "["<<i<<"] ";
        std::cout << RESET << GREEN;
        std::cout << vec[i] << "\n";
    }
    std::cout << "\n";
    std::cout << RESET;
    return 0;
}

static bool isIntIncludedInArray(int n, int *arr, int arrlen)
{
    for(int i = 0; i < arrlen; i++)
    {
        if(arr[i] == n) return true;
    }
    return false;
}

/**
 * Prints a normal string, but changing a specified char index(es)
 * and underlining said char.
 * Includes optional parameters in case you wanna underline more than 1 char.
 * Useful for showing a shortcut for a command! 
 * 
 * @param string to underline chars to
 * @param index of string to underline (can be multiple args. comma separated)
 * @return 0
 */
static int printWithUnderlineAt(std::string str,int i0 = -1,
                                                int i1 = -1,
                                                int i2 = -1,
                                                int i3 = -1,
                                                int i4 = -1,
                                                int i5 = -1)
{
    int arr[6] = {i0, i1, i2, i3, i4, i5};
    for(int i = 0; i < str.length(); i++)
    {
        if(isIntIncludedInArray(i, arr, 6))
            std::cout << RESET << UNDERLINE << BOLD;
        else
            std::cout << RESET;
        
        std::cout << str[i];
    }
    std::cout << RESET;
    return 0;
}

/**
 * Prints a normal string, but changing a specified char and coloring it
 * Useful for printing special characters explicitly.
 * Example: changing '\n' to '#'.
 * printReplacing("hello", 'l', 'w') ptints "hewwo"
 * 
 * @param str string to substitute on.
 * @param ch char to substitute.
 * @param ch char to replace
 * @return 0
 */
static int printReplacing(std::string str, char ch, char replacement)
{
    for(int i = 0; i < str.length(); i++)
    {
        if(str[i] == ch)
        {
            std::cout << RESET << GREEN;
            std::cout << replacement;
        }
        else
        {
            std::cout << RESET;
            std::cout << str[i];
        }
    }
    std::cout << RESET;
    return 0;
}

/**
 * Splits string into words in a string vector.
 * 
 * @param text string to split
 * @param delim delimiter to use to split string.
 * @return vector with strings.
 */
static std::vector<std::string> tokenize(std::string text, std::string delim)
{
    //std::string delim = " ";
    std::vector<std::string> words{};

    size_t pos = 0;
    while ((pos = text.find(delim)) != std::string::npos) {
        words.push_back(text.substr(0, pos));
        text.erase(0, pos + delim.length());
    }
    if (!text.empty())
        words.push_back(text.substr(0, pos));
    return words;
}

/**
 * Writes (or overwrites) a string into a given filename
 * 
 */
static int writeFile(std::string fileName, std::string str)
{
    remove(fileName.c_str());
    std::ofstream myfile;
    myfile.open (fileName);
    myfile << str;
    myfile.close();
    return 0;
}

/**
 * Reads a file and returns it (dividing by lines) into a vector.
 *
 */
static std::vector<std::string> readFileToVector(std::string fileName)
{
    std::vector<std::string> vec;
    std::ifstream myfile;
    myfile.open(fileName);
    std::string myline;
    if(myfile.is_open())
    {
        while ( myfile )
        {
            std::getline (myfile, myline);
            vec.push_back(myline);
        }
    }
    else
    {
        std::cout << RESET << RED;
        std::cout << "Couldn't open file!" << "\n";
        std::cout << RESET;
        std::cout   << "    is.badbit: " << myfile.badbit << "\n"
                    << "    is.failbit: " << myfile.failbit << "\n"
                    << "    is.eofbit: " << myfile.eofbit << "\n"
                    << "    is.bad(): " << myfile.bad() << "\n" 
                    << "    is.fail(): " << myfile.fail() << "\n"
                    << "    is.eof(): " << myfile.eof() << "\n"
                    << "    is.rdstate(): " << myfile.rdstate() << "\n";
        std::cout << RESET;
        return {};
    }
    myfile.close();
    // bug: last element is added twice
    vec.erase(vec.end());
    return vec;
}

/**
 * Error Handling. Prints error num to cerr.
 *
 * @param error number.
 * @return error number.
 **/
static int err(int n)
{
    std::cerr << RESET << RED;
    std::cerr << "ERROR(" << n << ")!\n";
    std::cerr << RESET;
    return n;
}

/**
 * Error Handling. Prints error num and message to cerr.
 *
 * @param error message.
 * @param error number.
 * @return error number.
 **/
static int err(std::string str, int n)
{
    std::cerr << RESET << RED;
    std::cerr << "ERROR(" << n << "): " << str << "\n";
    std::cerr << RESET;
    return n;
}

/**
 * Makes rand() give different results each file execution.
 **/
static void randomize()
{
    // random seed
    srand( time(NULL) );
}

/**
 * Generates a random int in the range [min, max] (inclusive).
 * Linear distribution.
 * Sourced from: https://stackoverflow.com/q/7560114
 *
 * @param min minimum number to pick from inclusive.
 * @param max maximun number to pick from inclusive.
 * @return random int in range.
 **/
static int random(int min, int max)
{
    //range : [min, max]
    return min + std::rand() % (( max + 1 ) - min);
}

/**
 * Transform char into c++ string.
 *
 * @param ch char to tranform.
 * @return c++ string.
 **/
static std::string chToStr(char ch)
{
    std::string str = "";
    str.push_back(ch);
    return str;
}

/**
 * Interprets a c++ string into a bool value.
 *
 * @param str string to interpret.
 * @return true if first char is a 'T' or 't' or '1'. False otherwise.
 **/
static bool strToBool(std::string str)
{
    if( (str.length()>0) && ((toupper(str[0])=='T')||((str[0])=='1')))
        return true;
    return false;
}

#endif