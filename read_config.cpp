#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>

// Function to split a string on a given delimiter.
std::vector<std::string> split(const std::string &s, const char * delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   
   // Stream through the given string
   std::istringstream tokenStream(s);

   // getline reads characters from an input stream and places them into a string
   while (std::getline(tokenStream, token, *delimiter))
   {
      tokens.push_back(token);
   }

   return (tokens);
}

// Returns the value of a given config parameter. 
// Gets the result from the config.cfg file. 
std::string get_config_param(std::ifstream * my_config_file, const char * param)
{
	// Delimiter in my config is ":"
	const char delimiter[2] = ":";

	char output[100];
	const char * output_ptr = output;

	while (!my_config_file->eof()) 
	{
		*my_config_file >> output;

		// If param is in output
		if (strstr(output_ptr, param))
		{		
			// Returns the part that is on the right of the delimiter in the config file
			return(split(output_ptr, delimiter)[1]);
		}
	}

	return ("");
}