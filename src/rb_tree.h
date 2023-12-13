#pragma once

#include <atomic>
#include <memory>
#include <sstream>
#include <iostream>

#define __TINY_MEMPOOL__

class tiny_mempool {
 protected:
 struct memNode { memNode *nextnode = nullptr; };

 protected:
  std::atomic<memNode*> m_free_head[16];

 private:
  tiny_mempool() {}

  ~tiny_mempool()
  { for (int i = 0; i < 16; i++)
    { if (m_free_head[i] != nullptr)
      { memNode *ptr = m_free_head[i];
        while (ptr != nullptr)
        { auto nptr = ptr->nextnode;
          free(ptr);
          ptr = nptr;
        }
      }
      m_free_head[i] = nullptr;
    }
  }

  int getindex(int size)
  { static const unsigned int sizetable[16]
    = { 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128 };
    int __distance = 0;
    for (; __distance < 16; __distance++)
      if (sizetable[__distance] >= size)
        break;
    return __distance;
  }

 public:
  static tiny_mempool &instance()
  { static tiny_mempool pool;
    return pool;
  }

 public:
  void *alloc(int size)
  { if (size > 128) return malloc(size);
    int index = getindex(size);
    int realsize = (index + 1) << 3;
    memNode *p = m_free_head[index];
    if (p == nullptr)
      return malloc(realsize);
    else
    { while (!m_free_head[index].compare_exchange_weak(p, p->nextnode))
        if (p == nullptr) return malloc(realsize);
      return p;
    }
    return nullptr;
  }

  void delloc(void *ptr, int size)
  { if (ptr == nullptr) return;
    if (size > 128) return free(ptr);
    int index = getindex(size);
    memNode *pNew = (memNode *)ptr;
    pNew->nextnode = m_free_head[index];
    while (!(m_free_head[index].compare_exchange_weak(pNew->nextnode, pNew)))
    ;
  }
  
  /**
   * @brief report memory distribute in the pool.
   * @attention May cause undefined result if 
   * allocate memory use current pool before this
   * function return.
   */
  void report()
  { printf("\033[32m\033[1mtiny_mempool report\033[0m\n");
    printf("\033[34mindex\tnode size   node count\033[0m\n");
    for (int i = 0; i < 16; ++i)
    { int n = 0;
      memNode *p = m_free_head[i];
      while (p)
      { n++;
        p = p->nextnode;
      }
      printf("\033[31m%5d\t %3d \033[35mbyte\033[31m   %10d"
             "\033[0m\n", i, (i + 1) << 3, n);
    }
  }

};

template<class T>
class tiny_allocator { 
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  tiny_allocator() {}

  tiny_allocator(tiny_allocator const &) {}

  tiny_allocator &operator=(tiny_allocator const &)
  { return *this; }

  template<class Other>
  tiny_allocator(tiny_allocator<Other> const &) {}

  template<class Other>
  tiny_allocator &operator=(tiny_allocator<Other> const &)
  { return *this; }

  pointer allocate(size_type count)
  { return (pointer)tiny_mempool::instance()
      .alloc(count * sizeof(value_type));
  }

  void deallocate(pointer ptr, size_type count)
  { return tiny_mempool::instance()
      .delloc(ptr, count * sizeof(value_type));
  }
};

template <typename _Tp1, typename _Tp2>
std::ostream& operator<<(std::ostream& os, 
  const std::pair<_Tp1, _Tp2>& pair) {
  return os << '{' << pair.first << ", " << pair.second << '}';
}

enum class rbcolor { red = false, blk = true };

struct rbnode_base {
  rbcolor m_color;
  rbnode_base* m_parent;
  rbnode_base* m_lchild;
  rbnode_base* m_rchild;
};

template <typename T>
struct rbnode : public rbnode_base {
  T m_data;

  T& data() const { return (T&)m_data; }

  rbcolor& color() const { return (rbcolor&)m_color; }

  rbnode*& parent() const { return (rbnode*&)m_parent; }

  rbnode*& lchild() const { return (rbnode*&)m_lchild; }

  rbnode*& rchild() const { return (rbnode*&)m_rchild; }

  rbnode*& brother() const {
    if (this == parent()->lchild())
      return parent()->rchild();
    else
      return parent()->lchild();
  }

  rbnode*& gparent() const { return parent()->parent(); }

  rbnode*& uncle() const {
    rbnode* father = parent();
    rbnode* grandfather = gparent();
    if (father == grandfather->lchild())
      return grandfather->rchild();
    else
      return grandfather->lchild();
  }

  bool isRed() const { return m_color == rbcolor::red; }

  bool isBlk() const { return m_color == rbcolor::blk; }

  void setRed() { m_color = rbcolor::red; }

  void setBlk() { m_color = rbcolor::blk; }

  static rbnode* prev(rbnode* x) {
    if (x->isRed() && x->gparent() == x) {
      x = x->rchild();
    } else if (x->lchild() != nullptr) {
      rbnode* y = x->lchild();
      while (y->rchild() != nullptr)
        y = y->rchild();
      x = y;
    } else {
      rbnode* y = x->parent();
      while (x == y->lchild()) {
        x = y;
        y = y->parent();
      }
      if (x->lchild() != y)
        x = y;
    }
    return x;
  }

  static rbnode* next(rbnode* x) {
    if (x->isRed() && x->gparent() == x) {
      x = x->lchild();
    } else if (x->rchild() != nullptr) {
      rbnode* y = x->rchild();
      while (y->lchild() != nullptr)
        y = y->lchild();
      x = y;
    } else {
      rbnode* y = x->parent();
      while (x == y->rchild()) {
        x = y;
        y = y->parent();
      }
      if (x->rchild() != y)
        x = y;
    }
    return x;
  }

};

template <typename T, typename Alloc=tiny_allocator<T>>
class rbtree {
 protected:
  template <typename _Tp, typename _Up>
  struct alloc_rebind {};

  template <template <typename, typename...> class _Template,
            typename _Up, typename _Tp, typename... _Types>
  struct alloc_rebind<_Template<_Tp, _Types...>, _Up>
  { using type = _Template<_Up, _Types...>; };

  using node = rbnode<T>;
  using allocator_type = typename alloc_rebind<Alloc, node>::type;

  struct rbtree_impl : public allocator_type
  { rbnode_base m_head;
    size_t m_node_count;
  };

  rbtree_impl m_impl;

 public:
  rbtree() { reset(); }

  rbtree(const rbtree& tree) { 
    if (tree.empty()) {
      reset();
      return;
    }
    m_impl.m_head.m_color = tree.m_impl.m_head.m_color;
    m_impl.m_node_count = tree.m_impl.m_node_count;
    root() = copyfrom(tree.root());
    root()->parent() = head();
    node* __min = root();
    node* __max = root();
    while (__min->lchild() != nullptr)
      __min = __min->lchild();
    while (__max->rchild() != nullptr)
      __max = __max->rchild();
    leftmost() = __min;
    rightmost() = __max;
  }

  rbtree(rbtree&& tree) {
    if (!tree.empty())
      movefrom(tree);
    else
      reset();
  }

  ~rbtree() { clear(); }

 public:
  bool insert(const T& value) 
  { return insert_unique(value); }

  bool insert_unique(const T& value) {
    node* pos = root();
    node* pos_parent = head();
    while (pos != nullptr) {  
      pos_parent = pos;
      if (less(value, pos->data()))
        pos = pos->lchild();
      else if (less(pos->data(), value))
        pos = pos->rchild();
      else
        return false;
    }
    __insert(pos_parent, value);
    ++m_impl.m_node_count;
    return true;
  }

  void insert_equal(const T& value) {
    node* pos = root();
    node* pos_parent = head();
    while (pos != nullptr) {  
      pos_parent = pos;
      if (less(value, pos->data()))
        pos = pos->lchild();
      else
        pos = pos->rchild();
    }
    __insert(pos_parent, value);
    ++m_impl.m_node_count;
  }

  size_t erase(const T& value) {
    node* lb = lower_bound(value);
    node* rb = upper_bound(value);
    size_t n = 0;
    while (lb != rb) {
      node* tmp = lb;
      lb = node::next(lb);
      __erase(tmp);
      ++n;
    }
    m_impl.m_node_count -= n;
    return n;
  }

  void clear() {
    if (!empty()) {
      __destroy(root());
      reset();
    }
  }

  size_t size() const
  { return m_impl.m_node_count; }

  bool empty() const
  { return size() == 0; }

  void disp() const {
    if (!empty())
      __disp(root());
    std::cout << std::endl;
  }

  template <typename U>
  node* lower_bound(const U& v) const {
    node* y = head();
    node* x = root();
    while (x != nullptr) {
      if (!less(x->m_data, v)) {
        y = x;
        x = x->lchild();
      } else {
        x = x->rchild();
      }
    }
    return y;
  }

  template <typename U>
  node* upper_bound(const U& v) const {
    node* y = head();
    node* x = root();
    while (x != nullptr) {
      if (less(v, x->m_data)) {
        y = x;
        x = x->lchild();
      } else {
        x = x->rchild();
      }
    }
    return y;    
  }

  rbtree& operator=(const rbtree& tree) {
    if (this == &tree) return *this;
    clear();
    new (this) rbtree(tree);
    return *this;
  }

  rbtree& operator=(rbtree&& tree) {
    if (this == &tree) return *this;
    clear();
    new (this) rbtree(std::move(tree));
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const rbtree& tree) {
    os << '{';
    auto begin = tree.leftmost();
    auto end = tree.head();
    if (begin != end) {
      os << begin->m_data;
      begin = node::next(begin);
      while (begin != end) {
        os << ", " << begin->m_data;
        begin = node::next(begin);
      }
    }
    return os << '}';
  }

 protected:
  node* head() const
  { return (node*)(&m_impl.m_head); }

  node*& root() const
  { return (node*&)m_impl.m_head.m_parent; }

  node*& leftmost() const
  { return (node*&)m_impl.m_head.m_lchild; }

  node*& rightmost() const
  { return (node*&)m_impl.m_head.m_rchild; }

  node* create_node(const T& x) { 
    node* tmp = m_impl.allocate(1);
    try {
       new (&tmp->m_data) T(x);
    } catch (...) {
      m_impl.deallocate(tmp, 1);
    }
    return tmp;
  }

  void destroy_node(node* p) {
    (&p->m_data)->~T();
    m_impl.deallocate(p, 1);
  }

  node* copyfrom(const node* rt) {
    if (rt == nullptr) return nullptr;
    node* p = create_node(rt->m_data);
    p->m_color = rt->m_color;
    node* lchild = copyfrom((const node*)rt->m_lchild);
    node* rchild = copyfrom((const node*)rt->m_rchild);
    p->lchild() = lchild;
    p->rchild() = rchild;
    if (lchild != nullptr)
      lchild->parent() = p;
    if (rchild != nullptr)
      rchild->parent() = p;
    return p;
  }

  void movefrom(rbtree& x) {
    head()->color() = x.head()->color();
    root() = x.root();
    leftmost() = x.leftmost();
    rightmost() = x.rightmost();
    root()->parent() = head();
    m_impl.m_node_count = x.m_impl.m_node_count;
    x.reset();
  }
  
  void reset() {
    m_impl.m_head.m_color = rbcolor::red;
    root() = nullptr;
    leftmost() = head();
    rightmost() = head();
    m_impl.m_node_count = 0;
  }

  template <typename _Tp>
  static bool less(const _Tp& x, const _Tp& y) 
  { return x < y; }

  template <typename _Tp1, typename _Tp2>
  static bool less(const std::pair<_Tp1, _Tp2>& x, 
            const std::pair<_Tp1, _Tp2>& y) {
    return x.first < y.first;
  }

  template <typename _Tp1, typename _Tp2>
  static bool less(const std::pair<_Tp1, _Tp2>& x, const _Tp1& y) {
    return x.first < y;
  }

  template <typename _Tp1, typename _Tp2>
  static bool less(const _Tp1& x, const std::pair<_Tp1, _Tp2>& y) {
    return x < y.first;
  }

 protected:
  void __leftRotate(node* x) {
    node* rchild = x->rchild();
    x->rchild() = rchild->lchild();
    if (rchild->lchild() != nullptr)
      rchild->lchild()->parent() = x;
    if (x == root())
      root() = rchild;
    else if (x == x->parent()->lchild())
      x->parent()->lchild() = rchild;
    else 
      x->parent()->rchild() = rchild;
    rchild->parent() = x->parent();
    rchild->lchild() = x;
    x->parent() = rchild;
  }

  void __rightRotate(node* x) {
    node* lchild = x->lchild();
    x->lchild() = lchild->rchild();
    if (lchild->rchild() != nullptr)
      lchild->rchild()->parent() = x;
    if (x == root())
      root() = lchild;
    else if (x == x->parent()->lchild())
      x->parent()->lchild() = lchild;
    else 
      x->parent()->rchild() = lchild;
    lchild->parent() = x->parent();
    lchild->rchild() = x;
    x->parent() = lchild;
  }

  void __disp(node* rt) const {
    if (rt->isRed())
      std::cout << "\033[1;31m" << rt->m_data;
    else 
      std::cout << "\033[1;34m" << rt->m_data;
    std::cout << "\033[0m";
    if (rt->lchild() || rt->rchild()) {
      std::cout << '(';
      if (rt->lchild())
        __disp(rt->lchild());
      if (rt->rchild()) {
        std::cout << ", ";
        __disp(rt->rchild());
      }
      std::cout << ')';
    }
  }

  void __destroy(node* pos) {
    while (pos != nullptr) {
      __destroy(pos->rchild());
      node* tmp = pos->lchild();
      destroy_node(pos);
      pos = tmp;
    }
  }

  void __insert(node* pos_parent, const T& value) {
    node* new_node = create_node(value);
    if (pos_parent == head()) {
      leftmost() = rightmost() = root() = new_node;
      new_node->setBlk();
      new_node->parent() = pos_parent;
      new_node->lchild() = nullptr;
      new_node->rchild() = nullptr;
      return;
    } else if (value < pos_parent->data()) {
      pos_parent->lchild() = new_node;
      if (pos_parent == leftmost())
        leftmost() = new_node;
    } else {
      pos_parent->rchild() = new_node;
      if (pos_parent == rightmost())
        rightmost() = new_node;
    }
    new_node->parent() = pos_parent;
    new_node->lchild() = nullptr;
    new_node->rchild() = nullptr;
    __preFixInsert(new_node);
  }

  void __erase(node* pos) {
    node* y = pos;
    node* x = nullptr;
    node* x_parent = nullptr;

    if (y->lchild() == nullptr) {
      x = y->rchild();
    } else {
      if (y->rchild() == nullptr) {
        x = y->lchild();
      } else {
        y = y->rchild();
        while (y->lchild() != nullptr)
          y = y->lchild();
        x = y->rchild();
      }
    }

    if (y != pos) {
      y->lchild() = pos->lchild();
      pos->lchild()->parent() = y;

      if (y != pos->rchild()) {
        x_parent = y->parent();
        if (x != nullptr)
          x->parent() = y->parent();
        y->parent()->lchild() = x;
        y->rchild() = pos->rchild();
        pos->rchild()->parent() = y;
      } else {
        x_parent = y;
      }

      if (pos == root())
        root() = y;
      else if (pos->parent()->lchild() == pos)
        pos->parent()->lchild() = y;
      else
        pos->parent()->rchild() = y;
      y->parent() = pos->parent();
      rbcolor tmp = y->color();
      y->color() = pos->color();
      pos->color() = tmp;
      y = pos;
    } else {
      x_parent = y->parent();
      if (x != nullptr)
        x->parent() = y->parent();
      
      if (pos == root()) {
        root() = x;
      } else {
        if (pos->parent()->lchild() == pos)
          pos->parent()->lchild() = x;
        else
          pos->parent()->rchild() = x;
      }

      if (pos == leftmost()) {
        if (pos->rchild() == nullptr)
          leftmost() = pos->parent();
        else
          leftmost() = x;
      }

      if (pos == rightmost()) {
        if (pos->lchild() == nullptr)
          rightmost() = pos->parent();
        else 
          rightmost() = x;
      }
    }

    if (x == root()) {
      if (x != nullptr)
        x->setBlk();
    } else if (y->isBlk())
      __preFixErase(x, x_parent);
    
    destroy_node(y);
  }

  void __preFixInsert(node* new_node) {
    new_node->setRed();
    if (new_node->parent()->isRed())
      __fixInsert(new_node);
  }

  void __preFixErase(node* successor, node* suc_parent) {
    if (successor != nullptr && successor->isRed()) {
      successor->setBlk();
      return;
    }
    __fixErase(successor, suc_parent);
  }

  void __fixInsert(node* new_node) {
    node* parent = new_node->parent();
    node* gparent = new_node->gparent();
    node* uncle = new_node->uncle();
    while (true) {
      if (parent == gparent->lchild()) {
        if (uncle == nullptr || uncle->isBlk()) {
          if (new_node == parent->rchild()) {
            __leftRotate(parent);
            new_node = parent;
            parent = new_node->parent();
          }
          parent->setBlk();
          gparent->setRed();
          __rightRotate(gparent);
          return;
        } else {
          parent->setBlk();
          uncle->setBlk();
                      
          if (gparent == root())
            return;
          gparent->setRed();
          if (gparent->parent()->isBlk())
            return;
          new_node = gparent;
          parent = new_node->parent();
          gparent = new_node->gparent();
          uncle = new_node->uncle();
          continue;
        }
      } else {
        if (uncle == nullptr || uncle->isBlk()) {
          if (new_node == parent->lchild()) {
            __rightRotate(parent);
            new_node = parent;
            parent = new_node->parent();
          }
          parent->setBlk();
          gparent->setRed();
          __leftRotate(gparent);
          return;
        } else {
          parent->setBlk();
          uncle->setBlk();
          if (gparent == root())
            return;
          gparent->setRed();
          if (gparent->parent()->isBlk())
            return;
          new_node = gparent;
          parent = new_node->parent();
          gparent = new_node->gparent();
          uncle = new_node->uncle();
          continue;
        }
      }
    }
    return; // never reach.
  }

  void __fixErase(node* successor, node* suc_parent) {
    node* parent = suc_parent;
    node* brother;
    while (true) {
      if (successor == parent->lchild()) {
        brother = parent->rchild();
        if (brother->isRed()) {
          parent->setRed();
          brother->setBlk();
          __leftRotate(parent);
          brother = parent->rchild();
        }
        if ((brother->lchild() == nullptr || brother->lchild()->isBlk()) 
         && (brother->rchild() == nullptr || brother->rchild()->isBlk())) {
          brother->setRed();
          if (parent->isRed()) {
            parent->setBlk();
            return;
          }
          if (parent == root())
            return;
          successor = parent;
          parent = successor->parent();
          brother = successor->brother();
          continue;
        } else {
          if (brother->rchild() == nullptr || brother->rchild()->isBlk()) {
            brother->setRed();
            brother->lchild()->setBlk();
            __rightRotate(brother);
            brother = parent->rchild();
          }
          brother->color() = parent->color();
          parent->setBlk();
          brother->rchild()->setBlk();
          __leftRotate(parent);
          return;
        }
      } else {
        brother = parent->lchild();
        if (brother->isRed()) {
          parent->setRed();
          brother->setBlk();
          __rightRotate(parent);
          brother = parent->lchild();
        }
        if ((brother->rchild() == nullptr || brother->rchild()->isBlk()) 
         && (brother->lchild() == nullptr || brother->lchild()->isBlk())) {
          brother->setRed();
          if (parent->isRed()) {
            parent->setBlk();
            return;
          }
          if (parent == root())
            return;
          successor = parent;
          parent = successor->parent();
          brother = successor->brother();
          continue;
        } else {
          if (brother->lchild() == nullptr || brother->lchild()->isBlk()) {
            brother->setRed();
            brother->rchild()->setBlk();
            __leftRotate(brother);
            brother = parent->lchild();
          }
          brother->color() = parent->color();
          parent->setBlk();
          brother->lchild()->setBlk();
          __rightRotate(parent);
          return;
        }
      }
    }
    return; // never reach.
  }
  
};

template <typename T>
class set {
 protected:
  rbtree<T> m_tree;

 public:
  set() = default;

  set(std::initializer_list<T> l) : m_tree() {
    for (auto&& x : l)
      m_tree.insert(x);
  }

  bool insert(const T& x)
  { return m_tree.insert_unique(x); }

  bool erase(const T& x)
  { return m_tree.erase(x) != 0; }

  bool empty() const 
  { return m_tree.empty(); }

  size_t size() const 
  { return m_tree.size(); }

  void clear()
  { m_tree.clear(); }

  void disp() const
  { m_tree.disp(); }

  friend std::ostream& operator<<(std::ostream& os, const set& x)
  { return os << x.m_tree; }

};

template <typename K, typename V>
class map {
 protected:
  rbtree<std::pair<K, V>> m_tree;

 public:
  map() = default;

  map(std::initializer_list<std::pair<K, V>> l) : m_tree() {
    for (auto&& x : l)
      m_tree.insert(x);
  }

  bool insert(const std::pair<K, V>& x)
  { return m_tree.insert_unique(x); }

  bool erase(const std::pair<K, V>& x)
  { return m_tree.erase(x) != 0; }

  bool empty() const 
  { return m_tree.empty(); }

  size_t size() const 
  { return m_tree.size(); }

  void clear()
  { m_tree.clear(); }

  void disp() const
  { m_tree.disp(); }

  friend std::ostream& operator<<(std::ostream& os, const map& x)
  { return os << x.m_tree; }

  V& operator[](const K& key) {
    m_tree.insert_unique(std::pair<K, V>(key, V()));
    rbnode<std::pair<K, V>>* lower = m_tree.lower_bound(key);
    return lower->m_data.second;
  }

  const V& operator[](const K& key) const {
    rbnode<std::pair<K, V>>* lower = m_tree.lower_bound(key);
    if (lower->m_data.first != key) {
      std::ostringstream msg;
      msg << "KeyError: " << key;
      throw std::logic_error(msg.str());
    }
    else
      return lower->m_data.second;
  }

};
