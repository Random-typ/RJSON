#include "include/RJSON.h"

// testing
#include <iostream>
#include <fstream>


int main()
{
	//RJSON::JSONElement json(
	//	RJSON::JSONElement("root"),
	//	RJSON::JSONElement("root"),
	//	RJSON::JSONElement("root"),
	//	RJSON::JSONElement("root"),
	//	);





	//std::fstream fs("examples/test.json");
	std::fstream fs("C:/Users/Random-Typ/source/repos/RMonitor - Server Monitor/RMonitor - Server Monitor/environment/config.json");
	std::string JSON((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	fs.close();
	RJSON::JSONElement json = RJSON::RJSON::load(JSON);
	auto config = json.get("config");
	std::cout << json.asJSON(true);
	return 0;

	{
	}
}