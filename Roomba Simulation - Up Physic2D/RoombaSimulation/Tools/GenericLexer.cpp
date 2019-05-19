#include "GenericLexer.h"

#include <stack>
#include <iostream> // Logiquement TEMPORAIRE

GenericLexer::GenericLexer()
{
	_maxStackCall = 80;
}

GenericLexer::~GenericLexer()
{
}

void GenericLexer::AddToken(const std::string & str, int id, int param)
{
	if (!str.size())
		return;
	_tokens[str] = Token(str, id, param, "");
	_orderedTokens[str.size()].push_back(&_tokens[str]);
}

void GenericLexer::AddToken(const std::string & str, int id, int param, const std::string & info)
{
	if (!str.size())
		return;
	_tokens[str] = Token(str, id, param, info);
	_orderedTokens[str.size()].push_back(&_tokens[str]);
}

void GenericLexer::SetTokenInherit(const std::string & str, int inherit)
{
	if (_tokens.count(str))
		_tokens.at(str).inherit = inherit;
}

void GenericLexer::Lex(const std::string & data, std::vector<LexData>& lexedData)
{
	LexTree *tree = new LexTree;

	GoRecLex(data, tree);
	FlatTree(tree, lexedData);
}

void GenericLexer::RemoveDataByToken(std::vector<LexData>& data, std::vector<int> tokenValues)
{
	std::vector<GenericLexer::LexData>	cleanData;	
	bool								isOk;

	for (size_t inc = 0; inc < data.size(); inc++)
	{
		isOk = true;
		for (size_t j = 0; j < tokenValues.size() && isOk; j++)
			if (tokenValues.at(j) == data.at(inc).token)
				isOk = false;
		if (isOk)
			cleanData.push_back(data.at(inc));
		
	}
	data = cleanData;
}


void GenericLexer::ShowLexData(const std::vector<LexData>& lexedData)
{
	std::string buffer;

	for (size_t inc = 0; inc < lexedData.size(); inc++)
		std::cout << "Data: (" << ((lexedData.at(inc).data == "\n") ? "\"\\n\"" : lexedData.at(inc).data) << ") Token: (" << lexedData.at(inc).token << ") Info: (" << lexedData.at(inc).info << ")\n";
}

/// ///////////////////////////////////// ///
/// RECURSIVE WAY - EASILy Fuck The STACK ///
/// ///////////////////////////////////// ///
/// But Fast...

void GenericLexer::GoRecLex(const std::string & data, LexTree *tree)
{
	std::vector<RemaingParse>	remaining;
	RemaingParse				toParse;

	toParse.node = tree;
	toParse.str = data;
	remaining.push_back(toParse);
	while (remaining.size())
	{
		_stackCall = 0;
		toParse = remaining.back();
		remaining.pop_back();
		RecLex(toParse.str, toParse.node, remaining);
	}
}

// SHOULD OPTIMIZE 
// -> By Removing Std::string and use char* -> Should be faster cause avoid creating temporary std::string with substr and only using "part" of char* with (offset and size)
void GenericLexer::RecLex(const std::string & data, LexTree * tree, std::vector<RemaingParse>& remaining)
{
	if (!data.size())
		return;
	bool			found = false;
	size_t			inc, pos;
	std::map<size_t, std::vector<Token*>>::reverse_iterator itWords = _orderedTokens.rbegin();

	_stackCall++;
	if (_stackCall > _maxStackCall)
	{
		RemaingParse toParse;

		toParse.str = data;
		toParse.node = tree;
		remaining.push_back(toParse);
		return;
	}
	if (data.size() < _orderedTokens.size())
		for (; (itWords != _orderedTokens.rend() && itWords->first < data.size()); itWords++);
	for (; itWords != _orderedTokens.rend() && !found; itWords++)
		for (inc = 0; inc < itWords->second.size() && !found; inc++)
			if ((pos = data.find(itWords->second.at(inc)->word, 0)) != std::string::npos)
			{
				found = true;
				tree->data.data = data.substr(pos, itWords->first); // Its The Only Substr that should stay
				tree->data.token = itWords->second.at(inc)->id;
				tree->data.info = itWords->second.at(inc)->info;
				tree->data.inherit = itWords->second.at(inc)->inherit;
				tree->left = new LexTree;
				tree->right = new LexTree;
				RecLex(data.substr(0, pos), tree->left, remaining);
				RecLex(data.substr((pos + itWords->first), std::string::npos), tree->right, remaining);
			}
	if (!found && data.size())
	{
		tree->data.data = data;
		tree->data.token = GenericLexer::LexToken::UNKNOWN;
	}
}

/*
void GenericLexer::RecLex(const std::string & data, LexTree *tree)
{
	bool			found = false;
	size_t			inc, pos;
	std::map<int, std::vector<Token*>>::reverse_iterator itWords = _orderedTokens.rbegin();

	
	if (data.size() < _orderedTokens.size())
		for (; (itWords != _orderedTokens.rend() && itWords->first < data.size()); itWords++);
	for (; itWords != _orderedTokens.rend() && !found; itWords++)
		for (inc = 0; inc < itWords->second.size() && !found; inc++)
			if ((pos = data.find(itWords->second.at(inc)->word, 0)) != std::string::npos)
			{
				found = true;
				tree->data.data = data.substr(pos, itWords->first);
				tree->data.token = itWords->second.at(inc)->id;
				tree->left = new LexTree;
				tree->right = new LexTree;
				RecLex(data.substr(0, pos), tree->left);
				RecLex(data.substr((pos + itWords->first), std::string::npos), tree->right);
			}
	if (!found && data.size())
	{
		tree->data.data = data;
		tree->data.token = GenericLexer::LexToken::UNKNOWN;
	}
}
*/


void GenericLexer::FlatTree(LexTree * tree, std::vector<LexData>& lexedData)
{
	std::stack<LexTree*>	pile;
	LexTree					*current = tree;

	while (!pile.empty() || current)
	{
		if (current) 
		{
			pile.push(current);
			current = current->left;
		}
		else 
		{
			current = pile.top();
			pile.pop();
			if (current->data.data.size())
				lexedData.push_back(current->data);
			current = current->right;
		}
	}
}

/*
void GenericLexer::FlatTree(LexTree * tree, std::vector<LexData>& lexedData, std::vector<LexTree*> remaining)
{
	_stackCall++;
	if (_stackCall > _maxStackCall)
	{
		std::cout << "STOOOOOOOOOOP" << std::endl;
		remaining.push_back(tree);
		return;
	}
	if (tree->left)
		FlatTree(tree->left, lexedData, remaining);
	lexedData.push_back(tree->data);
	if (tree->right)
		FlatTree(tree->right, lexedData, remaining);
}
*/

GenericLexer::Token::Token(const std::string & aWord, const int & aid, const int & aparam, const std::string & ainfo)
{
	word = aWord;
	id = aid;
	param = aparam;
	info = ainfo;
	inherit = 0;
}

GenericLexer::Token::Token()
{
	id = 0;
	param = 0;
	inherit = 0;
}

GenericLexer::Token::~Token()
{
}

GenericLexer::LexTree::LexTree()
{
	left = NULL;
	right = NULL;
}

GenericLexer::LexTree::~LexTree()
{
}

GenericLexer::LexData::LexData()
{
	token = 0;
	inherit = 0;
}
