#pragma once

#include <iterator>
#include <memory>
#include <iostream>

namespace container {

template<class T, class Allocator = std::allocator<T>>
class list {
private:
    struct node_t;
    size_t size = 0;
    
public:
    struct forward_iterator {
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        explicit forward_iterator(node_t* ptr);
        T& operator*();
        forward_iterator& operator++();
        forward_iterator operator++(int);
        bool operator==(const forward_iterator& it) const;
        bool operator!=(const forward_iterator& it) const;
        private:
            node_t* ptr_;
            friend list;
    };

    forward_iterator begin();
	forward_iterator end();
	void push(const T& value);
	void push_b(const T& value);
	T& front();
	T& back();
	void popFront();
	void popBack();
	size_t length();
	bool empty();
	void erase(forward_iterator d_it);
	void erase(size_t N);
	void insert_by_it(forward_iterator ins_it, T& value);
	void insert(size_t N, T& value);
	list& operator=(list& other);
	T& operator[](size_t index);

private:
    using allocator_type = typename Allocator::template rebind<node_t>::other;
		
		struct deleter {
		private:
			allocator_type* allocator_;
		public:
			deleter(allocator_type* allocator) : allocator_(allocator) {}

			void operator() (node_t* ptr) {
				if (ptr != nullptr) {
					std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
					allocator_->deallocate(ptr, 1);
				}
			}

		};

    using unique_ptr = std::unique_ptr<node_t, deleter>;
		
		struct node_t {
			T value;
			unique_ptr next_element = { nullptr, deleter{nullptr} };
			node_t* prev_element = nullptr;
			node_t(const T& value_) : value(value_) {}
			forward_iterator next();
		};

	allocator_type allocator_{};
	unique_ptr head{ nullptr, deleter{nullptr} };
	node_t* tail = nullptr;
};

template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator list<T, Allocator>::begin() {//+
		return forward_iterator(head.get());
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator list<T, Allocator>::end() {//+
		return forward_iterator(nullptr);
	}
	template<class T, class Allocator>
	size_t list<T, Allocator>::length() {
		return size;
	}
	template<class T, class Allocator>
	bool list<T, Allocator>::empty() {
		return length() == 0;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::push(const T& value) {
		size++;
		node_t* result = this->allocator_.allocate(1);
		std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
		unique_ptr tmp = std::move(head);
		head = unique_ptr(result, deleter{ &this->allocator_ });
		head->next_element = std::move(tmp);
		if(head->next_element != nullptr)
			head->next_element->prev_element = head.get();
		if (size == 1) {
			tail = head.get();
		}
		if (size == 2) {
			tail = head->next_element.get();
		}
	}
	
	template<class T, class Allocator>
	void list<T, Allocator>::push_b(const T& value) {
		node_t* result = this->allocator_.allocate(1); 
		std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
		if (!size) {
			head = unique_ptr(result, deleter{ &this->allocator_ });
			tail = head.get();
			size++;
			return;
		}
		tail->next_element = unique_ptr(result, deleter{ &this->allocator_ });
		node_t* temp =  tail;
		tail = tail->next_element.get();
		tail->prev_element = temp;
		size++;
	}


	template<class T, class Allocator>
	void list<T, Allocator>::popFront() {
		if (size == 0) {
			throw std::logic_error("Deleting from empty list");
		}
		if (size == 1) {
			head = nullptr;
			tail = nullptr;
			size--;
			return;
		}
		unique_ptr tmp = std::move(head->next_element);
		head = std::move(tmp);
		head->prev_element = nullptr;
		size--;
	}
	
	template<class T, class Allocator>
	void list<T, Allocator>::popBack() {
		if (size == 0) {
			throw std::logic_error("Deleting from empty list");
		}
		if (tail->prev_element){
			node_t* tmp = tail->prev_element;
			tail->prev_element->next_element = nullptr;
			tail = tmp;
		}
		else{
			head = nullptr;
			tail = nullptr;
		}
		size--;
	}

	template<class T, class Allocator>
	T& list<T, Allocator>::front() {
		if (size == 0) {
			throw std::logic_error("No elements");
		}
		return head->value;
	}

	template<class T, class Allocator>
	list<T,Allocator>& list<T, Allocator>::operator=(list<T, Allocator>& other) {
		size = other.size;
		head = std::move(other.head);
	}

	template<class T, class Allocator>
	void list<T, Allocator>::erase(container::list<T, Allocator>::forward_iterator d_it) {
		if (d_it == this->end()) throw std::logic_error("Out of bounds");
		if (d_it == this->begin()) {
			this->popFront();
			return;
		}
		if (d_it.ptr_ == tail) {
			this->popBack();
			return;
		}
		if (d_it.ptr_ == nullptr) throw std::logic_error("Out of bounds");
		auto temp = d_it.ptr_->prev_element;
		unique_ptr temp1 = std::move(d_it.ptr_->next_element);
		d_it.ptr_ = d_it.ptr_->prev_element;
		d_it.ptr_->next_element = std::move(temp1);
		d_it.ptr_->next_element->prev_element = temp;
		size--;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::erase(size_t N) {
		forward_iterator it = this->begin();
		for (size_t i = 0; i < N; ++i) {
			++it;
		}
		this->erase(it);
	}

	template<class T, class Allocator>
	void list<T, Allocator>::insert_by_it(container::list<T, Allocator>::forward_iterator ins_it, T& value) {
		
		if (ins_it == this->begin()) {
			this->push(value);
			return;
		}
		if(ins_it.ptr_ == nullptr){
			this->push_b(value);
			return;
		}
        
		node_t* tmp = this->allocator_.allocate(1);
		std::allocator_traits<allocator_type>::construct(this->allocator_, tmp, value);

		tmp->prev_element = ins_it.ptr_->prev_element;
		ins_it.ptr_->prev_element = tmp;
		tmp->next_element = std::move(tmp->prev_element->next_element);
		tmp->prev_element->next_element = unique_ptr(tmp, deleter{ &this->allocator_ });

		size++;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::insert(size_t N, T& value) {
		forward_iterator it = this->begin();
		if (N >= this->length())
			it = this->end();
		else
		for (size_t i = 0; i < N; ++i) {
			++it;
		} 
		this->insert_by_it(it, value);
	}
	
	template<class T, class Allocator>
	typename list<T,Allocator>::forward_iterator list<T, Allocator>::node_t::next() {
		return forward_iterator(this->next_element.get());
	}

	template<class T, class Allocator>
	list<T, Allocator>::forward_iterator::forward_iterator(container::list<T, Allocator>::node_t *ptr) {
		ptr_ = ptr;
	}

	template<class T, class Allocator>
	T& list<T, Allocator>::forward_iterator::operator*() {
		return this->ptr_->value;
	}
	template<class T, class Allocator>
	T& list<T, Allocator>::operator[](size_t index) {
		if (index < 0 || index >= size) {
			throw std::out_of_range("Out of list bounds");
		}
		forward_iterator it = this->begin();
		for (size_t i = 0; i < index; i++) {
			it++;
		}
		return *it;
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator& list<T, Allocator>::forward_iterator::operator++() {
		if (ptr_ == nullptr) throw std::logic_error("Out of list bounds");
		*this = ptr_->next();
		return *this;
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator list<T, Allocator>::forward_iterator::operator++(int) {
		forward_iterator old = *this;
		++*this;
		return old;
	}

	template<class T, class Allocator>
	bool list<T, Allocator>::forward_iterator::operator==(const forward_iterator& other) const {
		return ptr_ == other.ptr_;
	}

	template<class T, class Allocator>
	bool list<T, Allocator>::forward_iterator::operator!=(const forward_iterator& other) const {
		return ptr_ != other.ptr_;
	}
}
