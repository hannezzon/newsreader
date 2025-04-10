#include "../include/cli_parser.hpp"

std::map<std::string, std::string> CLIParser::parseOptions(const std::vector<std::string>& args) {
    std::map<std::string, std::string> options;
    
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        
        // Check if the argument is an option (starts with - or --)
        if (arg.size() >= 2 && arg[0] == '-') {
            std::string optionName;
            
            // Handle long options (--option)
            if (arg.size() >= 3 && arg[1] == '-') {
                optionName = arg.substr(2);
            } else {
                // Handle short options (-o)
                optionName = arg.substr(1);
            }
            
            // Check if the next argument is a value (not an option)
            if (i + 1 < args.size() && args[i + 1][0] != '-') {
                options[optionName] = args[i + 1];
                ++i; // Skip the next argument as it's the value
            } else {
                // Option without a value, set it to "true"
                options[optionName] = "true";
            }
        }
    }
    
    return options;
}

bool CLIParser::hasOption(const std::map<std::string, std::string>& options, const std::string& optionName) {
    return options.find(optionName) != options.end();
}

std::string CLIParser::getOption(const std::map<std::string, std::string>& options, 
                                 const std::string& optionName, 
                                 const std::string& defaultValue) {
    auto it = options.find(optionName);
    if (it != options.end()) {
        return it->second;
    }
    return defaultValue;
}
