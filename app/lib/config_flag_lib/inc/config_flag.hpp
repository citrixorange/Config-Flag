#ifndef CONFIG_FLAG_HPP
#define CONFIG_FLAG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <optional>
#include <variant>
#include "error_handler.hpp"

using namespace std;

#define UNEXPECTED_ERROR_MESSAGE "Unexpected Error."

enum ConfigError {
    CONFIG_NOT_FOUND,
    CONFIG_ALREADY_EXIST,
    UNEXPECTED_ERROR
};

template class Callback<tuple<ConfigError, string>, ConfigError>;
template class CallbackManager<tuple<ConfigError, string>, ConfigError>;
template class ConfigErrorManager<tuple<ConfigError, string>, ConfigError>;

template<class T>
using Result = variant<T, ConfigError>;

template<class T>
class Config {
    
    private:
        unordered_map<string, T> configs;
        ConfigErrorManager<tuple<ConfigError, string>, ConfigError> error_handler;

    public:
        Config();
        void set_parameter(string config, T value);
        Result<T> get_parameter(string config);

};


#endif