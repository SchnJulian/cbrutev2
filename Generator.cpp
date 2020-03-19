//
// Created by Julian Schnabel on 18.02.20.
//

#include <set>
#include "Generator.h"


Generator::Generator(int argc, char **argv) { parse_arguments(argc, argv); }

Generator::~Generator() {
    free(char_pool);
    fclose(output_file);
    if (append)
        free(append_str);
    if (prepend)
        free(prepend_str);
}

void Generator::get_comb_k(const std::string &prefix, std::size_t k) {

    // Base case: k is 0,
    // print prefix
    if (k == 0) {
        {
            std::lock_guard<std::mutex> lock(m);
            combinations.emplace(prefix.c_str());
        }
        return;
    }

    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < element_count; i++) {
        std::string new_prefix;

        // Next character of input added
        new_prefix = prefix + char_pool[i];

        // k is decreased, because
        // we have added a new character
        get_comb_k(new_prefix, k - 1);
    }
}

void Generator::get_comb() {
    for (auto i = length_min; i <= length_max; i++) {
        get_comb_k("", i);
    }
}

bool Generator::parse_arguments(int argc, char **args) {

    std::map<std::string, std::string> argument_map;
    auto length_min_in = std::make_pair("-lmin", "");
    auto length_max_in = std::make_pair("-lmax", "");
    auto characters = std::make_pair("-c", "");
    auto file_path = std::make_pair("-f", "");
    auto perm = std::make_pair("-perm", "");
    auto log = std::make_pair("-log", "");
    auto alphabetic = std::make_pair("-alphabetic", "");
    auto digit = std::make_pair("-digit", "");
    auto special = std::make_pair("-special", "");
    auto ascii = std::make_pair("-ascii", "");
    auto exclude = std::make_pair("-x", "");
    auto prepend = std::make_pair("-prepend", "");
    auto append = std::make_pair("-append", "");
    auto threads = std::make_pair("-threads", "");

    argument_map.insert(length_min_in);
    argument_map.insert(length_max_in);
    argument_map.insert(characters);
    argument_map.insert(file_path);
    argument_map.insert(perm);
    argument_map.insert(log);
    argument_map.insert(alphabetic);
    argument_map.insert(digit);
    argument_map.insert(special);
    argument_map.insert(ascii);
    argument_map.insert(exclude);
    argument_map.insert(prepend);
    argument_map.insert(append);
    argument_map.insert(threads);

    // Iterate through arg string
    for (int i = 1; i < argc - 1; ++i) {
        if (argument_map.find(args[i]) != std::end(argument_map)) {
            argument_map.find(args[i])->second = args[i + 1];
        }
    }
    // Check last argument
    if (argument_map.find(args[argc - 1]) != std::end(argument_map))
        argument_map.find(args[argc - 1])->second = "1";

    try {
        // Check if user wants to permute a string
        this->permute = !(argument_map.at(perm.first).empty());

        if (this->permute) {
            this->permute_str = argument_map.at(perm.first);
        } else {
            this->length_min = std::stoul(argument_map.at(length_min_in.first));
            this->length_max = std::stoul(argument_map.at(length_max_in.first));
        }

        // Check if user wants to print the combinations/permutations to a file
        this->path = argument_map.at(file_path.first);
        if (!path.empty()) {
            file_mode = true;
            if (path.find(DEFAULT_FILE_EXTENSION) == std::string::npos) {
                // Add missing file extension
                path.append(DEFAULT_FILE_EXTENSION);
            }
        }

        this->console_log = !(argument_map.at(log.first).empty());

        // String to prepend before
        if (!(argument_map.at(prepend.first).empty())) {
            prepend_str = static_cast<char *>(
                    malloc(argument_map.at(append.first).size() * sizeof(char)));
            strcpy(prepend_str, argument_map.at(prepend.first).c_str());
            this->prepend = true;
        }

        // String to append to
        if (!(argument_map.at(append.first).empty())) {
            append_str = static_cast<char *>(
                    malloc(argument_map.at(append.first).size() * sizeof(char)));
            strcpy(append_str, argument_map.at(append.first).c_str());
            this->append = true;
        }

        // Check if full ascii set was activated
        if (!(argument_map.at(ascii.first).empty())) {
            char_pool = (char *) malloc(sizeof(Generator::ASCII));
            strcpy(char_pool, Generator::ASCII);
        } else {
            // Initialize alphabetic character set
            if (argument_map.at(alphabetic.first) == "u") {

                if (char_pool != nullptr) {
                    char *buf = (char *) malloc(strlen(char_pool) * sizeof(char));
                    memcpy(buf, char_pool, strlen(char_pool) + 1);
                    char_pool = util::concat(buf, Generator::ALPHABETIC_U);
                    // free buffer
                    free(buf);
                } else {
                    char_pool = (char *) malloc(sizeof(Generator::ALPHABETIC_U));
                    memcpy(char_pool, Generator::ALPHABETIC_U,
                           strlen(Generator::ALPHABETIC_U) + 1);
                }

            } else if (argument_map.at(alphabetic.first) == "l") {

                if (char_pool != nullptr) {
                    char *buf = (char *) malloc(strlen(char_pool) * sizeof(char));
                    memcpy(buf, char_pool, strlen(char_pool) + 1);
                    char_pool = util::concat(buf, Generator::ALPHABETIC_L);
                    // free buffer
                    free(buf);
                } else {
                    char_pool = (char *) malloc(sizeof(Generator::ALPHABETIC_L));
                    memcpy(char_pool, Generator::ALPHABETIC_L,
                           strlen(Generator::ALPHABETIC_L) + 1);
                }

            } else if (!(argument_map.at(alphabetic.first).empty())) {
                if (char_pool != nullptr) {
                    char *buf = (char *) malloc(strlen(char_pool) * sizeof(char));
                    memcpy(buf, char_pool, strlen(char_pool) + 1);
                    char_pool = util::concat(buf, Generator::ALPHABETIC);
                    // free buffer
                    free(buf);
                } else {
                    char_pool = (char *) malloc(sizeof(Generator::ALPHABETIC));
                    memcpy(char_pool, Generator::ALPHABETIC,
                           strlen(Generator::ALPHABETIC) + 1);
                }
            }

            // Initialize numeric character set
            if (!(argument_map.at(digit.first).empty())) {
                if (char_pool != nullptr) {
                    char *buf = (char *) malloc(strlen(char_pool) * sizeof(char));
                    memcpy(buf, char_pool, strlen(char_pool) + 1);
                    char_pool = util::concat(buf, Generator::NUMERIC);
                    // free buffer
                    free(buf);
                } else {
                    char_pool = (char *) malloc(sizeof(Generator::NUMERIC));
                    memcpy(char_pool, Generator::NUMERIC, strlen(Generator::NUMERIC) + 1);
                }
            }

            // Initialize special character set
            if (!(argument_map.at(special.first).empty())) {
                if (char_pool != nullptr) {
                    char *buf = (char *) malloc(strlen(char_pool) * sizeof(char));
                    memcpy(buf, char_pool, strlen(char_pool) + 1);
                    char_pool = util::concat(buf, Generator::SPECIAL);
                    // free buffer
                    free(buf);
                } else {
                    char_pool = (char *) malloc(sizeof(Generator::SPECIAL));
                    memcpy(char_pool, Generator::SPECIAL, strlen(Generator::SPECIAL) + 1);
                }
            }

            if (!(argument_map.at(characters.first).empty())) {
                // Initialize character set
                auto temp = argument_map.at(characters.first);

                if (char_pool != nullptr) {
                    char *buf = (char *) malloc(strlen(char_pool) * sizeof(char));
                    memcpy(buf, char_pool, strlen(char_pool) + 1);
                    char_pool = util::concat(buf, temp.c_str());
                    // free buffer
                    free(buf);
                } else {
                    char_pool = (char *) malloc(temp.size());
                    memcpy(char_pool, temp.c_str(), strlen(temp.c_str()) + 1);
                }
            }
            if (!permute) {
                std::sort(char_pool, char_pool + strlen(char_pool));
                util::remove_duplicates(char_pool, strlen(char_pool));
                element_count = strlen(char_pool);
            } else {
                std::sort(std::begin(permute_str), std::end(permute_str));
            }

            if (!argument_map.at(exclude.first).empty()) {
                auto tmp = std::string(this->char_pool);
                auto rmv = std::string(argument_map.at(exclude.first));
                std::sort(std::begin(rmv), std::end(rmv));

                tmp.erase(std::remove_if(std::begin(tmp), std::end(tmp), [rmv](auto x) {
                    return std::binary_search(std::begin(rmv), std::end(rmv), x);
                }));

                free(char_pool);
                char_pool = (char *) malloc(tmp.size());
                memcpy(char_pool, tmp.c_str(), strlen(tmp.c_str()) + 1);
            }
        }

    } catch (std::invalid_argument &e) {
        return false;
    }
    return true;
}

bool Generator::init_file() {
    output_file = fopen(this->path.c_str(), "w");
    return (output_file != nullptr);
}

bool Generator::start() {
    begin = std::chrono::high_resolution_clock::now();
    get_total_n();
    get_approximate_memory();

    if (file_mode) {
        if (permute) {
            if (init_file()) {
                std::thread generate_t(&Generator::get_permutations, this);
                std::thread tptr;
                running = true;

                if (append && prepend) {
                    std::thread print_t(&Generator::print_append_prepend, this);
                    tptr = std::move(print_t);
                } else if (prepend) {
                    std::thread print_t(&Generator::print_prepend, this);
                    tptr = std::move(print_t);
                } else if (append) {
                    std::thread print_t(&Generator::print_append, this);
                    tptr = std::move(print_t);
                } else {
                    std::thread print_t(&Generator::print_comb, this);
                    tptr = std::move(print_t);
                }

                if (generate_t.joinable()) {
                    generate_t.join();
                    running = false;
                }
                if (tptr.joinable()) {
                    tptr.join();
                }
            }
        } else {
            if (init_file()) {
                std::thread generate_t(&Generator::get_comb, this);
                running = true;
                std::thread tptr;
                if (append && prepend) {
                    std::thread print_t(&Generator::print_append_prepend, this);
                    tptr = std::move(print_t);
                } else if (prepend) {
                    std::thread print_t(&Generator::print_prepend, this);
                    tptr = std::move(print_t);
                } else if (append) {
                    std::thread print_t(&Generator::print_append, this);
                    tptr = std::move(print_t);
                } else {
                    std::thread print_t(&Generator::print_comb, this);
                    tptr = std::move(print_t);
                }

                if (generate_t.joinable()) {
                    generate_t.join();
                    running = false;
                }

                if (tptr.joinable()) {
                    tptr.join();
                }
            }
        }
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    print_report(std::cout);
    return true;
}

void Generator::print_comb() {
    todo_print = total_N;
    while (todo_print > 0) {
        if (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::queue<std::string> temp;
            { // Lock only taken for this section
                // Lock this area to avoid adding and printing to/from the vector
                // simultaneously
                std::lock_guard<std::mutex> lock(m);
                temp = std::move(combinations);
                combinations = std::queue<std::string>();
            }
            while (!temp.empty()) {
                fprintf(output_file, "%s\n", temp.front().c_str());
                temp.pop();
                todo_print--;
            }
        } else {
            while (!combinations.empty()) {
                fprintf(output_file, "%s\n", combinations.front().c_str());
                combinations.pop();
                todo_print--;
            }
        }
    }
}

void Generator::print_append() {
    todo_print = total_N;
    while (todo_print > 0) {
        if (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::queue<std::string> temp;
            {   // Lock only taken for this section
                // Lock this area to avoid adding and printing to/from the vector
                // simultaneously
                std::lock_guard<std::mutex> lock(m);
                temp = std::move(combinations);
            }
            while (!temp.empty()) {
                fprintf(output_file, "%s%s\n", temp.front().c_str(), append_str);
                temp.pop();
                todo_print--;
            }
        } else {
            while (!combinations.empty()) {
                fprintf(output_file, "%s%s\n", combinations.front().c_str(), append_str);
                combinations.pop();
                todo_print--;
            }
        }
    }
}

void Generator::print_prepend() {
    todo_print = total_N;
    while (todo_print > 0) {
        if (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::queue<std::string> temp;
            { // Lock only taken for this section
                // Lock this area to avoid adding and printing to/from the vector
                // simultaneously
                std::lock_guard<std::mutex> lock(m);
                temp = std::move(combinations);
            }
            while (!temp.empty()) {
                fprintf(output_file, "%s%s\n", prepend_str, temp.front().c_str());
                temp.pop();
                todo_print--;
            }
        } else {
            while (!combinations.empty()) {
                fprintf(output_file, "%s%s\n", prepend_str, combinations.front().c_str());
                combinations.pop();
                todo_print--;
            }
        }
    }
}

void Generator::print_append_prepend() {
    todo_print = total_N;
    while (todo_print > 0) {
        if (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::queue<std::string> temp;
            { // Lock only taken for this section
                // Lock this area to avoid adding and printing to/from the vector
                // simultaneously
                std::lock_guard<std::mutex> lock(m);
                temp = std::move(combinations);
            }
            while (!temp.empty()) {
                fprintf(output_file, "%s%s%s\n", prepend_str, temp.front().c_str(), append_str);
                temp.pop();
                todo_print--;
            }
        } else {
            while (!combinations.empty()) {
                fprintf(output_file, "%s%s%s\n", prepend_str, combinations.front().c_str(), append_str);
                combinations.pop();
                todo_print--;
            }
        }
    }
}

std::size_t Generator::get_approximate_memory() {
    // Reset memory
    approximate_memory = 0;
    if (permute) {
        approximate_memory = get_total_n() * permute_str.size();
    } else {
        // Memory for combinations
        for (auto i = length_min; i <= length_max; i++) {
            approximate_memory += sizeof(char) * (i + 1) * get_single_n(i);
        }
    }

    if (append) {
        approximate_memory += total_N * strlen(append_str) * sizeof(char);
    }
    if (prepend) {
        approximate_memory += total_N * strlen(prepend_str) * sizeof(char);
    }
    return approximate_memory;
}

std::size_t Generator::get_single_n(std::size_t length) {
    return std::pow(element_count, length);
}

std::size_t Generator::get_total_n() {
    if (permute) {

        auto N = util::factorial(permute_str.length());
        auto d = 1UL;
        auto permute_set = std::set<char>(std::begin(permute_str), std::end(permute_str));
        for (auto x : permute_set) {
            d *= util::factorial(std::count(std::begin(permute_str), std::end(permute_str), x));
        }

        total_N = N / d;
    } else {
        for (auto l = length_min; l <= length_max; ++l) {
            total_N += std::pow(element_count, l);
        }
    }

    return total_N;
}

void Generator::print_report(std::ostream &os) {
    const auto ud = util::get_unit(approximate_memory);
    os << "\n---\n"
       << total_N << " "
       << "Combinations generated in " << duration.count() << " milliseconds."
       << std::endl
       << "Total disk usage: " << approximate_memory / ud.second << " "
       << ud.first << std::endl;
}

void Generator::get_permutations() {
    do {
        {
            std::lock_guard<std::mutex> lock(m);
            combinations.emplace(permute_str.c_str());
        }
    } while (std::next_permutation(std::begin(permute_str), std::end(permute_str)));
}
