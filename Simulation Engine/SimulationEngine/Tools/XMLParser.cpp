#include "XMLParser.h"

#include <iostream>


XMLParser::XMLParser()
{
	_info = new XMLNode();
	_tree = _info;
	_currentNode = _info;
	_currentNode->SetName("root");
	_value = "";
}

bool XMLParser::Parse(const std::string & path)
{
	_file.open(path);

	if (!_file.is_open())
	{
		std::cerr << "XMLParser::Parse ERROR can't open (" << path << ")" << std::endl;
		return (false);
	}

	InitParsing();

	while (!_file.eof())
	{
		_file >> _cbuffer;
		(this->*_currentFunction)();
		//std::cout << "(" << _cbuffer << ")" << std::endl;
		_pos += 1;
	}
	_file.close();
	if (_error.size() > 0)
		return (false);
	return (true);
}

const std::string & XMLParser::GetError() const
{
	return (_error);
}

XMLNode*	XMLParser::GetTree() const
{
	return (_tree);
}

void XMLParser::CloseNode()
{
}


void XMLParser::InitParsing()
{
	_file >> std::noskipws;
	_currentFunction = &XMLParser::Indecis;
	_pos = 0;
}


void XMLParser::Indecis()
{
	//std::cout << "\nIndecis";
	if (_cbuffer == '<')
		_currentFunction = &XMLParser::InOpening;
	else
		_value += _cbuffer;
}

void XMLParser::InOpening()
{
	//std::cout << "\nInOpening";
	if (_cbuffer == '?')
	{
		_currentFunction = &XMLParser::InInfo;
	}
	else if (_cbuffer == '!')
	{
		_buffer = "";
		_currentFunction = &XMLParser::InComment;
	}
	else if (_cbuffer == '/')
	{
		_buffer = "";
		_currentFunction = &XMLParser::InCloseNode;
	}
	else
	{
		_buffer = _cbuffer;
		_currentFunction = &XMLParser::InUnamedOpenNode;
	}
}


void XMLParser::InCloseNode()
{
	//std::cout << "\nInCloseNode";
	if (_cbuffer == '>')
	{
		if (_value.size()) // Clear from free space and bl front and back
		{
			size_t i = 0;
			size_t size = 0;
			while (i < _value.size() && _value.at(i) <= ' ') // Bad First Char
				i += 1;
			if (i < _value.size())
			{
				size = _value.size() - i;
				_value = _value.substr(i, size);
			}
			else
				_value = "";
		}
		_currentNode->SetValue(_value); //// Should be cleared from free space and bl front and back


		CloseCurrentNode();
		_value = "";
		CloseNode();
		_currentFunction = &XMLParser::Indecis;
	}
	else
	{
		_buffer += _cbuffer;
	}
}

void XMLParser::InComment()
{
	//std::cout << "\nInComment";
	if (_cbuffer == '>')
	{
		_currentFunction = &XMLParser::Indecis;
		return;
	}
	else
	{
		_buffer += _cbuffer;
		if (_buffer.size() == 2)
		{
			if (_buffer == "--")
			{
				_currentFunction = &XMLParser::InCommentLong;
			}
			else
			{
				_currentFunction = &XMLParser::InCommentShort;
			}
		}

	}
}

void XMLParser::InCommentLong()
{
	//std::cout << "\nInCommentLong";
	if (_cbuffer == '-')
	{
		std::string check = GetNextString(2);
		if (check == "->")
		{
			//std::cout << "Comments: (" << _buffer << ")" << std::endl;
			_currentFunction = &XMLParser::Indecis;
		}
	}
	else
	{
		_buffer += _cbuffer;
	}
}

void XMLParser::InCommentShort()
{
	//std::cout << "\nInCommentShort";
	if (_cbuffer == '>')
	{
		_currentFunction = &XMLParser::Indecis;
		return;
	}
}

void XMLParser::InUnamedOpenNode()
{
	//std::cout << "\nInUnamedOpenNode";
	if (_cbuffer == '>')
	{
		CreateNode();
		_buffer = "";
		_value = "";
		_currentFunction = &XMLParser::Indecis;
	}
	else if (_cbuffer == '=')
	{
		CreateNode();
		GetNextChar();
		if (_cbuffer != '\"')
		{
			CrashError("In value assignation");
			return;
		}
		_buffer = "";
		_currentFunction = &XMLParser::InValue;
	}
	else if (_cbuffer == '/')
	{
		CreateNode();
		CheckAndClose();
	}
	else if (_cbuffer != ' ')
	{
		_buffer += _cbuffer;
	}
	else
	{
		CreateNode();
		_buffer = "";
		_currentFunction = &XMLParser::InOpenNode;
	}
}

void XMLParser::InOpenNode()
{
	//std::cout << "\nInOpenNode";
	if (_cbuffer == '=')
	{
		_currentValue = _buffer;
		GetNextChar();
		if (_cbuffer != '\"')
		{
			CrashError("In value assignation");
			return;
		}
		_buffer = "";
		_currentFunction = &XMLParser::InTextField;
	}
	else if (_cbuffer == ' ')
	{
		_buffer = "";
	}
	else if (_cbuffer == '/')
	{
		CheckAndClose();
	}
	else if (_cbuffer == '>')
	{
		//std::cout << "Closing Node" << std::endl;
		_buffer = "";
		_currentFunction = &XMLParser::Indecis;
	}
	else
	{
		_buffer += _cbuffer;
	}
}

void XMLParser::InInfo()
{
	//std::cout << "\nInInfo";
	//// Get xml string
	_buffer += _cbuffer;
	_buffer += GetNextString(2);
	if (_buffer != "xml")
	{
		CrashError("in info \"xml\" missing");
		return;
	}
	else
	{
		_buffer = "";
		_currentNode = _info;
		_currentFunction = &XMLParser::InOpenNode;
	}
}


void XMLParser::InTextField()
{
	//std::cout << "\nInTextField";
	if (_cbuffer == '"')
	{
		_currentNode->SetValue(_currentValue, _buffer);
		_buffer = "";
		_currentValue = "";
		_currentFunction = &XMLParser::InOpenNode;
	}
	else
	{
		_buffer += _cbuffer;
	}
}

void XMLParser::InValue()
{
	//std::cout << "\nInValue";
	if (_cbuffer == '"')
	{
		_currentNode->SetValue(_buffer);
		_buffer = "";
		_currentValue = "";
		_currentFunction = &XMLParser::InOpenNode;
	}
	else
	{
		_buffer += _cbuffer;
	}
}

void XMLParser::CrashError(std::string error)
{
	_error = error;
	_file.seekg(0, std::ios::end);
	_file >> _cbuffer;
	std::cerr << "XMLParser::Parse ERROR " << _error << " at pos (" << _pos << ")" << std::endl;
}

void XMLParser::CloseCurrentNode()
{
	_currentNode = _currentNode->GetParent();
}

void XMLParser::CreateNode()
{
	XMLNode*	parent;

	parent = _currentNode;

	/*if (parent->children.count(_buffer))
	{
		_currentNode = parent->children.at(_buffer);
		return;
	}*/
	_value = "";
	_currentNode = new XMLNode();
	_currentNode->SetName(_buffer);
	parent->PushBackChild(_currentNode);
}

void XMLParser::CheckAndClose()
{
	GetNextChar();
	if (_cbuffer != '>')
	{
		CrashError("At closing node");
		return;
	}
	CloseCurrentNode();
	_currentFunction = &XMLParser::Indecis;
}

void XMLParser::SkipChar(unsigned int nb)
{
	_file.seekg(nb, std::ios::cur);
}

void XMLParser::GetNextChar()
{
	_file >> _cbuffer;
	_pos += 1;
}

std::string XMLParser::GetNextString(unsigned int size)
{
	std::string buffer;
	buffer.resize(size, ' ');    ////  reserve spaces
	_file.read((char*)(&*buffer.begin()), size);
	_pos += size;
	return (buffer);
}

void XMLNodeFreeTree(XMLNode * tree)
{
	if (tree == NULL)
		return;

	LinkedList<XMLNode*>::Node* cursor = tree->GetChildren().GetFront();
	
	while (cursor)
	{
		XMLNodeFreeTree(cursor->GetData());
		cursor = cursor->GetNext();
	}
	
	delete (tree);
}
