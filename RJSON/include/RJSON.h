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
	struct JSONElement;

	class JSONElementArray;
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

	class JSONElementArray : public std::vector<JSONElement>
	{
	public:
		friend JSONElementArray operator+(const JSONElementArray& _right, const JSONElementArray& _left);
		friend JSONElementArray operator+(const JSONElementArray& _right, const JSONElementArrayPTR& _left);

		friend JSONElementArray operator+=(JSONElementArray& _right, const JSONElementArray& _left);
		friend JSONElementArray operator+=(JSONElementArray& _right, const JSONElementArrayPTR& _left);

		/*
		friend bool				operator==(const JSONElementArray& _right, const JSONElementArray& _left);
		friend bool				operator==(const JSONElementArray& _right, const JSONElementArrayPTR& _left);

		friend bool				operator!=(const JSONElementArray& _right, const JSONElementArray& _left);
		friend bool				operator!=(const JSONElementArray& _right, const JSONElementArrayPTR& _left);
		*/
	};

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


	struct JSONElement
	{
		JSONElement();

		//JSONElement::JSONElement(JSONElement _elem);;

		JSONElement(int _val);
		JSONElement(float _val);
		JSONElement(bool _val);
		JSONElement(std::string _nameORjson);

		JSONElement(std::string _name, int _val);
		JSONElement(std::string _name, float _val);
		JSONElement(std::string _name, bool _val);
		JSONElement(std::string _name, std::string _val);

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

		// Check if element contains children
		// @return True if element doesn't contain children
		const bool						isEmpty();

		// Erase this element from it's parent
		// @return True on success
		const bool						erase();

		// Get the index of this element from parents perpective
		// @return if an error occurs the return value is less tha zero.
		const __int64					getIndex();

		// Convert this element to a json string
		// @return std::string
		const std::string               asJSON();

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
		template<typename _json>
		RJSON(_json _JSONElements...);

		static JSONElement				EmptyElem;
		static JSONElement				load(std::string _jsonstructure);

	private:
		static JSONElement				parseElement(const std::string& _data, size_t& _off, JSONType _type);
		static JSONElement				parse(const std::string& _data, size_t& _off, std::string _name = "");
		
		static const char*				JSONWhitespace;
	};
}
#endif // !__RJSON__