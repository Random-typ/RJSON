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
#define isFollowed _offset + 1 >= _argc

#define checkFollowing \
if (isFollowed)\
{\
    std::cout << "Error: missing argument.";\
    return;\
}

    static void iterateArgs(int argc, char** argv, size_t _offset, RJSON::JSONElement& _element);
    static void iterateArgs(size_t _offset, int _argc, RJSON::JSONElement& element, std::vector<RJSON::JSONElement>& pastSelected);

    static void findEndOfBlock(int _argc, size_t _offset);

    static void get(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void index(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void iterate(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void up(RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void root(RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void check(bool& _executeBlock, int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected);
    static void count(const RJSON::JSONElement& _element);
    static void exists(bool& _executeBlock, int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void name(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void value(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void json(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void delim(int _argc, size_t& _offset);

    static void setName(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void setValue(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void add(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void adds(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void addn(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void addv(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void adda(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void addas(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void adde(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void addes(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void remove(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void removes(int _argc, size_t& _offset, RJSON::JSONElement& _element);
    static void clear(RJSON::JSONElement& _element);


    static char** argv;
    static const char* delimiter;
};

