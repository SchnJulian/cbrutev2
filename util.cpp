//
// Created by Julian Schnabel on 18.02.20.
//
#include <set>
#include "util.h"

namespace util {
    std::size_t remove_duplicates(char *arr, std::size_t n) {
        if (n == 0 || n == 1)
            return n;

        // To store index of next unique element
        std::size_t j = 0;

        // Doing same as done in Method 1
        // Just maintaining another updated index i.e. j
        for (std::size_t i = 0; i < n - 1; i++)
            if (arr[i] != arr[i + 1])
                arr[j++] = arr[i];

        arr[j++] = arr[n - 1];

        return j;
    }

    char *concat(const char *s_1, const char *s_2) {
        const size_t len_1 = strlen(s_1);
        const size_t len_2 = strlen(s_2);
        char *result = (char *) malloc(len_1 + len_2 + 1); // +1 for the null-terminator
        // in real code you would check for errors in malloc here
        memcpy(result, s_1, len_1);
        memcpy(result + len_1, s_2, len_2 + 1); // +1 to copy the null-terminator
        return result;
    }

    std::pair<std::string, long double> get_unit(long double bytes) {
        std::string unit = "B";
        auto divisor = 1L;
        if (bytes >= 1000 && bytes < 1000000) {
            unit = "KB ";
            divisor = 1000;
        } else if (bytes >= 1000000 && bytes < 1000000000) {
            unit = "MB";
            divisor = 1000000;
        } else if (bytes >= 1000000000 && bytes < 1000000000000) {
            unit = "GB";
            divisor = 1000000000;
        } else if (bytes >= 1000000000000) {
            unit = "TB";
            divisor = 1000000000000;
        }
        return std::make_pair(unit, divisor);
    }

    void clear_console() {
#if defined _WIN32
        system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");
#elif defined (__APPLE__)
        system("clear");
#endif
    }

    void logo() {
        std::string logo =
                R"(
          $$\                             $$\
          $$ |                            $$ |
 $$$$$$$\ $$$$$$$\   $$$$$$\  $$\   $$\ $$$$$$\    $$$$$$\
$$  _____|$$  __$$\ $$  __$$\ $$ |  $$ |\_$$  _|  $$  __$$\
$$ /      $$ |  $$ |$$ |  \__|$$ |  $$ |  $$ |    $$$$$$$$ |
$$ |      $$ |  $$ |$$ |      $$ |  $$ |  $$ |$$\ $$   ____|
\$$$$$$$\ $$$$$$$  |$$ |      \$$$$$$  |  \$$$$  |\$$$$$$$\
 \_______|\_______/ \__|       \______/    \____/  \_______|
)";
        std::cout << "\n\n\n" << logo << "\n\n\n";
    }

    bool disclaimer() {
        std::string disclaimer =
                R"(
Any actions and or activities related to the material contained within this program is solely your responsibility.
The misuse of the information in this website can result in criminal charges brought against the persons in question.
The author will not be held responsible in the event any criminal charges be brought against any individuals misusing cbrute to break the law.

**Your usage of cbrute constitutes your agreement to the following terms.**

1. All the information provided on this site are for **educational purposes only**. cbrute's developer is no not responsible for any misuse of the information.
2. cbrute is a program related to **Computer Security** and not a tool that promotes hacking / cracking.
3. I reserve the right to modify the disclaimer at any time without notice.

)";
        std::cout << "\n\n\n"
                  << disclaimer
                  << "I have read and agree to the terms and conditions: [y/n]: ";
        char input;
        std::cin >> input;
        return (tolower(input) == 'y');
    }

    bool memory_warning(long double bytes) {
        auto ud = get_unit(bytes);
        std::cout << "cbrute will generate a " << bytes / ud.second << " " << ud.first << " text file." << std::endl <<
                  "Start process? [y/n] ";
        char input;
        std::cin >> input;
        return (tolower(input) == 'y');
    }

    void help_menu() {
        std::string help =
                R"(

### CBRUTE 0.1 ###

    Generate combinations/permutations with cbrute!

USAGE:
    cbrute [arguments]

OPTIONS:
    -h                      This help menu
    -log                    Print results to the console [deprecated]

ARGUMENTS:
    -lmin <value>           Minimum length of the combination
    -lmax <value
    -c <values>             Character-set
    -f <path>               Path for output file with the combinations
    -perm                   No repetitions [deprecated]
                            cbrute will generate every possible permutation of the given characters
                            A custom -l argument will be ignored
    -ascii                  Full (printable) ASCII character-set
    -alphabetic <value>     Add full latin alphabet to character-set
                            l: lowercase letters
                            u: uppercase letters
    -alphabetic             Add full latin alphabet to character-set with both uppercase and lowercase
                            characters
    -digit                  Add digits from 0-9 to the character-set
    -special                Add special characters to the character-set
    -x <values>             Excluded characters
    -prepend <value>        Prepend <string>
    -append <value>         Append <string>
    -threads <value>        Manual thread override: Calculate combinations with the amount of threads specified by
                            the user. (Not recommended by default!) [deprecated]
    )";
        std::cout << help << std::endl;
    }

    unsigned long factorial(unsigned long n) {
        auto n_res = 1UL;
        for (int i = 2; i <= n; ++i) {
            n_res *= i;
        }
        return n_res;
    }

}
