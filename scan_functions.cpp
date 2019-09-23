#include <regex>
#include <iterator>
#include <iostream>

// Function to scan any given list of entities on a given string. Returns ALL hits.
std::string entity_scan_all_hits(const std::regex entity_regex, const std::string text)
{
	// Scan text for an entity
	auto words_begin = std::sregex_iterator(text.begin(), text.end(), entity_regex);
	auto words_end = std::sregex_iterator();

	// How many hits?
	// if (std::distance(words_begin, words_end) > 0)
	// {
	// 	std::cout << "Found " 
 //              << std::distance(words_begin, words_end) 
 //              << " hits." 
 //              << std::endl;
	// }
	
    // Return all hits
  	std::string results = "";
	for (std::sregex_iterator i = words_begin; i != words_end; ++i)
	{
		std::smatch match = *i;                                                 
        results += match.str() + ", ";
	}

	return (results);
}

// Function to scan any given list of entities on a given string. If there is a match, returns all hits.
std::string entity_scan(const std::regex entity_regex, const std::string text)
{
	// Scan text for an entity
	std::smatch hit;

	// If there is a hit
	if(std::regex_search(text, hit, entity_regex))
	{		
		return (entity_scan_all_hits(entity_regex, text));
	}

	return ("");
}