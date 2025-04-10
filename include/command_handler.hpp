#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <vector>
#include <functional>
#include <map>

/**
 * @class CommandHandler
 * @brief Handles execution of CLI commands
 */
class CommandHandler {
public:
    CommandHandler();
    ~CommandHandler() = default;
    
    /**
     * @brief Handle a command
     * @param command The command to handle
     * @param args The arguments for the command
     * @return True if the command was handled successfully, false otherwise
     */
    bool handleCommand(const std::string& command, const std::vector<std::string>& args);
    
    /**
     * @brief Display help for a specific command
     * @param command The command to display help for
     */
    void handleHelp(const std::string& command);

private:
    // Map of command names to their handler functions
    std::map<std::string, std::function<bool(const std::vector<std::string>&)>> commandHandlers;
    
    // Map of command names to their help messages
    std::map<std::string, std::string> commandHelp;
    
    // Initialize command handlers
    void initializeHandlers();
};

#endif // COMMAND_HANDLER_HPP
