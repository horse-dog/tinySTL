#include "tiny_iterator.h"

namespace tinySTL 
{

template <typename _Container>
class back_insert_iterator {

private:
  using container_type = _Container;
  using iterator_type = container_type::iterator;

public:
  using iterator_category = iterator_traits<iterator_type>::iterator_category;
  using value_type = iterator_traits<iterator_type>::value_type;
  using difference_type = iterator_traits<iterator_type>::difference_type;
  using pointer = iterator_traits<iterator_type>::pointer;
  using reference = iterator_traits<iterator_type>::reference;

  explicit back_insert_iterator(_Container& c) : container(&c) {}

  back_insert_iterator& operator=(const value_type& value) 
  {
    container->push_back(value);
    return *this;
  }

  back_insert_iterator& operator*() { return *this; }
  back_insert_iterator& operator++() { return *this; }
  back_insert_iterator  operator++(int) { return *this; }

private:
  _Container* container;

};

template <typename _Container>
class front_insert_iterator {

private:
  using container_type = _Container;
  using iterator_type = container_type::iterator;

public:
  using iterator_category = iterator_traits<iterator_type>::iterator_category;
  using value_type = iterator_traits<iterator_type>::value_type;
  using difference_type = iterator_traits<iterator_type>::difference_type;
  using pointer = iterator_traits<iterator_type>::pointer;
  using reference = iterator_traits<iterator_type>::reference;

  explicit front_insert_iterator(_Container& c) : container(&c) {}

  front_insert_iterator& operator=(const value_type& value) 
  {
    container->push_front(value);
    return *this;
  }

  front_insert_iterator& operator*() { return *this; }
  front_insert_iterator& operator++() { return *this; }
  front_insert_iterator  operator++(int) { return *this; }

private:
  _Container* container;

};


template <typename _Container>
class insert_iterator {

private:
  using container_type = _Container;
  using iterator_type = container_type::iterator;

public:
  using iterator_category = iterator_traits<iterator_type>::iterator_category;
  using value_type = iterator_traits<iterator_type>::value_type;
  using difference_type = iterator_traits<iterator_type>::difference_type;
  using pointer = iterator_traits<iterator_type>::pointer;
  using reference = iterator_traits<iterator_type>::reference;

  explicit insert_iterator(_Container& c, iterator_type it) : container(&c), iter(it) {}

  insert_iterator& operator=(const value_type& value) 
  {
    iter = container->insert(iter, value); ++iter;
    return *this;
  }

  insert_iterator& operator*() { return *this; }
  insert_iterator& operator++() { return *this; }
  insert_iterator& operator++(int) { return *this; }

private:
  _Container* container;
  iterator_type iter;
};

template <class _Container>
back_insert_iterator<_Container> 
back_inserter(_Container& c) 
{
  return back_insert_iterator<_Container>(c);
}

template <class _Container>
front_insert_iterator<_Container> 
front_inserter(_Container& c) 
{
  return front_insert_iterator<_Container>(c);
}

template <class _Container>
insert_iterator<_Container> 
inserter(_Container& c, typename _Container::iterator it) 
{
  return insert_iterator<_Container>(c, it);
}

}