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

	JSONElement::JSONElement(const JSONElementArray& _elements) : children(_elements) {
		type = JSONTypes::Object;
	}

	JSONElement::JSONElement(string _name, JSONType _type)
		: name(_name), type(_type) {}


	JSONElement::JSONElement(const string& _name, size_t _val)
	: name(_name){
		name = _name;
		value = std::to_string(_val);
		type = JSONTypes::Integer;
	}

	JSONElement::JSONElement(const string& _name, float _val) 
	: name(_name){
		value = std::to_string(_val);
		type = JSONTypes::Float;
	}

	JSONElement::JSONElement(const string& _name, bool _val) 
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

	JSONElement::JSONElement(const string& _name, JSONElementArray _json, JSONType _type)
		: name(_name), 
		children(_json),
		type(_type){
	}

	JSONElement::JSONElement(const string& _name, const string& _val)
		: name(_name),
		value(_val)
	{
		type = JSONTypes::String;
	}

	JSONElement::~JSONElement()
	{

	}


	void JSONElement::copyError(const JSONElement& _elem)
	{
		error = _elem.error;
		errorLocation = _elem.errorLocation;
	}

	bool JSONElement::hasError() const
	{
		return error != JSONErrors::OK;
	}
	string JSONElement::getErrorText() const
	{
		string text;
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

	JSONElement& JSONElement::get(const string& _name)
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

	JSONElementArrayPTR JSONElement::getAll(const string& _name)
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

	JSONElement& JSONElement::addChild(const string& _name, const string& _value, JSONType _type)
	{
		JSONElement elem;
		elem.name = _name;
		elem.value = _value;
		elem.type = _type;
		children.emplace_back(elem);
		return children.back();
	}
	
	JSONElement& JSONElement::addChild(const string& _name, const char* _value, JSONType _type)
	{
		JSONElement elem;
		elem.name = _name;
		elem.value = _value;
		elem.type = _type;
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const string& _name, size_t _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const string& _name, int _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const string& _name, unsigned int _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const string& _name, long long _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const string& _name, double _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Float);
	}
	
	JSONElement& JSONElement::addChild(const string& _name, long double _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Float);
	}

	JSONElement& JSONElement::addChild(const string& _name, bool _value)
	{
		if (_value)
		{
			return addChild(_name, "true", JSONType::Boolean);
		}
		return addChild(_name, "false", JSONType::Boolean);
	}

	JSONElement& JSONElement::addChild(const string& _name, const std::vector<JSONElement>& _elements)
	{
		JSONElement elem(_name);
		elem.children = _elements;
		elem.type = JSONType::Array;
		children.emplace_back(elem);
		return elem;
	}

	JSONElement& JSONElement::addChild(const string& _name, const stringArray& _array)
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

	JSONElement& JSONElement::addChild(const string& _name, const std::vector<size_t>& _array)
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

	JSONElement& JSONElement::addChild(const string& _name, const std::vector<int>& _array)
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

	JSONElement& JSONElement::addChild(const string& _name, const std::vector<double>& _array)
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

	JSONElement& JSONElement::addChild(const string& _name, const std::vector<bool>& _array)
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

	bool JSONElement::removeChild(const string& _name)
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

	bool JSONElement::hasChild(const string& _name) const
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

	string JSONElement::asJSON(bool _formatted, string _whitespace) const
	{
		if (_formatted)
		{
			string indent;
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

	
	string JSONElement::asJSONFormatted(string& _indent, string _whitespace) const
	{
		if (name.empty())
		{
			return asJSONInnerFormatted(_indent, _whitespace);
		}
		_indent += _whitespace;
		if (type == JSONTypes::Array)
		{
			string front = "[\n" + asJSONInnerFormatted(_indent, _whitespace);
			_indent.pop_back();
			return front + _indent + "\n]";
		}
		string front = "{\n" + asJSONInnerFormatted(_indent, _whitespace);
		_indent.pop_back();
		return front + _indent + "\n}";
	}

	string JSONElement::asJSONInnerFormatted(string& _indent, string _whitespace) const
	{
		string json;
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

	string JSONElement::asJSONInner() const
	{
		string json;
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

	string JSONElement::rawValue() const
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
			string valueModified = value;
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
		if (value.find_first_not_of("-1234567890.") == string::npos)// e and E are not included here | Eulers numbers not supported
		{// Number
			if (value.find('.') != string::npos)
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

	string JSONElement::valueAsString() const
	{
		return value;
	}

	bool JSONElement::valueAsBool() const
	{
		if (value.length() && value[0] == 't' || value[0] == 'T')
			return true;
		return false;
	}

	stringArray JSONElement::asArray() const
	{
		stringArray arr;
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

	bool JSONElement::isInteger()
	{
		return type == JSONTypes::Integer;
	}

	bool JSONElement::isFloat()
	{
		return type == JSONTypes::Float;
	}

	bool JSONElement::isString()
	{
		return type == JSONTypes::String;
	}

	bool JSONElement::isBool()
	{
		return type == JSONTypes::Boolean;
	}

	bool JSONElement::isNull()
	{
		return type == JSONTypes::Null;
	}

	bool JSONElement::isObject()
	{
		return type == JSONTypes::Object;
	}

	bool JSONElement::isArray()
	{
		return false;
	}

	JSONElement& JSONElement::operator[](const size_t _index)
	{
		return get(_index);
	}

	JSONElement& JSONElement::operator[](const string& _name)
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
	
	//
	// RJSONStream
	//

	size_t RJSONStream::defaultBufferSize = 64;

	RJSONStream::RJSONStream(size_t bufferSize)
		: buffSize(bufferSize) {
		buffer = new char(buffSize);
	}

	RJSONStream::~RJSONStream()
	{
		if (buffer)
		{
			delete buffer;
		}
	}

	JSONElement RJSONStream::parseStream(const char* _path)
	{
		JSONElement elem;
		fs.open(_path);
		if (!fs.is_open())
		{
			elem.error = JSONErrors::FailedToOpenFile;
			return elem;
		}
		elem = parseStream();
		if (hasError())
		{
			elem.error = error;
			elem.errorLocation = errorLocation;
		}
		fs.close();
		return elem;
	}

	JSONElement RJSONStream::parseStream()
	{
		JSONElement json;

		if (!findEndOfWhitespace())
		{
			return json;
		}
		for (; off < size; )
		{
			if (!findEndOfWhitespace())
			{
				return json;
			}
			switch (buffer[off])
			{
			case '{':
				json.type = JSONTypes::Object;
				off++;
				parseObject(json);
				return json;
			case '[':
				json.type = JSONTypes::Array;
				off++;
				parseArray(json);
				return json;
			default:
				json.errorLocation = totalSizeBefore + off;
				json.error = JSONErrors::Unexpected_Character;
				return json;
			}

			if (off + 1 >= size)
			{
				readBuffer();
				continue;
			}
			off++;
		}

		return json;
	}

	JSONErrors RJSONStream::getError()
	{
		return error;
	}

	bool RJSONStream::hasError()
	{
		return error != JSONErrors::OK;
	}

	void RJSONStream::parseValue(JSONElement& json)
	{
		if (!findEndOfWhitespace())
		{
			errorLocation = totalSizeBefore + off - 1;
			error = JSONErrors::UnexpectedControl_Character;
			return;
		}
		switch (buffer[off])
		{
		case '\"':
			json.type = JSONTypes::String;
			off++;
			json.value = parseString();
			return;
		case '-':
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
		{
			json.type = JSONTypes::Integer;
			// parse number
			// find first of not a number
			size_t start = off;
			if (!findEndOfNumber(start, json.value))
			{
				errorLocation = totalSizeBefore + off - 1;
				error = JSONErrors::UnexpectedEndOfValue;
				return;
			}
			json.value.append(buffer + start, off - start);
			return;
		}
		case '{':
		{
			JSONTypes::Object;
			off++;
			parseObject(json);
			return;
		}
		case '[':
		{
			json.type = JSONTypes::Array;
			off++;
			parseArray(json);
			return;
		}
		case 't':
		{
			json.type = JSONTypes::Boolean;
			size_t start = off;
			if (off + 3 >= size)
			{
				json.value.append(buffer + start, size - off);
				readBuffer();
				start = 0;
				if (off + 4 + json.value.size() >= size)
				{
					errorLocation = totalSizeBefore + off - 1;
					error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			if (json.value.size() - 4)
			{
				json.value.append(buffer + start, 4 - json.value.size());
			}
			if (json.value != "true")
			{
				errorLocation = totalSizeBefore + off - 1;
				error = JSONErrors::Unexpected_Character;
				return;
			}
			off += 4;
			return;
		}
		case 'f':
		{
			json.type = JSONTypes::Boolean;
			size_t start = off;
			if (off + 4 >= size)
			{
				json.value.append(buffer + start, size - off);
				readBuffer();
				if (off + 5 + json.value.size() >= size)
				{
					errorLocation = totalSizeBefore + off - 1;
					error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			if (json.value.size() - 5)
			{
				json.value.append(buffer + start, json.value.size() - 5);
			}

			if (json.value != "false")
			{
				errorLocation = totalSizeBefore + off - 1;
				error = JSONErrors::Unexpected_Character;
				return;
			}
			off += 5;
			break;
		}
		case 'n':
		{
			json.type = JSONTypes::Boolean;
			size_t start = off;
			if (off + 3 >= size)
			{
				json.value.append(buffer + start, size - off);
				readBuffer();
				if (off + 4 + json.value.size() >= size)
				{
					errorLocation = totalSizeBefore + off - 1;
					error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			if (json.value.size() - 4)
			{
				json.value.append(buffer + start, json.value.size() - 4);
			}
			if (json.value != "null")
			{
				errorLocation = totalSizeBefore + off - 1;
				error = JSONErrors::Unexpected_Character;
				return;
			}
			off += 4;
			break;
		}
		default:
			errorLocation = totalSizeBefore + off - 1;
			error = JSONErrors::Unexpected_Character;
			return;
		}
	};

	std::string RJSONStream::parseString()
	{
		string string;
		size_t stringStart = off;
		if (off + 1 >= size)
		{
			readBuffer();
			stringStart = 0;
		}
		for (; off < size;)
		{
			if (buffer[off] == '\"')
			{// found end of string
				break;
			}
			// control character handling
			if (buffer[off] == '\\')
			{// parse control characters
				// Add string found from stringStart to _off
				size_t stringSize = string.size();
				string.append(buffer + stringStart, off - stringStart);
				if (++off == size)
				{
					readBuffer();
				}
				stringStart = off + 1;
				switch (buffer[off])
				{
				case '\"':
					string += '\"';
					break;
				case '\\':
					string += '\\';
					break;
				case '/':
					string += '/';
					break;
				case 'b':
					string += '\b';
					break;
				case 'f':
					string += '\f';
					break;
				case 'n':
					string += '\n';
					break;
				case 'r':
					string += '\r';
					break;
				case 't':
					string += '\t';
					break;
				case 'u':
				{
					if (off + 4 >= size)
					{
						stringStart = 1;// set to one to be set to zero below
						readBuffer();
						if (size <= 4)
						{
							errorLocation = totalSizeBefore + off - 1;// -1 due to offset from   stringStart = _off + 1;
							error = JSONErrors::UnexpectedControl_Character;
							return string;
						}
					}
					char hexBuff[5]{ 0 };
					stringStart--;
					memcpy_s(
						hexBuff,
						sizeof(hexBuff),
						buffer + stringStart * sizeof(char),
						4);

					for (size_t i = 0; i < sizeof(hexBuff) / sizeof(hexBuff[0]); i++)
					{
						if ((hexBuff[i] < '0' || hexBuff[i] > '9') && 
							(hexBuff[i] < 'a' || hexBuff[i] > 'f') &&
							(hexBuff[i] < 'A' || hexBuff[i] > 'F'))
						{
							errorLocation = totalSizeBefore + off;
							error = JSONErrors::UnexpectedControl_Character;
							return string;
						}

					}

					char* _Eptr;
					const char _Ans = _CSTD strtol(hexBuff, &_Eptr, 16);

					if (hexBuff == _Eptr) {
						// invalid stoi argument
						// to prevent injection, skip
						stringStart += 4;
						break;
					}
					string += _Ans;
					stringStart += 4;
					break;
				}
				default:
					errorLocation = totalSizeBefore + off - 1;// -1 due to offset from   stringStart = _off + 1;
					error = JSONErrors::UnexpectedControl_Character;
					return string;
				}
			}
			if (off + 1 >= size)
			{
				string.append(buffer + stringStart, size - stringStart);
				readBuffer();
				stringStart = 0;
				continue;
			}
			off++;
		}
		if (off - stringStart)
		{
			string.append(buffer + stringStart, off - stringStart);
		}
		return string;
	}
	void RJSONStream::parseObject(JSONElement& object)
	{
		while (true)
		{
			if (!findEndOfWhitespace())
			{
				errorLocation = totalSizeBefore + off;
				error = JSONErrors::Unexpected_Character;
				return;
			}
			switch (buffer[off])
			{
			case '}':
				return;
			case ',':
				off++;
				if (!findEndOfWhitespace() || buffer[off] != '\"')
				{
					errorLocation = totalSizeBefore + off;
					error = JSONErrors::Unexpected_Character;
					return;
				}
				// buffer[_off] == '\"'
				/* fallthrough */
			case '\"':
			{
				JSONElement element;
				off++;
				element.name = parseString();
				off++;
				if (hasError())
				{
					return;
				}
				if (!findEndOfWhitespace())
				{
					errorLocation = totalSizeBefore + off;
					error = JSONErrors::Unexpected_Character;
					return;
				}
				if (buffer[off] != ':')
				{
					errorLocation = totalSizeBefore + off;
					error = JSONErrors::Unexpected_Character;
					return;
				}
				off++;
				parseValue(element);
				off++;
				if (hasError())
				{
					return;
				}
				object.children.emplace_back(element);
				break;
			}
			default:
				errorLocation = totalSizeBefore + off;
				error = JSONErrors::Unexpected_Character;
				return;
			}
		}
	}

	void RJSONStream::parseArray(JSONElement& object)
	{
		while (true)
		{
			if (!findEndOfWhitespace())
			{
				errorLocation = totalSizeBefore + off;
				error = JSONErrors::Unexpected_Character;
				return;
			}
			switch (buffer[off])
			{
			case ']':
				return;
			case ',':
				off++;
				if (!findEndOfWhitespace())
				{
					errorLocation = totalSizeBefore + off;
					error = JSONErrors::Unexpected_Character;
					return;
				}
				break;
			}
			JSONElement value;
			parseValue(value);
			off++;
			if (hasError())
			{
				return;
			}
			object.children.emplace_back(value);
		}
	}



	bool RJSONStream::readBuffer()
	{
		totalSizeBefore += size;
		fs.read(buffer, buffSize);
		size = fs.gcount();
		if (size)
		{// read something
			off = 0;
			return true;
		}
		return false;
	}

	bool RJSONStream::findEndOfWhitespace()
	{
		do
		{
			for (; off < size; off++)
			{
				if (buffer[off] != ' ' &&
					buffer[off] != '\t' &&
					buffer[off] != '\r' &&
					buffer[off] != '\n'
					)
				{
					return true;
				}
			}
		} while (readBuffer());
		return false;
	}

	bool RJSONStream::findEndOfNumber(size_t& start, std::string& value)
	{// "-0123456789.Ee+
		do
		{
			for (; off < size; off++)
			{
				if (buffer[off] < '0' || buffer[off] > '9' && 
					buffer[off] != '-' &&
					buffer[off] != '.' &&
					buffer[off] != 'E' &&
					buffer[off] != 'e' &&
					buffer[off] != '+'
					)
				{
					return true;
				}
				if (off - start)
				{
					//continue;
				}
				value.append(buffer + start, off - start);
				start = 0;
			}
		} while (readBuffer());
		return false;
	}




	JSONElement RJSON::load(const string& _jsonStructure)
	{
		if (_jsonStructure.empty())
		{
			JSONElement json;
			json.error = JSONErrors::JSONisEmpty;
			json.errorLocation = 0;
			return json;
		}
		try
		{
			size_t pos = 0;
			return parse(_jsonStructure, pos);
		}
		catch (const std::exception&)
		{
			JSONElement json;
			json.error = JSONErrors::UnhandledException;
			json.errorLocation = 0;
			return json;
		}
	}

	JSONElement RJSON::loadFile(const string& _path)
	{
		std::fstream fs(_path);
		return loadFile(fs);
	}

	JSONElement RJSON::loadFile(std::fstream& _fs)
	{
		if (!_fs.is_open())
		{
			JSONElement json;
			json.error = JSONErrors::FailedToOpenFile;
			return json;
		}
		size_t off = 0;
		return parseStream(_fs, off);
	}

	JSONElement RJSON::parseStream(std::fstream& _fs, size_t& _off)
	{
		const int whitespaceLen = strlen(JSONWhitespace);
		char buffer[buffSize];
		size_t size = 0; // <= buffSize
		size_t totalSizeBefore = 0; // total bytes read not including offset. totalOffset = totalSizeBefore + _off

		//
		// Lambdas
		//
		std::function<void(JSONElement&)> parseObject;
		std::function<void(JSONElement&)> parseArray;


		// reads data into the buffer from _fs and sets _off to zero and size to new size if successful
		// @returns true if data was read, false otherwise and sets size to zero
		auto readBuffer = [&buffer, &totalSizeBefore, &size, &_fs, &_off]() -> bool {
			totalSizeBefore += size;
			_fs.read(buffer, buffSize);
			size = _fs.gcount();
			if (size)
			{// read something
				_off = 0;
				return true;
			}
			return false;
			};

		// starts at _off and end _off at the non-whitespace character
		// returns false if whitespace has no end, true otherwise
		auto findEndOfWhiteSpace = [&readBuffer, &size, &_fs, &buffer, whitespaceLen, &_off]() -> bool {
			auto isWhitespace = [&whitespaceLen, &buffer, &_off]() -> bool {
				for (size_t j = 0; j < whitespaceLen; j++)
				{
					if (buffer[_off] == JSONWhitespace[j])
					{
						return true;
					}
				}
				return false;
				};
			do
			{
				for (; _off < size; _off++)
				{
					if (!isWhitespace())
					{
						return true;
					}
				}
			} while (readBuffer());
			return false;
			};

		// parse string
		// expects _off to be after string start ("\"")
		// _off will be at ending "\"" when return unless error
		// @param json is used for setting errors
		// @returns string
		auto parseString = [&_off, &size, &buffer, &readBuffer, &totalSizeBefore](JSONElement& json) -> string {
			string string;
			size_t stringStart = _off;
			if (_off + 1 >= size)
			{
				readBuffer();
				stringStart = 0;
			}
			for (; _off < size;)
			{
				if (buffer[_off] == '\"')
				{// found end of string
					break;
				}
				// control character handling
				if (buffer[_off] == '\\')
				{// parse control characters
					// Add string found from stringStart to _off
					size_t stringSize = string.size();
					string.append(buffer + stringStart, _off - stringStart);
					if (++_off == size)
					{
						readBuffer();
					}
					stringStart = _off + 1;
					switch (buffer[_off])
					{
					case '\"':
						string += '\"';
						break;
					case '\\':
						string += '\\';
						break;
					case '/':
						string += '/';
						break;
					case 'b':
						string += '\b';
						break;
					case 'f':
						string += '\f';
						break;
					case 'n':
						string += '\n';
						break;
					case 'r':
						string += '\r';
						break;
					case 't':
						string += '\t';
						break;
					case 'u':
					{
						if (_off + 4 >= size)
						{
							stringStart = 1;// set to one to be set to zero below
							readBuffer();
							if (size <= 4)
							{
								json.errorLocation = totalSizeBefore + _off - 1;// -1 due to offset from   stringStart = _off + 1;
								json.error = JSONErrors::UnexpectedControl_Character;
								return string;
							}
						}
						char hexBuff[5]{ 0 };
						stringStart--;
						memcpy_s(
							hexBuff,
							sizeof(hexBuff),
							buffer + stringStart * sizeof(char),
							4);

						char* _Eptr;
						const char _Ans = _CSTD strtol(hexBuff, &_Eptr, 16);

						if (hexBuff == _Eptr) {
							// invalid stoi argument
							// to prevent injection, skip
							stringStart += 4;
							break;
						}
						string += _Ans;
						stringStart += 4;
						break;
					}
					default:
						json.errorLocation = totalSizeBefore + _off - 1;// -1 due to offset from   stringStart = _off + 1;
						json.error = JSONErrors::UnexpectedControl_Character;
						return string;
					}
				}
				if (_off + 1 >= size)
				{
					string.append(buffer + stringStart, size - stringStart);
					readBuffer();
					stringStart = 0;
					continue;
				}
				_off++;
			}
			if (_off - stringStart)
			{
				string.append(buffer + stringStart, _off - stringStart);
			}
			return string;
			};

		
		std::function<JSONElement()> parseJSON;
		// expects _off to be after ':'
		// _off will be at end of value e.g '}' or '\"'
		std::function<void(JSONElement&)> parseValue = [&parseArray, &readBuffer, &parseJSON, &parseObject, &parseString, &findEndOfWhiteSpace, &buffer, &size, &_off, &totalSizeBefore](JSONElement& json) -> void {
			if (!findEndOfWhiteSpace())
			{
				json.errorLocation = totalSizeBefore + _off - 1;
				json.error = JSONErrors::UnexpectedControl_Character;
				return;
			}
			switch (buffer[_off])
			{
			case '\"':
				json.type = JSONTypes::String;
				_off++;
				json.value = parseString(json);
				break;
			case '-':
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
			{
				json.type = JSONTypes::Integer;
				// parse number
				// find first of not a number
				size_t start = _off;
				auto findEndOfNumber = [&start, &readBuffer, &_off, &size, &buffer, &json]() {
					constexpr const char* numberSymbols = "-0123456789.Ee+";
					auto isNumber= [&numberSymbols, &buffer, &_off]() -> bool {
						for (size_t j = 0; j < strlen(numberSymbols); j++)
						{
							if (buffer[_off] == numberSymbols[j])
							{
								return true;
							}
						}
						return false;
						};
					do
					{
						for (; _off < size; _off++)
						{
							if (!isNumber())
							{
								return true;
							}
						}
						if (_off - start)
						{
							//continue;
						}
						json.value.append(buffer + start, _off - start);
						start = 0;
					} while (readBuffer());
					return false;
					};

				if (!findEndOfNumber())
				{
					json.errorLocation = totalSizeBefore + _off - 1;
					json.error = JSONErrors::UnexpectedEndOfValue;
					return;
				}
				json.value.append(buffer + start, _off - start);
				break;
			}	
			case '{':
			{
				json.type = JSONTypes::Object;
				_off++;
				//JSONElement object;
				parseObject(json);
				//json.children.emplace_back(object);
				break;
			}
			case '[':
			{
				json.type = JSONTypes::Array;
				_off++;
				parseArray(json);
				break;
			}
			case 't':
			{
				json.type = JSONTypes::Boolean;
				size_t start = _off;
				if (_off + 3 >= size)
				{
					json.value.append(buffer + start, size - _off);
					readBuffer();
					start = 0;
					if (_off + 4 + json.value.size() >= size)
					{
						json.errorLocation = totalSizeBefore + _off - 1;
						json.error = JSONErrors::Unexpected_Character;
						return;
					}
				}
				if (json.value.size() - 4)
				{
					json.value.append(buffer + start, 4 - json.value.size());
				}
				if (json.value != "true")
				{
					json.errorLocation = totalSizeBefore + _off - 1;
					json.error = JSONErrors::Unexpected_Character;
					return;
				}
				_off += 4;
				break;
			}
			case 'f':
			{
				json.type = JSONTypes::Boolean;
				size_t start = _off;
				if (_off + 4 >= size)
				{
					json.value.append(buffer + start, size - _off);
					readBuffer();
					if (_off + 5 + json.value.size() >= size)
					{
						json.errorLocation = totalSizeBefore + _off - 1;
						json.error = JSONErrors::Unexpected_Character;
						return;
					}
				}
				if (json.value.size() - 5)
				{
					json.value.append(buffer + start, json.value.size() - 5);
				}

				if (json.value != "false")
				{
					json.errorLocation = totalSizeBefore + _off - 1;
					json.error = JSONErrors::Unexpected_Character;
					return;
				}
				_off += 5;
				break;
			}				
			case 'n':
			{
				json.type = JSONTypes::Boolean;
				size_t start = _off;
				if (_off + 3 >= size)
				{
					json.value.append(buffer + start, size - _off);
					readBuffer();
					if (_off + 4 + json.value.size() >= size)
					{
						json.errorLocation = totalSizeBefore + _off - 1;
						json.error = JSONErrors::Unexpected_Character;
						return;
					}
				}
				if (json.value.size() - 4)
				{
					json.value.append(buffer + start, json.value.size() - 4);
				}
				if (json.value != "null")
				{
					json.errorLocation = totalSizeBefore + _off - 1;
					json.error = JSONErrors::Unexpected_Character;
					return;
				}
				_off += 4;
				break;
			}
			default:
				json.errorLocation = totalSizeBefore + _off - 1;
				json.error = JSONErrors::Unexpected_Character;
				return;
			}
			};

		// expects _off to be after '{'
		// _off will be at '}' unless error
		parseObject = [&parseValue, &findEndOfWhiteSpace, &totalSizeBefore, &_off, &buffer, &parseString](JSONElement& object) -> void {
			while (true)
			{
				if (!findEndOfWhiteSpace())
				{
					object.errorLocation = totalSizeBefore + _off;
					object.error = JSONErrors::Unexpected_Character;
					return;
				}
				switch (buffer[_off])
				{
				case '}':
					return;
				case ',':
					_off++;
					if (!findEndOfWhiteSpace() || buffer[_off] != '\"')
					{
						object.errorLocation = totalSizeBefore + _off;
						object.error = JSONErrors::Unexpected_Character;
						return;
					}
					// buffer[_off] == '\"'
				case '\"':
				{
					JSONElement element;
					_off++;
					element.name = parseString(object);
					_off++;
					if (object.hasError())
					{
						return;
					}
					if (!findEndOfWhiteSpace())
					{
						object.errorLocation = totalSizeBefore + _off;
						object.error = JSONErrors::Unexpected_Character;
						return;
					}
					if (buffer[_off] != ':')
					{
						object.errorLocation = totalSizeBefore + _off;
						object.error = JSONErrors::Unexpected_Character;
						return;
					}
					_off++;
					parseValue(element);
					_off++;
					if (element.hasError())
					{
						object.copyError(element);
						return;
					}
					object.children.emplace_back(element);
					break;
				}
				default:
					object.errorLocation = totalSizeBefore + _off;
					object.error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			};

		// expects _off to be after '['
		// _off will be at ']' unless error
		parseArray = [&parseValue, &findEndOfWhiteSpace, &totalSizeBefore, &_off, &buffer, &parseString](JSONElement& object) -> void {
			while (true)
			{
				if (!findEndOfWhiteSpace())
				{
					object.errorLocation = totalSizeBefore + _off;
					object.error = JSONErrors::Unexpected_Character;
					return;
				}
				switch (buffer[_off])
				{
				case ']':
					return;
				case ',':
					_off++;
					if (!findEndOfWhiteSpace())
					{
						object.errorLocation = totalSizeBefore + _off;
						object.error = JSONErrors::Unexpected_Character;
						return;
					}
					break;
				}
				JSONElement value;
				parseValue(value);
				_off++;
				if (value.hasError())
				{
					object.copyError(value);
					return;
				}
				object.children.emplace_back(value);
			}
			};

		// parses json
		parseJSON = [&parseArray, &parseObject, &parseValue, &parseString, &readBuffer, &findEndOfWhiteSpace , &buffer, &totalSizeBefore, &_off, &size]() -> JSONElement {
			JSONElement json;

			if (!findEndOfWhiteSpace())
			{
				return json;
			}
			for (; _off < size; )
			{
				if (!findEndOfWhiteSpace())
				{
					return json;
				}
				switch (buffer[_off])
				{
				case '{':
					json.type = JSONTypes::Object;
					_off++;
					parseObject(json);
					return json;
				case '[':
					json.type = JSONTypes::Array;
					_off++;
					parseArray(json);
					return json;

					break;
				default:
					json.errorLocation = totalSizeBefore + _off;
					json.error = JSONErrors::Unexpected_Character;
					return json;
				}

				if (_off + 1 >= size)
				{
					readBuffer();
					continue;
				}
				_off++;
			}
			};

		//
		// start
		//
		
		

		return parseJSON();
	}


	JSONElement RJSON::parse(const string& _data, size_t& _off)
	{
		JSONElement elem;
		AfterWhiteSpace;

		switch (_data[_off])
		{
		case '{':
			_off++;
			AfterWhiteSpace;
			while (_data[_off] != '}' && _data[_off] != ']')
			{
				elem.type = JSONTypes::Object;
				elem.children.emplace_back(parse(_data, _off));
				if (elem.children.back().hasError())
				{// error
					elem.copyError(elem.children.back());
					return elem;
				}
				
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
				if (elem.children.back().hasError())
				{
					elem.copyError(elem.children.back());
					return elem;
				}
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
			if (elem.children.back().hasError())
			{// error
				elem.copyError(elem.children.back());
				return elem;
			}
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
				if (elem.children.back().hasError())
				{// error
					elem.copyError(elem.children.back());
					return elem;
				}
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
			if (elem.hasError())
			{// error
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
					/*fallthrough*/
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

	string RJSON::parseString(JSONElement& _elem, const string& _data, size_t& _off)
	{
		string name;
	parseName:
		size_t start = _off + 1;
		_off = _data.find_first_of("\\\"", _off + 1);
		if (_off == string::npos) {
			_elem.error = JSONErrors::UnexpectedEndOfString;
			_elem.errorLocation = start;
			return string();
		}
		
		if (_data[_off] == '\\')
		{// control character
			name += _data.substr(start, _off - start);
			_off++;
			if (_off == string::npos) {
				_elem.error = JSONErrors::UnexpectedEndOfString;
				_elem.errorLocation = start;
				return string();
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
				return string();
			}
			goto parseName;
		}
		else if (name.empty())
		{
			name = _data.substr(start, _off - start);
		}

		return name;
	}
	
	void RJSON::parseValue(JSONElement& _elem, const string& _data, size_t& _off)
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
			_off = _data.find_first_of(string(JSONWhitespace) + ",}]", _off);
			if (_off == string::npos) {
				_elem.errorLocation = _off;
				_elem.error = JSONErrors::UnexpectedEndOfString;
				return;
			}
			_elem.value = _data.substr(start, _off - start);

			if (_elem.value.find('.') != string::npos)
			{
				_elem.type = JSONTypes::Float;
				break;
			}
			_elem.type = JSONTypes::Integer;
			break;
		case 't':
		case 'f':
			_off = _data.find_first_of(string(JSONWhitespace) + ",}]", _off);
			if (_off == string::npos) return;
			_elem.value = _data.substr(start, _off - start);
			_elem.type = JSONTypes::Boolean;
			break;
		case 'n':
			_off = _data.find_first_of(string(JSONWhitespace) + ",}]", _off);
			if (_off == string::npos) return;
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
