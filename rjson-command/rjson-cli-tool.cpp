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
* -get <Element Name>               | retrieves an element by its name and selects it
* -i/-index <index>                 | retrieves an element by its index and selects it
* -up                               | retrieves and selects the parent of the currently selected object.
*                                     if the currently selected element is the root nothing will change and 
*                                     no error will be returned.
* -root                             | retrieves and selects the root element.
* -check                            | checks the json for parsing errors
* -count                            | returns the number of children the element has
* -exists                           | returns 0 if the element does not exist 1 otherwise.
*                                     Can be used like an if. See below.
* -iterate                          | iterates over the children of the currently selected element.
*                                     Can be used like a forEach. See below.
* -name                             | returns the name of the element
* -value                            | returns the value of the element
* -json <format (0 or 1)>           | returns the current object converted back to json. If the 
*                                     parameter is 1 the returned string will be formatted nicely.
*                                     if the parameter is 0 the json will be formatted with almost no whitespace.
* -delim/-delimiter  <delimiter>    | sets a delimiter which is printed between the returned values.
* 
* 
* Modify JSON:
* -add <name>                       | add element
* -adds <name>                      | add element and selected it
* -addv <value>                     | add element with value
* -addvs <value>                    | add element with value and select it
* -adde                             | add empty element
* -addes                            | add empty element and select it
* -adda                             | add array element
*       
* Note that in this context everything inside braces is called a "block". 
* Blocks don't have to be closed if you want to include everything after the open brace in the block.
* 
* 
* 
* 
* Can be used like an if:
* What does that mean?
* For example, -exists can be used like this:
* -exists ( -i 0 -value )
* The arguments inside the braces will only be executed if the element exists.
* 
* Note that the braces "(" and ")" are infixed postfixed by a space.
* 
* Can be used like a forEach:
* What does that mean?
* For example, -iterate can be used like this:
* -iterate ( -i 0 -value )
* or like this:
* -iterate -value
* 
* Note that the braces are not mandatory unless you want to use multiple arguments in the loop.
* Note that the braces "(" and ")" are infixed postfixed by a space.
* Arguments executed inside a iteration block do no change the selected element outside of the block.
* 
* Note:
* If an error is returned it will always start with "Error: ".
* also an error will not be returned if for example -get is used an an element which does not exist.
* 
* 
* 
* 
* How does everything come together?
* Example:
* rjson "test.json" -get plugins -i 0 -name
* 
* rjson "test.json" -delim "," -get plugins -iterate ( -get name -value -up -get author -value ) -i 1 -exists ( -get description -value ) -up -get tags -iterate -value
*/

#define checkFollowing \
if (_offset + 1 >= argc)\
{\
    std::cout << "Error: missing argument.";\
    return;\
}

const char* delimiter = "";

void iterateArgs(int argc, char* argv[], size_t _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    auto findEndOfBlock = [argc, argv, &_offset]() {
        _offset++;
        if (strcmp(argv[_offset], "("))
        {// block is just one argument
            _offset++;
            return;
        }

        for (size_t blockCount = 0; _offset < argc; _offset++)
        {
            if (!strcmp(argv[_offset], "("))
            {
                blockCount++;
                continue;
            }
            if (!strcmp(argv[_offset], ")"))
            {
                if (blockCount <= 0)
                {
                    std::cout << "Error: Missing opening braces.";
                    exit(1);
                    return;
                }
                blockCount--;
                if (!blockCount)
                {
                    break;
                }
            }
        }
        };

    // boolean to track if the next block should be executed
    bool executeBlock = true;
    for (; _offset < argc; _offset++)
    {
        if (!strcmp(argv[_offset], "("))
        {
            if (executeBlock)
            {// execute block
                continue;
            }
            // skip until the block is closed
            _offset--;
            findEndOfBlock();
        }
        if (!strcmp(argv[_offset], ")"))
        {// end of block
            continue;
        }

        executeBlock = true;

        if (!strcmp(argv[_offset], "-get"))
        {
            checkFollowing;
            _pastSelected.emplace_back(_element);
            _element = (RJSON::JSONElement)_element.get(argv[_offset + 1]);
            _offset++;
            continue;
        }
        if (!strcmp(argv[_offset], "-i") || !strcmp(argv[_offset], "-index"))
        {
            checkFollowing;
            _pastSelected.emplace_back(&_element);
            _element = (RJSON::JSONElement)_element.get(std::stoi(argv[_offset + 1]));
            _offset++;
            continue;
        }
        if (!strcmp(argv[_offset], "-iterate"))
        {
            size_t offsetOld = _offset + 1;
            findEndOfBlock();// _offset marks the end of the block
            // iterate over every child and execute the code block for each
            for (auto child : _element.children)
            {
                iterateArgs(_offset/*end of block*/, argv, offsetOld, child, _pastSelected);
            }
            continue;
        }
        if (!strcmp(argv[_offset], "-up"))
        {
            if (_pastSelected.empty())
            {
                continue;
            }
            _element = _pastSelected.back();
            _pastSelected.pop_back();
            continue;
        }
        if (!strcmp(argv[_offset], "-root"))
        {
            _element = _pastSelected.front();
            _pastSelected.clear();
            _pastSelected.emplace_back(&_element);
            continue;
        }
        if (!strcmp(argv[_offset], "-check"))
        {
            executeBlock = _element.hasError();
            if (_offset + 1 < argc && !strcmp(argv[_offset + 1], "("))
            {
                continue;
            }
            std::cout << executeBlock << delimiter;
            continue;
        }
        if (!strcmp(argv[_offset], "-count"))
        {
            std::cout << _element.children.size() << delimiter;
            continue;
        }
        if (!strcmp(argv[_offset], "-exists"))
        {
            executeBlock = _element.exists();
            if (_offset + 1 < argc && !strcmp(argv[_offset + 1], "("))
            {
                continue;
            }
            std::cout << executeBlock << delimiter;
            continue;
        }
        if (!strcmp(argv[_offset], "-name"))
        {
            std::cout << _element.name << delimiter;
            continue;
        }
        if (!strcmp(argv[_offset], "-value"))
        {
            std::cout << _element.value << delimiter;
            continue;
        }
        if (!strcmp(argv[_offset], "-json"))
        {
            checkFollowing;
            if (argv[++_offset][0] == '1')
            {
                std::cout << _element.asJSON(true);
                continue;
            }
            std::cout << _element.asJSON() << delimiter;
            continue;
        }
        if (!strcmp(argv[_offset], "-delim") || !strcmp(argv[_offset], "-delimiter"))
        {
            checkFollowing;
            delimiter = argv[++_offset];
            continue;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Error: Not enough arguments.";
        return 1;
    }
    // open and load file
    std::fstream fs(argv[1]);
    if (!fs.is_open())
    {
        std::cout << "Error: Failed to open file.";
        return 1;
    }

    std::string fileContent((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement root = RJSON::RJSON::load(fileContent);

    std::vector<RJSON::JSONElement> pastSelected;
    size_t offset = 2;
    iterateArgs(argc, argv, offset, root, pastSelected);

    return 0;
}
