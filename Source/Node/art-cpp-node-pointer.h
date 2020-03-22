#pragma once
#include <iostream>
#include "art-cpp-node.h"

namespace art
{
	namespace addons
	{
		template <typename Type>
		class NodePointer
		{
		private:
			Node<Type>* node_ = NULL;

			void copy(const NodePointer<Type>*& node_pointer);
			static void throw_exception();

		public:
			NodePointer() {};
			NodePointer(const Node<Type>*& node);
			NodePointer(const NodePointer<Type>*& node_pointer);
			~NodePointer(){};

			Node<Type>* get_node() const;
			void set_node(Node<Type>* node);

			bool is_null() const;
			
			NodePointer<Type>& operator++();
			NodePointer<Type> operator++(int);

			NodePointer<Type>& operator--();
			NodePointer<Type> operator--(int);
		};

		template <typename Type>
		void NodePointer<Type>::copy(const NodePointer<Type>*& node_pointer)
		{
			set_node(node_pointer->get_node());
		}

		template <typename Type>
		void NodePointer<Type>::throw_exception()
		{
			throw "Error: out of range";
		}

		template <typename Type>
		NodePointer<Type>::NodePointer(const Node<Type>*& node)
		{
			set_node(node);
		}

		template <typename Type>
		NodePointer<Type>::NodePointer(const NodePointer<Type>*& node_pointer)
		{
			copy(node_pointer);
		}

		template <typename Type>
		Node<Type>* NodePointer<Type>::get_node() const
		{
			return node_;
		}

		template <typename Type>
		void NodePointer<Type>::set_node(Node<Type>* node)
		{
			node_ = node;
		}

		template <typename Type>
		bool NodePointer<Type>::is_null() const
		{
			return Node<Type>::is_null(get_node());
		}

		template <typename Type>
		NodePointer<Type>& NodePointer<Type>::operator++()
		{
			if (this->is_null() || Node<Type>::is_null(get_node()->get_next()))
			{
				throw_exception();
			}

			set_node(get_node()->get_next());
			return *this;
		}

		template <typename Type>
		NodePointer<Type> NodePointer<Type>::operator++(int)
		{
			++(*this);
			return *this;
		}

		template <typename Type>
		NodePointer<Type>& NodePointer<Type>::operator--()
		{
			if (this->is_null() || Node<Type>::is_null(get_node()->get_prev()))
			{
				throw_exception();
			}

			set_node(get_node()->get_prev());
			return *this;
		}

		template <typename Type>
		NodePointer<Type> NodePointer<Type>::operator--(int)
		{
			--(*this);
			return *this;
		}
	}
}