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

    GenericConfig generic_configs;

    string generic_config_name_one("GENERIC_CONFIG_1");
    string generic_config_name_two("GENERIC_CONFIG_2");
    string generic_config_name_three("GENERIC_CONFIG_3");
    string generic_config_name_four("GENERIC_CONFIG_4");
    string generic_changed_config_name_one("GENERIC_CHANGED_CONFIG_1");

    string generic_config_value_one("GENERIC_CONFIG_1");
    int generic_config_value_two = 22;
    vector<string> generic_config_value_three = {{"GENERIC_CONFIG_3_A"},{"GENERIC_CONFIG_3_B"},{"GENERIC_CONFIG_3_C"}};
    float generic_config_value_four = 1.23;

    auto res_one = generic_configs.set_parameter<string>(generic_config_name_one, generic_config_value_one);
    auto res_two = generic_configs.set_parameter<int>(generic_config_name_two, generic_config_value_two);
    auto res_three = generic_configs.set_parameter<vector<string>>(generic_config_name_three, generic_config_value_three);
    auto res_four = generic_configs.set_parameter<float>(generic_config_name_four, generic_config_value_four);

    Result<string> fetched_config_one = generic_configs.get_parameter<string>(generic_config_name_one);

    if(holds_alternative<string>(fetched_config_one)) {
        cout << get<string>(fetched_config_one) << endl;
    } else {
        cout << get<ConfigError>(fetched_config_one) << endl;
    }

    Result<int> fetched_config_two = generic_configs.get_parameter<int>(generic_config_name_two);

    if(holds_alternative<int>(fetched_config_two)) {
        cout << get<int>(fetched_config_two) << endl;
    } else {
        cout << get<ConfigError>(fetched_config_two) << endl;
    }

    Result<vector<string>> fetched_config_three = generic_configs.get_parameter<vector<string>>(generic_config_name_three);

    if(holds_alternative<vector<string>>(fetched_config_three)) {
        for (const std::string& element : get<vector<string>>(fetched_config_three)) {
            cout << element << " ";
        }
        cout << endl;
    } else {
        cout << get<ConfigError>(fetched_config_three) << endl;
    }

    Result<float> fetched_not_supported_config = generic_configs.get_parameter<float>(generic_config_name_four);

    if(holds_alternative<float>(fetched_not_supported_config)) {
        cout << get<float>(fetched_not_supported_config) << endl;
    } else {
        cout << get<ConfigError>(fetched_not_supported_config) << endl;
    }

    auto res_changed_one = generic_configs.set_parameter<string>(generic_config_name_one, generic_changed_config_name_one);

    Result<string> fetched_changed_config_one = generic_configs.get_parameter<string>(generic_config_name_one);

    if(holds_alternative<string>(fetched_changed_config_one)) {
        cout << get<string>(fetched_changed_config_one) << endl;
    } else {
        cout << get<ConfigError>(fetched_changed_config_one) << endl;
    }

    auto res_already_exist = generic_configs.set_parameter<int>(generic_config_name_one, 2);

    if(holds_alternative<int>(res_already_exist)) {
        cout << get<int>(res_already_exist) << endl;
    } else {
        cout << get<ConfigError>(res_already_exist) << endl;
    }

    return 0;
}