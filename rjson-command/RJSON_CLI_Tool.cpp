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
        if (!strcmp(argv[_offset], "-index") || !strcmp(argv[_offset], "-i"))
        {
            index(_argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-iterate") || !strcmp(argv[_offset], "-it"))
        {
            iterate(_argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-up"))
        {
            up(_element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-root") || !strcmp(argv[_offset], "-rt"))
        {
            root(_element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-check") || !strcmp(argv[_offset], "-ch"))
        {
            check(executeBlock, _argc, _offset, _element, _pastSelected);
            continue;
        }
        if (!strcmp(argv[_offset], "-count") || !strcmp(argv[_offset], "-c"))
        {
            count(_element);
            continue;
        }
        if (!strcmp(argv[_offset], "-exists") || !strcmp(argv[_offset], "-e"))
        {
            exists(executeBlock, _argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-name") || !strcmp(argv[_offset], "-n"))
        {
            name(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-value") || !strcmp(argv[_offset], "-v"))
        {
            value(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-json") || !strcmp(argv[_offset], "-j"))
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
        if (!strcmp(argv[_offset], "-add") || !strcmp(argv[_offset], "-a"))
        {
            add(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-adds") || !strcmp(argv[_offset], "-as"))
        {
            adds(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-addn") || !strcmp(argv[_offset], "-an"))
        {
            addn(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-addv") || !strcmp(argv[_offset], "-av"))
        {
            addv(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-adda") || !strcmp(argv[_offset], "-aa"))
        {
            adda(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-addas") || !strcmp(argv[_offset], "-aas"))
        {
            addas(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-adde") || !strcmp(argv[_offset], "-ae"))
        {
            adde(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-addes") || !strcmp(argv[_offset], "-aes"))
        {
            addes(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-remove") || !strcmp(argv[_offset], "-r"))
        {
            remove(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-removes") || !strcmp(argv[_offset], "-rs"))
        {
            removes(_argc, _offset, _element);
            continue;
        }
        if (!strcmp(argv[_offset], "-clear"))
        {
            clear(_element);
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

RJSON::JSONType RJSON_CLI_Tool::getValueType(const char* _value)
{
    return RJSON::JSONTypes::Unknown;
    if (!strcmp(_value, "true"))
    {// is boolean
        return RJSON::JSONTypes::Boolean;
    }

    if (!strcmp(_value, "false"))
    {// is boolean
        return RJSON::JSONTypes::Boolean;
    }

    if (!strcmp(_value, "null"))
    {// is boolean
        return RJSON::JSONTypes::Null;
    }

    for (size_t i = 0; ; i++)
    {
        if (_value[i] < '0' || _value[i] > '9')
        {// not a number
            if (_value[i] == '.')
            {// is float
                return RJSON::JSONTypes::Float;
            }
            // is string
            return RJSON::JSONTypes::String;
        }
    }
    return RJSON::JSONTypes::Integer;
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
    _element.type = getValueType(argv[_offset + 1]);

    _element.value = argv[++_offset];
}

void RJSON_CLI_Tool::add(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    if (hasCodeBlock)
    {
        size_t start = _offset + 1;
        findEndOfBlock(_argc, _offset);
        const char* name;
        for (bool first = true ; start < _offset; start++)
        {
            if (first)
            {
                // name
                name = argv[start];
                if (start + 1 == _offset)
                {
                    _element.addChild(std::string(name));
                    break;
                }
                first = false;
                continue;
            }
            // value
            _element.addChild(std::string(name), argv[start]);
        }
        return;
    }
    _element.addChild(std::string(argv[++_offset]));
}

void RJSON_CLI_Tool::adds(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    add(_argc, _offset, _element);
    // select added element
    _pastSelected.emplace_back(_element);
    _element = _element.children.back();
}

void RJSON_CLI_Tool::addn(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    if (hasCodeBlock)
    {
        size_t start = _offset + 1;
        findEndOfBlock(_argc, _offset);
        const char* name;
        for (bool first = true; start < _offset; start++)
        {
            _element.addChild(std::string(name));
        }
        return;
    }
    _element.addChild(std::string(argv[++_offset]));
}

void RJSON_CLI_Tool::addns(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    addn(_argc, _offset, _element);
    // select added element
    _pastSelected.emplace_back(_element);
    _element = _element.children.back();
}

void RJSON_CLI_Tool::addv(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    if (hasCodeBlock)
    {
        size_t start = _offset + 1;
        findEndOfBlock(_argc, _offset);
        const char* name;
        for (bool first = true; start < _offset; start++)
        {
            _element.addChild("", name);
        }
        return;
    }
    _element.addChild(std::string(argv[++_offset]));
}

void RJSON_CLI_Tool::addvs(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    addv(_argc, _offset, _element);
    // select added element
    _pastSelected.emplace_back(_element);
    _element = _element.children.back();
}

void RJSON_CLI_Tool::adda(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
    checkFollowing;
    if (hasCodeBlock)
    {
        size_t start = _offset + 1;
        findEndOfBlock(_argc, _offset);
        const char* name;
        
        std::vector<RJSON::JSONElement> values;
        RJSON::JSONType type = getValueType(argv[start]);
        for (bool first = true; start < _offset; start++)
        {
            if (type != getValueType(argv[start]) && type != RJSON::JSONTypes::String)
            {
                type = RJSON::JSONTypes::String;
            }
            values.emplace_back("", argv[start]);
        }
        _element.addChild("", values);
        return;
    }
    _element.addChild("", "", RJSON::JSONTypes::Array);
}

void RJSON_CLI_Tool::addas(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    adda(_argc, _offset, _element);
    // select added element
    _pastSelected.emplace_back(_element);
    _element = _element.children.back();
}

void RJSON_CLI_Tool::adde(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
}

void RJSON_CLI_Tool::addes(int _argc, size_t& _offset, RJSON::JSONElement& _element)
{
}

void RJSON_CLI_Tool::remove(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
}

void RJSON_CLI_Tool::removes(int _argc, size_t& _offset, RJSON::JSONElement& _element, std::vector<RJSON::JSONElement>& _pastSelected)
{
    ;
}

void RJSON_CLI_Tool::clear(RJSON::JSONElement& _element)
{
    _element.children.clear();
}
