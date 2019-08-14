#include <string>
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