#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

template<class T>
class deque {
public:
	struct tail_node {
		tail_node *next, *pre;
		tail_node() :next(NULL), pre(NULL) {};
		virtual ~tail_node() {}
	};
	struct node :public tail_node {
		T v;
		node(const T &value):v(value),tail_node() {}
		node(T &value):v(value),tail_node() {}
		node(node* other) :v(other->v), tail_node() {}
	};
	tail_node *head;
	node *last_at;
	tail_node *tail;
	int tot,lat;
	class const_iterator;
	class iterator {
	private:
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		node* pointer;
		deque* belong;
		iterator(deque* d, node* p) :belong(d), pointer(p) {
			return;
		}
		//iterator(iterator &other) :belong(other.belong), pointer(other.pointer) {}
		iterator() = default;
		iterator& operator=(iterator other) {
			belong = other.belong;
			pointer = other.pointer;
			return *this;
		}

		iterator operator+(const int &n) const {
			//TODO
			int x = 0;
			iterator tmp(belong, pointer);
			if (n > 0) {
				while (x < n && tmp.pointer->next) {
					tmp.pointer = static_cast<node*>(tmp.pointer->next);
					x++;
				}
			}
			else {
				while (x < -n && tmp.pointer->pre) {
					tmp.pointer = static_cast<node*>(tmp.pointer->pre);
					x++;
				}
			}
			return tmp;
		}
		iterator operator-(const int &n) const {
			//TODO
			int x = 0;
			iterator tmp(belong, pointer);
			if (n > 0) {
				while (tmp.pointer->pre && x < n) {
					tmp.pointer = static_cast<node*>(tmp.pointer->pre);
					x++;
				}
			}
			else {
				while (tmp.pointer->next && x < -n) {
					tmp.pointer = static_cast<node*>(tmp.pointer->next);
					x++;
				}
			}
			return tmp;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invalid_iterator.
		int operator-(const iterator &rhs) const {
			if (belong != rhs.belong) throw invalid_iterator();
			iterator tmp(rhs.belong,rhs.pointer);

			int i = 0;
			while (tmp.pointer->next) {
				if (tmp.pointer == pointer) return i;
				tmp.pointer = static_cast<node*>(tmp.pointer->next);
				i++;
			}
			if (tmp.pointer == pointer) return i;

			i = 0;
			iterator tst(rhs.belong, rhs.pointer);
			while (tst.pointer->pre) {
				if (tst.pointer == pointer) return i;
				tst.pointer = static_cast<node*>(tst.pointer->pre);
				i--;
			}
			if (tst.pointer == pointer) return i;
		}
		iterator operator+=(const int &n) {
			//TODO
			int x = 0;
			if (n < 0) {
				while (pointer->pre && x < -n) {
					x++;
					pointer = static_cast<node*>(pointer->pre);
				}
			}
			else {
				while (pointer->next && x < n) {
					x++;
					pointer = static_cast<node*>(pointer->next);
				}
			}
			return *this;
		}
		iterator operator-=(const int &n) {
			//TODO
			int x = 0;
			if (n > 0) {
				while (pointer->pre && x < n) {
					x++;
					pointer = static_cast<node*>(pointer->pre);
				}
			}
			else {
				while (pointer->next && x < -n) {
					x++;
					pointer = static_cast<node*>(pointer->next);
				}
			}
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp(belong, pointer);
			pointer = static_cast<node*>(pointer->next);
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			pointer = static_cast<node*>(pointer->next);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp(belong, pointer);
			pointer = static_cast<node*>(pointer->pre);
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			pointer = static_cast<node*>(pointer->pre);
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
			if (pointer == belong->tail) throw invalid_iterator();
			if (!pointer) throw invalid_iterator();
			return pointer->v;
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {
			return &pointer->v;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return (belong == rhs.belong && pointer == rhs.pointer);
		}
		bool operator==(const const_iterator &rhs) const {
			return (belong == rhs.belong && pointer == rhs.pointer);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(belong == rhs.belong && pointer == rhs.pointer);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(belong == rhs.belong && pointer == rhs.pointer);
		}
	};
	class const_iterator {
	private:
	public:
		/**
		* return a new iterator which pointer n-next elements
		*   even if there are not enough elements, the behaviour is **undefined**.
		* as well as operator-
		*/
		node* pointer;
		const deque* belong;
		const_iterator(const deque* d, node* p) :belong(d), pointer(p) {}
		const_iterator(const_iterator &other) :belong(other.belong), pointer(other.pointer) {}
		const_iterator(const_iterator &&other) :belong(other.belong), pointer(other.pointer) {}
		const_iterator() = default;
		const_iterator& operator=(const_iterator other) {
			belong = other.belong;
			pointer = other.pointer;
			return *this;
		}

		const_iterator operator+(const int &n) const {
			//TODO
			int x = 0;
			const_iterator tmp(belong, pointer);
			if (n > 0) {
				while (tmp.pointer->next && x < n) {
					x++;
					tmp.pointer = static_cast<node*>(tmp.pointer->next);
				}
			}
			else {
				while (tmp.pointer->pre && x < -n) {
					x++;
					tmp.pointer = static_cast<node*>(tmp.pointer->pre);
				}
			}
			return tmp;
		}
		const_iterator operator-(const int &n) const {
			//TODO
			int x = 0;
			const_iterator tmp(belong,pointer);
			if (n > 0) {
				while (tmp.pointer && tmp.pointer->pre && x < n) {
					x++;
					tmp.pointer = static_cast<node*>(tmp.pointer->pre);
				}
			}
			else {
				while (tmp.pointer && tmp.pointer->next && x < -n) {
					x++;
					tmp.pointer = static_cast<node*>(tmp.pointer->next);
				}
			}
			return tmp;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invalid_iterator.
		int operator-(const const_iterator &rhs) const {
			if (belong != rhs.belong) throw invalid_iterator();
			const_iterator tmp(rhs.belong,rhs.pointer);
			int i = 0;
			while (tmp.pointer->next) {
				if (tmp.pointer == pointer) return i;
				tmp.pointer = static_cast<node*>(tmp.pointer->next);
				i++;
			}
			if (tmp.pointer == pointer) return i;
			i = 0;
			const_iterator tst(rhs.belong, rhs.pointer);
			while (tst.pointer->pre) {
				if (tst.pointer == pointer) return i;
				tst.pointer = static_cast<node*>(tst.pointer->pre);
				i--;
			}
			if (tst.pointer == pointer) return i;
		}
		const_iterator operator+=(const int &n) {
			//TODO
			int x = 0;
			if (n < 0) {
				while (pointer->pre && x < -n) {
					x++;
					pointer = static_cast<node*>(pointer->pre);
				}
			}
			else {
				while (pointer->next && x < n) {
					x++;
					pointer = static_cast<node*>(pointer->next);
				}
			}
			return *this;
		}
		const_iterator operator-=(const int &n) {
			//TODO
			int x = 0;
			if (n > 0) {
				while (pointer->pre && x < n) {
					x++;
					pointer = static_cast<node*>(pointer->pre);
				}
			}
			else {
				while (pointer->next && x < -n) {
					x++;
					pointer = static_cast<node*>(pointer->next);
				}
			}
			return *this;
		}
		/**
		* TODO iter++
		*/
		const_iterator operator++(int) {
			const_iterator tmp(belong,pointer);
			pointer = static_cast<node*>(pointer->next);
			return tmp;
		}
		/**
		* TODO ++iter
		*/
		const_iterator& operator++() {
			pointer = static_cast<node*>(pointer->next);
			return *this;
		}
		/**
		* TODO iter--
		*/
		const_iterator operator--(int) {
			const_iterator tmp(belong, pointer);
			pointer = static_cast<node*>(pointer->pre);
			return tmp;
		}
		/**
		* TODO --iter
		*/
		const_iterator& operator--() {
			pointer = static_cast<node*>(pointer->pre);
			return *this;
		}
		/**
		* TODO *it
		*/
		T& operator*() const { return pointer->v; }
		/**
		* TODO it->field
		*/
		T* operator->() const noexcept {
			return &pointer->v;
		}
		/**
		* a operator to check whether two iterators are same (pointing to the same memory).
		*/
		bool operator==(const iterator &rhs) const {
			return (belong == rhs.belong && pointer == rhs.pointer);
		}
		bool operator==(const const_iterator &rhs) const {
			return (belong == rhs.belong && pointer == rhs.pointer);
		}
		/**
		* some other operator for iterator.
		*/
		bool operator!=(const iterator &rhs) const {
			return !(belong == rhs.belong && pointer == rhs.pointer);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(belong == rhs.belong && pointer == rhs.pointer);
		}
	};
	/**
	 * TODO Constructors
	 */
	deque() {
		tail = new tail_node();
		head = tail;
		lat=-5;
		tot = 0;
	}
	deque(const deque &other) {
        lat=-5;
		if (!other.tot) {
			tail = new tail_node();
			head = tail;
			tot = 0;
			return;
		}
		head = new node(static_cast<node*>(other.head));
		tot = other.tot;
		tail_node *now_other = other.head, *now = head, *tmp;
		while (now_other->next != other.tail) {
			now->next = new node(static_cast<node*>(now_other->next));
			tmp = now;
			now = now->next;
			now_other = now_other->next;
			now->pre = tmp;
		}
		tail = new tail_node();
		tail->pre = now;
		now->next = tail;
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
		tail_node *tmp = tail;
		while (tmp != head) {
			if (tmp->pre) tmp->pre->next = tmp;
			tmp = tmp->pre;
			delete tmp->next;
		}
		delete tmp;
	}
	/**
	 * TODO assignment operator
	 */

	void clear() {
		tot = 0;
		tail_node *tmp = tail;
		while (tmp != head) {
			tmp = tmp->pre;
			delete tmp->next;
		}
		delete tmp;

		tail = new tail_node();
		head = tail;
	}

	deque &operator=(const deque &other) {
		if (this == &other) return *this;

		tail_node *tmp;
		clear();

		if (!other.tot) {
			tail = new tail_node();
			head = tail;
			tot = 0;
			return *this;
		}

		head = new node(static_cast<node*>(other.head));
		tail_node *now_other = other.head, *now = head;
		tot = other.tot;
		while (now_other->next != other.tail) {
			now->next = new node(static_cast<node*>(now_other->next));
			tmp = now;
			now = now->next;
			now_other = now_other->next;
			now->pre = tmp;
		}
		tail = new tail_node();
		tail->pre = now;
		now->next = tail;

		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
		if (pos < 0 || pos >= tot) throw index_out_of_bound();
		if (lat + 1 == pos) {
			lat = pos;
			last_at = static_cast<node*>(last_at->next);
			return last_at->v;
		}
		int cnt = 0;
		tail_node *now = head;
		while (cnt != pos) {
			cnt++;
			now = now->next;
		}
		last_at = static_cast<node*>(now);
		lat = pos;
		return last_at->v;
	}
	const T & at(const size_t &pos) const {
		if (pos < 0 || pos >= tot) throw index_out_of_bound();
		/*
		if (lat + 1 == pos) {
		lat = pos;
		last_at = static_cast<node*>(last_at->next);
		return last_at->v;
		}
		*/
		int cnt = 0;
		tail_node *now = head;
		while (cnt != pos) {
			cnt++;
			now = now->next;
		}
		//last_at = static_cast<node*>(now);
		//lat = pos;
		return static_cast<node*>(now)->v;
	}
	T & operator[](const size_t &pos) {
		if (pos < 0 || pos >= tot) throw index_out_of_bound();
		if (lat + 1 == pos) {
			lat = pos;
			last_at = static_cast<node*>(last_at->next);
			return last_at->v;
		}
		int cnt = 0;
		tail_node *now = head;
		while (cnt != pos) {
			cnt++;
			now = now->next;
		}
		last_at = static_cast<node*>(now);
		lat = pos;
		return last_at->v;
	}
	const T & operator[](const size_t &pos) const {
		if (pos < 0 || pos >= tot) throw index_out_of_bound();
		/*
		if (lat + 1 == pos) {
			lat = pos;
			last_at = static_cast<node*>(last_at->next);
			return last_at->v;
		}
		*/
		int cnt = 0;
		tail_node *now = head;
		while (cnt != pos) {
			cnt++;
			now = now->next;
		}
		//last_at = static_cast<node*>(now);
		//lat = pos;
		return static_cast<node*>(now)->v;
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
		if (!tot) throw container_is_empty();
		return static_cast<node*>(head)->v;
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
		if (!tot) throw container_is_empty();
		return static_cast<node*>(tail->pre)->v;
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, static_cast<node*>(head));
	}
	const_iterator cbegin() const {
		return const_iterator(this, static_cast<node*>(head));
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, static_cast<node*>(tail));
	}
	const_iterator cend() const {
		return const_iterator(this, static_cast<node*>(tail));
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const { return !tot; }
	/**
	 * returns the number of elements
	 */
	size_t size() const { return tot; }
	/**
	 * clears the contents
	 */

	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
		if (pos.belong != this) throw invalid_iterator();
		lat = -5;
		tail_node *now = pos.pointer, *tmp;
		if (!tot) {
			head = new node(value);
			head->next = tail;
			tail->pre = head;
			tot++;
			return iterator(this, static_cast<node*>(head));
		}
		tmp = now->pre;
		now->pre = new node(value);
		now->pre->next = now;
		if (tmp) tmp->next = now->pre;
		now->pre->pre = tmp;
		if (head == now) head = now->pre;
		tot++;
		return iterator(this, static_cast<node*>(now->pre));
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
		if (!tot) throw container_is_empty();
		if (pos.belong != this) throw invalid_iterator();
		if (pos.pointer == tail) throw invalid_iterator();
		lat = -5;
		tail_node *now = pos.pointer, *tmp = now->next;
		if (now == head) {
			head = head->next;
			delete head->pre;
			head->pre = NULL;
			tot--;
			return iterator(this, static_cast<node*>(head));
		}
		now->pre->next = now->next;
		now->next->pre = now->pre;
		delete now;
		tot--;
		return iterator(this, static_cast<node*>(tmp));
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
		node *tmp = new node(value);
		if (!tot) {
			head = tmp;
			head->next = tail;
			tail->pre = head;
			tot++;
			return;
		}
		tmp->pre = tail->pre;
		tmp->next = tail;
		tail->pre->next = tmp;
		tail->pre = tmp;
		tot++;
		return;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
		if (!tot) throw container_is_empty();
		tail_node* tmp = tail->pre;
		if (tot == 1) {
			delete head;
			head = tail;
			tot--;
		}
		else {
			tail->pre = tmp->pre;
			tail->pre->next = tail;
			delete tmp;
			tot--;
		}
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
		lat--;
		if (!tot) {
			head = new node(value);
			head->next = tail;
			tail->pre = head;
			tot++;
			return;
		}
		head->pre = new node(value);
		head->pre->next = head;
		head = head->pre;
		tot++;
		return;
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
		lat--;
		if (!tot) throw container_is_empty();
		if (tot == 1) {
			delete head;
			head = tail;
			tot = 0;
		}
		else {
			head = head->next;
			delete head->pre;
			head->pre = NULL;
			tot--;
		}
	}
};

}

#endif

