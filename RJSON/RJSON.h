#pragma once
#include <string>
#include <vector>

#ifdef _HAS_CXX17
	#ifndef __RJSON__
		#define __RJSON__
	#endif
#else
	#error "RJSON needs at least C++17."
#endif // !_HAS_CXX17

#ifdef __RJSON__
namespace RJSON {
	constexpr const char* JSONWhitespace = " \t\n\r";
	struct JSONElement;

	enum class JSONTypes
	{
		Unknown,
		Integer,
		Float,
		String,
		Boolean,
		Null,
		Object,
		Array
	} typedef JSONType;

	class JSONElementArray : public std::vector<JSONElement>
	{
	public:
		
	};

	struct JSONElement
	{
	public:
		// get element by its name
		// if level < 0 global search will be performed
		JSONElement&					get(std::string _name);
		JSONElement&					get(std::string _name, int _level);


		JSONElement&					addChild(std::string _name, std::string _value = "", JSONType _type = JSONTypes::Unknown);
		JSONElement&					addChild(JSONElement _jsonElement);

		void                            removeChild(std::string _name, size_t _index = 0);
		void                            removeChild(size_t _index);

		const unsigned int              countChildren(std::string _name);

		const bool                      hasChild(std::string _name);

		const bool						isEmpty();

		const std::string               asJSON();

		const JSONType					getType();

		const void						autoType();

		const int						valueAsInt();

		const float						valueAsFloat();

		const std::string				valueAsString();

		const bool						valueAsBool();

		friend const bool				operator==(const JSONElement& _right, const JSONElement& _left);
		friend const bool				operator!=(const JSONElement& _right, const JSONElement& _left);

		JSONElement&					operator[](const size_t _index);
		JSONElement&					operator[](const std::string _name);


		std::string name;
		std::string value;
		JSONElement*					parent = nullptr;
		JSONElementArray				children;
		JSONType						type = JSONTypes::Unknown;

	private:
		const std::string               asJSONInner();
		const std::string				rawValue();
	};


	class RJSON
	{
	public:
		static JSONElement EmptyElem;
		static JSONElement load(std::string _jsonstructure);

	private:
		static JSONElement parseElement(const std::string& _data, size_t& _off, JSONType _type);
		static JSONElement parse(const std::string& _data, size_t& _off, std::string _name = "");
	};
}
#endif // !__RJSON__