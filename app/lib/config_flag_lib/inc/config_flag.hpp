#ifndef CONFIG_FLAG_HPP
#define CONFIG_FLAG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <optional>
#include <variant>
#include "error_handler.hpp"

namespace std {

    #define UNEXPECTED_ERROR_MESSAGE "Unexpected Error."

    enum ConfigError {
        CONFIG_NOT_FOUND,
        CONFIG_ALREADY_EXIST,
        TYPE_NOT_SUPPORTED,
        UNEXPECTED_ERROR
    };

    vector<tuple<ConfigError, string>> error_messages = {
        {CONFIG_NOT_FOUND, "Configuration not found."},
        {CONFIG_ALREADY_EXIST, "Configuration already exists."},
        {TYPE_NOT_SUPPORTED, "Configuration Type Parameter is not supported."},
        {UNEXPECTED_ERROR, UNEXPECTED_ERROR_MESSAGE}
    };

    ConfigError error_handler_cb(tuple<ConfigError, string> handler) {
        cout << "Error: " << get<string>(handler) << endl;
        return get<ConfigError>(handler);
    }

    ConfigError handleError(ConfigErrorManager<tuple<ConfigError, string>, ConfigError> handler, ConfigError error) {
        return (handler.handleError(
                error, 
                make_tuple(
                    error, 
                    handler.getErrorMessage(error).value_or(UNEXPECTED_ERROR_MESSAGE)
                )
            )
        ).value_or(ConfigError::UNEXPECTED_ERROR);
    }

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

            Config() {
                this->error_handler.registerErrorMessages(error_messages);
                this->error_handler.registerErrorCallback(ConfigError::CONFIG_ALREADY_EXIST, error_handler_cb);
                this->error_handler.registerErrorCallback(ConfigError::CONFIG_NOT_FOUND, error_handler_cb);
                this->error_handler.registerErrorCallback(ConfigError::TYPE_NOT_SUPPORTED, error_handler_cb);
                this->error_handler.registerErrorCallback(ConfigError::UNEXPECTED_ERROR, error_handler_cb);
            }

            void set_parameter(string config, T value) {
                this->configs[config] = value;
            }

            Result<T> get_parameter(string config) {
                if(this->configs.find(config) != this->configs.end()) {
                    return this->configs[config];
                } else {
                    return handleError(this->error_handler, ConfigError::CONFIG_NOT_FOUND);
                }
            }

    };

    class GenericConfig {

        private:
            set<string> configs;
            Config<string> string_configs;
            Config<int> int_configs;
            Config<vector<string>> string_vector_configs;
            ConfigErrorManager<tuple<ConfigError, string>, ConfigError> error_handler;
        public:

            GenericConfig() {
                this->error_handler.registerErrorMessages(error_messages);
                this->error_handler.registerErrorCallback(ConfigError::CONFIG_ALREADY_EXIST, error_handler_cb);
                this->error_handler.registerErrorCallback(ConfigError::CONFIG_NOT_FOUND, error_handler_cb);
                this->error_handler.registerErrorCallback(ConfigError::TYPE_NOT_SUPPORTED, error_handler_cb);
                this->error_handler.registerErrorCallback(ConfigError::UNEXPECTED_ERROR, error_handler_cb);
            }

            template<class T>
            Result<T> set_parameter(string config, T value) {
                    
                auto current_config = this->configs.find(config);

                if(current_config == this->configs.end()) {
                    this->configs.insert(config);
                    if constexpr(is_integral<T>::value) {
                        this->int_configs.set_parameter(config, value);
                        return value;
                    } else if constexpr(is_same<T, string>::value) {
                        this->string_configs.set_parameter(config, value);
                        return value;
                    } else if constexpr(is_same<T, vector<string>>::value) {
                        this->string_vector_configs.set_parameter(config, value);
                        return value;
                    } else {
                        return handleError(this->error_handler, ConfigError::TYPE_NOT_SUPPORTED);
                    }
                } else {
                    if constexpr(is_integral<T>::value) {
                        if (holds_alternative<int>(this->int_configs.get_parameter(config))) {
                            this->int_configs.set_parameter(config, value);
                            return value;
                        } else {
                            return handleError(this->error_handler, ConfigError::CONFIG_ALREADY_EXIST);
                        }
                    } else if constexpr(is_same<T, string>::value) {
                        if (holds_alternative<string>(this->string_configs.get_parameter(config))) {
                            this->string_configs.set_parameter(config, value);
                            return value;
                        } else {
                            return handleError(this->error_handler, ConfigError::CONFIG_ALREADY_EXIST);
                        }
                    } else if constexpr(is_same<T, vector<string>>::value) {
                        if (holds_alternative<vector<string>>(this->string_vector_configs.get_parameter(config))) {
                            this->string_vector_configs.set_parameter(config, value);
                            return value;
                        } else {
                            return handleError(this->error_handler, ConfigError::CONFIG_ALREADY_EXIST);
                        }
                    } else {
                        return handleError(this->error_handler, ConfigError::TYPE_NOT_SUPPORTED);
                    }
                }
            }

            template<class T>
            Result<T> get_parameter(string config) {
                if constexpr(is_integral<T>::value) {
                    auto res = this->int_configs.get_parameter(config);
                    if (holds_alternative<int>(res)) {
                        return res;
                    } else {
                        return handleError(this->error_handler, ConfigError::CONFIG_NOT_FOUND);
                    }
                } else if constexpr(is_same<T, string>::value) {
                    auto res = this->string_configs.get_parameter(config);
                    if (holds_alternative<string>(res)) {
                        return res;
                    } else {
                        return handleError(this->error_handler, ConfigError::CONFIG_NOT_FOUND);
                    }
                } else if constexpr(is_same<T, vector<string>>::value) {
                    auto res = this->string_vector_configs.get_parameter(config);
                    if (holds_alternative<vector<string>>(res)) {
                        return res;
                    } else {
                        return handleError(this->error_handler, ConfigError::CONFIG_NOT_FOUND);
                    }
                } else {
                    return handleError(this->error_handler, ConfigError::TYPE_NOT_SUPPORTED);
                }
            }

    };

}


#endif