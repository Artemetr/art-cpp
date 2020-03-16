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

			void copy(const Node<Type>& node);
			static void throw_exception();
			
		public:
			Node() {};
			Node(Type value);
			Node(const Node<Type>& node);
			~Node();

			Node<Type>& operator= (const Node<Type>& node);
			Node<Type>& operator++ ();
			Node<Type>& operator-- ();

			Type get_value() const;
			void set_value(const Type value);

			Node<Type>* get_next();
			void set_next(Node<Type>* node);
			
			Node<Type>* get_prev();
			void set_prev(Node<Type>* node);

			template<typename AnyType>
			static bool is_nullable(AnyType value);

			template <typename AnyType>
			friend std::ostream& operator<< (std::ostream& out, Node<AnyType>& node);
			template <typename AnyType>
			friend std::istream& operator>> (std::istream& in, Node<AnyType>& node);
		};

		template <typename Type>
		void Node<Type>::copy(const Node<Type>& node)
		{
			this->set_value(node.get_value());
		}

		template <typename Type>
		void Node<Type>::throw_exception()
		{
			throw "Error: going beyond!";
		}

		template <typename Type>
		Node<Type>::Node(Type value)
		{
			this->set_value(value);
		}

		template <typename Type>
		Node<Type>::Node(const Node<Type>& node)
		{
			this->copy(node);
		}

		template <typename Type>
		Node<Type>::~Node()
		{			
			if(!Node::is_nullable(this->get_prev()))
			{
				this->get_prev()->set_next(this->get_next());
			}

			if (!Node::is_nullable(this->get_next()))
			{
				this->get_next()->set_prev(this->get_prev());
			}
		}

		template <typename Type>
		Node<Type>& Node<Type>::operator=(const Node<Type>& node)
		{
			this->copy(node);
			return *this;
		}

		template <typename Type>
		Node<Type>& Node<Type>::operator++()
		{
			if (Node::is_nullable(this->get_next()))
			{
				this->throw_exception();
			}
			return this->get_next();
		}

		template <typename Type>
		Node<Type>& Node<Type>::operator--()
		{
			if (Node::is_nullable(this->get_prev()))
			{
				this->throw_exception();
			}
			return this->get_prev();
		}

		template <typename Type>
		Type Node<Type>::get_value() const
		{
			return this->value_;
		}

		template <typename Type>
		void Node<Type>::set_value(const Type value)
		{
			this->value_ = value;
		}

		template <typename Type>
		Node<Type>* Node<Type>::get_next()
		{
			return this->next_;
		}

		template <typename Type>
		void Node<Type>::set_next(Node<Type>* node)
		{
			this->next_ = node;
		}

		template <typename Type>
		Node<Type>* Node<Type>::get_prev()
		{
			return this->prev_;
		}

		template <typename Type>
		void Node<Type>::set_prev(Node<Type>* node)
		{
			this->prev_ = node;
		}

		template <typename Type>
		template <typename AnyType>
		bool Node<Type>::is_nullable(AnyType value)
		{
			return value == NULL;
		}

		template <typename AnyType>
		std::ostream& operator<< (std::ostream& out, Node<AnyType>& node)
		{
			out << node.get_value();
			return out;
		}

		template <typename AnyType>
		std::istream& operator>> (std::istream& in, Node<AnyType>& node)
		{
			AnyType type;
			in >> type;
			node.set_value(type);
			
			return in;
		}
	}
}