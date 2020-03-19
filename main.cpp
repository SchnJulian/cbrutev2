// C++ program to print all
// possible strings of length k
#include "Generator.h"
#include "util.h"

// Driver Code
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Not enough arguments. See -h for help." << std::endl;
        std::exit(1);
    }
    // Print logo and legal disclaimer
    util::logo();
    if (!util::disclaimer()) {
        std::exit(1);
    }
    util::clear_console();

    // Check if user wants help menu
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-h") {
            util::help_menu();
            return 0;
        }
    }

    Generator g(argc, argv);
    if (util::memory_warning(g.get_approximate_memory())) {
        g.start();
    }
    return 0;
}