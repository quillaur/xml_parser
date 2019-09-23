// Function to get the text from a wanted node
std::string search_xml(pugi::xml_node main_node, 
						const char * search_node_name,
						const char * search_node_attr_name = nullptr,
						const char * search_node_attr_value = nullptr)