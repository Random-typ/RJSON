#pragma once
#include "../RJSON/include/RJSON.h"
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "../RJSON/lib/RJSON_d.lib")
#else
#pragma comment(lib, "../RJSON/lib/RJSON.lib")
#endif // _DEBUG



class RJSON_CLI_Tool
{
#define checkFollowing \
if (_offset + 1 >= _argc)\
{\
    std::cout << "Error: missing argument.";\
    return;\
}
    static void iterateArgs(int argc, char* argv[], size_t _offset, RJSON::JSONElement& _element);
    static void iterateArgs(size_t _offset, int _argc, RJSON::JSONElement& element, std::vector<RJSON::JSONElement>& pastSelected);

    static void findEndOfBlock(int _argc, size_t _offset);

    static void get(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void index(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void iterate(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void up(RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void root(RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void check(bool& _executeBlock, int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);

    static char* argv[];
    static const char* delimiter;
};

