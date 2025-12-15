#include <unordered_map>
#include <iostream>
#include <cstring>
#include <vector>

class FixDecoder {
    private:
    std::vector<std::string> split(std::string target, char delimiter) {
        std::string accumulated_string = "";
        std::vector<std::string> result;

        for (int i = 0; i < target.size() + 1; ++i) {
            if (target[i] == delimiter || i == target.size()) {
                result.push_back(accumulated_string);
                accumulated_string = "";
                continue;
            }

            accumulated_string += target[i];
        }

        return result;
    }

    const char* message;

    public:
    // TODO: Does this mean we have to create new FixDecoder everytime
    //       we need to decode? Why do you need to store the message as a class 
    //       member? decode() is a stateless function - it doesn't need to remember 
    //       anything to parse your message right? 
    FixDecoder(const char* _message) {
        // TODO: What if message == nullptr? It's a crash immediately
        this->message = _message;
    }

    // TODO: This could be 
    //       std::unordered_map< uint8_t, std::string > decode( const char* message ) { ... }
    std::unordered_map<u_int8_t, std::string> decode() {
        std::unordered_map<u_int8_t, std::string> result;
        std::string accumulated_string = "";

        // TODO: why can't you 
        //       auto length = strlen( message );
        //       for( std::size_t i = 0; i < length; ++i ) { ... }
        //       This will do strlen for each loop (or it relies on compiler optimization 
        //       level to fix it)
        // TODO: What if message doesn't have a \0 at the end, 
        //       strlen will crash        
        for (size_t i = 0; i < strlen(message); ++i) {
            char current_character = message[i];

            // TODO: What if the message is like this: 
            //       "8=FIX4.2" 
            //       Valid FIX message, but no delimitter
            if (current_character == '\x01') {
                // TODO: Imagine a FIX message like this: 
                //       "8=FIX4.2\00135=8\001<othertags>\00158="Reason: tag value = 10, not processing"\001<othertags>\00110=32"
                std::vector<std::string> split_string = split(accumulated_string, '=');
                std::string key_string = split_string[0];
                // TODO: Who says all tags are string values? Eg: tag 38 / 44 / ... 
                std::string value_string = split_string[1];

                result.insert(std::pair<u_int8_t, std::string>((u_int8_t) std::stoi(key_string), value_string));

                accumulated_string = "";
                continue;
            }

            // Why do you do this? can't you split( message, '\001' ); ??
            accumulated_string += current_character;
        }

        return result;
    }
};
