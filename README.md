# RJSON
C++ JSON parser.
Details about the usage of the C++ library can be found [here](README_LIB_DOC.md).
# CLI Tool
RJSON is not only a C++ library it is also a CLI tool which can be used for very basic and advanced json queries, with an easy to learn syntax.
## Usage

The first parameter is always the path to the json file.  

    rjson "example.json"

When a json is loaded the selected element is the root of the json.  
Parameters can be used to find, read and iterate over elements.

### Code Blocks
A code block is usually encased in braces and is only executed if a criteria is met or it is iterated over.
Code blocks dont have to be closed, however if they are not closed everything after the open brace is part of the block.
Example:  
`-exists ( -get author -value )`  
In the example above, first it will be checked wether the selected element exists.  
If it exists the block is executed. First `-get` selects the child of the selected element with the name "author".  
Then `-value` follows which will print the value of the element selected by `-get`. The output of this could look like this:  

    rjson example.json -i 0 -exists ( -get author -value )
    Random Typ

Note that the selected element after the block is still "author". 
To go back to the previous element `-up` can be used, Here `-up`should be used inside the the block after `-value`.
This way if the block wont get executed the selected element also wont change. 

## Parameters
* ### `-get <element name>`
  Selects a child of the selected element by its name.
  Note that if the element does not exist an empty element will be selected.
  Use `-exists` to check if the elements exists.
* ### `-i <index>` or `-index <index>`
  Selects a child of the selected element by index. If the index is out of range an empty element is selected. 
  Use `-exists` to check if the elements exists.
* ### `-json <format>`
  Prints the selected element as a json. This parameter has to be followed by either 0 or 1.
  If format is 0 it will print the json in a compact hard to read format. If format is 1 the json is more human readble.
* ### `-delim <delimiter>` or `-delimiter <delimiter>`
  Specifies a delimiter which is going to sit between every value that is printed.
  this parameter should be used right after the json path.
* ### `-up`
  Selects the parent element of the selected element. If the selected element is the root nothing will happen.
* ### `-count`
  Prints the number of children the selected element has.
* ### `-root`
  Selects the root element of the json.
* ### `-name`
  Prints the name of the current element.
* ### `-value`
  Prints the value of the current element.
* ### `-exists`
  Prints 0 if the element does not exist 0 otherwise. If a code block follows, nothing will be printed and the block is only  
  executed if the selected element exists.
* ### `-check`
  This parameter will check the json for formatting errors. If errors are found it will print 1 otherwise it will print 0.  
  If a code block follows `-count` it will only be executed if there are no formatting errors in the json.
* ### `-iterate`
  Iterates over every child of the selected element, executing the following code block for each.  
  Note that here the code block will not change the selected element outside of the block.  
  Also note that any parameter following `-iterate` will be treated like a code block. Which means that this is valid:  
  `... -iterate -value ...`

# Example
For the following examples I will be using [this](example.json) json file.

`rjson example.json -delim " " -iterate ( -get author -value -up get name -value -up ) -get tags -iterate value`

It lists the name, author and the tags of every book in the json.
