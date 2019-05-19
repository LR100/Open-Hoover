#pragma once

#include <deque>
#include <vector>
#include <sstream> // for ToString methods

#include "Maths/AABB.h"
#include "Tools/DList.h"

/// //// ///
////TREE////
/// //// ///

/////////////////// TMPPPPPPPPPPPPPPPPP //////////////

#include "Graphic/IDrawer2D.h"
#include "Graphic/IWindow.h"

extern IDrawer2D*	QuadtreeDrawer;
extern IWindow*		QuadtreeWindow;

/////////////////// TMPPPPPPPPPPPPPPPPP END //////////////

// QuadTree AABB to fit and subdivide all AABB
// to the best precise and smallest location (sorted by size)
template <class T, class O>
class QuadTreeAABB2T
{
public:
	QuadTreeAABB2T(const AABB2T<T>& _boundary, unsigned short maxDepth)
	{
		_nodeCount = 1;
		_depthMax = maxDepth;
		_depthSizes = new Vec2[_depthMax];
		_depthSizes[0] = Vec2((float)_boundary.halfSize.x, (float)_boundary.halfSize.y);
		for (unsigned short i = 1; i < _depthMax; i += 1) // Init Size Per Depth
			_depthSizes[i] = (_depthSizes[i - 1] * 0.5f);

		_root = new QuadTreeAABB2T::Node(_boundary, NULL, this, 0);
	}
	virtual ~QuadTreeAABB2T() {};

	/// //// ///
	////NODE//// ////////////////////////////
	/// //// ///

	typedef class Node
	{
	public:

		friend class QuadTreeAABB2T;

		Node(Node* parent, QuadTreeAABB2T<T, O>* tree, const unsigned short& depth)
		{
			_parent = parent;
			_tree = tree;
			_depth = depth;
			_childrenIndexesCount = 0;
			_children = new Node*[4];
			_children[0] = NULL; // NW
			_children[1] = NULL; // NE
			_children[2] = NULL; // SW
			_children[3] = NULL; // SE

			// Others
			//_indexes.reserve(10); // Reserve At Least 10 Indexes
		}
		Node(const AABB2T<T>& boundary, Node* parent, QuadTreeAABB2T<T, O>* tree, const unsigned short& depth)
		{
			_boundary = boundary;
			_parent = parent;
			_tree = tree;
			_depth = depth;
			_childrenIndexesCount = 0;
			_children = new Node*[4];
			_children[0] = NULL; // NW
			_children[1] = NULL; // NE
			_children[2] = NULL; // SW
			_children[3] = NULL; // SE

			// Others
			_boundary.ComputeMinMax();
			//_indexes.reserve(10); // Reserve At Least 10 Indexes
		}
		virtual ~Node() {};

		// INDEX /////////////////////////////////

		// Index is linked to an extern AABB
		// If AABB is modified ask for a Relocation
		class Index
		{
		public:
			friend class Node;

			Index(const AABB2T<T>& aabb, const size_t& id, O data) : _aabb(aabb), _id(id), _data(data)
			{
				_node = NULL;
				_indexNode = NULL;
				//std::cout << "New Index AABB C( " << aabb.center.ToString() << " )";
				//std::cout << " S( " << aabb.halfSize.ToString() << " )" << std::endl;
			}
			virtual ~Index() {};

			//// METHODS /////

			/// Getters ///

			const O				GetData() const { return (_data); };
			const size_t&		GetID() const { return (_id); };
			const AABB2T<T>&	GetAABB() const { return (_aabb); };

			/// Setters ///

			// Remove Index from his Node
			void				Remove() { _node->RemoveIndex(this); };
			// Relocate Index at Good Postion from Top of Tree
			bool				RelocateFromTop() { return (_node->RelocateIndexFromTop(this)); };
			// Relocate Index at Good Postion from Current Location
			bool				RelocateFromBot() { return (_node->RelocateIndexFromBot(this)); };


		private:

			typename DList<Index*>::Node*	_indexNode;
			const AABB2T<T>&				_aabb;
			const size_t					_id;
			Node*							_node;
			O 								_data;

		};
		//////////////////////////////////////////////////////////////////////////////////////


		//// METHODS /////

		/// Getters ///

		const DList<Index*>&		GetIndexes() const { return (_indexes); };
		const std::vector<Index*>&	GetIndexesParents() const { return (_indexesParents); };
		const AABB2T<T>&			GetBoundary() const { return (_boundary); };
		const Node**				GetChildren() const { return ((const Node**)_children); };
		const unsigned int&			GetChildrenIndexesCount() const { return (_childrenIndexesCount); };
		const Node*					GetParent() const { return (_parent); };
		const QuadTreeAABB2T*		GetTree() const { return (_tree); };
		const unsigned short&		GetDepth() const { return (_depth); };

		bool						IsEmpty() const { return (_childrenIndexesCount == 0); };
		inline bool					IsLeaf() const { return (_children[0] == NULL); };

		const Node*					GetDeepestNodeAtPosition(const Vec2T<T> & position) const
		{
			if (_boundary.ContainsPoint(position))
			{
				if (_children[0] == NULL)
				{
					return (this); // Last Node Containing Point
				}
				else
				{
					const Node* goodNode;
					if ((goodNode = _children[0]->GetDeepestNodeAtPosition(position))) { return (goodNode); }
					if ((goodNode = _children[1]->GetDeepestNodeAtPosition(position))) { return (goodNode); }
					if ((goodNode = _children[2]->GetDeepestNodeAtPosition(position))) { return (goodNode); }
					if ((goodNode = _children[3]->GetDeepestNodeAtPosition(position))) { return (goodNode); }
				}
			}
			return (NULL);
		}

		void						GetIndexesAtPosition(const Vec2T<T>& position, std::vector<const Index*>& indexes) const
		{
			if (_boundary.ContainsPoint(position))
			{
				if (_indexes.GetSize())
				{
					typename DList<Index*>::Node* cursor = _indexes.GetFront();
					for (; cursor; cursor = cursor->next)
					{
				//for (size_t i = 0; i < _indexes.size(); i += 1)
				//{
						
						if (cursor->data->GetAABB().ContainsPoint(position))
							indexes.push_back(cursor->data);
				//}
					}
				}

				if (_children[0] != NULL)
				{
					_children[0]->GetIndexesAtPosition(position, indexes);
					_children[1]->GetIndexesAtPosition(position, indexes);
					_children[2]->GetIndexesAtPosition(position, indexes);
					_children[3]->GetIndexesAtPosition(position, indexes);
				}
			}
		}

		/// Setters ///

		bool						Subdivide()
		{
			if (_depth > _tree->GetMaxDepth())
				return (false);
			//std::cout << "Subdivide" << std::endl;

			// Compute Size of Children Boundaries
			_tree->_nM.vec.x = (T)(_tree->_depthSizes[_depth].x);
			_tree->_nM.vec.y = (T)(_tree->_depthSizes[_depth].y);

			// northWest
			_children[0] = _tree->GetReleasedNode(this, (_depth + 1));
			_children[0]->_boundary.halfSize.x = _tree->_nM.vec.x;
			_children[0]->_boundary.halfSize.y = _tree->_nM.vec.y;
			_children[0]->_boundary.center.x = (_boundary.center.x - _tree->_nM.vec.x);
			_children[0]->_boundary.center.y = (_boundary.center.y - _tree->_nM.vec.y);
			_children[0]->_boundary.ComputeMinMax();

			// northEast
			_children[1] = _tree->GetReleasedNode(this, (_depth + 1));
			_children[1]->_boundary.halfSize.x = _tree->_nM.vec.x;
			_children[1]->_boundary.halfSize.y = _tree->_nM.vec.y;
			_children[1]->_boundary.center.x = (_boundary.center.x + _tree->_nM.vec.x);
			_children[1]->_boundary.center.y = (_boundary.center.y - _tree->_nM.vec.y);
			_children[1]->_boundary.ComputeMinMax();

			// southWest
			_children[2] = _tree->GetReleasedNode(this, (_depth + 1));
			_children[2]->_boundary.halfSize.x = _tree->_nM.vec.x;
			_children[2]->_boundary.halfSize.y = _tree->_nM.vec.y;
			_children[2]->_boundary.center.x = (_boundary.center.x - _tree->_nM.vec.x);
			_children[2]->_boundary.center.y = (_boundary.center.y + _tree->_nM.vec.y);
			_children[2]->_boundary.ComputeMinMax();

			// southEast
			_children[3] = _tree->GetReleasedNode(this, (_depth + 1));
			_children[3]->_boundary.halfSize.x = _tree->_nM.vec.x;
			_children[3]->_boundary.halfSize.y = _tree->_nM.vec.y;
			_children[3]->_boundary.center.x = (_boundary.center.x + _tree->_nM.vec.x);
			_children[3]->_boundary.center.y = (_boundary.center.y + _tree->_nM.vec.y);
			_children[3]->_boundary.ComputeMinMax();

			return (true);
		}

		std::string	ToString() const
		{
			std::stringstream ss;
			ss << "Addr (" << (size_t)this << ") Depth (" << _depth << ") Boundary (" << _boundary.ToString() << ") Indexes (" << _indexes.GetSize() << ")";
			return (ss.str());
		}

	protected:

		void			SetIndexesParents(const std::vector<Index*>& indexes) { _indexesParents = indexes; }

		//private:

		void						ReleaseChildren()
		{
			_children[0] = _tree->SetReleasedNode(_children[0]); // Set Children Indicator to NULL
			_children[1] = _tree->SetReleasedNode(_children[1]); // And also preload Children 
			_tree->SetReleasedNode(_children[2]);
			_tree->SetReleasedNode(_children[3]);
		}

		void						ReleaseChildrenPrepared()
		{
			_children[1] = _tree->SetReleasedNode(_children[1]); // And also preload Children 
			_tree->SetReleasedNode(_children[2]);
			_tree->SetReleasedNode(_children[3]);
		}

		void						ReleaseChildrenAll()
		{
			if (!IsLeaf())
			{
				_children[0]->ReleaseChildrenAll();
				_children[1]->ReleaseChildrenAll();
				_children[2]->ReleaseChildrenAll();
				_children[3]->ReleaseChildrenAll();

				ReleaseChildren();
			}
			else if (_children[1] != NULL)
			{
				ReleaseChildrenPrepared();
			}
		}

		// Index Functions
		bool						InsertIndex(Index * index)
		{
			//std::cout << "Insert Index" << std::endl;

			if (_boundary.ContainsAABB(index->GetAABB()))
			{
				if (_children[0] == NULL)
				{
					if (_children[1] == NULL) // Children Are Not Prepared
					{
						if (!Subdivide())
							return (EmplaceIndex(index));
					}
					else // Children Are Prepared
					{
						// Reset NorthWest
						_children[0] = _tree->GetReleasedNode(this, (_depth + 1));
						_children[0]->_boundary.halfSize.x = _children[1]->_boundary.halfSize.x;
						_children[0]->_boundary.halfSize.y = _children[1]->_boundary.halfSize.y;
						_children[0]->_boundary.center.x = (_boundary.center.x - _children[1]->_boundary.halfSize.x);
						_children[0]->_boundary.center.y = (_boundary.center.y - _children[1]->_boundary.halfSize.y);
						_children[0]->_boundary.ComputeMinMax();
					}
					// Check Can probably be improved
					if (_children[0]->_boundary.ContainsAABB(index->GetAABB()))
						_tree->_nM.pos = 0;
					else if (_children[3]->_boundary.ContainsAABB(index->GetAABB()))
						_tree->_nM.pos = 3;
					else if (_children[1]->_boundary.ContainsAABB(index->GetAABB()))
						_tree->_nM.pos = 1;
					else if (_children[2]->_boundary.ContainsAABB(index->GetAABB()))
						_tree->_nM.pos = 2;
					else
					{
						_children[0] = _tree->SetReleasedNode(_children[0]);
						return (EmplaceIndex(index));
					}
					return (_children[_tree->_nM.pos]->InsertIndex(index));
				}
				else // NO Subdivide - Children Exists
				{
					// Check Can probably be improved
					if (_children[0]->_boundary.ContainsAABB(index->GetAABB()))
						return (_children[0]->InsertIndex(index));
					else if (_children[3]->_boundary.ContainsAABB(index->GetAABB()))
						return (_children[3]->InsertIndex(index));
					else if (_children[1]->_boundary.ContainsAABB(index->GetAABB()))
						return (_children[1]->InsertIndex(index));
					else if (_children[2]->_boundary.ContainsAABB(index->GetAABB()))
						return (_children[2]->InsertIndex(index));
					else
					{
						return (EmplaceIndex(index));
					}
				}
			}
			return (false);
		}

		bool						EmplaceIndex(Index* index)
		{
			if (index->_node == this)
			{
				
				//std::cout << "SAME Emplace" << std::endl;
				// Already Inside (Not Inserted -> false)
				// Or Node is FULL
				return (false);
			}
			
			//std::cout << "Emplacing GO at (" << (size_t)this << ")" << std::endl;
			// If (Was Inside a indexes node) -> Remove It
			

			//index->_pos = _indexes.size();

			/// MUST Be Befor Removing Node in case have common nodes and so deleting himself before insertion
			
			DList<Index*>::Node* nnode = _indexes.PushBack(index);
			//std::cout << "Indexes Size (" << _indexes.GetSize() << ")" << std::endl;
			this->ModifyParentIndexesCount(1);
			//_indexes.push_back(index);

			if (index->_node)
				index->Remove();

			index->_node = this;
			index->_indexNode = nnode;
			// At Every New Insert Notify this Node is FRESH

			//std::cout << "Emplacing At (" << ToString() << ")" << std::endl;
			//std::cout << "OK Emplace" << std::endl;
			//std::cout << "New Objects Size (" << objects.size() << ")" << std::endl;
			//std::cout << "Is Inserted In AABB " << _boundary.ToString() << "" << std::endl;
			return (true);
		}

		bool						RelocateIndexFromTop(Index* index)
		{
			//std::cout << " GO Relocate From Top " << std::endl;

			//_tree->_nM.posOld = index->_pos; // Old Position in Objects
			_tree->_nM.cursor = _tree->GetRoot(); // Top Of Tree

			if (_tree->_nM.cursor->InsertIndex(index)) // IF New Insertion -> Notify Changed and removed from this node
			{
				/// OBJECT IS RELOCATED -> -1 To ChildrenObjectCount
				//std::cout << " RELOCATED " << std::endl;
				
				//std::cout << "Relocated Node at Depth (" << _depth << ")" << std::endl;
				//std::cout << "Old Objects Size (" << objects.size() << ")" << std::endl;
				//_indexes.erase(_indexes.begin() + _tree->_nM.posOld); // Remove Object from his oldPos

				////std::cout << "New Objects Size (" << objects.size() << ")" << std::endl;
				//for (_tree->_nM.i = _tree->_nM.posOld; _tree->_nM.i < _indexes.size(); _tree->_nM.i += 1)
				//{
				//	_indexes.at(_tree->_nM.i)->_pos -= 1; // ReOffset all others objects
				//}
				//this->ModifyParentIndexesCount(-1);
			}
			return (true);
		}

		bool						RelocateIndexFromBot(Index* index)
		{
			//std::cout << "\nRelocate Object From Bot" << std::endl;
			//_tree->_nM.posOld = index->_pos; // Old Position in Objects
			_tree->_nM.cursor = index->_node;

			while (_tree->_nM.cursor != NULL && !_tree->_nM.cursor->_boundary.ContainsAABB(index->_aabb))
			{
				//std::cout << "Getting Location From Bot" << std::endl;
				_tree->_nM.cursor = _tree->_nM.cursor->_parent;
			}
			if (_tree->_nM.cursor != NULL)
			{
				if (_tree->_nM.cursor->InsertIndex(index)) // IF New Insertion -> Notify Changed and removed from this node
				{
					//std::cout << "Relocated Node at Depth (" << index->_node->ToString() << ")" << std::endl;
					//std::cout << "Old Objects Size (" << objects.size() << ")" << std::endl;

					
					//_indexes.erase(_indexes.begin() + _tree->_nM.posOld); // Remove Object from his oldPos

					////std::cout << "New Objects Size (" << objects.size() << ")" << std::endl;
					//for (_tree->_nM.i = _tree->_nM.posOld; _tree->_nM.i < _indexes.size(); _tree->_nM.i += 1)
					//{
					//	_indexes.at(_tree->_nM.i)->_pos -= 1; // ReOffset all others objects
					//}
					//this->ModifyParentIndexesCount(-1);
				}
				else
				{
					//std::cout << "Relocated At Same Node " << _depth << ")" << std::endl;
				}
				//_tree->NotifyChangedNode(index->node);
				return (true);
			}
			//std::cout << "END Relocate No Insert" << std::endl;
			return (false);
		}

		void						RemoveIndex(Index* index)
		{
			//std::cout << "Remove Index From His Node" << std::endl;
			//_indexes.erase(_indexes.begin() + index->_pos); // Remove Object from his Position
			if (index->_indexNode)
			{
				index->_indexNode->Remove();
				index->_indexNode = NULL;
				this->ModifyParentIndexesCount(-1);
			}
		}

		void						ModifyParentIndexesCount(const int& modificator)
		{
			//std::cout << "BF Children Indexes Count : (" << _childrenIndexesCount << ") Modificator (" << modificator << ")" << std::endl;
			_childrenIndexesCount += modificator;
			//std::cout << "Children Indexes Count : (" << _childrenIndexesCount << ")" << std::endl;
			if (_childrenIndexesCount == 0)
			{
				//std::cout << "Remove All Children From This Node" << std::endl;
				ReleaseChildrenAll();
			}
			if (_parent)
				_parent->ModifyParentIndexesCount(modificator);
		}

		DList<Index*>			_indexes;
		std::vector<Index*>		_indexesParents;
		AABB2T<T>				_boundary;
		Node**					_children;
		unsigned int			_childrenIndexesCount;
		Node*					_parent;
		QuadTreeAABB2T*			_tree;
		unsigned short			_depth;
	};

	/// //// ///////////////////////////////////////////////////////////////////////////
	////TREE////
	/// //// ///

	//// METHODS /////

	/// Getters ///

	Node*								GetRoot() const { return (_root); };
	const unsigned short&				GetMaxDepth() const { return (_depthMax); };
	const unsigned int&					GetIndexesCount() const { return (_root->GetChildrenIndexesCount()); };

	/*std::vector<const Node::Index*>		GetIndexesAtAABB(const AABB& aabb) const
	{
		std::vector<const typename QuadTreeAABB2T<T, O>::Node::Index*> indexes;

		return (indexes);
	}*/


	const Node*	GetDeepestNodeAtPosition(const Vec2T<T>& position) const
	{
		return (_root->GetDeepestNodeAtPosition(position));
	}

	std::vector<const typename QuadTreeAABB2T<T, O>::Node::Index*>		GetIndexesAtPosition(const Vec2T<T>& position) const
	{
		std::cout << "std::vector<const typename QuadTreeAABB2T<T, O>::Node::Index*>		GetIndexesAtPosition(const Vec2T<T>& position) const" << std::endl;
		std::vector<const typename QuadTreeAABB2T<T, O>::Node::Index*> indexes;

		_root->GetIndexesAtPosition(position, indexes);

		return (indexes);
	}
	

	/// Setters ///

	typename Node::Index*		InsertData(const AABB2T<T> & aabb, O data)
	{
		typename QuadTreeAABB2T<T, O>::Node::Index* index;
		index = new typename QuadTreeAABB2T<T, O>::Node::Index(aabb, (size_t)&data, data);

		_root->InsertIndex(index);

		return (index);
	}

	// ACCESS  For Inherited Class
protected:

	/// (To Nodes)

	void																NodeSetIndexesParents(Node* node, const std::vector<typename Node::Index*>& indexes) { node->SetIndexesParents(indexes); };
	std::vector<typename Node::Index*>&									NodeGetIndexesParents(Node* node) { return (node->_indexesParents); };
	DList<typename Node::Index*>&										NodeGetIndexes(Node* node) { return (node->_indexes); };
	Node**																NodeGetChildren(Node* node) { return (node->_children); };
	void																NodeReleaseChildrenAll(Node* node) { node->ReleaseChildrenAll(); };


	// END ACCESS For Inherited Class
private:

	// Released Node
	Node*						SetReleasedNode(Node* node)
	{
		node->_children[0] = NULL;
		node->_children[1] = NULL;
		node->_parent = NULL;
		node->_childrenIndexesCount = 0;
		_nodeCount -= 1;
		_releasedNodes.push_front(node);
		//std::cout << "Set Released Node: (" << _releasedNodes.size() << ")" << std::endl;
		return (NULL);
	}
	Node*						UseReleasedNode(const AABB2T<T> & aabb, Node* parent, const unsigned short& depth)
	{
		_nodeCount += 1;

		//std::cout << "Use Released Node: (" << _releasedNodes.size() << ")" << std::endl;
		if (_releasedNodes.size())
		{
			Node * node = _releasedNodes.back();

			_releasedNodes.pop_back();

			node->_boundary = aabb;
			node->_boundary.ComputeMinMax();
			node->_depth = depth;
			node->_parent = parent;

			return (node);
		}
		return (new Node(aabb, parent, this, depth));
	}
	Node*						GetReleasedNode(Node* parent, const unsigned short& depth)
	{
		_nodeCount += 1;

		//std::cout << "Get Released Node: (" << _releasedNodes.size() << ")" << std::endl;
		if (_releasedNodes.size())
		{
			Node * node = _releasedNodes.back();
			_releasedNodes.pop_back();
			node->_depth = depth;
			node->_parent = parent;
			return (node);
		}
		return (new Node(parent, this, depth));;
	}


	Node*					_root;
	unsigned short			_depthMax;
	Vec2*					_depthSizes;

	// Managed Memory QuadNodes
	std::deque<Node*>		_releasedNodes;
	unsigned int			_nodeCount;

	// Optimization Struct
	struct NodeManipulation
	{
		Vec2T<T>	vec;
		size_t		pos;
		size_t		posOld;
		size_t		i;
		Node*		cursor;
	};

	NodeManipulation	_nM;
};

// Float 
template class QuadTreeAABB2T<float, AABB2>;
typedef QuadTreeAABB2T<float, AABB2> QuadTreeAABB2;

template class QuadTreeAABB2T<int, AABB2i>;
typedef QuadTreeAABB2T<int, AABB2i> QuadTreeAABB2i;

