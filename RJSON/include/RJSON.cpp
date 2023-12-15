#include "RJSON.h"

#ifdef __RJSON__
namespace RJSON
{
	JSONElement RJSON::EmptyElem;
	const char* RJSON::JSONWhitespace = " \t\n\r";

	JSONElement::JSONElement() {}

	JSONElement::JSONElement(const JSONElement& _elem)
		:
		name(_elem.name),
		value(_elem.value),
		children(_elem.children),
		type(_elem.type),
		error(_elem.error),
		errorLocation(_elem.errorLocation)
	{}
	

	//JSONElement::JSONElement(std::initializer_list<JSONElement> _json)
	//{
	//	_json.size();
	//}

	JSONElement::JSONElement(size_t _val) {
		value = std::to_string(_val);
		type = JSONTypes::Integer;
	}

	JSONElement::JSONElement(float _val) {
		value = std::to_string(_val);
		type = JSONTypes::Float;
	}

	JSONElement::JSONElement(bool _val) {
		type = JSONTypes::Boolean;
		if (_val)
		{
			value = "true";
			return;
		}
		value = "false";
	}

	JSONElement::JSONElement(const char* _name)
		: name(_name) {
		type = JSONTypes::Object;
	}

	JSONElement::JSONElement(std::string _name, JSONType _type)
		: name(_name), type(_type) {}


	JSONElement::JSONElement(const std::string& _name, size_t _val)
	: name(_name){
		name = _name;
		value = std::to_string(_val);
		type = JSONTypes::Integer;
	}

	JSONElement::JSONElement(const std::string& _name, float _val) 
	: name(_name){
		value = std::to_string(_val);
		type = JSONTypes::Float;
	}

	JSONElement::JSONElement(const std::string& _name, bool _val) 
		: name(_name)
	{
		type = JSONTypes::Boolean;
		if (_val)
		{
			value = "true";
			return;
		}
		value = "false";
	}

	JSONElement::JSONElement(const std::string& _name, JSONElementArray _json, JSONType _type)
		: name(_name), 
		children(_json),
		type(_type){
	}

	JSONElement::JSONElement(const std::string& _name, const std::string& _val)
		: name(_name),
		value(_val)
	{
		type = JSONTypes::String;
	}

	JSONElement::~JSONElement()
	{

	}


	bool JSONElement::hasError() const
	{
		return error != JSONErrors::OK;
	}
	std::string JSONElement::getErrorText() const
	{
		std::string text;
		switch (error)
		{
		case JSONErrors::OK:
			return text;
		case JSONErrors::MissingColon:
			text = "Colon missing at ";
			break;
		case JSONErrors::Unexpected_Character:
			text = "Unexpected character at ";
			break;
		case JSONErrors::UnexpectedControl_Character:
			text = "Unexpected control character at ";
			break;
		case JSONErrors::UnexpectedEndOfString:
			text = "Unexpected end of string at ";
			break;
		case JSONErrors::UnexpectedEndOfObject:
			text = "Unexpected end of object at ";
			break;
		case JSONErrors::JSONisEmpty:
			text = "JSON was empty.";
			return text;
		case JSONErrors::UnhandledException:
			text = "An unhandled exception was thrown.";
			return text;
		}
		text += std::to_string(errorLocation) + ".";
		return text;
	}

	JSONElement& JSONElement::get(const std::string& _name)
	{
		for (JSONElement& elem : children)
			if (elem.name == _name)
				return elem;
		RJSON::EmptyElem.error = error;
		RJSON::EmptyElem.errorLocation = errorLocation;
		return RJSON::EmptyElem;
	}

	JSONElement& JSONElement::get(size_t _index)
	{
		if (_index >= children.size())
		{
			return RJSON::EmptyElem;
		}
		return children[_index];
	}

	JSONElementArrayPTR JSONElement::getAll(const std::string& _name)
	{
		JSONElementArrayPTR elements;
		for (size_t i = 0; i < children.size(); i++)
		{
			elements += children[i].getAll(_name);
			if (!children.empty())
				children[i].getAll(_name);
			if (children[i].name == _name)
			{
				elements.emplace_back(&children[i]);
			}
		}
		return elements;
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::string& _value, JSONType _type)
	{
		JSONElement elem;
		elem.name = _name;
		elem.value = _value;
		elem.type = _type;
		children.emplace_back(elem);
		return children.back();
	}
	
	JSONElement& JSONElement::addChild(const std::string& _name, const char* _value, JSONType _type)
	{
		JSONElement elem;
		elem.name = _name;
		elem.value = _value;
		elem.type = _type;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const std::string& _name, size_t _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const std::string& _name, int _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const std::string& _name, unsigned int _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const std::string& _name, long long _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const std::string& _name, double _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Float);
	}
	
	JSONElement& JSONElement::addChild(const std::string& _name, long double _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Float);
	}

	JSONElement& JSONElement::addChild(const std::string& _name, bool _value)
	{
		if (_value)
		{
			return addChild(_name, "true", JSONType::Boolean);
		}
		return addChild(_name, "false", JSONType::Boolean);
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::vector<JSONElement>& _elements)
	{
		JSONElement elem;
		elem.name = _name;
		for (auto& i : _elements)
		{
			elem.children.emplace_back(i);
		}
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::vector<std::string>& _array)
	{
		JSONElement elem;
		elem.name = _name;
		for (auto& i : _array)
		{
			JSONElement element;
			element.type = JSONType::String;
			element.value = i;
			elem.children.emplace_back(element);
		}
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::vector<size_t>& _array)
	{
		JSONElement elem;
		elem.name = _name;
		for (auto& i : _array)
		{
			JSONElement element;
			element.type = JSONType::Integer;
			element.value = std::to_string(i);
			elem.children.emplace_back(element);
		}
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::vector<int>& _array)
	{
		JSONElement elem;
		elem.name = _name;
		for (auto& i : _array)
		{
			JSONElement element;
			element.type = JSONType::Integer;
			element.value = std::to_string(i);
			elem.children.emplace_back(element);
		}
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::vector<double>& _array)
	{
		JSONElement elem;
		elem.name = _name;
		for (auto& i : _array)
		{
			JSONElement element;
			element.type = JSONType::Float;
			element.value = std::to_string(i);
			elem.children.emplace_back(element);
		}
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const std::string& _name, const std::vector<bool>& _array)
	{
		JSONElement elem;
		elem.name = _name;
		for (auto i : _array)
		{
			JSONElement element;
			element.type = JSONType::Boolean;
			if (i)
				element.value = "true";
			else
				element.value = "false";
			
			elem.children.emplace_back(element);
		}
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const JSONElement& _jsonElement)
	{
		children.emplace_back(_jsonElement);
		return children.back();
	}

	bool JSONElement::removeChild(const std::string& _name)
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

	bool JSONElement::hasChild(const std::string& _name) const
	{
		for (const JSONElement& elem : children)
			if (elem.name == _name)
				return true;
		return false;
	}

	bool JSONElement::exists() const
	{
		return !name.empty() || !value.empty() || !children.empty();
	}

	bool JSONElement::hasChildren() const
	{
		return !children.empty();
	}

	//const bool JSONElement::erase()
	//{
	//	if (parent == nullptr)
	//	{
	//		return false;
	//	}
	//	if (__int64 i = getIndex(); i < 0)
	//	{
	//		parent->children.erase(parent->children.begin() + i);
	//		return true;
	//	}
	//	return false;
	//}

	//const __int64 JSONElement::getIndex()
	//{
	//	if (parent == nullptr)
	//	{
	//		return -1;
	//	}
	//	for (size_t i = 0; i < parent->children.size(); i++)
	//		if (parent->children[i].name == name)
	//		{
	//			return i;
	//		}
	//	return -1;
	//}

	std::string JSONElement::asJSON(bool _formatted, std::string _whitespace) const
	{
		if (_formatted)
		{
			std::string indent;
			return asJSONFormatted(indent, _whitespace);
		}
		if (name.empty())
		{
			return asJSONInner();
		}
		/*if (type == JSONTypes::Array)
		{
			return "[" + asJSONInner() + "]";
		}*/
		return "{" +  asJSONInner() + "}";
	}

	
	std::string JSONElement::asJSONFormatted(std::string& _indent, std::string _whitespace) const
	{
		if (name.empty())
		{
			return asJSONInnerFormatted(_indent, _whitespace);
		}
		_indent += _whitespace;
		if (type == JSONTypes::Array)
		{
			std::string front = "[\n" + asJSONInnerFormatted(_indent, _whitespace);
			_indent.pop_back();
			return front + _indent + "\n]";
		}
		std::string front = "{\n" + asJSONInnerFormatted(_indent, _whitespace);
		_indent.pop_back();
		return front + _indent + "\n}";
	}

	std::string JSONElement::asJSONInnerFormatted(std::string& _indent, std::string _whitespace) const
	{
		std::string json;
		if (!name.empty())
		{
			json += _indent + "\"" + name + "\": ";
		}
		if (type == JSONTypes::Array)
		{
			json += "[\n";
			_indent += _whitespace;
		}
		else
			if (type == JSONTypes::Object)
			{
				json += "{\n";
				_indent += _whitespace;
			}
			else
			{
				json += rawValue();
				return json;
			}
		for (size_t i = 0; i < children.size();)
		{
			if (type == JSONTypes::Array)
			{
				json += _indent;
			}
			json += children[i].asJSONInnerFormatted(_indent, _whitespace);
			// check if this was the last element
			if (++i < children.size())
			{
				json += ',';
			}
			json += '\n';
		}
		if (type == JSONTypes::Array)
		{
			_indent.pop_back();
			json += _indent + ']';
		}
		if (type == JSONTypes::Object)
		{
			_indent.pop_back();
			json += _indent + '}';
		}
		return json;
	}

	std::string JSONElement::asJSONInner() const
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

	std::string JSONElement::rawValue() const
	{// check for controlcharacters
		switch (type)
		{
		case JSONTypes::Integer:
			return value;
		case JSONTypes::Float:
			return value;
		case JSONTypes::Boolean:
			return value;
		case JSONTypes::Null:
			return value;
		case JSONTypes::String:
		default:
			// Handle control characters
			std::string valueModified = value;
			for (size_t i = 0; i < valueModified.size(); i++)
			{
				if (valueModified[i] == '"' || 
					//valueModified[i] == '/' ||
					valueModified[i] == '\\' ||
					valueModified[i] == '\b' ||
					valueModified[i] == '\f' || 
					valueModified[i] == '\n' || 
					valueModified[i] == '\r' || 
					valueModified[i] == '\t'
					// u+hex
					)
				{
					switch (valueModified[i])
					{
					case '\b':
						valueModified[i] = 'b';
						break;
					case '\f':
						valueModified[i] = 'f';
						break;
					case '\n':
						valueModified[i] = 'n';
						break;
					case '\r':
						valueModified[i] = 'r';
						break;
					case '\t':
						valueModified[i] = 't';
						break;
					}
					valueModified.insert(valueModified.begin() + i, '\\');
					i++;
					continue;
				}

				if (valueModified[i] != '\\')
				{
					continue;
				}
				if (i + 1 == valueModified.size())
				{
					valueModified += '\\';
					break;
				}
				switch (valueModified[i + 1])
				{
				case '"':
				case '\\':
				//case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'u':
					i++;
					break;
				default:
					valueModified.insert(valueModified.begin() + i, '\\');
					i++;
					break;
				}
			}
			return "\"" + valueModified + "\"";
		}
	}

	JSONType JSONElement::getType() const
	{
		return type;
	}

	void JSONElement::autoType()
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

	long long JSONElement::valueAsInt() const
	{
		if (value.empty() || type != JSONTypes::Integer)
		{
			return 0;
		}
		return stoll(value);
	}

	long double JSONElement::valueAsFloat() const
	{
		if (value.empty() || type != JSONTypes::Float)
		{
			return 0;
		}
		char* _Eptr;// that would be for error checking but im not gonna do that
		return strtold(value.c_str(), &_Eptr);
	}

	std::string JSONElement::valueAsString() const
	{
		return value;
	}

	bool JSONElement::valueAsBool() const
	{
		if (value.length() && value[0] == 't' || value[0] == 'T')
			return true;
		return false;
	}

	std::vector<std::string> JSONElement::asArray() const
	{
		std::vector<std::string> arr;
		for (auto& i : children)
		{
			arr.emplace_back(i.value);
		}
		return arr;
	}

	void JSONElement::fixValue()
	{
		if (type != JSONTypes::String && type != JSONTypes::Unknown)
		{
			return;
		}
		//for (size_t i = value.find_first_of("\\\"/"); i < length; i++)
		//{

		//}



	}

	JSONElement& JSONElement::operator[](const size_t _index)
	{
		return get(_index);
	}

	JSONElement& JSONElement::operator[](const std::string& _name)
	{
		return get(_name);
	}

	JSONElement& JSONElement::operator=(size_t _value)
	{
		value = std::to_string(_value);
		type = JSONTypes::Integer;
		return *this;
	}

	JSONElement& JSONElement::operator=(float _value)
	{
		value = std::to_string(_value);
		type = JSONTypes::Float;
		return *this;
	}

	JSONElement& JSONElement::operator=(bool _value)
	{
		type = JSONTypes::Boolean;
		if (_value)
		{
			value = "true";
			return *this;
		}
		value = "false";
		return *this;
	}

	JSONElement& JSONElement::operator=(const char* _value)
	{
		value = _value;
		type = JSONTypes::String;
		return *this;
	}

	JSONElement& JSONElement::operator=(const JSONElement& _json)
	{
		name = _json.name;
		value = _json.value;
		children = _json.children;
		type = _json.type;

		error = _json.error;
		errorLocation = _json.errorLocation;
		return *this;
	}





	//JSONElementArray operator+(const JSONElementArray& _right, const JSONElementArray& _left)
	//{
	//	JSONElementArray elements;
	//	elements.insert(elements.begin(), _right.begin(), _right.end());
	//	elements.insert(elements.end(), _left.begin(), _left.end());
	//	// Alternative: memcpy
	//	//elements.resize(_right.size() + _left.size());
	//	//memcpy_s(&elements[0], _right.size(), &_right[0], _right.size());
	//	//memcpy_s(&elements[_right.size() - 1], _left.size(), &_left[0], _left.size());
	//	return elements;
	//}

	//JSONElementArray operator+(const JSONElementArray& _left, const JSONElementArrayPTR& _right)
	//{
	//	JSONElementArray elements;
	//	elements.insert(elements.begin(), _left.begin(), _left.end());
	//	for (const auto elem : _right)
	//	{
	//		elements.push_back(*elem);
	//	}
	//	return elements;
	//}

	//JSONElementArray operator+=(JSONElementArray& _left, const JSONElementArray& _right)
	//{
	//	_left.insert(_left.end(), _right.begin(), _right.end());
	//	return _left;
	//}

	//JSONElementArray operator+=(JSONElementArray& _left, const JSONElementArrayPTR& _right)
	//{
	//	for (const auto elem : _right)
	//	{
	//		_left.push_back(*elem);
	//	}
	//	return _left;
	//}
	//
	//void JSONElementArray::addElement(JSONElement _jsonElement)
	//{
	//	push_back(_jsonElement);
	//}

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
			elements.emplace_back(&elem);
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
			_left.emplace_back(&elem);
		}
		return _left;
	}
	
	JSONElement RJSON::load(std::string _jsonstructure)
	{
		if (_jsonstructure.empty())
		{
			JSONElement json;
			json.error = JSONErrors::JSONisEmpty;
			json.errorLocation = 0;
			return json;
		}
		try
		{
			size_t pos = 0;
			return parse(_jsonstructure, pos);
		}
		catch (const std::exception&)
		{
			JSONElement json;
			json.error = JSONErrors::UnhandledException;
			json.errorLocation = 0;
			return json;
		}
	}


	JSONElement RJSON::parse(const std::string& _data, size_t& _off)
	{
		JSONElement elem;
		if (error != JSONErrors::OK)
		{// error
			return elem;
		}
		AfterWhiteSpace;

		switch (_data[_off])
		{
		case '{':
			_off++;
			AfterWhiteSpace;
			while (_data[_off] != '}')
			{
				elem.type = JSONTypes::Object;
				JSONElement elem2 = parse(_data, _off);
				if (elem2.error != JSONErrors::OK)
				{// error
					return elem2;
				}
				elem.children.emplace_back(elem2);
				if (_data[_off] == '"')
				{
					_off++;
				}
				if (_data[_off] == ',')
				{
					_off++;
				}
				//_data.find('}', _off);
				AfterWhiteSpace;
			}
			return elem;
		case '[':
			elem.type = JSONTypes::Array;
			_off++;
			break;
		case '}':
		case ']':
			return elem;
			break;
		}
		
		isEOD;
		AfterWhiteSpace;

		// array parsing
		while (elem.type == JSONTypes::Array)
		{
			if (_data[_off] == '{' || _data[_off] == '[')
			{
				_off;
				elem.children.emplace_back(parse(_data, _off));
				AfterWhiteSpace;
				_off++;
				AfterWhiteSpace;
				if (_data[_off] == ',')
				{
					_off++;
					AfterWhiteSpace;
				}
				if (_data[_off] == ']')
				{
					break;
				}
				continue;
			}


			JSONElement arrElem;
			parseValue(arrElem, _data, _off);
			elem.children.emplace_back(arrElem);
			_off++;
			AfterWhiteSpace;
			if (_data[_off] == ',')
			{
				_off++;
				AfterWhiteSpace;
			}
			if (_data[_off] == ':')
			{
				elem.children.emplace_back(parse(_data, _off));
			}
			if (_data[_off] == ']')
			{
				return elem;
			}
		}

		// object parsing
		switch (_data[_off])
		{
		case '"':
			// parse the elements name
			elem.name = parseString(elem, _data, _off);
			if (elem.error != JSONErrors::OK)
			{// error in parseString()
				return elem;
			}
			_off++;
			AfterWhiteSpace;
			// parse element
			switch (_data[_off])
			{
			case ':':
				_off++;
				AfterWhiteSpace;
				switch (_data[_off])
				{
				case '{':
					elem.type = JSONTypes::Object;
				case '[':
					if (_data[_off] == '[')
					{
						elem.type = JSONTypes::Array;
					}
					if (_data[_off + 1] == ']')
					{
						_off++;
						AfterWhiteSpace;
						_off++;
						break;
					}
					elem.children = parse(_data, _off).children;
					_off++;
					/*if (_data[_off] != '{')
					{
						elem.type = JSONTypes::Array;
					}
					while (_data[_off] != ']' && _data[_off] != '}')
					{
						JSONElement parseElem = parse(_data, _off);
						if (parseElem.type == JSONTypes::Array && parseElem.name.empty())
						{
							elem.children = parseElem.children;
							elem.type = JSONTypes::Array;
						}
						else
						{
							elem.children.emplace_back(parseElem);
						}
						elem.error = parseElem.error;
						elem.errorLocation = parseElem.errorLocation;
						if (_data[_off] == ']' || _data[_off] == '}')
						{
							_off++;
							if (_data[_off] == ',')
								_off++;
							break;
						}
						_off++;
						AfterWhiteSpace;
						if (_data[_off] == ',')
						{
							_off++;
							AfterWhiteSpace;
						}
					}*/
					break;
				default:
					parseValue(elem, _data, _off);
					break;
				}// parse next element

				return elem;
			default:
				// error
				elem.error = JSONErrors::MissingColon;
				elem.errorLocation = _off;
				return elem;
				break;
			}
			break;
		case '}':
		case ']':
				break;
		default:
			elem.error = JSONErrors::Unexpected_Character;
			elem.errorLocation = _off;
			break;
		}
		return elem;
	}

	std::string RJSON::parseString(JSONElement& _elem, const std::string& _data, size_t& _off)
	{
		std::string name;
	parseName:
		size_t start = _off + 1;
		_off = _data.find_first_of("\\\"", _off + 1);
		if (_off == std::string::npos) {
			_elem.error = JSONErrors::UnexpectedEndOfString;
			_elem.errorLocation = start;
			return std::string();
		}
		
		if (_data[_off] == '\\')
		{// control character
			name += _data.substr(start, _off - start);
			_off++;
			if (_off == std::string::npos) {
				_elem.error = JSONErrors::UnexpectedEndOfString;
				_elem.errorLocation = start;
				return std::string();
			}
			switch (_data[_off])
			{
			case '"':
				name += '"';
				break;
			case '\\':
				name += '\\';
				break;
			case '/':
				name += '/';
				break;
			case 'b':
				name += '\b';
				break;
			case 'f':
				name += '\f';
				break;
			case 'n':
				name += '\n';
				break;
			case 'r':
				name += '\r';
				break;
			case 't':
				name += '\t';
				break;
			case 'u':
				// skip hex parsing
				_off += 'u';
				break;
			default:
				// error
				_elem.error = JSONErrors::UnexpectedControl_Character;
				_elem.errorLocation = _off;
				return std::string();
			}
			goto parseName;
		}
		else if (name.empty())
		{
			name = _data.substr(start, _off - start);
		}

		return name;
	}

	void RJSON::parseValue(JSONElement& _elem, const std::string& _data, size_t& _off)
	{
		size_t start = _off;
		switch (_data[_off])
		{
		case '"':
			_elem.value = parseString(_elem, _data, _off);
			_elem.type = JSONTypes::String;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			_off = _data.find_first_of(std::string(JSONWhitespace) + ",}]", _off);
			if (_off == std::string::npos) {
				_elem.error = JSONErrors::UnexpectedEndOfString;
				return;
			}
			_elem.value = _data.substr(start, _off - start);

			if (_elem.value.find('.') != std::string::npos)
			{
				_elem.type = JSONTypes::Float;
				break;
			}
			_elem.type = JSONTypes::Integer;
			break;
		case 't':
		case 'f':
			_off = _data.find_first_of(std::string(JSONWhitespace) + ",}]", _off);
			if (_off == std::string::npos) return;
			_elem.value = _data.substr(start, _off - start);
			_elem.type = JSONTypes::Boolean;
			break;
		case 'n':
			_off = _data.find_first_of(std::string(JSONWhitespace) + ",}]", _off);
			if (_off == std::string::npos) return;
			_elem.value = _data.substr(start, _off - start);
			_elem.type = JSONTypes::Null;
			break;
		default:
			_elem.error = JSONErrors::Unexpected_Character;
			_elem.errorLocation = _off;
			break;
		}
	}
}
#endif // __RJSON__
