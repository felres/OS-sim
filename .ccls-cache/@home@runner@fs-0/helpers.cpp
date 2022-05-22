#ifndef HELPERS_CPP
#define HELPERS_CPP

#include <string>
#include <iostream>

/* MACROS */

/* COLORES ANSI 
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

/* FUNCIONES */

/*
 * Manejo de errores.
 */
static int err(int n)
{
    std::cerr << "ERROR(" << n << ")!\n";
    return n;
}
static int err(std::string str, int n)
{
    std::cerr << "ERROR(" << n << "): " << str << "\n";
    return n;
}

// https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
static int random(int min, int max) //range : [min, max]
{
   return min + std::rand() % (( max + 1 ) - min);
}

static std::string chToStr(char ch)
{
    std::string str = "";
    str.push_back(ch);
    return str;
}

#endif