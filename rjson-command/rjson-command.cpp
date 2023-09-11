#include <iostream>
#include <fstream>
#include "../RJSON/include/RJSON.h"
#pragma comment(lib, "../RJSON/lib/RJSON.lib")

/*
* Commands:
* Note: The capitalization of the arguments does not matter.
*       The first argument MUST always be the path to the json file.
*       The root element is selected when loading the file
* 
* 
* -get <Element Name>        | retrieves a element by its name
* -i/-index <index>          | retrieves a element by its index
* -count                     | retrieves the number of children the element has
* -exists                    | returns 0 if the element does not exist 1 otherwise
* 
* Example
* rjson test.json -get 
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
        if (argv[i] == "-get")
        {
            checkFollowing;
            root = root.get(argv[i + 1]);
            i++;
        }
        if (argv[i] == "-i" || argv[i] == "-index")
        {
            checkFollowing;
            root = root.get(std::stoi(argv[i + 1]));
            i++;
        }




    }





    
    return 0;
}
