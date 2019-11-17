#pragma once

#include <string>
#include <fstream>

#include "XMLNode.h"

void	XMLNodeFreeTree(XMLNode* tree);

class XMLParser
{
public:

	XMLParser();
	virtual ~XMLParser() {};

	bool				Parse(const std::string& path);
	const std::string&	GetError() const;
	XMLNode*			GetTree() const;

protected:

	virtual void	CloseNode(); // Can Be Override to interact at close node

private:

	void	InitParsing();

	// Parse Functions
	void	Indecis();
	//
	void	InOpening();
	////
	void	InComment();
	void	InCommentLong();
	void	InCommentShort();
	void	InInfo();
	/////
	void	InUnamedOpenNode();
	void	InOpenNode();
	void	InCloseNode();
	//////
	void	InTextField();
	void	InValue();
	//// 

	///// Tools
	void			SkipChar(unsigned int nb);
	void			GetNextChar();
	std::string		GetNextString(unsigned int size);
	void			CrashError(std::string error);
	

	void			CloseCurrentNode();
	void			CreateNode();
	void			CheckAndClose();


	typedef void(XMLParser::*ParseFunction)(void);

	ParseFunction	_currentFunction;
	char			_cbuffer;
	std::string		_buffer;
	std::ifstream	_file;

	std::string		_currentValue;
	std::string		_value;
	XMLNode*		_currentNode;
	XMLNode*		_tree;
	XMLNode*		_info;

	unsigned int	_pos;

	std::string		_error;
};
