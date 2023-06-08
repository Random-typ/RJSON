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


	//std::vector<RJSON::JSONElement> arr;
	//RJSON::JSONElement eeee("hallo", (std::string)"servus");

	//for (size_t i = 0; i < 20; i++)
	//{

	//	arr.push_back(eeee);
	//}

	//std::fstream fs("examples/test.json");
	std::fstream fs("C:/Users/Random-Typ/source/repos/RMonitor - Server Monitor/RMonitor PluginsDB/pluginsDB.json");
	std::string JSON((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	fs.close();
	RJSON::JSONElement json = RJSON::RJSON::load(JSON);
	std::cout << json.asJSON(true);
	return 0;

	{
	}
}