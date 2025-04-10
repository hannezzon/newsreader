#include <iostream>
#include <string>
#include <vector>
#include "../include/cli_parser.hpp"
#include "../include/command_handler.hpp"

const std::string VERSION = "1.0.1";
const std::string APP_NAME = "NewsReader CLI";

void printUsage() {
    std::cout << "Usage: newsreader [command] [options]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  help                Display this help message" << std::endl;
    std::cout << "  version             Display version information" << std::endl;
    std::cout << "  news                Display latest news from New York Times" << std::endl;
    std::cout << "  example             Example command" << std::endl;
    std::cout << std::endl;
    std::cout << "For more information, use 'newsreader help [command]'" << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args;

    // Convert arguments to vector of strings
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    // Create command handler
    CommandHandler handler;

    // If no arguments provided, show usage
    if (args.empty()) {
        printUsage();
        return 0;
    }

    // Parse the command
    std::string command = args[0];

    // Handle basic commands
    if (command == "help") {
        if (args.size() > 1) {
            handler.handleHelp(args[1]);
        } else {
            printUsage();
        }
    } else if (command == "version") {
        std::cout << APP_NAME << " version " << VERSION << std::endl;
    } else {
        // Process other commands
        if (!handler.handleCommand(command, args)) {
            std::cerr << "Unknown command: " << command << std::endl;
            std::cerr << "Run 'newsreader help' for usage information." << std::endl;
            return 1;
        }
    }

    return 0;
}
