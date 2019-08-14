// Author: Aurélien Quillet
// Purpose: My first C++ script. I decided to try to open and parse several XML files and compare performances with an equivalent python script.
// I extend the purpose of this script to scan any given words in the text parts of the XML files.
// Compilation: g++ parse_xml.cpp -o xml_parse -std=c++17 -Wall
// Comments: GL & HF !

#include "parser_functions.cpp"
#include "scan_functions.cpp"
#include <filesystem>
#include <fstream>
#include <typeinfo>
#include <algorithm>

int main()
{
	// Which PMC to analyse?
	const std::filesystem::path main_pmc_dir = std::filesystem::u8path("/data/quillaur/Documents/pmc");

	// Which words to scan text for?
	std::string entity_names = "Neuropeptides|vertebrates";

	// Initialize /pmc/sources directory in order to avoid it later in the loop.
	const std::filesystem::path sources_dir = std::filesystem::u8path("/data/quillaur/Documents/pmc/sources");
	const char * pointer_sources = sources_dir.c_str();
	
	// Write results to file
	std::ofstream my_csv;
	my_csv.open("cpp_test_xml_parsing.csv");
	
	// Check file opening
	if (!my_csv.is_open())
		return (-1);

	// Header
	my_csv << "PMCID;Title;Hit" << std::endl;

	// For each PMC sub-dirs
	std::filesystem::directory_iterator sub_dirs_list(main_pmc_dir);
	
	for (auto pointer_dir = sub_dirs_list; pointer_dir != std::filesystem::directory_iterator(); ++pointer_dir)
	{
		// Check sub-dir path versus sources sub-directory
		// Do not take the "sources" sub-directory into consideration
		// To avoid it, I compare the dir path to sources path with strcomp.
		// strcomp takes a const char *
		const char * pointer_path = pointer_dir->path().c_str();
		
		// Avoid the /pmc/sources directory
		if (strcmp(pointer_sources, pointer_path) != 0)
		{
			// Parse each PMC
			std::filesystem::directory_iterator files_list(pointer_dir->path());
			
			for (auto pointer_file = files_list; pointer_file != std::filesystem::directory_iterator(); ++pointer_file)
			{
				// Declare xml_document variable
				pugi::xml_document doc;

				// Test print to see which file is analyzed
				// std::cout << "File: " << pointer_file->path().string() << std::endl;

				// load_file needs a const char *
				const char * char_pointer_file = pointer_file->path().c_str();
				
				// Open, load file content and parse it
				if (!doc.load_file(char_pointer_file))
				{
					// Close file
					my_csv.close();
					return (-1);
				}

				// Get title
				std::string title = search_xml(doc, "article-title");
				// Remove '\n' from title string
				title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());

				// Get PMCID
				std::string pmcid = search_xml(doc, "article-id", "pub-id-type", "pmc");

				// Get text to scan
				std::string text = search_xml(doc, "abstract") + " " + search_xml(doc, "body");

				std::string hits = entity_scan(entity_names, text);

				if(hits != "")
				{
					// Print to standard output
					std::cout << "PMCID: " << pmcid << std::endl;
					std::cout << "Title: " << title << std::endl;
					std::cout << "Match: " << hits << std::endl;

					// Write to file
					my_csv << pmcid << ";" << title << ";" << hits << std::endl;
				}

			}
		}
	}

	// Close file
	my_csv.close();

	// End program
	return (0);
}
