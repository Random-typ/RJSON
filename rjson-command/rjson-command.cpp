#include <iostream>
#include <fstream>
#include "../RJSON/include/RJSON.h"
#ifdef _DEBUG
#pragma comment(lib, "../RJSON/lib/RJSON_d.lib")
#else
#pragma comment(lib, "../RJSON/lib/RJSON.lib")
#endif // _DEBUG


/*
* Commands:
* Note: The capitalization of the arguments does not matter.
*       The first argument MUST always be the path to the json file.
*       The root element is selected when loading the file
* 
* 
* -get <Element Name>        | retrieves an element by its name
* -i/-index <index>          | retrieves an element by its index
* -count                     | returns the number of children the element has
* -exists                    | returns 0 if the element does not exist 1 otherwise
* -name                      | returns the name of the element
* -value                     | returns the value of the element
* 
* Example
* rjson "test.json" -get plugins -i 0 -name
*/

#define checkFollowing \
if (i + 1 >= argc)\
{\
    std::cout << "Error: missing argument.\n";\
    return 1;\
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Error: Not enough arguments.\n";
        return 1;
    }
    // open and load file
    std::fstream fs(argv[1]);
    if (!fs.is_open())
    {
        std::cout << "Error: Failed to open file.\n";
        return 1;
    }
    
    std::string fileContent((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement root = RJSON::RJSON::load(fileContent);

    for (size_t i = 2; i < argc; i++)
    {
        if (!strcmp(argv[i], "-get"))
        {
            checkFollowing;
            root = (RJSON::JSONElement)root.get(argv[i + 1]);
            i++;
        }
        if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "-index"))
        {
            checkFollowing;
            root = (RJSON::JSONElement)root.get(std::stoi(argv[i + 1]));
            i++;
        }
        if (!strcmp(argv[i], "-count"))
        {
            std::cout << root.children.size();
            return 0;
        }
        if (!strcmp(argv[i],"-exists"))
        {
            std::cout << root.exists();
            return 0;
        }
        if (!strcmp(argv[i],"-name"))
        {
            std::cout << root.name;
            return 0;
        }
        if (!strcmp(argv[i],"-value"))
        {
            std::cout << root.value;
            return 0;
        }
    }
    return 0;
}
