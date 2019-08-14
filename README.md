# xml_parser
Little script to parse XML PMC files and extract the PMCID, the title, and the text parts. The goal is then to scan the text for given words and write the search results in a CSV file.

# Compilation
```shell
g++ parse_xml.cpp -o xml_parse -std=c++17 -Wall
```

# Run program
```shell
./xml_parse
```

# Comments
Feel free to drop your suggestions on how to improve this script! ^^
