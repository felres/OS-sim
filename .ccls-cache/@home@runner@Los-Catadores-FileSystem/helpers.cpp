#ifndef HELPERS_CPP
#define HELPERS_CPP

#include <string>
#include <iostream>
#include <stdio.h>

/* MACROS */
#define SIZE 100

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

/* FUNCTIONS */

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
 * Generates a random int in the range [min, max] (inclusive).
 * Lineal distribution.
 * Sourced from: https://stackoverflow.com/q/7560114
 *
 * @param min minimum number to pick from inclusive.
 * @param max maximun number to pick from inclusive.
 * @return random int.
 **/
static int random(int min, int max) //range : [min, max]
{
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
 * @return true if first char is a 'T' or 't'. False otherwise.
 **/
static bool strToBool(std::string str)
{
    if( (str.length()>0) && (toupper(str[0])=='T') )
        return true;
    return false;
}

#endif