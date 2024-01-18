#include "config_flag.hpp"

template class Config<string>;

vector<tuple<ConfigError, string>> error_messages = {
    {CONFIG_NOT_FOUND, "Configuration not found."},
    {CONFIG_ALREADY_EXIST, "Configuration already exists."},
    {UNEXPECTED_ERROR, UNEXPECTED_ERROR_MESSAGE}
};

ConfigError error_handler_cb(tuple<ConfigError, string> handler) {
    cout << "Error: " << get<string>(handler) << endl;
    return get<ConfigError>(handler);
}

template<class T>
Config<T>::Config() {
    this->error_handler.registerErrorMessages(error_messages);
    this->error_handler.registerErrorCallback(ConfigError::CONFIG_ALREADY_EXIST, error_handler_cb);
    this->error_handler.registerErrorCallback(ConfigError::CONFIG_NOT_FOUND, error_handler_cb);
}

template<class T>
void Config<T>::set_parameter(string config, T value) {
    this->configs[config] = value;
}

template<class T>
Result<T> Config<T>::get_parameter(string config) {
    if(this->configs.find(config) != this->configs.end()) {
        return this->configs[config];
    } else {

        
        return (this->error_handler.handleError(
            ConfigError::CONFIG_NOT_FOUND, 
            make_tuple(
                ConfigError::CONFIG_NOT_FOUND, 
                this->error_handler.getErrorMessage(ConfigError::CONFIG_NOT_FOUND).value_or(UNEXPECTED_ERROR_MESSAGE)
            )
        )).value_or(ConfigError::UNEXPECTED_ERROR);
    }
}