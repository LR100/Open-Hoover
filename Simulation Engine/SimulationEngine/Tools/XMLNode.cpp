#include "XMLNode.h"

std::string XMLNode::ToString() const
{
	std::string margin;
	std::string text;

	for (unsigned short i = 0; i < _depth; i += 1)
		margin += "  ";
	// Display Values of This Node with Margin
	text += (margin + "<" + GetName());
	
	std::map<std::string, std::string>::const_iterator it = _values.begin();
	std::map<std::string, std::string>::const_iterator itEnd = _values.end();

	for (; it != itEnd; it++)
		text += (" " + it->first + "=\"" + it->second + "\"");

	bool isNotOver;

	if (_children.GetSize())
	{
		if (_value.size())
		{
			text += ">\n";
			text += _value;
			text += "\n";
		}
		else
		{
			text += ">\n";
		}
		// Then Set Children
		LinkedList<XMLNode*>::Node* cursor = _children.GetFront();

		while (cursor)
		{
			text += cursor->GetData()->ToString();
			cursor = cursor->GetNext();
		}

		text += (margin + "</" + GetName() + ">\n");
	}
	else if (_value.size())
	{
		text += "/>";
		text += _value;
		text += (margin + "</" + GetName() + ">\n");
	}
	else // Close
	{
		text += "/>\n";
	}

	return (text);
}

#include <fstream>

void XMLNode::Export(const std::string & pathFile) const
{
	std::ofstream file(pathFile);
	file << ToString();
	file.close();
}

unsigned int hash(const char* s, unsigned int seed = 0)
{
	unsigned int hash = seed;
	while (*s)
	{
		hash = hash * 101 + *s++;
	}
	return (hash);
}

void XMLNode::SetName(const std::string & name)
{
	_name = name;
	_nameHash = hash(_name.c_str());
}

const std::string & XMLNode::GetName() const
{
	return (_name);
}

const unsigned int & XMLNode::GetNameHash() const
{
	return (_nameHash);
}

void XMLNode::SetValue(const std::string & value)
{
	_value = value;
}

const std::string & XMLNode::GetValue() const
{
	return (_value);
}

//void XMLNode::SetValue(const std::string & name, const std::string & value)
//{
//	_values[name] = value;
//}

const std::string & XMLNode::GetValue(const std::string & name) const
{
	return (_values.at(name));
}

void XMLNode::Remove()
{
	// TO FINISH !!! Probably should Delete Nodes and also childrens !!!!!!!!!!!!!!!!!!!
	_parentNode->Remove();
}

XMLNode::XMLNode()
{
	_depth = 0;
	_parent = NULL;
}

void XMLNode::PushBackChild(XMLNode * child)
{
	child->_parent = this;
	child->_depth = (_depth + 1);
	child->_parentNode = _children.PushBack(child);
	// And Actualize Depth of children of pushed node to properly locate it in tree
	child->ActualizeChildrenDepth();
}

unsigned short XMLNode::GetDepth() const
{
	return (_depth);
}

XMLNode * XMLNode::GetParent() const
{
	return (_parent);
}

LinkedList<XMLNode*> XMLNode::GetChildren() const
{
	return (_children);
}

XMLNode * XMLNode::GetNodeNamed(const std::string & name, const unsigned short nth, const unsigned short depthMax)
{
	unsigned short	cnth = 0;
	unsigned int	nameHash = hash(name.c_str());

	return (GetNodeNamedIn(cnth, nameHash, nth, depthMax));
}

XMLNode * XMLNode::GetNodeNamedWithValue(const std::string & name, const std::string & nameValue, const std::string & value, const unsigned short depthMax)
{	
	unsigned short	cnth = 0;
	unsigned int	nameHash = hash(name.c_str());

	return (GetNodeNamedWithValueIn(nameHash, nameValue, value, depthMax));
}

XMLNode * XMLNode::GetNodeNamedAtDepth(const std::string & name, const unsigned short depth)
{
	return nullptr;
}

XMLNode * XMLNode::GetNodeNamedIn(unsigned short & cnth, const unsigned int & nameHash, const unsigned short nth, const unsigned short depthMax)
{
	if (_depth > depthMax)
		return (NULL);
	if (nameHash == _nameHash)
	{
		if (cnth == nth)
		{
			return (this);
		}
		cnth += 1;
	}
	XMLNode* goodNode;

	LinkedList<XMLNode*>::Node* cursor = _children.GetFront();

	while (cursor)
	{
		if ((goodNode = cursor->GetData()->GetNodeNamedIn(cnth, nameHash, nth, depthMax)))
			return (goodNode);
		cursor = cursor->GetNext();
	}

	return (NULL);
}

XMLNode * XMLNode::GetNodeNamedWithValueIn(const unsigned int & nameHash, const std::string & nameValue, const std::string & value, const unsigned short depthMax)
{
	if (_depth > depthMax)
		return (NULL);
	if (nameHash == _nameHash)
	{
		if (_values.count(nameValue))
			if (_values.at(nameValue) == value)
				return (this);
	}
	XMLNode* goodNode;
	LinkedList<XMLNode*>::Node* cursor = _children.GetFront();
	for (; cursor; cursor = cursor->GetNext())
		if ((goodNode = cursor->GetData()->GetNodeNamedWithValueIn(nameHash, nameValue, value, depthMax)))
			return (goodNode);

	return (NULL);
}

XMLNode * XMLNode::GetNodeNamedAtDepthIn(const unsigned int& nameHash, const unsigned short depth)
{
	return nullptr;
}

void XMLNode::ActualizeChildrenDepth()
{
	if (_children.GetSize())
	{
		LinkedList<XMLNode*>::Node* cursor = _children.GetFront();
		while (cursor)
		{
			cursor->GetData()->_depth = _depth + 1;
			cursor->GetData()->ActualizeChildrenDepth();
			cursor = cursor->GetNext();
		}
	}
}

