#pragma once

template <class O>
class LinkedList
{
public:

	class Node
	{
	public:
		Node(LinkedList* list, Node* prev, Node* next, O data)
		{
			_list = list;
			_prev = prev;
			_next = next;
			_data = data;
			_list->_size += 1;
			if (_next)
				_next->_prev = this;
			if (_prev)
				_prev->_next = this;
		}

		Node*	GetNext() const { return (_next); }
		Node*	GetPrev() const { return (_prev); }
		O		GetData() const { return (_data); }

		Node*	Remove() // Return Next so new one at this postion
		{
			if (_prev)
				_prev->_next = _next;
			else // Is Front
				_list->_front = _next;
			if (_next)
				_next->_prev = _prev;
			else // Is Back
				_list->_back = _prev;
			_list->_size -= 1;
			return (_next);
		}

	private:
		O				_data;
		Node*			_prev;
		Node*			_next;
		LinkedList*		_list;
	};

	Node*		PushFront(O data)
	{
		if (_front == NULL)
			return (FirstNode(data));
		Node*	node = new Node(this, NULL, _front, data);
		_front = node;
		return (node);
	}

	Node*		PushBack(O data)
	{
		if (_front == NULL)
			return (FirstNode(data));

		Node*	node = new Node(this, _back, NULL, data);
		_back = node;
		return (node);
	}

	LinkedList();
	virtual ~LinkedList();

	Node*			GetFront() const { return (_front); }
	Node*			GetBack() const { return (_back); }
	const size_t&	GetSize() const { return (_size); }

	void			Clear()
	{
		Node*	cursor = _front;
		Node*	cursorOld = NULL;

		while (cursor)
		{
			if (cursorOld)
				delete (cursorOld);
			cursorOld = cursor;
			cursor = cursor->GetNext();
		}
		if (cursorOld)
			delete (cursorOld);
	}

private:
	

	Node*		FirstNode(O data)
	{
		Node*	node = new Node(this, NULL, NULL, data);
		_front = node;
		_back = node;
		return (node);
	}

	size_t		_size;
	Node*		_front;
	Node*		_back;
};

template<class O>
inline LinkedList<O>::LinkedList()
{
	_front = NULL;
	_back = NULL;
	_size = 0;
}

template<class O>
inline LinkedList<O>::~LinkedList()
{
	Clear();
}
