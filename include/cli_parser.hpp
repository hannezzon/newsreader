#ifndef CLI_PARSER_HPP
#define CLI_PARSER_HPP

#include <string>
#include <vector>
#include <map>
#include <optional>

/**
 * @class CLIParser
 * @brief Parses command line arguments and options
 */
class CLIParser {
public:
    CLIParser() = default;
    ~CLIParser() = default;
    
    /**
     * @brief Parse command line arguments
     * @param args Vector of command line arguments
     * @return Map of parsed options and their values
     */
    std::map<std::string, std::string> parseOptions(const std::vector<std::string>& args);
    
    /**
     * @brief Check if an option exists in the parsed options
     * @param options The parsed options map
     * @param optionName The name of the option to check
     * @return True if the option exists, false otherwise
     */
    bool hasOption(const std::map<std::string, std::string>& options, const std::string& optionName);
    
    /**
     * @brief Get the value of an option
     * @param options The parsed options map
     * @param optionName The name of the option to get
     * @param defaultValue The default value to return if the option doesn't exist
     * @return The value of the option or the default value
     */
    std::string getOption(const std::map<std::string, std::string>& options, 
                          const std::string& optionName, 
                          const std::string& defaultValue = "");
};

#endif // CLI_PARSER_HPP
