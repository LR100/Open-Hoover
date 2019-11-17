#ifndef _GENERICLEXER_H_
#define _GENERICLEXER_H_

#include <string>
#include <map>
#include <vector>

class GenericLexer
{
public:

	GenericLexer();
	~GenericLexer();

	enum LexToken : int
	{
		UNKNOWN = 0,
		DELIM = -36,
		NEWLINE = -42
	};

	struct LexData
	{
		LexData();
		std::string data;
		std::string info;
		int			token;
		int			inherit; // Other Token To know inheritance
	};

	struct LexTree
	{
		LexTree();
		~LexTree();

		LexData		data;
		LexTree		*right;
		LexTree		*left;
	};

	void AddToken(const std::string& str, int id, int param = 0); // Param are unused for the moment // But could be used later like a flag param for type of parsing (ex: number 234564234.3  .....)
	void AddToken(const std::string& str, int id, int param, const std::string& info); // Param are unused for the moment // But could be used later like a flag param for type of parsing (ex: number 234564234.3  .....)
	void SetTokenInherit(const std::string& str, int inherit);
	void Lex(const std::string& data, std::vector<LexData>& lexedData);
	void RemoveDataByToken(std::vector<LexData>& lexedData, std::vector<int> tokenValues);
	void ShowLexData(const std::vector<LexData>& lexedData);

private:

	struct RemaingParse
	{
		LexTree		*node;
		std::string	str;
	};


	void GoRecLex(const std::string & data, LexTree *tree);
	void RecLex(const std::string& data, LexTree* tree, std::vector<RemaingParse>& remaining);

	//void RecLex(const std::string& data, LexTree* tree);
	void FlatTree(LexTree* tree, std::vector<LexData>& lexedData);

	GenericLexer(GenericLexer& other); // Uncopyable
	GenericLexer& operator=(GenericLexer& other) {}; // --

	class Token
	{
	public:
		Token(const std::string& aWord, const int& aid, const int& aparam, const std::string& ainfo);
		Token();
		~Token();
		std::string word;
		std::string info;
		int			id;
		int			param;
		int			inherit;
	};

	size_t	_stackCall;
	size_t	_maxStackCall;

	std::map<std::string, Token>				_tokens;
	// Size of STR
	std::map<size_t, std::vector<Token*>>			_orderedTokens;
};


#endif /* !_GENERICLEXER_H_ */
