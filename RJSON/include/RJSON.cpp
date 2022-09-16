#include "RJSON.h"
#ifdef __RJSON__
namespace RJSON
{
	inline JSONElement RJSON::EmptyElem{ "", "" };

	JSONElement& JSONElement::get(std::string _name)
	{
		for (JSONElement& elem : children)
			if (elem.name == _name)
				return elem;
		return RJSON::EmptyElem;
	}
	JSONElement& JSONElement::get(std::string _name, int _level)
	{
		for (JSONElement& elem : children)
			if (elem.name == _name)
				return elem;

		if (_level != 0 && !children.empty())
			return get(_name, --_level);
		return RJSON::EmptyElem;
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
	void JSONElement::removeChild(std::string _name, size_t _index)
	{
		size_t count = 0;
		for (size_t i = 0; i < children.size(); i++)
			if (children[i].name == _name)
				if (count == _index)
				{
					children.erase(children.begin() + i);
				}
				else
					count++;
	}
	void JSONElement::removeChild(size_t _index)
	{
		children.erase(children.begin() + _index);
	}
	const unsigned int JSONElement::countChildren(std::string _name)
	{
		int count = 0;
		for (const JSONElement& elem : children)
			if (elem.name == _name)
				count++;
		return count;
	}
	const bool JSONElement::hasChild(std::string _name)
	{
		for (const JSONElement& elem : children)
			if (elem.name == _name)
				return true;
		return false;
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
			json += rawValue() + ",";
			return;
		}
		for (JSONElement elem : children)
		{
			json += elem.asJSONInner();
		}
		if (json.back() == ',')
		{
			json.pop_back();
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
		if (value[0] == 't')
			return true;
		return false;
	}

	// friend
	const bool operator==(const JSONElement& _right, const JSONElement& _left)
	{
		if (sizeof(_right) == sizeof(_left))
		{
			return !memcmp(&_right, &_left, sizeof(_right));
		}
		return false;
	}

	// friend
	const bool operator!=(const JSONElement& _right, const JSONElement& _left)
	{
		return !(_right == _left);
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
	JSONElement RJSON::load(std::string _jsonstructure)
	{
		size_t pos = 0;
		return parse(_jsonstructure, pos);
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
