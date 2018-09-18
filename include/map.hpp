/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	struct node {
		node *l, *r, *p;
		int s;
		value_type *v;
		node() :l(NULL), r(NULL), s(0), v(NULL), p(NULL) {}
		node(value_type &obj) :l(NULL), r(NULL), s(1), v(new value_type(obj)), p(NULL) {}
		node(value_type &&obj) :l(NULL), r(NULL), s(1), v(new value_type(obj)), p(NULL) {}
		node(node* i) :l(NULL), r(NULL), s(i->s), v(new value_type(*i->v)), p(NULL) {}
		~node() {
			if (v) delete v;
		}
	};
	node *l(node *t) const { return t ? t->l : NULL; }
	node *r(node *t) const { return t ? t->r : NULL; }
	int s(node *t) const { return t ? t->s : 0; }

	node *root;
	int tot;
	void left_rotate(node *&t) {
		node* k;
		k = t->r; t->r = k->l; k->l = t;

		k->s = t->s;
		t->s = s(l(t)) + s(r(t)) + 1;

		if (t->r) t->r->p = t;
		k->p = t->p; t->p = k;

		t = k;
	}
	void right_rotate(node *&t) {
		node* k;
		k = t->l; t->l = k->r; k->r = t;

		k->s = t->s;
		t->s = s(l(t)) + s(r(t)) + 1;

		if (t->l) t->l->p = t;
		k->p = t->p; t->p = k;

		t = k;
	}
	void maintain(node *&t, bool flag) {
		if (!flag) {
			if (s(l(l(t))) > s(r(t))) right_rotate(t);
			else if (s(r(l(t))) > s(r(t))) {
				left_rotate(t->l);
				right_rotate(t);
			}
			else return;
		}
		else {
			if (s(r(r(t))) > s(l(t))) left_rotate(t);
			else if (s(l(r(t))) > s(l(t))) {
				right_rotate(t->r);
				left_rotate(t);
			}
			else return;
		}
		maintain(t->l, false); maintain(t->r, true);
		maintain(t, true); maintain(t, false);
	}
	void deep_copy(node *&t, node* other) {
		if (!other) return;
		t = new node(other);
		deep_copy(t->l, other->l);
		deep_copy(t->r, other->r);
		if (t->l) t->l->p = t;
		if (t->r) t->r->p = t;
	}
	void deep_clear(node *&t) {
		if (!t) return;
		deep_clear(t->l);
		deep_clear(t->r);
		node *tmp = t;
		delete tmp;
		t = NULL;
	}
	void deep_insert(node *&t, value_type& v) {
		if (!t) {
			tot++; t = new node(v);
			return;
		}
		t->s++;
		if (Compare()(t->v->first, v.first)) { deep_insert(t->r, v); t->r->p = t; }
		else { deep_insert(t->l, v); t->l->p = t; }
		maintain(t, Compare()(t->v->first, v.first));
	}
	int rank(node *t, value_type &v) const {
		if (!Compare()(t->v->first, v.first) && !Compare()(v.first, t->v->first)) return s(l(t)) + 1;
		if (Compare()(t->v->first, v.first)) return s(l(t)) + 1 + rank(t->r, v);
		else return rank(t->l, v);
	}
	value_type* select(int k) const  {
		node* now = root;
		while (now) {
			if (k == s(l(now)) + 1) return now->v;
			if (k <= s(l(now))) now = now->l;
			else { k -= s(l(now)) + 1; now = now->r; }
		}
	}
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
	public:
		map* belong;
		node* pt;
		iterator() :belong(NULL), pt(NULL) {}
		iterator(const iterator &other) :belong(other.belong), pt(other.pt) {}
		iterator(iterator &&other) :belong(other.belong), pt(other.pt) {}
		iterator(map* b, node *pot) :belong(b), pt(pot) {}
		iterator &operator=(iterator &other){
			if (this == &other) return *this;
		    belong=other.belong;
		    pt=other.pt;
		    return *this;
		}
		iterator &operator=(iterator &&other){
			if (this == &other) return *this;
			belong=other.belong;
		    pt=other.pt;
		    return *this;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if (!pt) throw invalid_iterator();
			iterator tmp(*this);
			if (pt->r) {
				pt = pt->r;
				while (pt->l) pt = pt->l;
			}
			else {
				while (pt->p && pt->p->r == pt) pt = pt->p;
				pt = pt->p;
			}
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if (!pt) throw invalid_iterator();
			iterator tmp(*this);
			if (pt->r) {
				pt = pt->r;
				while (pt->l) pt = pt->l;
			}
			else {
				while (pt->p && pt->p->r == pt) pt = pt->p;
				pt = pt->p;
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp(*this);
			if (!pt) {
				pt = belong->root;
				if (!pt) throw invalid_iterator();
				while (pt->r) pt = pt->r;
				return tmp;
			}
			if (pt->l) {
				pt = pt->l;
				while (pt->r) pt = pt->r;
			}
			else {
				while (pt->p && pt->p->l == pt) pt = pt->p;
				if (!pt->p) { pt = tmp.pt; throw invalid_iterator(); }
				else pt = pt->p;
			}
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if (!pt) {
				pt = belong->root;
				if (!pt) throw invalid_iterator();
				while (pt->r) pt = pt->r;
				return *this;
			}
			iterator tmp(*this);
			if (pt->l) {
				pt = pt->l;
				while (pt->r) pt = pt->r;
			}
			else {
				while (pt->p && pt->p->l == pt) pt = pt->p;
				if (!pt->p) { pt = tmp.pt; throw invalid_iterator(); }
				else pt = pt->p;
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			if (!pt) throw invalid_iterator();
			return *pt->v;
		}
		bool operator==(const iterator &rhs) const {
			return (belong == rhs.belong && pt == rhs.pt);
		}
		bool operator==(const const_iterator &rhs) const {
			return (belong == rhs.belong && pt == rhs.pt);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(belong == rhs.belong && pt == rhs.pt);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(belong == rhs.belong && pt == rhs.pt);
		}

		/**
		 * for the support of it->first.
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return pt->v;
		}
	};
	class const_iterator {
	private:
	public:
		const map* belong;
		node* pt;
		const_iterator() :belong(NULL), pt(NULL) {}
		const_iterator(const const_iterator &other) :belong(other.belong), pt(other.pt) {}
		const_iterator(iterator &other) :belong(other.belong), pt(other.pt) {}
		const_iterator(iterator &&other) :belong(other.belong), pt(other.pt) {}
		const_iterator(map* b, node *t) :belong(b), pt(t) {}
		const_iterator(const map* b, node *t) :belong(b), pt(t) {}
		/**
		* return a new iterator which pointer n-next elements
		*   even if there are not enough elements, just return the answer.
		* as well as operator-
		*/
		/**
		* TODO iter++
		*/
		const_iterator &operator=(iterator &&other){
		    belong=other.belong;
		    pt=other.pt;
		    return *this;
		}
		const_iterator operator++(int) {
			if (!pt) throw invalid_iterator();
			const_iterator tmp(*this);
			if (pt->r) {
				pt = pt->r;
				while (pt->l) pt = pt->l;
			}
			else {
				while (pt->p && pt->p->r == pt) pt = pt->p;
				pt = pt->p;
			}
			return tmp;
		}
		/**
		* TODO ++iter
		*/
		const_iterator & operator++() {
			if (!pt) throw invalid_iterator();
			const_iterator tmp(*this);
			if (pt->r) {
				pt = pt->r;
				while (pt->l) pt = pt->l;
			}
			else {
				while (pt->p && pt->p->r == pt) pt = pt->p;
				pt = pt->p;
			}
			return *this;
		}
		/**
		* TODO iter--
		*/
		const_iterator operator--(int) {
			const_iterator tmp(*this);
			if (!pt) {
				pt = belong->root;
				if (!pt) throw invalid_iterator();
				while (pt->r) pt = pt->r;
				return tmp;
			}
			if (pt->l) {
				pt = pt->l;
				while (pt->r) pt = pt->r;
			}
			else {
				while (pt->p && pt->p->l == pt) pt = pt->p;
				if (!pt->p) { pt = tmp.pt; throw invalid_iterator(); }
				else pt = pt->p;
			}
			return tmp;
		}
		/**
		* TODO --iter
		*/
		const_iterator & operator--() {
			if (!pt) {
				pt = belong->root;
				if (!pt) throw invalid_iterator();
				while (pt->r) pt = pt->r;
				return *this;
			}
			const_iterator tmp(*this);
			if (pt->l) {
				pt = pt->l;
				while (pt->r) pt = pt->r;
			}
			else {
				while (pt->p && pt->p->l == pt) pt = pt->p;
				if (!pt->p) { pt = tmp.pt; throw invalid_iterator(); }
				else pt = pt->p;
			}
			return *this;
		}
		/**
		* a operator to check whether two iterators are same (pointing to the same memory).
		*/
		value_type & operator*() const {
			if (!pt) throw invalid_iterator();
			return *pt->v;
		}
		bool operator==(const iterator &rhs) const {
			return (belong == rhs.belong && pt == rhs.pt);
		}
		bool operator==(const const_iterator &rhs) const {
			return (belong == rhs.belong && pt == rhs.pt);
		}
		/**
		* some other operator for iterator.
		*/
		bool operator!=(const iterator &rhs) const {
			return !(belong == rhs.belong && pt == rhs.pt);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(belong == rhs.belong && pt == rhs.pt);
		}

		/**
		* for the support of it->first.
		* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		*/
		value_type* operator->() const noexcept {
			return pt->v;
		}
	};
	/**
	 * TODO two constructors
	 */
	map() :root(NULL), tot(0) {}
	map(const map &other):root(NULL),tot(other.tot) {
		deep_copy(root, other.root);
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if (this == &other) return *this;
		deep_clear(root);
		deep_copy(root, other.root);
		tot = other.tot;
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		deep_clear(root);
		tot = 0;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return now->v->second;
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
		throw index_out_of_bound();
	}
	const T & at(const Key &key) const {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return now->v->second;
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
		throw index_out_of_bound();
	}
	/**
	 * TODO
	 * access specified element
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return now->v->second;
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
		value_type x(key, T());
		deep_insert(root, x);
		now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return now->v->second;
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return now->v->second;
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
		throw index_out_of_bound();
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		node* now = root;
		while (now && now->l) now = now->l;
		return iterator(this, now);
	}
	const_iterator cbegin() const {
		node* now = root;
		while (now && now->l) now = now->l;
		return const_iterator(this, now);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		return iterator(this, NULL);
	}
	const_iterator cend() const {
		return const_iterator(this, NULL);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const { return !tot; }
	/**
	 * returns the number of elements.
	 */
	size_t size() const { return tot; }
	/**
	 * clears the contents
	 */
	void clear() {
		deep_clear(root);
		tot = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion),
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, value.first) && !Compare()(value.first, now->v->first)) return pair<iterator, bool>(iterator(this, now), false);
			if (Compare()(now->v->first, value.first)) now = now->r;
			else now = now->l;
		}
		value_type tmp(value);
		deep_insert(root, tmp);
		now = root;
		while (now) {
			if (!Compare()(now->v->first, value.first) && !Compare()(value.first, now->v->first)) return pair<iterator, bool>(iterator(this, now), true);
			if (Compare()(now->v->first, value.first)) now = now->r;
			else now = now->l;
		}
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if (!pos.pt || this!=pos.belong) throw invalid_iterator();
		tot--;
		node *now = pos.pt, *t, *f = NULL, *tmp;
		now = now->p;
		while (now) {
			now->s--; now = now->p;
		}
		t = pos.pt; f = t->p; now = pos.pt;
		if (!t->l && !t->r) {
			if (f) {
				if (f->l == t) f->l = NULL;
				else f->r = NULL;
				delete t;
			}
			else { delete t; root = NULL; }
			return;
		}
		if (t->l && !t->r) {
			if (f) {
				if (f->l == t) f->l = t->l;
				else f->r = t->l;
				t->l->p = f;
				delete t;
			}
			else { root = t->l; root->p = NULL; delete t; }
			return;
		}
		if (!t->l && t->r) {
			if (f) {
				if (f->l == t) f->l = t->r;
				else f->r = t->r;
				t->r->p = f;
				delete t;
			}
			else {
				root = t->r; root->p = NULL; delete t;
			}
			return;
		}

		f = now; now = now->l; f->s--;
		while (now->r) { f = now; now = now->r; f->s--; }

		if (!now->l && !now->r) {
			if (f->l == now) f->l = NULL;
			else f->r = NULL;
			now->l = t->l;
			if (t->l) t->l->p = now;
			now->r = t->r;
			if (t->r) t->r->p = now;
			now->p = t->p;
			if (t->p) if (t->p->l == t) t->p->l = now;
			else t->p->r = now;
			else root = now;
			now->s = t->s;
			delete t;
		}
		else {
			if (f->l == now) f->l = now->l;
			else f->r = now->l;
			now->l->p = f;
			now->l = t->l;
			if (t->l) t->l->p = now;
			now->r = t->r;
			if (t->r) t->r->p = now;
			now->p = t->p;
			if (t->p) if (t->p->l == t) t->p->l = now;
			else t->p->r = now;
			else root = now;
			now->s = t->s;
			delete t;
		}
	}
	/**
	* Finds an element with key equivalent to key.
	* key value of the element to search for.
	* Iterator to an element with key equivalent to key.
	*   If no such element is found, past-the-end (see end()) iterator is returned.
	*/
	iterator find(const Key &key) {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return iterator(this, now);
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
		return iterator(this, NULL);
	}
	const_iterator find(const Key &key) const {
		node *now = root;
		while (now) {
			if (!Compare()(now->v->first, key) && !Compare()(key, now->v->first)) return const_iterator(this, now);
			if (Compare()(now->v->first, key)) now = now->r;
			else now = now->l;
		}
		return const_iterator(this, NULL);
	}
	/**
	 * Returns the number of elements with key
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		const_iterator i(find(key));
		if (i.pt) return 1;
		else return 0;
	}
};

}

#endif
