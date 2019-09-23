// Function to split a string on a given delimiter.
std::vector<std::string> split(const std::string &s, const char * delimiter)

// Returns the value of a given config parameter. 
// Gets the result from the config.cfg file. 
std::string get_config_param(std::ifstream * my_config_file, const char * param)