#include "config_flag.hpp"

template class Config<string>;

template<class T>
void Config<T>::set_parameter(string config, T value) {
    this->configs[config] = value;
}

template<class T>
Result<T> Config<T>::get_parameter(string config) {
    if(this->configs.find(config) != this->configs.end()) {
        return this->configs[config];
    } else {
        return ConfigError::CONFIG_NOT_FOUND;
    }
}