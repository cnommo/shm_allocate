#ifndef _SHM_ALLOCATOR_H_
#define _SHM_ALLOCATOR_H_

#include <iostream>
#include <cstdlib>
#include <new>
#include <bits/functexcept.h>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>

namespace shm
{
  using std::size_t;
  using std::ptrdiff_t;

  template<typename _Tp>
	  class malloc_allocator
	  {
		  public:
			  typedef size_t     size_type;
			  typedef ptrdiff_t  difference_type;
			  typedef _Tp*       pointer;
			  typedef const _Tp* const_pointer;
			  typedef _Tp&       reference;
			  typedef const _Tp& const_reference;
			  typedef _Tp        value_type;

			  template<typename _Tp1> 
				  struct rebind {typedef malloc_allocator<_Tp1> other; };

			  malloc_allocator() throw() { }

			  malloc_allocator(const malloc_allocator&) throw() { }

			  template<typename _Tp1>
				  malloc_allocator(const malloc_allocator<_Tp1>&) throw() { }

			  ~malloc_allocator() throw() { }

			  pointer address(reference __x) const { return &__x; }

			  const_pointer address(const_reference __x) const { return &__x; }

			  // NB: __n is permitted to be 0.  The C++ standard says nothing
			  // about what the return value is when __n == 0.
			  pointer allocate(size_type __n, const void* = 0)
			  {
				  if (__builtin_expect(__n > this->max_size(), false))
					  std::__throw_bad_alloc();

				  pointer __ret = static_cast<_Tp*>(malloc(__n * sizeof(_Tp)));
				  if (!__ret)
					  std::__throw_bad_alloc();
				  std::cout << "allocate:" << (int)__n << ":" << (void*)__ret <<std::endl;
				  return __ret;
			  }

			  // __p is not permitted to be a null pointer.
			  void deallocate(pointer __p, size_type)
			  { 
				  if((void *)__p == NULL)
					  std::cout << "NULL!!!" << std::endl;
				  std::cout << "deallocate:" << (void *)__p << std::endl;
				  free(static_cast<void*>(__p)); 
			  }

			  size_type max_size() const throw() 
			  { return size_t(-1) / sizeof(_Tp); }

			  // _GLIBCXX_RESOLVE_LIB_DEFECTS
			  // 402. wrong new expression in [some_] allocator::construct
			  void construct(pointer __p, const _Tp& __val) 
			  {
				  std::cout << "construct..." << std::endl;
				  ::new(__p) value_type(__val); 
			  }

			  void destroy(pointer __p) 
			  {
				  std::cout << "destory..." << std::endl;
				  __p->~_Tp(); 
			  }
	  };

  template<typename _Tp>
	  inline bool operator==(const malloc_allocator<_Tp>&, const malloc_allocator<_Tp>&)
	  { return true; };

  template<typename _Tp>
	  inline bool operator!=(const malloc_allocator<_Tp>&, const malloc_allocator<_Tp>&)
	  { return false; };

  typedef std::basic_string<char, std::char_traits<char>, malloc_allocator<char> > string;
  typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, malloc_allocator<wchar_t> > wstring;
  template<typename _Tp>
	  class vector : public std::vector<_Tp, malloc_allocator<_Tp> >{};
  template<typename _Tp>
	  class list :	public std::list<_Tp, malloc_allocator<_Tp> >{};
  template<typename _Tp>
	  class deque : public std::deque<_Tp, malloc_allocator<_Tp> >{};
  template<typename _Tp>
	  class queue : public std::queue<_Tp, deque<_Tp> >{};
  template<typename _Tp>
	  class stack : public std::stack<_Tp, deque<_Tp> >{};
  template<typename _Key, typename _Compare = std::less<_Key> >
	  class set : public std::set<_Key, _Compare, malloc_allocator<_Key> >{};
  template<typename _Key, typename _Compare = std::less<_Key> >
	  class multiset : public std::multiset<_Key, _Compare, malloc_allocator<_Key> >{};
  template<typename _Key, typename _Tp, typename _Compare = std::less<_Key> >
	  class map : public std::map<_Key, _Tp, _Compare, malloc_allocator<std::pair<const _Key, _Tp> > >{};
  template<typename _Key, typename _Tp, typename _Compare = std::less<_Key> >
	  class multimap : public std::multimap<_Key, _Tp, _Compare, malloc_allocator<std::pair<const _Key, _Tp> > >{};
}

#endif
