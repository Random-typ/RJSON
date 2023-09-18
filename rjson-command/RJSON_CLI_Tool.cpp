#include "RJSON_CLI_Tool.h"

char** RJSON_CLI_Tool::argv = {};
const char* RJSON_CLI_Tool::delimiter = "";

void RJSON_CLI_Tool::iterateArgs(int _argc, char** _argv, size_t _offset, RJSON::JSONElement& _element)
{
    argv = _argv;
    
    std::vector<RJSON::JSONElement> pastElements;
    iterateArgs(_offset, _argc, _element, pastElements);
}

void RJSON_CLI_Tool::iterateArgs(size_t _offset, int _argc, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{


    // boolean to track if the next block should be executed
    bool executeBlock = true;
    for (; _offset < _argc; _offset++)
    {
        if (!strcmp(argv[_offset], "("))
        {
            if (executeBlock)
            {// execute block
                continue;
            }
            // skip until the block is closed
            _offset--;
            findEndOfBlock(_argc, _offset);
        }
        if (!strcmp(argv[_offset], ")"))
        {// end of block
            continue;
        }

        executeBlock = true;

        if (!strcmp(argv[_offset], "-get"))// -get
        {
            get(_argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-i") || !strcmp(argv[_offset], "-index"))
        {
            index(_argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-iterate"))
        {
            iterate(_argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-up"))
        {
            up(_element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-root"))
        {
            root(_element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-check"))
        {
            check(executeBlock, _argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-count"))
        {
            count(_element);
            continue;
        }
        if (!strcmp(argv[_offset], "-exists"))
        {
            exists(executeBlock, _argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-name"))
        {
            name(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-value"))
        {
            value(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-json"))
        {
            json(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-delim") || !strcmp(argv[_offset], "-delimiter"))
        {
            delim(_argc, _offset);
            continue;
        }
        // modify
        if (!strcmp(argv[_offset], "-setname") || !strcmp(argv[_offset], "-sn"))
        {
            value(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-setvalue") || !strcmp(argv[_offset], "-sv"))
        {
            value(_argc, _offset, _element);
            continue;
        }
    }
}

void RJSON_CLI_Tool::findEndOfBlock(int _argc, size_t _offset)
{
    _offset++;
    if (strcmp(argv[_offset], "("))
    {// block is just one argument
        _offset++;
        return;
    }

    for (size_t blockCount = 0; _offset < _argc; _offset++)
    {
        if (!strcmp(argv[_offset], "("))
        {
            blockCount++;
            continue;
        }
        if (!strcmp(argv[_offset], ")"))
        {
            if (blockCount <= 0)
            {
                std::cout << "Error: Missing opening braces.";
                exit(1);
                return;
            }
            blockCount--;
            if (!blockCount)
            {
                break;
            }
        }
    }
}

void RJSON_CLI_Tool::get(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    checkFollowing;
    _pastSelected.emplace_back(_element);
    _element = (RJSON::JSONElement)_element.get(argv[_offset + 1]);
    _offset++;
}

void RJSON_CLI_Tool::index(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    checkFollowing;
    _pastSelected.emplace_back(&_element);
    _element = (RJSON::JSONElement)_element.get(std::stoi(argv[_offset + 1]));
    _offset++;
}

void RJSON_CLI_Tool::iterate(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    size_t endOfBlock = _offset;
    findEndOfBlock(_argc, endOfBlock);// _offset marks the end of the block
    // iterate over every child and execute the code block for each
    for (auto child : _element.children)
    {
        iterateArgs(_offset + 1, endOfBlock, child, _pastSelected);
    }
}

void RJSON_CLI_Tool::up(RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    if (_pastSelected.empty())
    {
        return;
    }
    _element = _pastSelected.back();
    _pastSelected.pop_back();
}

void RJSON_CLI_Tool::root(RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    _element = _pastSelected.front();
    _pastSelected.clear();
    _pastSelected.emplace_back(&_element);
}

void RJSON_CLI_Tool::check(bool& _executeBlock, int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    _executeBlock = _element.hasError();
    if (_offset + 1 < _argc && !strcmp(argv[_offset + 1], "("))
    {
        return;
    }
    std::cout << _executeBlock << delimiter;
}

void RJSON_CLI_Tool::count(const RJSON::JSONElement& _element)
{
    std::cout << _element.children.size() << delimiter;
}

void RJSON_CLI_Tool::exists(bool& _executeBlock, int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    _executeBlock = _element.exists();
    if (_offset + 1 < _argc && !strcmp(argv[_offset + 1], "("))
    {
        return;
    }
    std::cout << _executeBlock << delimiter;
}

void RJSON_CLI_Tool::name(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    if (isFollowed && argv[_offset + 1][0] != '-')
    {// set name
        setName(_argc, _offset, _element);
        return;
    }
    std::cout << _element.name << delimiter;
}

void RJSON_CLI_Tool::value(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    if (isFollowed && argv[_offset + 1][0] != '-')
    {// set value
        setValue(_argc, _offset, _element);
        return;
    }
    std::cout << _element.value << delimiter;
}

void RJSON_CLI_Tool::json(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    if (argv[++_offset][0] == '1')
    {
        std::cout << _element.asJSON(true);
        return;
    }
    std::cout << _element.asJSON() << delimiter;
}

void RJSON_CLI_Tool::delim(int _argc, size_t& _offset)
{
    checkFollowing;
    delimiter = argv[++_offset];
}

void RJSON_CLI_Tool::setName(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    _element.name = argv[++_offset];
}

void RJSON_CLI_Tool::setValue(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    _element.type = RJSON::JSONTypes::Unknown;
    if (!strcmp(argv[++_offset], "true"))
    {// is boolean
        _element.type = RJSON::JSONTypes::Boolean;
    }

    if (!strcmp(argv[++_offset], "false"))
    {// is boolean
        _element.type = RJSON::JSONTypes::Boolean;
    }

    if (!strcmp(argv[++_offset], "null"))
    {// is boolean
        _element.type = RJSON::JSONTypes::Null;
    }

    if (_element.type == RJSON::JSONTypes::Unknown)
    {
        _element.type = RJSON::JSONTypes::Integer;
    }

    for (size_t i = 0; _element.type == RJSON::JSONTypes::Unknown; i++)
    {
        if (argv[++_offset][i] < '0' || argv[++_offset][i] > '9')
        {
            if (argv[++_offset][i] == '.')
            {// is float
                _element.type = RJSON::JSONTypes::Float;
                break;
            }
            // is string
            _element.type = RJSON::JSONTypes::String;
            break;
        }
        // is int -> don't change type
    }

    _element.value = argv[++_offset];
}
