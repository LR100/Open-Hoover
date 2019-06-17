#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "LinkedList.hpp"


class XMLNode
{
public:
	XMLNode();
	virtual ~XMLNode() {};

	// Should Build Tree from top if using this function
	void									PushBackChild(XMLNode* child);

	unsigned short							GetDepth() const;
	XMLNode*								GetParent() const;
	LinkedList<XMLNode*>					GetChildren() const;

	XMLNode*								GetNodeNamed(const std::string& name, const unsigned short nth = 0, const unsigned short depthMax = USHRT_MAX);
	XMLNode*								GetNodeNamedWithValue(const std::string& name, const std::string& nameValue, const std::string& value, const unsigned short depthMax = USHRT_MAX);
	XMLNode*								GetNodeNamedAtDepth(const std::string& name, const unsigned short depth = USHRT_MAX);

	std::string								ToString() const;
	void									Export(const std::string& pathFile) const;

	void									SetName(const std::string& name);
	const std::string&						GetName() const;
	const unsigned int&						GetNameHash() const;

	void									SetValue(const std::string& value);
	const std::string&						GetValue() const;

	//void									SetValue(const std::string& name, const std::string& value);

	template <typename T>
	void									SetValue(const std::string& name, T& value)
	{
		std::stringstream ss;
		ss << value;
		_values[name] = ss.str();
	}
	template <typename T>
	void									GetValue(const std::string& name, T& value) const
	{
		std::istringstream iss(_values.at(name));
		iss >> value;
	}
	const std::string&						GetValue(const std::string& name) const;

	const bool								ValueExists(const std::string& name) const { return (_values.count(name)); };

	void									Remove();


private:

	XMLNode*								GetNodeNamedIn(unsigned short& cnth, const unsigned int& nameHash, const unsigned short nth = 0, const unsigned short depthMax = USHRT_MAX);
	XMLNode*								GetNodeNamedWithValueIn(const unsigned int& nameHash, const std::string& nameValue, const std::string& value, const unsigned short depthMax = USHRT_MAX);
	XMLNode*								GetNodeNamedAtDepthIn(const unsigned int& nameHash, const unsigned short depth = USHRT_MAX);

	void									ActualizeChildrenDepth();

	unsigned short							_depth;
	XMLNode*								_parent;
	LinkedList<XMLNode*>::Node*				_parentNode;
	LinkedList<XMLNode*>					_children;
	std::string								_value;
	std::map<std::string, std::string>		_values;
	std::string								_name;
	unsigned int							_nameHash;
};