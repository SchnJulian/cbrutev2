//
// Created by Julian Schnabel on 18.02.20.
//

#ifndef CBRUTEV2_UTIL_H
#define CBRUTEV2_UTIL_H

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <map>
#include <queue>
#include <thread>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <iostream>

namespace util {
    std::size_t remove_duplicates(char *arr, std::size_t n);

    char *concat(const char *s_1, const char *s_2);

    std::pair<std::string, long double> get_unit(long double bytes);

    void clear_console();

    void logo();

    bool disclaimer();

    bool memory_warning(long double bytes);

    void help_menu();

    unsigned long factorial(unsigned long n);
}
#endif //CBRUTEV2_UTIL_H
