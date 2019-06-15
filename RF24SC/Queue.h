
/*
 Name:		Queue.h
 Created:	3/23/2019 9:20:14 AM
 Author:	Loic Robin
*/

template <class Q>
class Queue
{
public:

	class Node
	{
	public:
		Node() {}
		Q		data;
		Node*	next;
	};

	Queue()
	{
		_front = NULL;
		_back = NULL;
		_nodeCount = 0;
	};

	virtual ~Queue() { Clear(); };

	void	PopFront()
	{
		if (_front)
		{
			Node* node = _front;
			if (_front == _back) // Last one
				_back = NULL;
			_front = _front->next;
			delete (node);
			_nodeCount -= 1;
		}
	}

	void	PushBack(Q data)
	{
		if (!_front)
		{
			_front = new Node();
			_front->next = NULL;
			_back = _front;
		}
		else
		{
			_back->next = new Node();
			_back->next->next = NULL;
			_back = _back->next;
		}
		_back->data = data;
		_nodeCount += 1;
	};

	void		Clear()
	{
		Node*		cursor = _front;
		Node*		cursorOld = NULL;

		while (cursor != NULL)
		{
			cursorOld = cursor;
			cursor = cursor->next;
			delete (cursorOld);
		}
		_front = NULL;
		_back = NULL;
		_nodeCount = 0;
	};

	const Q&		Front() const { return (_front->data); };
	const Q&		Back() const { return (_back->data); };
	const uint32_t&	Size() const { return (_nodeCount); };
	bool			Empty() const { return (_front == NULL); };

private:

	uint32_t	_nodeCount;
	Node*		_front; // Front Is FIRST
	Node*		_back; // Back Is LAST
};