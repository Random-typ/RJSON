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
#include <span>

#include <iostream>

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

	enum class JSONTypes : __int8
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
	//static JSONElementArray EmptyJSONArray;


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

	class SmallString {
	public:
		SmallString() {
			value = "\0";
		}
		SmallString(const SmallString& _str);
		SmallString(const char* _str);
		SmallString(const char* _str, size_t _len);
		~SmallString();

		size_t size() const;

		void append(const char _str);
		void append(const char* _str, size_t _count);

		void setStr(const char* _str);
		void setStr(const char* _str, size_t _len);

		const char* c_str() const;

		operator const char*() const;

		void operator=(const SmallString& _str);
	private:
		std::string_view value;
	};

	class HeapString {
	public:
		HeapString() : str(nullptr), refCount(new uint64_t(1)) {};
		HeapString(HeapString&& _heapString);
		HeapString(const HeapString& _heapString);
		HeapString(const char* _str);
		HeapString(const char* _str, size_t _len);
		~HeapString();

		size_t size();

		void setStr(const char* _str);
		void setStr(const char* _str, size_t _len);

		void append(char _src);
		void append(const char* _src, size_t _count);

		char* getPtr() const;

		void free();

		operator const char*() const;

	private:
		char* str;
		uint64_t* refCount;
	};

	class JSONData {
	public:
		JSONData() : type(JSONTypes::Unknown), dummy{ 0 } { cleanup(); }
		JSONData(const JSONData& _jsonData);

		~JSONData();

		JSONType getType() const;
		void setType(JSONType _type);
		
		const char* getName() const;
		// MUST be null terminated
		void setName(const char* _name);

		// converts value to std::string if not already
		std::string getString() const;
		
		std::string& getRawString();

		const char* getStringPtr() const;
		// MUST be null terminated
		void setString(const char* _string);
		void setString(const std::string& _string);

		long long getInteger() const;
		void setInteger(long long _integer);

		long double getDouble() const;
		void setDouble(long double _double);

		bool getBool() const;
		void setBool(bool _bool);

		JSONElementArray* getArrayPtr() const;
		void setArray(const JSONElementArray& _array);
		
		void setNull();

		void operator=(const JSONData& _jsonData);
	private:
		// sets type to unknown and frees data
		void cleanup();

		JSONType type;
		std::string name;
		union {// value
			char		dummy[std::max(sizeof(std::string), sizeof(std::shared_ptr<JSONElementArray>))];
			std::string string;
			long long	integer;
			long double	dbl;
			bool		boolean;
			std::shared_ptr<JSONElementArray> array;
		};
	};



	enum class JSONErrors : size_t
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

	struct JSONError {
		JSONError();
		JSONError(size_t _location, JSONErrors _error);
		size_t		location : 56;
		JSONErrors	error : 8;
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
		JSONElement(long long _val);
		JSONElement(long double _val);
		JSONElement(bool _val);
		JSONElement(const JSONElementArray& _elements);
		JSONElement(const char* _name, JSONType _type = JSONTypes::Object);

		JSONElement(const char* _name, long long _val);
		JSONElement(const char* _name, long double _val);
		JSONElement(const char* _name, bool _val);
		JSONElement(const char* _name, JSONElementArray _json, JSONType _type = JSONTypes::Object);
		JSONElement(const char* _name, const string& _val);
		 
		~JSONElement();

		void							copyError(const JSONElement& _elem);
		bool							hasError() const;
		string							getErrorText() const;

		// Retrieves child element by it's name
		// @param _name: Element name
		// @return JSONElement&
		JSONElement&					get(const char* _name);

		// Retrieves child element by it's index
		// @param _index: Element index
		// @return JSONElement&
		JSONElement&					get(size_t _index);
		
		// Retrieves all children and childs children by their name
		// @param _name: Element name
		// @return JSONElementArrayPTR
		JSONElementArrayPTR				getAll(const char* _name);


		// Add a child to the current element
		// @param _name: Element name
		// @param _value: Value of the element
		// @param _type: Type of the element
		// @return JSONElement&
		JSONElement&					addChild(const char* _name, const string& _value = "", JSONType _type = JSONType::String);
		JSONElement&					addChild(const char* _name, const char* _value, JSONType _type = JSONType::String);
		JSONElement&					addChild(const char* _name, size_t _value);
		JSONElement&					addChild(const char* _name, int _value);
		JSONElement&					addChild(const char* _name, unsigned int _value);
		JSONElement&					addChild(const char* _name, long long _value);
		JSONElement&					addChild(const char* _name, double _value);
		JSONElement&					addChild(const char* _name, long double _value);
		JSONElement&					addChild(const char* _name, bool _value);
		JSONElement&					addChild(const char* _name, const std::vector<JSONElement>& _elements);
		JSONElement&					addChild(const char* _name, const stringArray& _array);
		JSONElement&					addChild(const char* _name, const std::vector<long long>& _array);
		JSONElement&					addChild(const char* _name, const std::vector<int>& _array);
		JSONElement&					addChild(const char* _name, const std::vector<long double>& _array);
		JSONElement&					addChild(const char* _name, const std::vector<bool>& _array);


		// Add a child to the current element
		// @param _jsonElement: JSON Element
		// @return JSONElement&
		JSONElement&					addChild(const JSONElement& _jsonElement);

		// Remove child by its name 
		// @param _name: Element nae
		// @return True on success false otherwise
		bool                            removeChild(const char* _name);

		// Check if child exists by name
		// @param _name: Element nae
		// @return True when child exists false otherwise
		bool							hasChild(const char* _name) const;

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
		string							asJSON(bool _formatted = false, string _whitespace = "\t") const;

		// Get element type
		// @return JSONType
		JSONType						getType() const;

		void							setType(JSONType _type);

		// Automatically set the type of this element
		// Deprecated!
		void							autoType();

		// get the element value as int
		// @return int
		long long						valueAsInt() const;

		// get the element value as float
		// @return float
		long double						valueAsFloat() const;

		// get the element value as string
		// converts it to string if not already
		// @return string
		string							valueAsString() const;

		// get the element value as bool
		// @return bool
		bool							valueAsBool() const;
		// get children values as array
		// @return std::vector<string>
		stringArray						asArray() const;

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
		JSONElement&					operator[](const char* _name);

		JSONElement&					operator=(size_t _value);
		JSONElement&					operator=(long double _value);
		JSONElement&					operator=(bool _value);
		JSONElement&					operator=(const char* _value);
		JSONElement&					operator=(const JSONElement& _json);

		bool							hasName() const;
		// null is a value
		bool							hasValue() const;
		// allocates space and copies _name
		void							setName(const char* _name);
		const char*						getName() const;

		// does create a copy of _str
		void							setValue(const char* _str);
		void							setValue(const string& _str);
		void							setValue(long long _integer);
		void							setValue(bool _bool);
		void							setValue(long double _bool);
		void							setValueNull();

		const char*						getValuePtr();

		string&							getRawString();

		JSONElementArray				children;

		JSONError						error;
	private:
		JSONData						data;
		string							asJSONFormatted(string& _indent, string _whitespace) const;
		string							asJSONInnerFormatted(string& _indent, string _whitespace) const;

		string							asJSONInner() const;
		string							valueAsFormattedString() const;
	};

	class RJSONStream {
	public:
		RJSONStream(size_t bufferSize = defaultBufferSize);
		~RJSONStream();
		
		JSONElement parseStream(const char* _path);

		JSONErrors getError();
		bool hasError();


		static size_t defaultBufferSize;
	private:
		JSONElement parseStream();
		void parseValue(JSONElement& json);
		string parseString();
		void parseObject(JSONElement& object);
		void parseArray(JSONElement& object);

		bool readBuffer();
		bool findEndOfWhitespace();
		bool findEndOfNumber(size_t& start, std::string& value);


		JSONError error;
		size_t totalSizeBefore = 0;
		size_t off = 0;
		size_t size = 0;
		size_t buffSize = 0;
		char* buffer = nullptr;
		std::fstream fs;
	};

	class RJSON
	{
	public:
		//template<typename _json>
		//RJSON(_json _JSONElements...);

		static JSONElement				EmptyElem;
		static JSONElement				load(const string& _jsonstructure);
		static JSONElement				loadFile(const string& _path);
		
		static constexpr size_t			buffSize = 128;
	private:
		//static JSONElement				parseElement(const string& _data, size_t& _off, JSONType _type);
		
		static JSONElement				parseStream(const char* _path);
		static JSONElement				parse(const string& _data, size_t& _off);
		static string					parseString(JSONElement& _elem, const string& _data, size_t& _off);
		static void						parseValue(JSONElement& _elem, const string& _data, size_t& _off);
	public:// remove
		static const char*			JSONWhitespace;
	};
}
#endif // !__RJSON__