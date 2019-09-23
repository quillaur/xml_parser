// Author: Aurélien Quillet
// Purpose: My first C++ script. I decided to try to open and parse several XML files and compare performances with an equivalent python script.
// I extend the purpose of this script to scan any given words in the text parts of the XML files.
// Compilation: g++ parse_xml.cpp -o xml_parse -std=c++17 -Wall -L/usr/lib/mariadb -lmariadbclient
// Comments: GL & HF !

#include "parser_functions.cpp"
#include "scan_functions.cpp"
#include "read_config.cpp"
#include "sql_functions.cpp"
#include <filesystem>
#include <typeinfo>
#include <algorithm>

int main()
{
	// Get configuration file
	std::ifstream my_config_file("config.cfg");

	// Check file opening
	if (!my_config_file.is_open())
	{
		std::cout << "config.cfg did not open !" << std::endl;
		return (-1);
	}
		
	std::string pmc_path = get_config_param(&my_config_file, "pmc_path");
	std::string sources_path = get_config_param(&my_config_file, "sources_path");

	my_config_file.close();

	// Which PMC to analyse?
	const std::filesystem::path main_pmc_dir(pmc_path);
	// Initialize /pmc/sources directory in order to avoid it later in the loop.
	const std::filesystem::path sources_dir(sources_path);

	// Which words to scan text for?
	std::vector<std::regex> name_regexes_vector; 
	build_entity_names_regex(name_regexes_vector);
	
	// Write results to file
	std::ofstream my_csv;
	my_csv.open("cpp_test_xml_parsing.csv");
	
	// Check file opening
	if (!my_csv.is_open())
	{
		std::cout << "cpp_test_xml_parsing.csv did not open !" << std::endl;
		return (-1);
	}
		
	// Header
	my_csv << "PMCID;Title;Hits" << std::endl;

	// For each PMC sub-dirs
	for (std::filesystem::directory_iterator pointer_dir(main_pmc_dir); 
			pointer_dir != std::filesystem::directory_iterator(); 
			++pointer_dir)
	{	
		// Avoid the /pmc/sources directory
		if (strcmp((sources_dir.c_str()), (pointer_dir->path().c_str())) != 0)
		{
			// Parse each PMC			
			for (std::filesystem::directory_iterator pointer_file(pointer_dir->path()); 
					pointer_file != std::filesystem::directory_iterator(); 
					++pointer_file)
			{
				// Declare xml_document variable
				pugi::xml_document doc;

				// Test print to see which file is analyzed
				// std::cout << "File: " << pointer_file->path().string() << std::endl;
				
				// Open, load file content and parse it
				if (!doc.load_file(pointer_file->path().c_str()))
				{
					// Close file
					my_csv.close();
					return (-1);
				}

				// // Get title
				std::string title = search_xml(doc, "article-title");
				// Remove '\n' from title string
				title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());

				// Get PMCID
				std::string pmcid = search_xml(doc, "article-id", "pub-id-type", "pmc");

				// Get text to scan
				std::string text = search_xml(doc, "abstract") + " " + search_xml(doc, "body");

				// Look for each entity name in the text
				for (auto it = name_regexes_vector.begin(); it != name_regexes_vector.end(); it++)
				{
					std::string hits = entity_scan(*it, text);

					if(hits != "")
					{
						// Print to standard output
						// std::cout << "PMCID: " << pmcid << std::endl;
						// std::cout << "Title: " << title << std::endl;
						// std::cout << "Match: " << hits << std::endl;

						// Write to file
						my_csv << pmcid << ";" << title << ";" << hits << std::endl;
					}
				}
			}
		}
	}

	// Close file
	my_csv.close();

	std::cout << "Run done." << std::endl;
	// End program
	return (0);
}
