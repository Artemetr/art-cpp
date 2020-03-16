#pragma once
#include "../Node/art-cpp-node.h"
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
			Node<Type>* current_ = NULL;
			
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
		public:
			List();
			explicit List(const Type value);
			List(const Type array[], const int length);
			~List();

			/*Adds its argument as a single element to the end of a list.
			 *The length of the list increases by one.*/
			void append(const Type value);
			void append(const Type array[], const int length);
			void append(const List<Type>& node);

			/*Inserts the element at the given index, shifting elements to the right*/
			void insert(const Type value, const int index);
			void insert(const Type array[], const int length, const int index);
			void insert(const List<Type>& node, const int index);

			/*Iterates over its argument and adding each element to the list and extending the list.
			 *The length of the list increases by number of elements in it’s argument.*/
			void extend(const Type value);
			void extend(const Type array[], const int length);
			void extend(const List<Type>& node);

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
			
			//std::string to_string();

			Type operator[](static int index);

			template <class Function>
			List<Type> map(Function function);
			template <class Function>
			Type* map(Function function);

			template <class Function>
			List<Type> filter(Function function);
			template <class Function>
			Type* filter(Function function);

			//template <typename AnyType>
			//friend std::ostream& operator<< (std::ostream& out, const List<AnyType>& list);
		};

		template <typename Type>
		void List<Type>::initializate(const Type value)
		{
			this->if_not_empty_execute(List::throw_already_initialized);

			Node<Type>* node = new Node<Type>(value);
			
			this->start_ = node;
			this->end_ = node;
			this->current_ = node;
			
			++this->length_;
		}

		template <typename Type>
		void List<Type>::go_to_node(const int index)
		{
			this->if_index_not_in_list(List::throw_index_out_of_range, index);

			if(this->length()/2 > index)
			{
				this->current_ = this->start_;
				for(int i = 0; i < index; i++)
				{
					++this->current_;
				}
			} else
			{
				this->current_ = this->end_;
				for(int i = this->length() - 1; i > index; --i)
				{
					--this->current_;
				}
			}
		}

		template <typename Type>
		template <class Function>
		void List<Type>::if_empty_execute(Function function)
		{
			if(this->is_empty())
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
			if(index < 0 || index >= this->length())
			{
				function();
			}
		}

		template <typename Type>
		void List<Type>::del_start()
		{
			this->if_empty_execute(List::throw_index_out_of_range);

			if(this->length() == 1)
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
				this->current_ = node;

				++this->length_;
				break;
			}
		}

		template <typename Type>
		void List<Type>::append(const Type array[], const int length)
		{
			for(int i = 0; i < length; i++)
			{
				this->append(array[i]);
			}
		}

		template <typename Type>
		void List<Type>::append(const List<Type>& node)
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
			switch (index)
			{
			case 0:
				this->extend(value);
				break;
			case this->length() - 1:
				this->append(value);
				break;
			default:
				this->go_to_node(index);

				Node<Type>* node = new Node<Type>(value);
				
				node->set_next(this->current_);
				node->set_prev(this->current_->get_prev());
				this->current_->set_prev(node);

				++this->length_;
				break;
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
		void List<Type>::insert(const List<Type>& node, const int index)
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
				this->current_ = node;
				
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
		void List<Type>::extend(const List<Type>& node)
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
			
			switch (index)
			{
			case 0:
				this->del_start();
				break;
			case this->length() - 1:
				this->del_end();
				break;
			default:
				this->go_to_node(index);
				delete this->current_;
				this->current_ = this->start_;
				--this->length_;
				break;
			}
		}

		template <typename Type>
		void List<Type>::remove(const Type value)
		{
			if(this->is_empty())
			{
				return;
			}

			this->current_ = this->start_;
			for (int i = 0; i < this->length(); i++)
			{
				if(this->current_->get_value() == value)
				{
					this->del(i);
					break;
				}

				if (i != this->length() - 1)
				{
					++this->current_;
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
			while(this->length() > 1)
			{
				this->del_start();
			}

			/*This place is memory stiller location.*/
			delete this->current_;
			
			this->start_ = NULL;
			this->end_ = NULL;
			this->current_ = NULL;
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
			
			this->current_ = this->start_;
			for(int i = 0; i < this->length(); i++)
			{
				array[i] = this->current_->get_value();
				
				if(i != this->length() - 1)
				{
					++this->current_;
				}
			}

			return array;
		}

		template <typename Type>
		Type List<Type>::operator[](int index)
		{
			this->go_to_node(index);

			return this->current_->get_value();
		}

		template <typename Type>
		template <class Function>
		List<Type> List<Type>::map(Function function)
		{
			if(this->is_empty())
			{
				return List<Type>();
			}

			List<Type> list = new List<Type>();

			this->current_ = this->start_;
			for (int i = 0; i < this->length(); i++)
			{
				list.append(function(this->current_->get_value()));

				if (i != this->length() - 1)
				{
					++this->current_;
				}
			}
			
			return list;
		}

		template <typename Type>
		template <class Function>
		Type* List<Type>::map(Function function)
		{
			if (this->is_empty())
			{
				return new Type[0];
			}

			Type* array = new Type[this->length()];

			this->current_ = this->start_;
			for (int i = 0; i < this->length(); i++)
			{
				array[i] = function(this->current_->get_value());

				if (i != this->length() - 1)
				{
					++this->current_;
				}
			}

			return array;
		}

		template <typename Type>
		template <class Function>
		List<Type> List<Type>::filter(Function function)
		{
			if (this->is_empty())
			{
				return List<Type>();
			}

			List<Type> list = new List<Type>();

			this->current_ = this->start_;
			for (int i = 0; i < this->length(); i++)
			{
				if (function(this->current_->get_value()))
				{
					list.append(this->current_->get_value());
				}

				if (i != this->length() - 1)
				{
					++this->current_;
				}
			}

			return list;
		}

		template <typename Type>
		template <class Function>
		Type* List<Type>::filter(Function function)
		{
			List<Type> list = this->filter(function);
			return list.to_array();
		}

		template <typename Type>
		List<Type>* to_list(const Type array[], const int length)
		{
			return new List<Type>(array, length);
		}

		template <typename Type>
		Type* to_array(List<Type>& list)
		{
			return list.to_array();
		}

		template <typename Type, class Function>
		List<Type> map(Function function, List<Type>& list)
		{
			return list.map(function);
		}

		template <typename Type, class Function>
		Type* map(Function function, List<Type>& list)
		{
			return list.map(function);
		}

		template <typename Type, class Function>
		List<Type> filter(Function function, List<Type>& list)
		{
			return list.filter(function);
		}

		template <typename Type, class Function>
		Type* filter(Function function, List<Type>& list)
		{
			return list.filter(function);
		}
	}
}
