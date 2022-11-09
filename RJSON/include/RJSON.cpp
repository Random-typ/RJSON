#include "RJSON.h"

#ifdef __RJSON__
namespace RJSON
{
	JSONElement RJSON::EmptyElem( "", "" );
	const char* RJSON::JSONWhitespace = " \t\n\r";

	JSONElement::JSONElement() {}
	

	JSONElement::JSONElement(int _val) {
		value = std::to_string(_val);
	}

	JSONElement::JSONElement(float _val) {
		value = std::to_string(_val);
	}

	JSONElement::JSONElement(bool _val) {
		value = std::to_string(_val);
	}

	JSONElement::JSONElement(std::string _name) {
		name = _name;
	}


	JSONElement::JSONElement(std::string _name, int _val) {
		name = _name;
		value = std::to_string(_val);
	}

	JSONElement::JSONElement(std::string _name, float _val) {
		name = _name;
		value = std::to_string(_val);
	}

	JSONElement::JSONElement(std::string _name, bool _val) {
		name = _name;
		value = std::to_string(_val);
	}

	JSONElement::JSONElement(std::string _name, std::string _val) {
		name = _name;
		value = _val;
	}

	JSONElement& JSONElement::get(std::string _name)
	{
		for (JSONElement& elem : children)
			if (elem.name == _name)
				return elem;
		return RJSON::EmptyElem;
	}


	JSONElementArrayPTR JSONElement::getAll(std::string _name)
	{
		JSONElementArrayPTR elements;
		for (size_t i = 0; i < children.size(); i++)
		{
			elements += children[i].getAll(_name);
			if (!children.empty())
				children[i].getAll(_name);
			if (children[i].name == _name)
			{
				elements.push_back(&children[i]);
			}
		}
		return elements;
	}

	JSONElement& JSONElement::addChild(std::string _name, std::string _value, JSONType _type)
	{
		JSONElement elem;
		elem.name = _name;
		elem.value = _value;
		elem.type = _type;
		return addChild(elem);
	}

	JSONElement& JSONElement::addChild(JSONElement _jsonElement)
	{
		children.push_back(_jsonElement);
		return children.back();
	}

	bool JSONElement::removeChild(std::string _name)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i].name == _name)
			{
				children.erase(children.begin() + i);
				return true;
			}
		}
		return false;
	}

	const bool JSONElement::hasChild(std::string _name)
	{
		for (const JSONElement& elem : children)
			if (elem.name == _name)
				return true;
		return false;
	}

	const bool JSONElement::isEmpty()
	{
		return children.empty();
	}

	const bool JSONElement::erase()
	{
		if (parent == nullptr)
		{
			return false;
		}
		if (__int64 i = getIndex(); i < 0)
		{
			parent->children.erase(parent->children.begin() + i);
			return true;
		}
		return false;
	}

	const __int64 JSONElement::getIndex()
	{
		if (parent == nullptr)
		{
			return -1;
		}
		for (size_t i = 0; i < parent->children.size(); i++)
			if (parent->children[i].name == name)
			{
				return i;
			}
		return -1;
	}

	const std::string JSONElement::asJSON()
	{
		if (name.empty())
		{
			return asJSONInner();
		}
		if (type == JSONTypes::Array)
		{
			return "[" + asJSONInner() + "]";
		}
		return "{" + asJSONInner() + "}";
	}

	const std::string JSONElement::asJSONInner()
	{
		std::string json;
		if (!name.empty())
		{
			json += "\"" + name + "\":";
		}
		if (type == JSONTypes::Array)
		{
			json += "[";
		}
		else
		if (type == JSONTypes::Object)
		{
			json += "{";
		}
		else
		{
			json += rawValue();
			return json;
		}
		for (size_t i = 0; i < children.size();)
		{
			json += children[i].asJSONInner();
			// check if this was the last element
			if (++i < children.size())
			{
				json += ",";
			}
		}
		if (type == JSONTypes::Array)
		{
			json += "]";
		}
		if (type == JSONTypes::Object)
		{
			json += "}";
		}
		return json;
	}

	const std::string JSONElement::rawValue()
	{
		switch (type)
		{
		case JSONTypes::String:
			return "\"" + value + "\"";
		case JSONTypes::Integer:
			return value;
		case JSONTypes::Float:
			return value;
		case JSONTypes::Boolean:
			return value;
		case JSONTypes::Null:
			return value;
		default:// Handle like string
			return "\"" + value + "\"";
		}
	}

	const JSONType JSONElement::getType()
	{
		return type;
	}

	const void JSONElement::autoType()
	{
		if (!children.empty())
		{
			if (children.front().name.empty())
			{
				type = JSONTypes::Array;
			}
			else
				type = JSONTypes::Object;
			return;
		}

		if (value.empty())
		{// The type could be anything 
			if (name.empty())
			{
				type = JSONTypes::Unknown;
			}
			else
				type = JSONTypes::String;
			return;
		}
		if (value.find_first_not_of("-1234567890.") == std::string::npos)// e and E are not included here | Eulers numbers not supported
		{// Number
			if (value.find('.') != std::string::npos)
			{// Float
				type = JSONTypes::Float;
			}
			type = JSONTypes::Integer;
			return;
		}
		if (value == "true" || value == "false")
		{// Boolean
			type = JSONTypes::Null;
			return;
		}
		if (value == "null")
		{
			type = JSONTypes::Null;
			return;
		}
		// String
		type = JSONTypes::String;
		return;
	}

	const int JSONElement::valueAsInt()
	{
		return stoi(value);
	}

	const float JSONElement::valueAsFloat()
	{
		char* _Eptr;// that would be for error checking but im not gonna do that
		return strtof(value.c_str(), &_Eptr);
	}

	const std::string JSONElement::valueAsString()
	{
		return value;
	}

	const bool JSONElement::valueAsBool()
	{
		if (value.length() && value[0] == 't' || value[0] == 'T')
			return true;
		return false;
	}

	JSONElementArray operator+(const JSONElementArray& _right, const JSONElementArray& _left)
	{
		JSONElementArray elements;
		elements.insert(elements.begin(), _right.begin(), _right.end());
		elements.insert(elements.end(), _left.begin(), _left.end());
		// Alternative: memcpy
		//elements.resize(_right.size() + _left.size());
		//memcpy_s(&elements[0], _right.size(), &_right[0], _right.size());
		//memcpy_s(&elements[_right.size() - 1], _left.size(), &_left[0], _left.size());
		return elements;
	}

	JSONElementArray operator+(const JSONElementArray& _left, const JSONElementArrayPTR& _right)
	{
		JSONElementArray elements;
		elements.insert(elements.begin(), _left.begin(), _left.end());
		for (const auto elem : _right)
		{
			elements.push_back(*elem);
		}
		return elements;
	}

	JSONElementArray operator+=(JSONElementArray& _left, const JSONElementArray& _right)
	{
		_left.insert(_left.end(), _right.begin(), _right.end());
		return _left;
	}

	JSONElementArray operator+=(JSONElementArray& _left, const JSONElementArrayPTR& _right)
	{
		for (const auto elem : _right)
		{
			_left.push_back(*elem);
		}
		return _left;
	}

	//bool operator==(const JSONElementArray& _left, const JSONElementArray& _right)
	//{
	//	if (_left.size() == _right.size())
	//	{
	//		return !memcmp(&_left[0], &_right[0], _left.size());
	//	}
	//	else
	//		return false;
	//}

	//bool operator==(const JSONElementArray& _left, const JSONElementArrayPTR& _right)
	//{
	//	if (_left.size() == _right.size())
	//	{
	//		for (const auto elem1 : _left)
	//		{
	//			for (const auto elem2 : _right)
	//			{
	//				if (elem1 != *elem2)
	//				{
	//					return false;
	//				}
	//			}
	//		}
	//		return true;
	//	}
	//	else
	//		return false;
	//}

	//bool operator!=(const JSONElementArray& _left, const JSONElementArray& _right)
	//{
	//	return !(_left == _right);
	//}

	//bool operator!=(const JSONElementArray& _left, const JSONElementArrayPTR& _right)
	//{
	//	return !(_left == _right);
	//}

	// PTR class

	JSONElementArrayPTR operator+(const JSONElementArrayPTR& _left, const JSONElementArrayPTR& _right)
	{
		JSONElementArrayPTR elements;
		elements.insert(elements.begin(), _left.begin(), _left.end());
		elements.insert(elements.end(), _right.begin(), _right.end());
		return elements;
	}

	JSONElementArrayPTR operator+(const JSONElementArrayPTR& _left, const JSONElementArray& _right)
	{
		JSONElementArrayPTR elements;
		elements.insert(elements.begin(), _left.begin(), _left.end());
		for (JSONElement elem : _right)
		{
			elements.push_back(&elem);
		}
		return elements;
	}

	JSONElementArrayPTR operator+=(JSONElementArrayPTR& _left, const JSONElementArrayPTR& _right)
	{
		_left.insert(_left.end(), _right.begin(), _right.end());
		return _left;
	}

	JSONElementArrayPTR operator+=(JSONElementArrayPTR& _left, const JSONElementArray& _right)
	{
		for (JSONElement elem : _right)
		{
			_left.push_back(&elem);
		}
		return _left;
	}

	//bool operator==(const JSONElementArrayPTR& _left, const JSONElementArrayPTR& _right)
	//{
	//	if (_left.size() == _right.size())
	//	{
	//		for (const auto elem1 : _left)
	//		{
	//			for (const auto elem2 : _right)
	//			{
	//				if (elem1 != *elem2)
	//				{
	//					return false;
	//				}
	//			}
	//		}
	//		return true;
	//	}
	//	else
	//		return false;
	//}

	//bool operator==(const JSONElementArrayPTR& _left, const JSONElementArray& _right)
	//{
	//	if (_left.size() == _right.size())
	//	{
	//		for (const auto elem1 : _left)
	//		{
	//			for (const auto elem2 : _right)
	//			{
	//				if (elem1 != *elem2)
	//				{
	//					return false;
	//				}
	//			}
	//		}
	//		return true;
	//	}
	//	else
	//		return false;
	//}

	//bool operator!=(const JSONElementArrayPTR& _left, const JSONElementArrayPTR& _right)
	//{
	//	return JSONElementArrayPTR();
	//}

	//bool operator!=(const JSONElementArrayPTR& _left, const JSONElementArray& _right)
	//{
	//	return JSONElementArrayPTR();
	//}

	//// friend
	//const bool operator==(const JSONElement& _left, const JSONElement& _right)
	//{
	//	if (sizeof(_left) == sizeof(_right))
	//	{
	//		return !memcmp(&_left, &_right, sizeof(_left));
	//	}
	//	return false;
	//}

	//// friend
	//const bool operator!=(const JSONElement& _left, const JSONElement& _right)
	//{
	//	return !(_left == _right);
	//}

	const JSONElement operator+(const JSONElement& _left, const JSONElement& _right)
	{
		JSONElement element;
		element.children = _left.children + _right.children;
		return element;
	}

	const JSONElement& operator+=(JSONElement& _left, const JSONElement& _right)
	{
		_left.children += _right.children;
		return _left;
	}


	JSONElement& JSONElement::operator[](const size_t _index)
	{
		return children.operator[](_index);
	}

	JSONElement& JSONElement::operator[](const std::string _name)
	{
		return get(_name);
	}


	// Public
	template<typename _json>
	RJSON::RJSON(_json _JSONElements...)
	{
	#if _json != JSONElement
		#error "RJSON::RJSON(). Can only be type JSONElement"
	#endif
		_JSONElements.get("");
	}
	
	
	JSONElement RJSON::load(std::string _jsonstructure)
	{
		try
		{
			size_t pos = 0;
			return parse(_jsonstructure, pos);
		}
		catch (const std::exception&)
		{
			return JSONElement();
		}
	}

	// Private
	JSONElement RJSON::parseElement(const std::string& _data, size_t& _off, JSONType _type) {

		JSONElement elem;
		if (_off == _data.length() - 1)
		{
			_off = _data.length() - 1;
			return elem;
		}
		if (_type == JSONTypes::Array)
		{
			size_t end = 0;
			size_t pos = _data.find('"', _off);
			for (end = _data.find('"', ++pos); end != std::string::npos && _data[end - 1] == '\\'; end = _data.find('"', ++pos));
			if (size_t off = _data.find_first_not_of(JSONWhitespace, end + 1); end != std::string::npos && off != std::string::npos && _data[off] == ',' || _data[off] == ']')
			{// array element is string
				elem.type = JSONTypes::String;
				_off = pos;
				elem.value = _data.substr(_off, end - _off);
				_off = end;
				return elem;
			}
		}

		size_t end = _data.find('"', ++_off);
		if (end == std::string::npos || _off == std::string::npos)
			return elem;

		elem.name = _data.substr(_off, end - _off);
		_off = ++end;
		_off = _data.find_first_of("\"{[-1234567890tfn", _off);
		if (_off == std::string::npos)
			return elem;

		if (_data[_off] == '"')
		{// string
			// parsing the content
			for (end = _data.find('"', ++_off); _data[end - 1] == '\\'; end = _data.find('"', ++_off));
			elem.value = _data.substr(_off, end - _off);
			_off = end;
			elem.type = JSONTypes::String;
			return elem;
		}
		else if (_data[_off] == '{')
		{// object
			elem.children = parse(_data, _off).children;
			elem.type = JSONTypes::Object;
			return elem;
		}
		else if (_data[_off] == '[')
		{// array
			elem.children = parse(_data, _off).children;
			elem.type = JSONTypes::Array;
			return elem;
		}
		else if (std::string str; &(str += _data[_off]) && str.find_first_of("-1234567890") != std::string::npos)
		{// number
			end = _data.find_first_of(",]}", _off);
			elem.value = _data.substr(_off, end - _off);
			_off = end - 1;
			if (elem.value.find('.') != std::string::npos)
			{// Float
				elem.type = JSONTypes::Float;
			}
			else
				elem.type = JSONTypes::Integer;
			return elem;
		}
		else if (_data[_off] == 't')
		{// boolean true
			end = _data.find_first_of(",]}", _off);
			elem.value = _data.substr(_off, end - _off);
			_off = end - 1;
			elem.type = JSONTypes::Boolean;
			return elem;
		}
		else if (_data[_off] == 'f')
		{// boolean false
			end = _data.find_first_of(",]}", _off);
			elem.value = _data.substr(_off, end - _off);
			_off = end - 1;
			elem.type = JSONTypes::Boolean;
			return elem;
		}
		else if (_data[_off] == 'n')
		{// null
			end = _data.find_first_of(",]}", _off);
			elem.value = _data.substr(_off, end - _off);
			_off = end - 1;
			elem.type = JSONTypes::Null;
			return elem;
		}
		throw "JSON is not formatted correctly.";
		return elem;
	}

	JSONElement RJSON::parse(const std::string& _data, size_t& _off, std::string _name)
	{
		JSONElement elem;
		_off = _data.find_first_of("[{", _off);
		if (_off == std::string::npos)
			return elem;

		if (_data[_off] == '{')
		{
			elem.type = JSONTypes::Object;
		}
		else if (_data[_off] == '[')
		{
			elem.type = JSONTypes::Array;
		}


		for (; _off < _data.length(); _off++)
		{
			size_t tmp = _off;
			tmp = _data.find_first_not_of(JSONWhitespace, ++tmp);
			if (tmp != std::string::npos)
				_off = tmp;

			if (_data[_off] == '}' || _data[_off] == ']')
			{
				return elem;
			}
			if (_data[_off] == '{' || _data[_off] == '[')
			{
				JSONElement JE = parse(_data, _off);
				JE.parent = &elem;
				elem.children.push_back(JE);
				if (_data[_off + 1] == '}' || _data[_off + 1] == ']')
				{
					_off++;
					return elem;
				}
				continue;
			}
			_off = _data.find('"', _off);
			JSONElement JE = parseElement(_data, _off, elem.type);
			JE.parent = &elem;
			elem.children.push_back(JE);
			if (_data[_off + 1] == '}' || _data[_off + 1] == ']')
			{
				_off++;
				return elem;
			}
		}
		return elem;
	}
}
#endif // __RJSON__
