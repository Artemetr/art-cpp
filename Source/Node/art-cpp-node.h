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
			Node<Type>* forward_ = NULL;
			Node<Type>* backward_ = NULL;

			void copy(const Node<Type>& node);
			static void throw_exception();
			
		public:
			Node();
			Node(Type value);
			Node(const Node<Type>& node);
			~Node();

			Node<Type>& operator= (const Node<Type>& node);
			Node<Type>& operator++ ();
			Node<Type>& operator-- ();

			Type get_value() const;
			void set_value(const Type value);

			Node<Type>* get_forward();
			void set_forward(Node<Type>* node);
			
			Node<Type>* get_backward();
			void set_backward(Node<Type>* node);

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
		Node<Type>::Node()
		{}

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
			if(!Node::is_nullable(this->get_backward()))
			{
				this->get_backward()->set_forward(this->get_forward());
			}

			if (!Node::is_nullable(this->get_forward()))
			{
				this->get_forward()->set_backward(this->get_backward());
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
			if (Node::is_nullable(this->get_forward()))
			{
				this->throw_exception();
			}
			return this->get_forward();
		}

		template <typename Type>
		Node<Type>& Node<Type>::operator--()
		{
			if (Node::is_nullable(this->get_backward()))
			{
				this->throw_exception();
			}
			return this->get_backward();
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
		Node<Type>* Node<Type>::get_forward()
		{
			return this->forward_;
		}

		template <typename Type>
		void Node<Type>::set_forward(Node<Type>* node)
		{
			this->forward_ = node;
		}

		template <typename Type>
		Node<Type>* Node<Type>::get_backward()
		{
			return this->backward_;
		}

		template <typename Type>
		void Node<Type>::set_backward(Node<Type>* node)
		{
			this->backward_ = node;
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