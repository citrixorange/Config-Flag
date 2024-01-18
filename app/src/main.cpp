#include "config_flag.hpp"

int main() {

    string config_name_one("CONFIG1");
    string config_name_two("CONFIG2");
    string config_value("CONFIG_VALUE_1");
    
    Config<string> config;
    config.set_parameter(config_name_one, config_value);
    
    Result<string> fetched_config = config.get_parameter(config_name_one);

    if(holds_alternative<string>(fetched_config)) {
        cout << get<string>(fetched_config) << endl;
    } else {
        cout << get<ConfigError>(fetched_config) << endl;
    }

    fetched_config = config.get_parameter(config_name_two);

    if(holds_alternative<string>(fetched_config)) {
        cout << get<string>(fetched_config) << endl;
    } else {
        cout << get<ConfigError>(fetched_config) << endl;
    }

    return 0;
}