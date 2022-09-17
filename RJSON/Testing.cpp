#include "include/RJSON.h"

// testing
#include <iostream>
#include <fstream>

int main()
{
    std::fstream fs("examples/test.json");
    std::string exampleJSON((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement json = RJSON::RJSON::load(exampleJSON);
    std::cout << json.asJSON();
    return 0;
}
