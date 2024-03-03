#include "RJSON.h"

#ifdef __RJSON__
namespace RJSON
{
	JSONElement RJSON::EmptyElem;
	const char* RJSON::JSONWhitespace = " \t\n\r";

	JSONElement::JSONElement() {}

	JSONElement::JSONElement(const JSONElement& _elem)
		:
		children(_elem.children),
		error(_elem.error)
	{
		setType(_elem.getType());
		setName(_elem.getName());
		data = _elem.data;
	}
	

	//JSONElement::JSONElement(std::initializer_list<JSONElement> _json)
	//{
	//	_json.size();
	//}

	JSONElement::JSONElement(long long _val) {
		setValue(_val);
		setType(JSONTypes::Integer);
	}

	JSONElement::JSONElement(long double _val) {
		setValue(_val);
		setType(JSONTypes::Float);
	}

	JSONElement::JSONElement(bool _val) {
		setType(JSONTypes::Boolean);
		setValue(_val);
	}

	JSONElement::JSONElement(const JSONElementArray& _elements) : children(_elements) {
		setType(JSONTypes::Object);
	}

	JSONElement::JSONElement(const char* _name, JSONType _type)
	{
		setType(_type);
		setName(_name);
	}


	JSONElement::JSONElement(const char* _name, long long _val) {
		setName(_name);
		setValue(_val);
		setType(JSONTypes::Integer);
	}

	JSONElement::JSONElement(const char* _name, long double _val) {
		setName(_name);
		setValue(_val);
		setType(JSONTypes::Float);
	}

	JSONElement::JSONElement(const char* _name, bool _val) {
		setName(_name);
		setType(JSONTypes::Boolean);
		setValue(_val);
	}

	JSONElement::JSONElement(const char* _name, JSONElementArray _json, JSONType _type) :
		children(_json)
	{
		setType(_type);
		setName(_name);
	}

	JSONElement::JSONElement(const char* _name, const string& _val)
	{
		setName(_name);
		setValue(_val.c_str());
		setType(JSONTypes::String);
	}

	JSONElement::~JSONElement()
	{

	}


	void JSONElement::copyError(const JSONElement& _elem)
	{
		error = _elem.error;
	}

	bool JSONElement::hasError() const
	{
		return error.error != JSONErrors::OK;
	}
	string JSONElement::getErrorText() const
	{
		string text;
		switch (error.error)
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
		text += std::to_string(error.location) + ".";
		return text;
	}

	JSONElement& JSONElement::get(const char* _name)
	{
		for (JSONElement& elem : children)
			if (elem.getName() == _name)
				return elem;
		RJSON::EmptyElem.error = error;
		RJSON::EmptyElem.error.location = error.location;
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

	JSONElementArrayPTR JSONElement::getAll(const char* _name)
	{
		JSONElementArrayPTR elements;
		for (size_t i = 0; i < children.size(); i++)
		{
			elements += children[i].getAll(_name);
			if (!children.empty())
				children[i].getAll(_name);
			if (children[i].getName() == _name)
			{
				elements.emplace_back(&children[i]);
			}
		}
		return elements;
	}

	JSONElement& JSONElement::addChild(const char* _name, const string& _value, JSONType _type)
	{
		JSONElement& elem = children.emplace_back();
		elem.setName(_name);
		elem.setValue(_value.c_str());
		elem.setType(_type);
		return children.back();
	}
	
	JSONElement& JSONElement::addChild(const char* _name, const char* _value, JSONType _type)
	{
		JSONElement elem;
		elem.setName(_name);
		elem.setValue(_value);
		elem.setType(_type);
		children.emplace_back(elem);
		return children.back();
	}

	JSONElement& JSONElement::addChild(const char* _name, size_t _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const char* _name, int _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const char* _name, unsigned int _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const char* _name, long long _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Integer);
	}

	JSONElement& JSONElement::addChild(const char* _name, double _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Float);
	}
	
	JSONElement& JSONElement::addChild(const char* _name, long double _value)
	{
		return addChild(_name, std::to_string(_value), JSONType::Float);
	}

	JSONElement& JSONElement::addChild(const char* _name, bool _value)
	{
		if (_value)
		{
			return addChild(_name, "true", JSONType::Boolean);
		}
		return addChild(_name, "false", JSONType::Boolean);
	}

	JSONElement& JSONElement::addChild(const char* _name, const std::vector<JSONElement>& _elements)
	{
		JSONElement& elem = children.emplace_back(_name);
		elem.children = _elements;
		elem.setType(JSONType::Array);
		return elem;
	}

	JSONElement& JSONElement::addChild(const char* _name, const stringArray& _array)
	{
		JSONElement& elem = children.emplace_back(_name);
		for (auto& i : _array)
		{
			JSONElement& element = elem.children.emplace_back();
			element.setType(JSONType::String);
			element.setValue(i.c_str());
		}
		elem.setType(JSONType::Array);
		return elem;
	}

	JSONElement& JSONElement::addChild(const char* _name, const std::vector<long long>& _array)
	{
		JSONElement& elem = children.emplace_back();
		elem.setName(_name);
		for (auto& i : _array)
		{
			JSONElement& element = elem.children.emplace_back();
			element.setType(JSONType::Integer);
			element.setValue(i);
		}
		elem.setType(JSONType::Array);
		return elem;
	}

	JSONElement& JSONElement::addChild(const char* _name, const std::vector<int>& _array)
	{
		JSONElement elem = children.emplace_back();
		elem.setName(_name);
		for (auto& i : _array)
		{
			JSONElement& element = elem.children.emplace_back();
			element.setType(JSONType::Integer);
			element.setValue((long long)i);
		}
		elem.setType(JSONType::Array);
		return elem;
	}

	JSONElement& JSONElement::addChild(const char* _name, const std::vector<long double>& _array)
	{
		JSONElement& elem = children.emplace_back();
		elem.setName(_name);
		for (auto& i : _array)
		{
			JSONElement element = elem.children.emplace_back();
			element.setType(JSONType::Float);
			element.setValue(i);
		}
		elem.setType(JSONType::Array);
		return elem;
	}

	JSONElement& JSONElement::addChild(const char* _name, const std::vector<bool>& _array)
	{
		JSONElement& elem = children.emplace_back();
		elem.setName(_name);
		for (auto i : _array)
		{
			JSONElement element = elem.children.emplace_back();
			element.setType(JSONType::Boolean);
			element.setValue(i);
		}
		elem.setType(JSONType::Array);
		return elem;
	}

	JSONElement& JSONElement::addChild(const JSONElement& _jsonElement)
	{
		return children.emplace_back(_jsonElement);
	}

	bool JSONElement::removeChild(const char* _name)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i].getName() == _name)
			{
				children.erase(children.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool JSONElement::hasChild(const char* _name) const
	{
		for (const JSONElement& elem : children)
			if (elem.getName() == _name)
				return true;
		return false;
	}

	bool JSONElement::exists() const
	{
		return hasName() || hasValue() || !children.empty();
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
		if (!hasName())
		{
			return asJSONInner();
		}
		/*if (getType() == JSONTypes::Array)
		{
			return "[" + asJSONInner() + "]";
		}*/
		return "{" +  asJSONInner() + "}";
	}

	
	string JSONElement::asJSONFormatted(string& _indent, string _whitespace) const
	{
		if (!hasName())
		{
			return asJSONInnerFormatted(_indent, _whitespace);
		}
		_indent += _whitespace;
		if (getType() == JSONTypes::Array)
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
		if (hasName())
		{
			json += _indent + "\"" + getName() + "\": ";
		}
		if (getType() == JSONTypes::Array)
		{
			json += "[\n";
			_indent += _whitespace;
		}
		else
			if (getType() == JSONTypes::Object)
			{
				json += "{\n";
				_indent += _whitespace;
			}
			else
			{
				json += valueAsFormattedString();
				return json;
			}
		for (size_t i = 0; i < children.size();)
		{
			if (getType() == JSONTypes::Array)
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
		if (getType() == JSONTypes::Array)
		{
			_indent.pop_back();
			json += _indent + ']';
		}
		if (getType() == JSONTypes::Object)
		{
			_indent.pop_back();
			json += _indent + '}';
		}
		return json;
	}

	string JSONElement::asJSONInner() const
	{
		string json;
		if (hasName())
		{
			json += std::string("\"") + getName() + "\":";
		}
		if (getType() == JSONTypes::Array)
		{
			json += "[";
		}
		else
		if (getType() == JSONTypes::Object)
		{
			json += "{";
		}
		else
		{
			json += valueAsFormattedString();
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
		if (getType() == JSONTypes::Array)
		{
			json += "]";
		}
		if (getType() == JSONTypes::Object)
		{
			json += "}";
		}
		return json;
	}

	string JSONElement::valueAsFormattedString() const
	{// check for control characters
		switch (getType())
		{
		case JSONTypes::Integer:
			return std::to_string(valueAsInt());
		case JSONTypes::Float:
			return std::to_string(valueAsFloat());
		case JSONTypes::Boolean:
			if (valueAsBool())
			{
				return "true";
			}
			return "false";
		case JSONTypes::Null:
			return "null";
		case JSONTypes::String:
		default:
			/* continue below */
			break;
		}
		std::string value = valueAsString();
		for (size_t i = 0; i < value.size(); i++)
		{
			if (value[i] == '"' ||
				//valueModified[i] == '/' ||
				value[i] == '\\' ||
				value[i] == '\b' ||
				value[i] == '\f' ||
				value[i] == '\n' ||
				value[i] == '\r' ||
				value[i] == '\t'
				)
			{
				switch (value[i])
				{
				case '\b':
					value[i] = 'b';
					break;
				case '\f':
					value[i] = 'f';
					break;
				case '\n':
					value[i] = 'n';
					break;
				case '\r':
					value[i] = 'r';
					break;
				case '\t':
					value[i] = 't';
					break;
				}
				value.insert(value.begin() + i, '\\');
				i++;
				continue;
			}
		}
		return "\"" + value + "\"";
	}

	JSONType JSONElement::getType() const
	{
		return data.getType();
	}

	void JSONElement::setType(JSONType _type)
	{
		data.setType(_type);
	}

	void JSONElement::autoType()
	{
		return;
		//if (!children.empty())
		//{
		//	if (children.front().getName())
		//	{
		//		setType(JSONTypes::Array;
		//	}
		//	else
		//		setType(JSONTypes::Object;
		//	return;
		//}

		//if (!hasValue())
		//{// The type could be anything 
		//	if (!hasName())
		//	{
		//		setType(JSONTypes::Unknown;
		//	}
		//	else
		//		setType(JSONTypes::String;
		//	return;
		//}
		//if (value.find_first_not_of("-1234567890.") == string::npos)// e and E are not included here
		//{// Number
		//	if (value.find('.') != string::npos)
		//	{// Float
		//		setType(JSONTypes::Float;
		//	}
		//	setType(JSONTypes::Integer;
		//	return;
		//}
		//if (value == "true" || value == "false")
		//{// Boolean
		//	setType(JSONTypes::Null;
		//	return;
		//}
		//if (value == "null")
		//{
		//	setType(JSONTypes::Null;
		//	return;
		//}
		//// String
		//setType(JSONTypes::String;
		//return;
	}

	long long JSONElement::valueAsInt() const
	{
		return data.getInteger();
	}

	long double JSONElement::valueAsFloat() const
	{
		return data.getDouble();
	}

	string JSONElement::valueAsString() const
	{
		return data.getString();
	}

	bool JSONElement::valueAsBool() const
	{
		return data.getBool();
	}

	stringArray JSONElement::asArray() const
	{
		stringArray arr;
		for (auto& i : children)
		{
			arr.emplace_back(i.valueAsString());
		}
		return arr;
	}

	void JSONElement::fixValue()
	{
		if (getType() != JSONTypes::String && getType() != JSONTypes::Unknown)
		{
			return;
		}
		//for (size_t i = value.find_first_of("\\\"/"); i < length; i++)
		//{

		//}



	}

	bool JSONElement::isInteger()
	{
		return getType() == JSONTypes::Integer;
	}

	bool JSONElement::isFloat()
	{
		return getType() == JSONTypes::Float;
	}

	bool JSONElement::isString()
	{
		return getType() == JSONTypes::String;
	}

	bool JSONElement::isBool()
	{
		return getType() == JSONTypes::Boolean;
	}

	bool JSONElement::isNull()
	{
		return getType() == JSONTypes::Null;
	}

	bool JSONElement::isObject()
	{
		return getType() == JSONTypes::Object;
	}

	bool JSONElement::isArray()
	{
		return false;
	}

	JSONElement& JSONElement::operator[](const size_t _index)
	{
		return get(_index);
	}

	JSONElement& JSONElement::operator[](const char* _name)
	{
		return get(_name);
	}

	JSONElement& JSONElement::operator=(size_t _value)
	{
		setValue((long long)_value);
		setType(JSONTypes::Integer);
		return *this;
	}

	JSONElement& JSONElement::operator=(long double _value)
	{
		setValue(_value);
		setType(JSONTypes::Float);
		return *this;
	}

	JSONElement& JSONElement::operator=(bool _value)
	{
		setType(JSONTypes::Boolean);
		setValue(_value);
		return *this;
	}

	JSONElement& JSONElement::operator=(const char* _value)
	{
		setValue(_value);
		setType(JSONTypes::String);
		return *this;
	}

	JSONElement& JSONElement::operator=(const JSONElement& _json)
	{
		setName(_json.getName());
		data = _json.data;
		children = _json.children;
		setType(_json.getType());

		error = _json.error;
		error.location = _json.error.location;
		return *this;
	}

	bool JSONElement::hasName() const
	{
		if (data.getName() == nullptr)
		{
			return false;
		}
		return strlen(data.getName());
	}

	bool JSONElement::hasValue() const
	{
		return children.empty();
	}

	void JSONElement::setName(const char* _name)
	{
		data.setName(_name);
	}


	const char* JSONElement::getName() const
	{
		return data.getName();
	}

	void JSONElement::setValue(const char* _str)
	{
		data.setString(_str);
	}

	void JSONElement::setValue(const HeapString& _str)
	{
		data.setString(_str);
	}

	void JSONElement::setValue(long long _integer)
	{
		data.setInteger(_integer);
	}

	void JSONElement::setValue(bool _bool)
	{
		data.setBool(_bool);
	}

	void JSONElement::setValue(long double _bool)
	{
		data.setDouble(_bool);
	}

	void JSONElement::setValueNull()
	{
		data.setNull();
	}

	const char* JSONElement::getValuePtr()
	{
		return data.getStringPtr();
	}

	HeapString& JSONElement::getValueHeapStr()
	{
		return data.getHeapString();
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
			elem.error.error = JSONErrors::FailedToOpenFile;
			return elem;
		}
		elem = parseStream();
		if (hasError())
		{
			elem.error.error = error.error;
			elem.error.location = error.location;
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
				json.setType(JSONTypes::Object);
				off++;
				parseObject(json);
				return json;
			case '[':
				json.setType(JSONTypes::Array);
				off++;
				parseArray(json);
				return json;
			default:
				json.error.location = totalSizeBefore + off;
				json.error.error = JSONErrors::Unexpected_Character;
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
		return error.error;
	}

	bool RJSONStream::hasError()
	{
		return error.error != JSONErrors::OK;
	}

	void RJSONStream::parseValue(JSONElement& json)
	{
		if (!findEndOfWhitespace())
		{
			error.location = totalSizeBefore + off - 1;
			error.error = JSONErrors::UnexpectedControl_Character;
			return;
		}
		switch (buffer[off])
		{
		case '\"':
			json.setType(JSONTypes::String);
			off++;
			json.setValue(parseString());
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
			json.setType(JSONTypes::Integer);
			// parse number
			// find first of not a number
			size_t start = off;
			if (!findEndOfNumber(start, json.getValueHeapStr()))
			{
				error.location = totalSizeBefore + off - 1;
				error.error = JSONErrors::UnexpectedEndOfValue;
				return;
			}
			json.getValueHeapStr().append(buffer + start, off - start);
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
			json.setType(JSONTypes::Array);
			off++;
			parseArray(json);
			return;
		}
		case 't':
		{
			json.setType(JSONTypes::Boolean);
			size_t start = off;
			if (off + 3 >= size)
			{
				json.getValueHeapStr().append(buffer + start, size - off);
				readBuffer();
				start = 0;
				if (off + 4 + json.getValueHeapStr().size() >= size)
				{
					error.location = totalSizeBefore + off - 1;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			if (json.getValueHeapStr().size() - 4)
			{
				json.getValueHeapStr().append(buffer + start, 4 - json.getValueHeapStr().size());
			}
			if (json.getValueHeapStr() != "true")
			{
				error.location = totalSizeBefore + off - 1;
				error.error = JSONErrors::Unexpected_Character;
				return;
			}
			off += 4;
			return;
		}
		case 'f':
		{
			json.setType(JSONTypes::Boolean);
			size_t start = off;
			if (off + 4 >= size)
			{
				json.getValueHeapStr().append(buffer + start, size - off);
				readBuffer();
				if (off + 5 + json.getValueHeapStr().size() >= size)
				{
					error.location = totalSizeBefore + off - 1;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			if (json.getValueHeapStr().size() - 5)
			{
				json.getValueHeapStr().append(buffer + start, json.getValueHeapStr().size() - 5);
			}

			if (json.getValueHeapStr() != "false")
			{
				error.location = totalSizeBefore + off - 1;
				error.error = JSONErrors::Unexpected_Character;
				return;
			}
			off += 5;
			break;
		}
		case 'n':
		{
			json.setType(JSONTypes::Boolean);
			size_t start = off;
			if (off + 3 >= size)
			{
				json.getValueHeapStr().append(buffer + start, size - off);
				readBuffer();
				if (off + 4 + json.getValueHeapStr().size() >= size)
				{
					error.location = totalSizeBefore + off - 1;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
			}
			if (json.getValueHeapStr().size() - 4)
			{
				json.getValueHeapStr().append(buffer + start, json.getValueHeapStr().size() - 4);
			}
			if (json.getValueHeapStr() != "null")
			{
				error.location = totalSizeBefore + off - 1;
				error.error = JSONErrors::Unexpected_Character;
				return;
			}
			off += 4;
			break;
		}
		default:
			error.location = totalSizeBefore + off - 1;
			error.error = JSONErrors::Unexpected_Character;
			return;
		}
	};

	HeapString RJSONStream::parseString()
	{
		HeapString string;
		size_t stringStart = off;
		if (off + 1 > size)
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
					string.append('\"');
					break;
				case '\\':
					string.append('\\');
					break;
				case '/':
					string.append('/');
					break;
				case 'b':
					string.append('\b');
					break;
				case 'f':
					string.append('\f');
					break;
				case 'n':
					string.append('\n');
					break;
				case 'r':
					string.append('\r');
					break;
				case 't':
					string.append('\t');
					break;
				case 'u':
				{
					if (off + 4 >= size)
					{
						stringStart = 1;// set to one to be set to zero below
						readBuffer();
						if (size <= 4)
						{
							error.location = totalSizeBefore + off - 1;// -1 due to offset from   stringStart = _off + 1;
							error.error = JSONErrors::UnexpectedControl_Character;
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
							error.location = totalSizeBefore + off;
							error.error = JSONErrors::UnexpectedControl_Character;
							return string.getPtr();
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
					string.append(_Ans);
					stringStart += 4;
					break;
				}
				default:
					error.location = totalSizeBefore + off - 1;// -1 due to offset from   stringStart = _off + 1;
					error.error = JSONErrors::UnexpectedControl_Character;
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
				error.location = totalSizeBefore + off;
				error.error = JSONErrors::Unexpected_Character;
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
					error.location = totalSizeBefore + off;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
				// buffer[_off] == '\"'
				/* fallthrough */
			case '\"':
			{
				off++;
				object.children.emplace_back(parseString());
				off++;
				if (hasError())
				{
					return;
				}
				if (!findEndOfWhitespace())
				{
					error.location = totalSizeBefore + off;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
				if (buffer[off] != ':')
				{
					error.location = totalSizeBefore + off;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
				off++;
				parseValue(object.children.back());
				off++;
				if (hasError())
				{
					return;
				}
				break;
			}
			default:
				error.location = totalSizeBefore + off;
				error.error = JSONErrors::Unexpected_Character;
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
				error.location = totalSizeBefore + off;
				error.error = JSONErrors::Unexpected_Character;
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
					error.location = totalSizeBefore + off;
					error.error = JSONErrors::Unexpected_Character;
					return;
				}
				break;
			}
			object.children.emplace_back();
			parseValue(object.children.back());
			off++;
			if (hasError())
			{
				return;
			}
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

	bool RJSONStream::findEndOfNumber(size_t& start, HeapString& value)
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
			json.error.error = JSONErrors::JSONisEmpty;
			json.error.location = 0;
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
			json.error.error = JSONErrors::UnhandledException;
			json.error.location = 0;
			return json;
		}
	}

	JSONElement RJSON::loadFile(const string& _path)
	{
		return parseStream(_path.c_str());
	}

	JSONElement RJSON::parseStream(const char* _path)
	{
		RJSONStream stream;
		return stream.parseStream(_path);
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
				elem.setType(JSONTypes::Object);
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
			elem.setType(JSONTypes::Array);
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
		while (elem.getType() == JSONTypes::Array)
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


			elem.children.emplace_back();
			parseValue(elem.children.back(), _data, _off);
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
			elem.setName(parseString(elem, _data, _off).c_str());
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
					elem.setType(JSONTypes::Object);
					/*fallthrough*/
				case '[':
					if (_data[_off] == '[')
					{
						elem.setType(JSONTypes::Array);
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
				elem.error.error = JSONErrors::MissingColon;
				elem.error.location = _off;
				return elem;
				break;
			}
			break;
		case '}':
		case ']':
				break;
		default:
			elem.error.error = JSONErrors::Unexpected_Character;
			elem.error.location = _off;
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
			_elem.error.error = JSONErrors::UnexpectedEndOfString;
			_elem.error.location = start;
			return string();
		}
		
		if (_data[_off] == '\\')
		{// control character
			name += _data.substr(start, _off - start);
			_off++;
			if (_off == string::npos) {
				_elem.error.error = JSONErrors::UnexpectedEndOfString;
				_elem.error.location = start;
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
				_elem.error.error = JSONErrors::UnexpectedControl_Character;
				_elem.error.location = _off;
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
			_elem.setValue(parseString(_elem, _data, _off).c_str());
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
				_elem.error.location = _off;
				_elem.error.error = JSONErrors::UnexpectedEndOfString;
				return;
			}
			{
				std::string num = _data.substr(start, _off - start);


				if (num.find('.') != string::npos)
				{
					_elem.setType(JSONTypes::Float);
					_elem.setValue(strtold(num.c_str(), nullptr));
					break;
				}
				_elem.setValue(strtoll(num.c_str(), nullptr, 10));
				_elem.setType(JSONTypes::Integer);
				break;
			}
		case 't':
		case 'f':
		{
			_off = _data.find_first_of(string(JSONWhitespace) + ",}]", _off);
			if (_off == string::npos) return;
			std::string value = _data.substr(start, _off - start);
			if (value == "true")
			{
				_elem.setValue(true);
			}
			else
			{
				_elem.setValue(true);

			}
			_elem.setType(JSONTypes::Boolean);
			break;
		}
		case 'n':
			_off = _data.find_first_of(string(JSONWhitespace) + ",}]", _off);
			if (_off == string::npos) return;
			_elem.setValueNull();
			_elem.setType(JSONTypes::Null);
			break;
		default:
			_elem.error.error = JSONErrors::Unexpected_Character;
			_elem.error.location = _off;
			break;
		}
	}

	JSONData::~JSONData()
	{
		if ((__int8)type & (__int8)JSONTypes::_deleteArray)
		{
			delete array;
		}
	}

	JSONType JSONData::getType() const
	{
		return type;
	}

	void JSONData::setType(JSONType _type)
	{
		if ((__int8)type & (__int8)JSONTypes::_deleteArray && _type != JSONTypes::_deleteArray)
		{
			delete array;
		}
		if (_type == JSONTypes::_deleteArray)
		{
			_type = (JSONType)((__int8)JSONTypes::_deleteArray | (__int8)JSONTypes::Array);
			return;
		}
		if (type == JSONType::String && _type != JSONType::String)
		{
			string.free();
		}
		type = _type;
	}

	const char* JSONData::getName() const
	{
		return name.getPtr();
	}

	void JSONData::setName(const char* _name)
	{
		if (!_name)
		{
			return;
		}
		name.setStr(_name);
	}

	HeapString& JSONData::getHeapString()
	{
		return string;
	}

	std::string JSONData::getString() const
	{
		switch (type)
		{
		case JSONTypes::Integer:
			return std::to_string(getInteger());
		case JSONTypes::Float:
			return std::to_string(getDouble());
		case JSONTypes::String:
			return std::string(string);
		case JSONTypes::Boolean:
			if (getBool())
			{
				return "true";
			}
			return "false";
		case JSONTypes::Null:
			return "Null";
		case JSONTypes::Unknown:
		case JSONTypes::Object:
		case JSONTypes::Array:
		case JSONTypes::_deleteArray:
		default:
			return std::string();
		}
	}

	const char* JSONData::getStringPtr() const
	{
		if (getType() != JSONTypes::String)
		{
			return "";
		}
		return string;
	}

	void JSONData::setString(const char* _string)
	{
		setType(JSONTypes::String);
		string = _string;
	}

	void JSONData::setString(const HeapString& _string)
	{
		string = _string;
	}

	long long JSONData::getInteger() const
	{
		if (getType() != JSONTypes::Integer)
		{
			return 0;
		}
		return integer;
	}

	void JSONData::setInteger(long long _integer)
	{
		setType(JSONTypes::Integer);
		integer = _integer;
	}

	long double JSONData::getDouble() const
	{
		if (getType() != JSONTypes::Float)
		{
			return 0;
		}
		return dbl;
	}

	void JSONData::setDouble(long double _double)
	{
		setType(JSONTypes::Float);
		dbl = _double;
	}

	bool JSONData::getBool() const
	{
		if (getType() != JSONTypes::Boolean)
		{
			return false;
		}
		return boolean;
	}

	void JSONData::setBool(bool _bool)
	{
		setType(JSONTypes::Boolean);
		boolean = _bool;
	}

	JSONElementArray* JSONData::getArrayPtr() const
	{
		if (getType() != JSONTypes::Array)
		{
			return nullptr;
		}
		return array;
	}

	void JSONData::setArray(JSONElementArray& _array)
	{
		setType(JSONTypes::_deleteArray);
		array = new JSONElementArray(_array);
	}

	void JSONData::setArray(JSONElementArray* _array)
	{
		setType(JSONTypes::Array);
		array = _array;
	}

	void JSONData::setNull()
	{
		setType(JSONTypes::Null);
	}

	void JSONData::copyValue(const JSONData& _jsonData)
	{
		setType(_jsonData.type);
		_jsonData.name = name;
	}

	HeapString::HeapString(HeapString&& _heapString)
	{
		refCount = _heapString.refCount;
		str = _heapString.str;
		_heapString.refCount = nullptr;
		_heapString.str = nullptr;
	}

	HeapString::HeapString(const HeapString& _heapString)
	{
		refCount = _heapString.refCount;
		str = _heapString.str;
		if (str)
		{
			(*refCount)++;
		}
	}

	HeapString::HeapString(const char* _str) : HeapString(_str, strlen(_str))
	{}

	HeapString::HeapString(const char* _str, size_t _len) : str(nullptr)
	{
		refCount = new uint64_t(1);
		setStr(_str, _len);
	}

	HeapString::~HeapString()
	{
		free();
	}

	size_t HeapString::size()
	{
		if (!str)
		{
			return 0;
		}
		return strlen(str);
	}

	void HeapString::setStr(const char* _str)
	{
		setStr(_str, strlen(_str));
	}

	void HeapString::setStr(const char* _str, size_t _len)
	{
		if (*refCount <= 1)
		{
			if (str)
			{
				std::cout << "setStr(2): free " << (size_t)str << "\n";
				delete[] str;
			}
		}
		else {
			(*refCount)--;
		}
		str = new char[_len + 1];
		std::cout << "setStr(2): alloc " << (size_t)str << "\n";
		//str = (char*)malloc(_len + 1);
		memset(str, 0, _len + 1);
		memcpy_s(str, _len, _str, _len);
	}

	void HeapString::append(char _src)
	{
		if (!str)
		{
			str = new char[2] { _src, 0 };
			std::cout << "alloc " << (size_t)str << "\n";
			return;
		}
		size_t oldSize = strlen(str);
		char* tmpPtr = new char[oldSize + 2](0);
		memcpy_s(tmpPtr, oldSize, str, oldSize);
		tmpPtr[oldSize] = _src;
		if (*refCount <= 1)
		{
			if (str)
			{
				std::cout << "append(1): freed " << (size_t)str << "\n";
				delete[] str;
			}
		}
		else {
			(*refCount)--;
		}
		
		str = tmpPtr;
		std::cout << "append(1): alloc " << (size_t)str << "\n";
	}

	void HeapString::append(const char* _src, size_t _count)
	{
		if (!str)
		{
			setStr(_src, _count);
			return;
		}
		size_t oldSize = strlen(str);
		char* tmpPtr = new char[oldSize + _count + 1](0);
		memcpy_s(tmpPtr, oldSize, str, oldSize);
		memcpy_s((char*)tmpPtr + oldSize, _count, _src, _count);
		if (*refCount <= 1)
		{
			if (str)
			{
				std::cout << "append(2): freed " << (size_t)str << "\n";
				delete[] str;
			}
		}
		else {
			(*refCount)--;
		}
		str = tmpPtr;
		std::cout << "append(2): alloc " << (size_t)str << "\n";
	}

	char* HeapString::getPtr() const
	{
		return str;
	}

	void HeapString::free()
	{
		(*refCount)--;
		if (*refCount == 0)
		{
			if (nullptr != str)
				delete str;

			delete refCount;
		}


		return;
		if (!refCount)
		{
			return;
		}
		if (!*refCount || *refCount > 5)
		{
			std::cout << 2;

		}
		(*refCount)--;
		if (!*refCount)
		{
			std::cout << "deleted: " << std::hex << refCount << "\n";
			std::cout << "~HeapString(): freed " << (size_t)str << "\n";
			delete refCount;
			delete[] str;
			refCount = nullptr;
			str = nullptr;
		}
	}

	HeapString::operator const char*() const
	{
		return getPtr();
	}

	HeapString& HeapString::operator=(const HeapString& _heapString)
	{
		free();
		refCount = _heapString.refCount;
		str = _heapString.str;
		if (str)
		{
			(*refCount)++;
		}
	}

	HeapString& HeapString::operator=(HeapString&& _heapString)
	{
		free();
		refCount = _heapString.refCount;
		str = _heapString.str;
		_heapString.refCount = nullptr;
		_heapString.str = nullptr;
	}

	JSONError::JSONError() : 
		location(0), error(JSONErrors::OK)
	{
	}

	JSONError::JSONError(size_t _location, JSONErrors _error) :
		location(_location), error(_error)
	{
	}
}
#endif // __RJSON__
