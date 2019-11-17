#ifndef _NTREE_H_
#define _NTREE_H_

#include <vector>

template<typename T>
class NTree
{
public:
	NTree();
	~NTree();

	template <typename T>
	class Node
	{
	public:
		Node();
		~Node();

		void AddChild(T* child);
		const std::vector<T*>& GetNodes();

	private:
		
		std::string		description;
		T*				parent;
		std::vector<T*> nodes;
	};

	Node*	GetRoot();
private:
	Node*	_root; // ROOT IS EMPTY
};

#endif /* !_NTREE_H_ */

template<typename T>
inline NTree<T>::Node<T>::Node()
{
	parent = NULL;
}

template<typename T>
inline NTree<T>::Node<T>::~Node()
{
}

template<typename T>
inline void NTree<T>::Node<T>::AddChild(T * child)
{
	child->parent = this;
	nodes.push_back(child);
}

template<typename T>
inline const std::vector<T*>& NTree<T>::Node<T>::GetNodes()
{
	return (nodes);
}

template<typename T>
inline NTree<T>::NTree()
{
	_root = new Node<T>();
}

template<typename T>
inline NTree<T>::~NTree()
{
}

template<typename T>
inline Node * NTree<T>::GetRoot()
{
	return NULL;
}
