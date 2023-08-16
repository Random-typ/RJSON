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
	//RJSON::JSONElement json = RJSON::RJSON::load("{\"result\":[{\"id\":\"a5acf5ed6859954c617df733fef5e6a2\",\"name\":\"aquamarinband.de\",\"status\":\"active\",\"paused\":false,\"type\":\"full\",\"development_mode\":0,\"name_servers\":[\"agustin.ns.cloudflare.com\",\"karsyn.ns.cloudflare.com\"],\"original_name_servers\":null,\"original_registrar\":null,\"original_dnshost\":null,\"modified_on\":\"2022 - 09 - 08T21:02 : 47.913937Z\",\"created_on\":\"2022 - 09 - 07T12:47 : 25.834488Z\",\"activated_on\":\"2022 - 09 - 07T12:53 : 35.449762Z\",\"meta\":{\"step\":2,\"custom_certificate_quota\":0,\"page_rule_quota\":3,\"phishing_detected\":false,\"multiple_railguns_allowed\":false},\"owner\":{\"id\":null,\"type\":\"user\",\"email\":null},\"account\":{\"id\":\"fa5eb97b53d289e16d7e60f4fde15563\",\"name\":\"No.name.typ2.0@gmail.com's Account\"},\"tenant\":{\"id\":null,\"name\":null},\"tenant_unit\":{\"id\":null},\"permissions\":[\"#zone:read\",\"#zone_settings:read\",\"#dns_records:read\",\"#dns_records:edit\"],\"plan\":{\"id\":\"0feeeeeeeeeeeeeeeeeeeeeeeeeeeeee\",\"name\":\"Free Website\",\"price\":0,\"currency\":\"USD\",\"frequency\":\"\",\"is_subscribed\":false,\"can_subscribe\":false,\"legacy_id\":\"free\",\"legacy_discount\":false,\"externally_managed\":false}},{\"id\":\"d2e5eb1888a443d707899146bc4bacb0\",\"name\":\"funkmates.de\",\"status\":\"active\",\"paused\":false,\"type\":\"full\",\"development_mode\":0,\"name_servers\":[\"agustin.ns.cloudflare.com\",\"karsyn.ns.cloudflare.com\"],\"original_name_servers\":null,\"original_registrar\":null,\"original_dnshost\":null,\"modified_on\":\"2022-03-22T19:12:29.051232Z\",\"created_on\":\"2022-03-22T18:56:20.213769Z\",\"activated_on\":\"2022-03-22T19:12:29.051232Z\",\"meta\":{\"step\":2,\"custom_certificate_quota\":0,\"page_rule_quota\":3,\"phishing_detected\":false,\"multiple_railguns_allowed\":false},\"owner\":{\"id\":null,\"type\":\"user\",\"email\":null},\"account\":{\"id\":\"fa5eb97b53d289e16d7e60f4fde15563\",\"name\":\"No.name.typ2.0@gmail.com's Account\"},\"tenant\":{\"id\":null,\"name\":null},\"tenant_unit\":{\"id\":null},\"permissions\":[\"#zone:read\",\"#zone_settings : read\",\"#dns_records : read\",\"#dns_records : edit\"],\"plan\":{\"id\":\"0feeeeeeeeeeeeeeeeeeeeeeeeeeeeee\",\"name\":\"Free Website\",\"price\":0,\"currency\":\"USD\",\"frequency\":\"\",\"is_subscribed\":false,\"can_subscribe\":false,\"legacy_id\":\"free\",\"legacy_discount\":false,\"externally_managed\":false}},{\"id\":\"e7bfeb7bfc316dc8f99aa46fe8f6ce12\",\"name\":\"retard - inc.com\",\"status\":\"active\",\"paused\":false,\"type\":\"full\",\"development_mode\":0,\"name_servers\":[\"agustin.ns.cloudflare.com\",\"karsyn.ns.cloudflare.com\"],\"original_name_servers\":[\"dns1.registrar - servers.com\",\"dns2.registrar - servers.com\"],\"original_registrar\":\"namecheap, inc. (id : 1068)\",\"original_dnshost\":null,\"modified_on\":\"2021 - 08 - 30T16:15 : 15.839845Z\",\"created_on\":\"2021 - 08 - 30T15:11 : 45.008972Z\",\"activated_on\":\"2021 - 08 - 30T16:06 : 12.622417Z\",\"meta\":{\"step\":2,\"custom_certificate_quota\":0,\"page_rule_quota\":3,\"phishing_detected\":false,\"multiple_railguns_allowed\":false},\"owner\":{\"id\":null,\"type\":\"user\",\"email\":null},\"account\":{\"id\":\"fa5eb97b53d289e16d7e60f4fde15563\",\"name\":\"No.name.typ2.0@gmail.com's Account\"},\"tenant\":{\"id\":null,\"name\":null},\"tenant_unit\":{\"id\":null},\"permissions\":[\"#zone:read\",\"#zone_settings:read\",\"#dns_records:read\",\"#dns_records:edit\"],\"plan\":{\"id\":\"0feeeeeeeeeeeeeeeeeeeeeeeeeeeeee\",\"name\":\"Free Website\",\"price\":0,\"currency\":\"USD\",\"frequency\":\"\",\"is_subscribed\":false,\"can_subscribe\":false,\"legacy_id\":\"free\",\"legacy_discount\":false,\"externally_managed\":false}}],\"result_info\":{\"page\":1,\"per_page\":20,\"total_pages\":1,\"count\":3,\"total_count\":3},\"success\":true,\"errors\":[],\"messages\":[]}");
	RJSON::JSONElement json = RJSON::RJSON::load("{\"succes");
	std::cout << json.asJSON(true);
	return 0;

	{
	}
}