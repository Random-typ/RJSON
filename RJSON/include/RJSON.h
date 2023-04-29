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
		MissingColon
	};

	// get index to first byte after whitespace
#define isEOD \
if (_off == std::string::npos)\
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
		JSONElement(std::string _nameORjson);

		JSONElement(std::string _name, size_t _val);
		JSONElement(std::string _name, float _val);
		JSONElement(std::string _name, bool _val);
		JSONElement(std::string _name, JSONElementArray _json, JSONType _type = JSONTypes::Object);
		JSONElement(std::string _name, std::string _val);

		~JSONElement();

		bool							hasError();
		std::string						getErrorText();

		// Retrieves child element by it's name
		// @param _name: Element name
		// @return JSONElement&
		JSONElement&					get(std::string _name);
		
		// Retrieves all children and childs children by their name
		// @param _name: Element name
		// @return JSONElementArrayPTR
		JSONElementArrayPTR				getAll(std::string _name);


		// Add a child to the current element
		// @param _name: Element name
		// @param _value: Value of the element
		// @param _type: Type of the element
		// @return JSONElement&
		JSONElement&					addChild(std::string _name, std::string _value = "", JSONType _type = JSONTypes::Unknown);
		JSONElement&					addChild(std::string _name, size_t _value);
		JSONElement&					addChild(std::string _name, int _value);
		JSONElement&					addChild(std::string _name, unsigned int _value);
		JSONElement&					addChild(std::string _name, float _value);
		JSONElement&					addChild(std::string _name, bool _value);
		JSONElement&					addChild(std::string _name, std::vector<JSONElement> _elements);
		JSONElement&					addChild(std::string _name, std::vector<std::string> _array);
		JSONElement&					addChild(std::string _name, std::vector<size_t> _array);
		JSONElement&					addChild(std::string _name, std::vector<int> _array);
		JSONElement&					addChild(std::string _name, std::vector<float> _array);
		JSONElement&					addChild(std::string _name, std::vector<bool> _array);


		// Add a child to the current element
		// @param _jsonElement: JSON Element
		// @return JSONElement&
		JSONElement&					addChild(JSONElement _jsonElement);

		// Remove child by its name 
		// @param _name: Element nae
		// @return True on success false otherwise
		bool                            removeChild(std::string _name);

		// Check if child exists by name
		// @param _name: Element nae
		// @return True when child exists false otherwise
		const bool                      hasChild(std::string _name);

		// Check if this elemt exists
		// @return True when element exists false otherwise
		const bool						exists();

		// Check if element contains children
		// @return True if element doesn't contain children
		const bool						isEmpty();

		// Erase this element from it's parent
		// @return True on success
		//const bool						erase();

		// Get the index of this element from parents perpective
		// @return if an error occurs the return value is less tha zero.
		//const __int64					getIndex();

		// Convert this element to a json string
		// @return std::string
		const std::string               asJSON(bool _formatted = false, std::string _whitespace = "\t");

		// Get element type
		// @return JSONType
		const JSONType					getType();

		// Automatically set the type of this element
		const void						autoType();

		// get the element value as int
		// @return int
		const int						valueAsInt();

		// get the element value as float
		// @return float
		const float						valueAsFloat();

		// get the element value as string
		// @return std::string
		const std::string				valueAsString();

		// get the element value as bool
		// @return bool
		const bool						valueAsBool();

		/*
		friend const bool				operator==(const JSONElement& _right, const JSONElement& _left);
		friend const bool				operator!=(const JSONElement& _right, const JSONElement& _left);
		*/

		friend const JSONElement		operator+(const JSONElement& _right, const JSONElement& _left);
		friend const JSONElement&		operator+=(const JSONElement& _right, const JSONElement& _left);

		JSONElement&					operator[](const size_t _index);
		JSONElement&					operator[](const std::string _name);


		std::string						name;
		std::string						value;
		JSONElementArray				children;
		JSONType						type = JSONTypes::Unknown;


		JSONErrors						error = JSONErrors::OK;
		size_t							errorLocation = 0;
	private:
		std::string				asJSONFormatted(std::string& _indent, std::string _whitespace);
		std::string               asJSONInnerFormatted(std::string& _indent, std::string _whitespace);

		std::string               asJSONInner();
		std::string				rawValue();
	};


	class RJSON
	{
	public:
		//template<typename _json>
		//RJSON(_json _JSONElements...);

		static JSONElement				EmptyElem;
		static JSONElement				load(std::string _jsonstructure);

	private:
		static JSONElement				parseElement(const std::string& _data, size_t& _off, JSONType _type);
		static JSONElement				parse(const std::string& _data, size_t& _off);
		static std::string				parseString(JSONElement& _elem, const std::string& _data, size_t& _off);
		static void						parseValue(JSONElement& _elem, const std::string& _data, size_t& _off);
		
		static const char*				JSONWhitespace;
	};
}
#endif // !__RJSON__