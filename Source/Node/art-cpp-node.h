#pragma once
#include <iostream>

namespace art
{
	namespace addons
	{
		template <typename Type>
		class Node
		{
		private:
			Type value_ = NULL;
			Node<Type>* next_ = NULL;
			Node<Type>* prev_ = NULL;

			void copy(const Node<Type>* node);
			
		public:
			Node() {};
			Node(const Type& value);
			Node(const Node<Type>* node);
			~Node();

			Type get_value() const;
			void set_value(const Type& value);

			Node<Type>* get_next() const;
			void set_next(Node<Type>* node);
			
			Node<Type>* get_prev() const;
			void set_prev(Node<Type>* node);

			template<typename AnyType>
			static bool is_null(const AnyType& node);

			template <typename AnyType>
			friend std::ostream& operator<<(std::ostream& out, const Node<AnyType>*& node);

			template <typename AnyType>
			friend std::istream& operator>> (std::istream& in, Node<AnyType>*& node);
		};

		template <typename AnyType>
		std::ostream& operator<<(std::ostream& out, const Node<AnyType>*& node)
		{
			out << node->get_value();
			return out;
		}

		template <typename AnyType>
		std::istream& operator>>(std::istream& in, Node<AnyType>*& node)
		{
			AnyType value;
			in >> value;
			node->set_value(value);
			
			return in;
		}

		template <typename Type>
		void Node<Type>::copy(const Node<Type>* node)
		{
			set_value(node->get_value());
		}

		template <typename Type>
		Node<Type>::Node(const Type& value)
		{
			set_value(value);
		}

		template <typename Type>
		Node<Type>::Node(const Node<Type>* node)
		{
			copy(node);
		}

		template <typename Type>
		Node<Type>::~Node()
		{
			if(!is_null(get_next()))
			{
				get_next()->set_prev(get_prev());
			}
			if (!is_null(get_prev()))
			{
				get_prev()->set_next(get_next());
			}
		}

		template <typename Type>
		Type Node<Type>::get_value() const
		{
			return value_;
		}

		template <typename Type>
		void Node<Type>::set_value(const Type& value)
		{
			value_ = value;
		}

		template <typename Type>
		Node<Type>* Node<Type>::get_next() const
		{
			return next_;
		}

		template <typename Type>
		void Node<Type>::set_next(Node<Type>* node)
		{
			next_ = node;
		}

		template <typename Type>
		Node<Type>* Node<Type>::get_prev() const
		{
			return prev_;
		}

		template <typename Type>
		void Node<Type>::set_prev(Node<Type>* node)
		{
			prev_ = node;
		}

		template <typename Type>
		template <typename AnyType>
		bool Node<Type>::is_null(const AnyType& node)
		{
			return node == NULL;
		}
	}
}