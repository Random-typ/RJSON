/*COPYRIGHT AND PERMISSION NOTICE
*                        __              __             __        ____  ___
*         /    ------   |  |    |     | |  \   //      |  |    | /     |   | |  |
*        /       /      |__/ __ |     | |__/  // ___   |__/    | \___  |   | |\ |
*       /       /       |\      |     | |  \    /__    |\      |     \ |   | | \|
*      /____.  /.       | \     |____ | |__/   ___/    | \  \__| ____/ |___| |  |
* Copyright (c) 2022, Luca Trefz, Luca-Trefz@web.de
*
* Permission to use, copy, modify, and distribute this software
* for any purpose with or without fee is hereby granted, provided
* that the above copyright notice and this permission notice
* appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <locale>
#include <codecvt>

#ifdef _HAS_CXX17
	#ifndef __RJSON__
		#define __RJSON__
	#endif
#else
	#error "RJSON needs at least C++17."
#endif // !_HAS_CXX17

#ifdef __RJSON__
namespace RJSON {
	class JSONElement;

	//class JSONElementArray;
	class JSONElementArrayPTR;

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

	using JSONElementArray = std::vector<JSONElement>;

	//class JSONElementArray : public std::vector<JSONElement>
	//{
	//public:
	//	friend JSONElementArray operator+(const JSONElementArray& _right, const JSONElementArray& _left);
	//	friend JSONElementArray operator+(const JSONElementArray& _right, const JSONElementArrayPTR& _left);

	//	friend JSONElementArray operator+=(JSONElementArray& _right, const JSONElementArray& _left);
	//	friend JSONElementArray operator+=(JSONElementArray& _right, const JSONElementArrayPTR& _left);
	//	void addElement(JSONElement _jsonElement);
	//	/*
	//	friend bool				operator==(const JSONElementArray& _right, const JSONElementArray& _left);
	//	friend bool				operator==(const JSONElementArray& _right, const JSONElementArrayPTR& _left);

	//	friend bool				operator!=(const JSONElementArray& _right, const JSONElementArray& _left);
	//	friend bool				operator!=(const JSONElementArray& _right, const JSONElementArrayPTR& _left);
	//	*/
	//};

	//class string : public std::wstring {
	//public:
	//	string() : std::wstring() {}
	//	string(const std::string& _str) : std::wstring(_str.begin(), _str.end()) {}
	//	string(const std::wstring& _str) : std::wstring(_str) {}
	//	string(const char* _str) : string(std::string(_str)) {}
	//	string(const wchar_t* _str) : std::wstring(_str) {}
	//	operator const  std::string&() {
	//		return std::string(this->begin(), this->end());
	//	}

	//	//string operator+(const wchar_t* _str) {
	//	//	return *this + std::wstring(_str);
	//	//}


	//	string operator+(const char* _str) {
	//		string str = *this;
	//		str.resize(str.size() + strlen(_str));
	//		size_t converted = 0;
	//		mbstowcs_s(&converted,
	//			str.data() + this->size() * sizeof(wchar_t),
	//			strlen(_str),
	//			_str,
	//			strlen(_str));
	//		return str;
	//	}

	//	std::wstring ws() {
	//		return *this;
	//	}

	//	std::string s() {
	//		return *this;
	//	}
	//};

	//inline string operator+(const char* _a, string _b) {
	//	return string(_a) + _b;
	//}

	//class stringArray : public std::vector<string> {
	//public:
	//	stringArray() {};
	//	stringArray(std::vector<std::string> _arr) {
	//		for (auto& i : _arr)
	//		{
	//			emplace_back(string(i));
	//		}
	//	};


	//	operator std::vector<std::string>() {
	//		std::vector<std::string> arr;
	//		for (auto& i : *this)
	//		{
	//			arr.emplace_back(string(i));
	//		}
	//		return arr;
	//	}
	//};

	using string = std::string;
	using stringArray = std::vector<std::string>;

	class JSONElementArrayPTR : public std::vector<JSONElement*>
	{
	public:
		friend JSONElementArrayPTR	operator+(const JSONElementArrayPTR& _right, const JSONElementArrayPTR& _left);
		friend JSONElementArrayPTR	operator+(const JSONElementArrayPTR& _right, const JSONElementArray& _left);

		friend JSONElementArrayPTR	operator+=(JSONElementArrayPTR& _right, const JSONElementArrayPTR& _left);
		friend JSONElementArrayPTR	operator+=(JSONElementArrayPTR& _right, const JSONElementArray& _left);

		/*
		friend bool					operator==(const JSONElementArrayPTR& _right, const JSONElementArrayPTR& _left);
		friend bool					operator==(const JSONElementArrayPTR& _right, const JSONElementArray& _left);

		friend bool					operator!=(const JSONElementArrayPTR& _right, const JSONElementArrayPTR& _left);
		friend bool					operator!=(const JSONElementArrayPTR& _right, const JSONElementArray& _left);
		*/
	};

	enum class JSONErrors
	{
		OK,
		Unexpected_Character,
		UnexpectedControl_Character,
		MissingColon,
		UnexpectedEndOfString,
		UnexpectedEndOfObject,
		UnexpectedEndOfValue,
		JSONisEmpty,
		UnhandledException,
		FailedToOpenFile
	};

	// get index to first byte after whitespace
#define isEOD \
if (_off == string::npos)\
		return elem;// eof/eod
#define AfterWhiteSpace \
	_off = _data.find_first_not_of(JSONWhitespace, _off);\
	isEOD

	class JSONElement
	{
	public:
		JSONElement();
		JSONElement(const JSONElement& _elem);

		//JSONElement::JSONElement(JSONElement _elem);;

		//JSONElement(std::initializer_list<JSONElement> _json);
		JSONElement(size_t _val);
		JSONElement(float _val);
		JSONElement(bool _val);
		JSONElement(const char* _name);
		JSONElement(const JSONElementArray& _elements);
		JSONElement(string _name, JSONType _type = JSONTypes::Object);

		JSONElement(const string& _name, size_t _val);
		JSONElement(const string& _name, float _val);
		JSONElement(const string& _name, bool _val);
		JSONElement(const string& _name, JSONElementArray _json, JSONType _type = JSONTypes::Object);
		JSONElement(const string& _name, const string& _val);

		~JSONElement();

		void							copyError(const JSONElement& _elem);
		bool							hasError() const;
		string						getErrorText() const;

		// Retrieves child element by it's name
		// @param _name: Element name
		// @return JSONElement&
		JSONElement&					get(const string& _name);

		// Retrieves child element by it's index
		// @param _index: Element index
		// @return JSONElement&
		JSONElement&					get(size_t _index);
		
		// Retrieves all children and childs children by their name
		// @param _name: Element name
		// @return JSONElementArrayPTR
		JSONElementArrayPTR				getAll(const string& _name);


		// Add a child to the current element
		// @param _name: Element name
		// @param _value: Value of the element
		// @param _type: Type of the element
		// @return JSONElement&
		JSONElement&					addChild(const string& _name, const string& _value = "", JSONType _type = JSONType::String);
		JSONElement&					addChild(const string& _name, const char* _value, JSONType _type = JSONType::String);
		JSONElement&					addChild(const string& _name, size_t _value);
		JSONElement&					addChild(const string& _name, int _value);
		JSONElement&					addChild(const string& _name, unsigned int _value);
		JSONElement&					addChild(const string& _name, long long _value);
		JSONElement&					addChild(const string& _name, double _value);
		JSONElement&					addChild(const string& _name, long double _value);
		JSONElement&					addChild(const string& _name, bool _value);
		JSONElement&					addChild(const string& _name, const std::vector<JSONElement>& _elements);
		JSONElement&					addChild(const string& _name, const stringArray& _array);
		JSONElement&					addChild(const string& _name, const std::vector<size_t>& _array);
		JSONElement&					addChild(const string& _name, const std::vector<int>& _array);
		JSONElement&					addChild(const string& _name, const std::vector<double>& _array);
		JSONElement&					addChild(const string& _name, const std::vector<bool>& _array);


		// Add a child to the current element
		// @param _jsonElement: JSON Element
		// @return JSONElement&
		JSONElement&					addChild(const JSONElement& _jsonElement);

		// Remove child by its name 
		// @param _name: Element nae
		// @return True on success false otherwise
		bool                            removeChild(const string& _name);

		// Check if child exists by name
		// @param _name: Element nae
		// @return True when child exists false otherwise
		bool							hasChild(const string& _name) const;

		// Check if this elemt exists
		// @return True when element exists false otherwise
		bool							exists() const;

		// Check if element contains children
		// @return True if element doesn't contain children
		bool							hasChildren() const;

		// Erase this element from it's parent
		// @return True on success
		//const bool						erase();

		// Get the index of this element from parents perpective
		// @return if an error occurs the return value is less tha zero.
		//const __int64					getIndex();

		// Convert this element to a json wstring
		// @return string
		string						asJSON(bool _formatted = false, string _whitespace = "\t") const;

		// Get element type
		// @return JSONType
		JSONType						getType() const;

		// Automatically set the type of this element
		void							autoType();

		// get the element value as int
		// @return int
		long long						valueAsInt() const;

		// get the element value as float
		// @return float
		long double						valueAsFloat() const;

		// get the element value as wstring
		// @return string
		string						valueAsString() const;

		// get the element value as bool
		// @return bool
		bool							valueAsBool() const;
		// get children values as array
		// @return std::vector<string>
		stringArray			asArray() const;

		void							fixValue();

		bool							isInteger();
		bool							isFloat();
		bool							isString();
		bool							isBool();
		bool							isNull();
		bool							isObject();
		bool							isArray();

		/*
		friend const bool				operator==(const JSONElement& _right, const JSONElement& _left);
		friend const bool				operator!=(const JSONElement& _right, const JSONElement& _left);
		*/

		friend const JSONElement		operator+(const JSONElement& _right, const JSONElement& _left);
		friend const JSONElement&		operator+=(const JSONElement& _right, const JSONElement& _left);

		JSONElement&					operator[](const size_t _index);
		JSONElement&					operator[](const string& _name);

		JSONElement&					operator=(size_t _value);
		JSONElement&					operator=(float _value);
		JSONElement&					operator=(bool _value);
		JSONElement&					operator=(const char* _value);
		JSONElement&					operator=(const JSONElement& _json);

		string						name;
		string					value;
		JSONElementArray				children;
		JSONType						type = JSONTypes::Object;


		JSONErrors						error = JSONErrors::OK;
		size_t							errorLocation = 0;
	private:
		string						asJSONFormatted(string& _indent, string _whitespace) const;
		string						asJSONInnerFormatted(string& _indent, string _whitespace) const;

		string						asJSONInner() const;
		string						rawValue() const;
	};


	class RJSON
	{
	public:
		//template<typename _json>
		//RJSON(_json _JSONElements...);

		static JSONElement				EmptyElem;
		static JSONElement				load(const string& _jsonstructure);
		static JSONElement				loadFile(const string& _path);
		static JSONElement				loadFile(std::fstream& _fs);
		
		static constexpr size_t			buffSize = 128;
	private:
		//static JSONElement				parseElement(const string& _data, size_t& _off, JSONType _type);
		
		static JSONElement				parseStream(std::fstream& _fs, size_t& _off);
		static JSONElement				parse(const string& _data, size_t& _off);
		static string					parseString(JSONElement& _elem, const string& _data, size_t& _off);
		static void						parseValue(JSONElement& _elem, const string& _data, size_t& _off);
		
		static const char*			JSONWhitespace;
	};
}
#endif // !__RJSON__