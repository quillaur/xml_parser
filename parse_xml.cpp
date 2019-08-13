// Author: Aurélien Quillet
// Purpose: My first C++ script. I decided to try to open and parse several XML files and compare performances with an equivalent python script.
// Compilation: g++ parse_xml.cpp -o xml_parse -std=c++17 -Wall
// Comments: GL & HF !

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <string.h>
#include <algorithm>
#include <string>
#include <filesystem>
#include <iterator>

// Which XML parser to use?
// Library must have a good doc and known for food perf.
// Choices : LibXML2, Xerces, RapidXml, PugiXML, TinyXML.
// RapidXml is stated to be more performant (5-30%) than PugiXML, but that was back in 2009.
// In the end, I took PugiXML because it has been recently updated (2018), the doc seems good, and several users are happy with its usage.
// Might be usefull to compare to RapidXml one day.
#include </usr/local/lib/pugixml-1.9/src/pugixml.hpp>

// Structure to identify whether a node is the one I am looking for or not
struct search_node
{
	// One can search a node by its name or attribute name and attribute value
	const char * search_node_name = nullptr; 
	const char * search_node_attr_name = nullptr;
	const char * search_node_attr_value = nullptr;

    bool operator()(pugi::xml_node node) const
    {	
    	// If searched attribute name and value have been set
    	// This is the case for the node containing the PMCID
    	if ((search_node_attr_name != nullptr) && 
    		(search_node_attr_value != nullptr))
    	{
    		// If the node contains this attribute
    		if (node.attribute(search_node_attr_name))
    		{
    			// If the value of this attribute is the one searched, return True
    			if (strcmp(node.attribute(search_node_attr_name).value(), search_node_attr_value) == 0)
		    		return (true);
    		}
			else
				return (false);   		
    	}
    	// Otherwise, search node by its name
    	// This is the case for the title node
    	else if (strcmp(node.name(), search_node_name) == 0)
    		return (true);      	
    
    	return (false);
    }
};

// Function to get the text from a wanted node
std::string search_xml(pugi::xml_node main_node, 
						const char * search_node_name,
						const char * search_node_attr_name = nullptr,
						const char * search_node_attr_value = nullptr)
{
	// Initialize node structure which will help us to figure if the node is the right one or not.
	search_node node_to_find;

	// Define the parameter of the node we are interested in.
	node_to_find.search_node_name = search_node_name;
	node_to_find.search_node_attr_name = search_node_attr_name;
	node_to_find.search_node_attr_value = search_node_attr_value;
	
	// Look for the node and return the text when the right node is found.
	pugi::xml_node found_node = main_node.find_node(node_to_find);
	std::string found_text = "";

	// Text is also store as sub-nodes
	// Sometimes, title contains tags (sub-nodes such as italic, strong and so on).
	// To make sure we have the full title, let's loop through all children and catch the text of each of them.
	if (found_node.first_child())
	{
		for(pugi::xml_node sub_node = found_node.first_child(); sub_node; sub_node = sub_node.next_sibling())
		{
			found_text += sub_node.text().get();
		}
	}

	return (found_text);
};

int main()
{
	// Which PMC to analyse?
	const std::filesystem::path main_pmc_dir = std::filesystem::u8path("/data/quillaur/Documents/pmc");

	// Avoid the /pmc/sources directory
	const std::filesystem::path sources_dir = std::filesystem::u8path("/data/quillaur/Documents/pmc/sources");
	const char * pointer_sources = sources_dir.c_str();
	
	// Write results to file
	std::ofstream my_csv;
	my_csv.open("cpp_test_xml_parsing.csv");
	// Check file opening
	if (!my_csv.is_open())
		return (-1);

	// Header
	my_csv << "PMCID;Title" << std::endl;

	// For each PMC sub-dirs
	std::filesystem::directory_iterator sub_dirs_list(main_pmc_dir);
	for (auto pointer_dir = sub_dirs_list; pointer_dir != std::filesystem::directory_iterator(); ++pointer_dir)
	{
		// Check sub-dir path versus sources sub-directory
		
		// Do not take the "sources" sub-directory into consideration
		// To avoid it, I compare the dir path to sources path with strcomp.
		// strcomp takes a const char *
		const char * pointer_path = pointer_dir->path().c_str();
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

				// Get PMCID
				std::string pmcid = search_xml(doc, "article-id", "pub-id-type", "pmc");
				
				// Print to standard output
				// std::cout << "PMCID: " << pmcid << std::endl;
				// std::cout << "Title: " << title << std::endl;

				// Remove '\n' from title string
				title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());

				// Write to file
				my_csv << pmcid << ";" << title << std::endl;
			}
		}
	}

	// Close file
	my_csv.close();

	// End program
	return (0);
}
