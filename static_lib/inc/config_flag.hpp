#ifndef CONFIG_FLAG_HPP
#define CONFIG_FLAG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <optional>
#include <variant>
#include "config_error.hpp"

using namespace std;

template<class T>
using Result = variant<T, ConfigError>;

template<class T>
class Config {
    
    private:
        unordered_map<string, T> configs;

    public:
        Config() {};
        void set_parameter(string config, T value);
        Result<T> get_parameter(string config);

};


#endif