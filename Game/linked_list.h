//////////////////////////////////////////////////////////////////////

#pragma once

// Deal with MSVC internal compiler error
// this can be removed when a fix is available

#pragma push_macro("VC_WORKAROUND")
#undef VC_WORKAROUND
#if defined(_MSC_VER)
#define VC_WORKAROUND NODE
#else
#define VC_WORKAROUND (linked_list_node<T> T::*)nullptr != NODE
#endif

//////////////////////////////////////////////////////////////////////

namespace chs
{

//////////////////////////////////////////////////////////////////////
// base list node class, 2 pointers

template <typename T>
class list_node_base
{
public:
	T *next;
	T *prev;
};

//////////////////////////////////////////////////////////////////////
// base node is wrapped so we can get the offset to it

template <typename T>
class linked_list_node
{
public:
	list_node_base<T> list_node;
};

//////////////////////////////////////////////////////////////////////
// template base

template <typename T, linked_list_node<T> T::*NODE, bool is_member>
class list_base
{
};

//////////////////////////////////////////////////////////////////////
// specialization for instances using linked_list_node as member field

template <typename T, linked_list_node<T> T::*NODE>
class list_base<T, NODE, true>
	: protected linked_list_node<T>
{
protected:
	static size_t offset()
	{
		linked_list_node<T> *b = &(((T *)0)->*NODE);
		return size_t(&b->list_node);
	}
};

//////////////////////////////////////////////////////////////////////
// specialization for instances deriving from linked_list_node

template <typename T, linked_list_node<T> T::*NODE>
class list_base<T, NODE, false>
	: protected linked_list_node<T>
{
	static_assert(!std::is_polymorphic<T>::value, "polymorphic! use the member-node version");
protected:
	static size_t offset()
	{
		list_node_base<T> T::*n = static_cast<list_node_base<T> T::*>(&T::list_node);
		return (size_t)(&(((T *)0)->*n));
	}
};

//////////////////////////////////////////////////////////////////////
// the actual list

template <typename T, linked_list_node<T> T::*NODE = nullptr>
class linked_list
	: protected list_base<T, NODE, VC_WORKAROUND>
{
public:

	using list_base<T, NODE, VC_WORKAROUND>::offset;
	using list_base<T, NODE, VC_WORKAROUND>::list_node;

	typedef T *			ptr;
	typedef T const *	const_ptr;
	typedef T &			ref;
	typedef T const &	const_ref;

	#if !defined(_CHS_LINKED_LIST_DONT_DEFINE_STL_ITERATORS_)

	typedef std::iterator<std::bidirectional_iterator_tag, T> iterator_base;

	struct const_iterator : iterator_base
	{
		const_iterator(const_ptr t) : p(t) {}
		const_iterator(const_iterator const &o) : p(o.p) {}
		const_iterator const &operator=(const_iterator const &o) { p = o.p; return *this; }
		const_iterator &operator++() { p = node(p).next; return *this; }
		const_iterator &operator--() { p = node(p).prev; return *this; }
		bool operator==(const_iterator const &o) { return p == o.p; }
		bool operator!=(const_iterator const &o) { return p != o.p; }
		const_ref operator *() { return *p; }
		const_ptr operator->() { return  p; }
	private:
		const_ptr p;
	};

	struct iterator : iterator_base
	{
		iterator(ptr t) : p(t) {}
		iterator(const_ptr *t) : p(t) {}
		iterator(iterator const &o) : p(o.p) {}
		iterator const &operator=(const_iterator const &o) { p = o.p; return *this; }
		iterator const &operator=(iterator const &o) { p = o.p; return *this; }
		iterator &operator++() { p = node(p).next; return *this; }
		iterator &operator--() { p = node(p).prev; return *this; }
		bool operator==(iterator const &o) { return p == o.p; }
		bool operator!=(iterator const &o) { return p != o.p; }
		ref operator *() { return *p; }
		ptr operator->() { return  p; }
	private:
		ptr p;
	};

	struct const_reverse_iterator : iterator_base
	{
		const_reverse_iterator(const_ptr t) : p(t) {}
		const_reverse_iterator(const_reverse_iterator const &o) : p(o.p) {}
		const_reverse_iterator const &operator=(const_reverse_iterator const &o) { p = o.p; return o; }
		const_reverse_iterator &operator++() { p = node(p).prev; return *this; }
		const_reverse_iterator &operator--() { p = node(p).next; return *this; }
		bool operator==(const_reverse_iterator const &o) { return p == o.p; }
		bool operator!=(const_reverse_iterator const &o) { return p != o.p; }
		const_ref operator *() { return *p; }
		const_ptr operator->() { return  p; }
	private:
		const_ptr p;
	};

	struct reverse_iterator : iterator_base
	{
		reverse_iterator(ptr t) : p(t) {}
		reverse_iterator(const_ptr t) : p(t) {}
		reverse_iterator(reverse_iterator const &o) : p(o.p) {}
		reverse_iterator const &operator=(const_reverse_iterator const &o) { p = o.p; return *this; }
		reverse_iterator const &operator=(reverse_iterator const &o) { p = o.p; return *this; }
		reverse_iterator &operator++() { p = node(p).prev; return *this; }
		reverse_iterator &operator--() { p = node(p).next; return *this; }
		bool operator==(reverse_iterator const &o) { return p == o.p; }
		bool operator!=(reverse_iterator const &o) { return p != o.p; }
		ref operator *() { return *p; }
		ptr operator->() { return  p; }
	private:
		ptr p;
	};

	iterator				begin()			{ return iterator(list_node.next); }
	const_iterator			begin()	const	{ return const_iterator(list_node.next); }

	iterator				end()			{ return iterator(rootp()); }
	const_iterator			end() const		{ return const_iterator(root()); }

	const_iterator			cbegin() const	{ return const_iterator(list_node.next); }
	const_iterator			cend() const	{ return const_iterator(root()); }

	reverse_iterator		rbegin() const	{ return reverse_iterator(list_node.prev); }
	reverse_iterator		rend() const	{ return reverse_iterator(rootp()); }

	const_reverse_iterator	crbegin() const	{ return const_reverse_iterator(list_node.prev); }
	const_reverse_iterator	crend() const	{ return const_reverse_iterator(root()); }

	#endif //!defined(_CHS_LINKED_LIST_DONT_DEFINE_STL_ITERATORS_)

	typedef T						value_type;

private:

	typedef list_node_base<T>		node_t;
	typedef node_t *				node_ptr;
	typedef node_t &				node_ref;
	typedef list_node_base<T> const	const_node_t;
	typedef const_node_t *			const_node_ptr;
	typedef const_node_t &			const_node_ref;

	// get an object's node
	static node_ref node(ptr obj)
	{
		return *reinterpret_cast<node_ptr>(reinterpret_cast<char *>(obj) + offset());
	}

	static const_node_ref node(const_ptr obj)
	{
		return *reinterpret_cast<const_node_ptr>(reinterpret_cast<char const *>(obj) + offset());
	}

	ptr rootp()
	{
		return reinterpret_cast<ptr>(reinterpret_cast<char *>(&list_node) - offset());
	}

	const_ptr root() const
	{
		return reinterpret_cast<const_ptr>(reinterpret_cast<char const *>(&list_node) - offset());
	}

public:

	// basic operations

	// make the list empty
	void clear()
	{
		list_node.next = rootp();
		list_node.prev = rootp();
	}

	// insert an object before another object
	void insert_before(ptr obj_before, ptr obj)
	{
		ptr &p = node(obj_before).prev;
		node_ref n = node(obj);
		node(p).next = obj;
		n.prev = p;
		p = obj;
		n.next = obj_before;
	}

	// insert an object after another object
	void insert_after(ptr obj_after, ptr obj)
	{
		ptr &n = node(obj_after).next;
		node_ref p = node(obj);
		node(n).prev = obj;
		p.next = n;
		n = obj;
		p.prev = obj_after;
	}

	// remove an object from the list
	ptr remove(ptr obj)
	{
		ptr p = prev(obj);
		ptr n = next(obj);
		node(p).next = n;
		node(n).prev = p;
		return obj;
	}

	linked_list()
	{
		clear();
	}

	bool empty() const
	{
		return list_node.next == root();
	}

	// non-stl style iteration using T* and T const *

	ptr			head()							{ return list_node.next; }
	ptr			tail()							{ return list_node.prev; }

	const_ptr	c_head() const					{ return list_node.next; }
	const_ptr	c_tail() const					{ return list_node.prev; }

	ptr			next(ptr obj)					{ return node(obj).next; }
	ptr			prev(ptr obj)					{ return node(obj).prev; }

	const_ptr	c_next(const_ptr obj) const		{ return node(obj).next; }
	const_ptr	c_prev(const_ptr obj) const		{ return node(obj).prev; }

	ptr			done()							{ return root(); }
	const_ptr	c_done() const					{ return root(); }

	// all these operations rely on the basic operations

	ptr			remove(ref obj)					{ return remove(&obj); }
													
	void		push_back(ref obj)				{ insert_before(*rootp(), obj); }
	void		push_back(ptr obj)				{ insert_before(rootp(), obj); }
													
	void		push_front(ref obj)				{ insert_after(*rootp(), obj); }
	void		push_front(ptr obj)				{ insert_after(rootp(), obj); }
													
	ptr			pop_front()						{ return !empty() ? remove(head()) : nullptr; }
	ptr			pop_back()						{ return !empty() ? remove(tail()) : nullptr; }

	void		insert_before(ref bef, ref obj)	{ insert_before(&bef, &obj); }
	void		insert_after(ref aft, ref obj)	{ insert_after(&aft, &obj); }

};

//////////////////////////////////////////////////////////////////////

} // chs

#pragma warning(push)
#pragma warning(disable: 4602)		// disable spurious warning
#pragma pop_macro("VC_WORKAROUND")
#pragma warning(pop)

