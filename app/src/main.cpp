#include "config_flag.hpp"

int main() {

    string config_name("CONFIG1");
    string config_value("CONFIG_VALUE_1");
    Config<string> config;
    config.set_parameter(config_name, config_value);
    Result<string> fetched_config = config.get_parameter(config_name);
    if(holds_alternative<string>(fetched_config)) {
        cout << get<string>(fetched_config) << endl;
    } else {
        cout << "Error" << endl;
    }
    return 0;
}