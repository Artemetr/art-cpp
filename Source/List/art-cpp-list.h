#pragma once
#include "../Node/art-cpp-node.h"
#include "../Node/art-cpp-node-pointer.h"
#include  <iostream>
#include <string>


namespace art
{
	namespace addons
	{
		template <typename Type>
		class List
		{
		private:
			Node<Type>* start_ = NULL;
			Node<Type>* end_ = NULL;
			NodePointer<Type>* pointer_ = new NodePointer<Type>();
			
			std::size_t length_ = 0;

			void initializate(const Type value);
			void go_to_node(const int index);

			template <class Function>
			void if_empty_execute(Function function);

			template <class Function>
			void if_not_empty_execute(Function function);

			template <class Function>
			void if_index_not_in_list(Function function, const int index);

			void del_start();
			void del_end();

			static void throw_index_out_of_range();
			static void throw_already_initialized();

			void set_pointer(Node<Type>* node) const;
			Node<Type>* get_pointer() const;
			void next() const;
			void prev() const;
			void to_start() const;
			void to_end() const;
			
		public:
			List();
			List(const Type value);
			List(const Type array[], const int length);
			List(List<Type>*& list);
			~List();

			/*Adds its argument as a single element to the end of a list.
			 *The length of the list increases by one.*/
			void append(const Type value);
			void append(const Type array[], const int length);
			void append(const List<Type>*& node);

			/*Inserts the element at the given index, shifting elements to the right*/
			void insert(const Type value, const int index);
			void insert(const Type array[], const int length, const int index);
			void insert(const List<Type>*& node, const int index);

			/*Iterates over its argument and adding each element to the list and extending the list.
			 *The length of the list increases by number of elements in it’s argument.*/
			void extend(const Type value);
			void extend(const Type array[], const int length);
			void extend(const List<Type>*& node);

			/*Remove items by index.*/
			void del(const int index);
			/*Remove an item by value.*/
			void remove(const Type value);
			/*Remove an item by index and get its value.*/
			Type pop(const int index);
			/*Remove all items.*/
			void clear();

			/*Return the length of the list.*/
			int length() const;

			/*Return the length of the list.*/
			bool is_empty() const;

			Type* to_array();
			
			std::string to_string() const;

			Type operator[](const int index);

			template <class Function>
			List<Type>* map(Function function);

			template <class Function>
			List<Type>* filter(Function function);

			template <typename AnyType>
			friend std::ostream& operator<< (std::ostream& out, const List<AnyType>& list);

			template<typename AnyType>
			friend std::istream& operator>> (std::istream& in, List<AnyType>& list);

			/*
			template <typename AnyType>
			friend std::string join(const std::string delimiter, const addons::List<AnyType>& subject);
			*/

			friend addons::List<std::string> split(const std::string delimiter, const std::string subject);
		};

		template <typename AnyType>
		std::ostream& operator<<(std::ostream& out, const List<AnyType>& list)
		{
			out << list.to_string();

			return out;
		}

		template <typename AnyType>
		std::string join(const std::string delimiter, const addons::List<AnyType>& subject)
		{
			std::string result = "";

			Node<AnyType>* node = subject.start_;

			for (int i = 0; i < subject.length(); i++)
			{
				result.append(node->get_value());

				if (i < subject.length() - 1)
				{
					result.append(delimiter);
				}
			}

			return result;
		}

		template <typename AnyType>
		std::istream& operator>>(std::istream& in, List<AnyType>& list)
		{
			AnyType value = AnyType();
			list.to_start();
			for(int i = 0; i < list.length(); i++)
			{
				in >> value;
				list.get_pointer()->set_value(value);

				if(i < list.length() - 1)
				{
					list.next();
				}
			}
			
			return in;
		}

		inline addons::List<std::string> split(const std::string delimiter, const std::string subject)
		{
			List<std::string> result;

			std::string tmp_subject = subject;

			const std::size_t delimiter_length = delimiter.length();
			std::size_t d_position = tmp_subject.find(delimiter);
			if (d_position == std::string::npos)
			{
				return result;
			}

			while (d_position != std::string::npos)
			{
				result.append(tmp_subject.substr(0, d_position));
				tmp_subject.erase(0, d_position + delimiter_length);
				d_position = tmp_subject.find(delimiter);
			}

			return result;
		}

		template <typename Type>
		void List<Type>::initializate(const Type value)
		{
			this->if_not_empty_execute(List::throw_already_initialized);

			Node<Type>* node = new Node<Type>(value);

			this->start_ = node;
			this->end_ = node;
			set_pointer(node);

			++this->length_;
		}

		template <typename Type>
		void List<Type>::go_to_node(const int index)
		{
			this->if_index_not_in_list(List::throw_index_out_of_range, index);

			if (this->length() / 2 > index)
			{
				pointer_->set_node(start_);
				for (int i = 0; i < index; i++)
				{
					next();
				}
			}
			else
			{
				pointer_->set_node(end_);
				for (int i = this->length() - 1; i > index; --i)
				{
					prev();
				}
			}
		}

		template <typename Type>
		template <class Function>
		void List<Type>::if_empty_execute(Function function)
		{
			if (this->is_empty())
			{
				function();
			}
		}

		template <typename Type>
		template <class Function>
		void List<Type>::if_not_empty_execute(Function function)
		{
			if (!this->is_empty())
			{
				function();
			}
		}

		template <typename Type>
		template <class Function>
		void List<Type>::if_index_not_in_list(Function function, const int index)
		{
			if (index < 0 || index >= this->length())
			{
				function();
			}
		}

		template <typename Type>
		void List<Type>::del_start()
		{
			this->if_empty_execute(List::throw_index_out_of_range);

			if (this->length() == 1)
			{
				this->clear();
				return;
			}

			this->start_ = this->start_->get_next();
			delete this->start_->get_prev();
			this->start_->set_prev(NULL); //mb needed

			--this->length_;
		}

		template <typename Type>
		void List<Type>::del_end()
		{
			this->if_empty_execute(List::throw_index_out_of_range);

			if (this->length() == 1)
			{
				this->clear();
				return;
			}

			this->end_ = this->end_->get_prev();
			delete this->end_->get_next();
			this->end_->set_prev(NULL); //mb needed

			--this->length_;
		}

		template <typename Type>
		void List<Type>::throw_index_out_of_range()
		{
			throw "IndexError: index out of range";
		}

		template <typename Type>
		void List<Type>::throw_already_initialized()
		{
			throw "InitialError: list has already initialized";
		}

		template <typename Type>
		void List<Type>::set_pointer(Node<Type>* node) const
		{
			pointer_->set_node(node);
		}

		template <typename Type>
		Node<Type>* List<Type>::get_pointer() const
		{
			return pointer_->get_node();
		}

		template <typename Type>
		void List<Type>::next() const
		{
			++(*pointer_);
		}

		template <typename Type>
		void List<Type>::prev() const
		{
			--(*pointer_);
		}

		template <typename Type>
		void List<Type>::to_start() const
		{
			pointer_->set_node(start_);
		}

		template <typename Type>
		void List<Type>::to_end() const
		{
			pointer_->set_node(end_);
		}

		template <typename Type>
		List<Type>::List()
		{
		}

		template <typename Type>
		List<Type>::List(const Type value)
		{
			this->initializate(value);
		}

		template <typename Type>
		List<Type>::List(const Type array[], const int length)
		{
			this->append(array, length);
		}

		template <typename Type>
		List<Type>::List(List<Type>*& list)
		{
			append(list);
		}

		template <typename Type>
		List<Type>::~List()
		{
			this->clear();
		}

		template <typename Type>
		void List<Type>::append(const Type value)
		{
			switch (this->length())
			{
			case 0:
				this->initializate(value);
				break;
			default:
				Node<Type>* node = new Node<Type>(value);

				node->set_prev(this->end_);
				this->end_->set_next(node);

				this->end_ = node;
				set_pointer(node);

				++this->length_;
				break;
			}
		}

		template <typename Type>
		void List<Type>::append(const Type array[], const int length)
		{
			for (int i = 0; i < length; i++)
			{
				this->append(array[i]);
			}
		}

		template <typename Type>
		void List<Type>::append(const List<Type>*& node)
		{
			for (int i = 0; i < node.length(); i++)
			{
				this->append(node[i]);
			}
		}

		template <typename Type>
		void List<Type>::insert(const Type value, const int index)
		{
			this->if_index_not_in_list(List::throw_index_out_of_range, index);
			const int first_case = this->length() - 1;

			if(index == 0)
			{
				this->extend(value);
			} else if (index  == length() - 1)
			{
				this->append(value);
			} else
			{
				this->go_to_node(index);

				Node<Type>* node = new Node<Type>(value);

				node->set_next(get_pointer());
				node->set_prev(get_pointer()->get_prev());
				get_pointer()->set_prev(node);

				++this->length_;
			}
		}

		template <typename Type>
		void List<Type>::insert(const Type array[], const int length, const int index)
		{
			for (int i = length - 1; i >= 0; i--)
			{
				this->insert(array[i], index);
			}
		}

		template <typename Type>
		void List<Type>::insert(const List<Type>*& node, const int index)
		{
			for (int i = length - 1; i >= 0; i--)
			{
				this->insert(node[i], index);
			}
		}

		template <typename Type>
		void List<Type>::extend(const Type value)
		{
			switch (this->length())
			{
			case 0:
				this->initializate(value);
				break;
			default:
				Node<Type>* node = new Node<Type>(value);

				node->set_prev(this->start_);
				this->start_->set_prev(node);

				this->start_ = node;
				set_pointer(node);

				++this->length_;
				break;
			}
		}

		template <typename Type>
		void List<Type>::extend(const Type array[], const int length)
		{
			for (int i = length - 1; i >= 0; i--)
			{
				this->extend(array[i]);
			}
		}

		template <typename Type>
		void List<Type>::extend(const List<Type>*& node)
		{
			for (int i = length - 1; i >= 0; i--)
			{
				this->extend(node[i]);
			}
		}

		template <typename Type>
		void List<Type>::del(const int index)
		{
			this->if_index_not_in_list(List::throw_index_out_of_range, index);

			if (index == 0)
			{
				this->del_start();
			} else if (index == this->length() - 1)
			{
				this->del_end();
			} else
			{
				this->go_to_node(index);
				delete get_pointer();
				pointer_->set_node(start_);
				--this->length_;
			}
		}

		template <typename Type>
		void List<Type>::remove(const Type value)
		{
			if (this->is_empty())
			{
				return;
			}

			pointer_->set_node(start_);
			for (int i = 0; i < this->length(); i++)
			{
				if (get_pointer()->get_value() == value)
				{
					this->del(i);
					break;
				}

				if (i != this->length() - 1)
				{
					next();
				}
			}
		}

		template <typename Type>
		Type List<Type>::pop(const int index)
		{
			this->if_index_not_in_list(List::throw_index_out_of_range, index);
			Type value = this[index];
			this->del(index);

			return value;
		}

		template <typename Type>
		void List<Type>::clear()
		{
			while (this->length() > 1)
			{
				this->del_start();
			}

			/*This place is memory stiller location.*/
			delete get_pointer();

			this->start_ = NULL;
			this->end_ = NULL;
			set_pointer(NULL);
			
			this->length_ = 0;
		}

		template <typename Type>
		int List<Type>::length() const
		{
			return this->length_;
		}

		template <typename Type>
		bool List<Type>::is_empty() const
		{
			return !(this->length() > 0);
		}

		template <typename Type>
		Type* List<Type>::to_array()
		{
			if (this->is_empty())
			{
				return new Type[0];
			}

			Type* array = new Type[this->length()];

			pointer_->set_node(start_);
			for (int i = 0; i < this->length(); i++)
			{
				array[i] = get_pointer()->get_value();

				if (i != this->length() - 1)
				{
					next();
				}
			}

			return array;
		}

		template <typename Type>
		std::string List<Type>::to_string() const
		{
			std::string result;
			result.append("[");

			to_start();
			for (int i = 0; i < this->length(); i++)
			{

					result.append(get_pointer()->get_value()->to_string());

				if (i < this->length() - 1)
				{
					next();
					result.append(", ");
				}
			}
			result.append("]");

			return result;
		}

		template <typename Type>
		Type List<Type>::operator[](const int index)
		{
			this->go_to_node(index);

			return get_pointer()->get_value();
		}

		template <typename Type>
		template <class Function>
		List<Type>* List<Type>::map(Function function)
		{
			if (this->is_empty())
			{
				return new List<Type>();
			}

			List<Type>* list = new List<Type>();

			pointer_->set_node(start_);
			for (int i = 0; i < this->length(); i++)
			{
				list->append(function(get_pointer()->get_value()));

				if (i != this->length() - 1)
				{
					next();
				}
			}

			return list;
		}

		template <typename Type>
		template <class Function>
		List<Type>* List<Type>::filter(Function function)
		{

			if (this->is_empty())
			{
				return new List<Type>();
			}

			List<Type>* list = new List<Type>();

			pointer_->set_node(start_);
			for (int i = 0; i < this->length(); i++)
			{
				if (function(get_pointer()->get_value()))
				{
					list->append(get_pointer()->get_value());
				}

				if (i != this->length() - 1)
				{
					next();
				}
			}

			return list;
		}

		template <typename Type>
		List<Type>* to_list(const Type array[], const int length)
		{
			return new List<Type>(array, length);
		}

		template <typename Type>
		Type* to_array(List<Type> & list)
		{
			return list.to_array();
		}

		template <typename Type, class Function>
		List<Type> map(Function function, List<Type> & list)
		{
			return list.map(function);
		}

		template <typename Type, class Function>
		List<Type>* map(Function function, List<Type>*& list)
		{
			return list->map(function);
		}

		template <typename Type, class Function>
		List<Type> filter(Function function, List<Type>& list)
		{
			return list.filter(function);
		}

		template <typename Type, class Function>
		List<Type>* filter(Function function, List<Type>*& list)
		{
			return list->filter(function);
		}

	}
}
