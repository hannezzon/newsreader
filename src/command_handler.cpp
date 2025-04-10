#include "../include/command_handler.hpp"
#include "../include/rss_feed.hpp"
#include "../include/cli_parser.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

CommandHandler::CommandHandler() {
    initializeHandlers();
}

void CommandHandler::initializeHandlers() {
    // Register command handlers
    commandHandlers["example"] = [this](const std::vector<std::string>& /* args */) -> bool {
        std::cout << "Executing example command" << std::endl;
        // Implement example command logic here
        return true;
    };

    // News command handler
    commandHandlers["news"] = [this](const std::vector<std::string>& args) -> bool {
        // Parse options
        CLIParser parser;
        auto options = parser.parseOptions(args);

        // Get the number of news items to display
        int count = 5; // Default to 5 items
        if (parser.hasOption(options, "count")) {
            try {
                count = std::stoi(parser.getOption(options, "count"));
            } catch (const std::exception& e) {
                std::cerr << "Invalid count value: " << parser.getOption(options, "count") << std::endl;
                return false;
            }
        }

        // Create RSS feed object
        RSSFeed feed;

        // Set progress callback
        feed.setProgressCallback([](int current, int total) {
            std::cout << "Fetching news items: " << current << "/" << total << "\r" << std::flush;
        });

        std::cout << "Fetching latest news from New York Times..." << std::endl;

        // Fetch news
        auto newsItems = feed.fetchNews(count);

        // Clear progress line
        std::cout << std::string(40, ' ') << "\r" << std::flush;

        // Check for errors
        if (newsItems.empty()) {
            std::cerr << "Error fetching news: " << feed.getLastError() << std::endl;
            return false;
        }

        // Display news items
        std::cout << "\nLatest News from New York Times:" << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        for (const auto& item : newsItems) {
            std::cout << "Title: " << item.title << std::endl;

            // Format and wrap the description
            std::istringstream descStream(item.description);
            std::string word;
            std::string line;
            const int maxLineLength = 80;

            std::cout << "Description: ";
            int linePos = 12; // Length of "Description: "

            while (descStream >> word) {
                if (linePos + word.length() + 1 > maxLineLength) {
                    std::cout << line << std::endl;
                    line = std::string(12, ' '); // Indent continuation lines
                    linePos = 12;
                }

                line += word + " ";
                linePos += word.length() + 1;
            }

            if (!line.empty()) {
                std::cout << line << std::endl;
            }

            std::cout << "Link: " << item.link << std::endl;
            std::cout << "Date: " << item.pubDate << std::endl;
            std::cout << std::string(80, '-') << std::endl;
        }

        return true;
    };

    // Register help messages for commands
    commandHelp["example"] = "Usage: newsreader example [options]\n"
                            "Options:\n"
                            "  --option1 VALUE    Description of option1\n"
                            "  --option2 VALUE    Description of option2\n";

    // Help message for news command
    commandHelp["news"] = "Usage: newsreader news [options]\n"
                         "Fetches and displays the latest news from New York Times.\n"
                         "\n"
                         "Options:\n"
                         "  --count VALUE    Number of news items to display (default: 5)\n";
}

bool CommandHandler::handleCommand(const std::string& command, const std::vector<std::string>& args) {
    auto it = commandHandlers.find(command);
    if (it != commandHandlers.end()) {
        // Call the command handler function
        return it->second(args);
    }
    return false;
}

void CommandHandler::handleHelp(const std::string& command) {
    auto it = commandHelp.find(command);
    if (it != commandHelp.end()) {
        std::cout << it->second << std::endl;
    } else {
        std::cout << "No help available for command: " << command << std::endl;
    }
}
