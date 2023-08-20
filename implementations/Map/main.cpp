#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

template <class Key, class Value> class Map {
private:
  struct Node {
    Node *left;
    Node *right;
    Node *parent;
    std::pair<Key, Value> pair;

    Node(Node *left_ = nullptr, Node *right_ = nullptr, Node *parent_ = nullptr,
         Key key = Key{}, Value val = Value{})
        : left(left_), right(right_), parent(parent_), pair({key, val}) {}
  };

  Node *root = nullptr;

  std::size_t size_ = 0;

  // Мой метод для упрощения copy конструктора
  void copy_tree(Node *node, Node *node2) {
    node2->pair = node->pair;

    if (node->left) {
      Node *tmp = new Node(nullptr, nullptr, node2);
      node2->left = tmp;
      copy_tree(node->left, node2->left);
    }

    if (node->right) {
      Node *tmp2 = new Node(nullptr, nullptr, node2);
      node2->right = tmp2;
      copy_tree(node->right, node2->right);
    }
  }

  // Мой метод поиска =)
  Node *search(const Key &key, Node *node) {
    if (node == nullptr) {
      return nullptr;
    }

    if (key == node->pair.first) {
      return node;
    }

    if (key < node->pair.first && node->left) {
      return search(key, node->left);
    }

    if (key > node->pair.first && node->right) {
      return search(key, node->right);
    }

    return nullptr;
  }

  // Тот же метод что и search только константный.
  const Node *search(const Key &key, Node *node) const {
    if (node == nullptr) {
      return nullptr;
    }

    if (key == node->pair.first) {
      return node;
    }

    if (key < node->pair.first && node->left) {
      return search(key, node->left);
    }

    if (key > node->pair.first && node->right) {
      return search(key, node->right);
    }

    return nullptr;
  }

  // Тот же search но через цикл
  Node *searchCycle(const Key &key, Node *node) {
    if (node == nullptr) {
      return nullptr;
    }

    if (key == node->pair.first) {
      return node;
    }

    while (node->left || node->right) {
      if (key < node->pair.first && node->left) {
        node = node->left;
        if (key == node->pair.first) {
          return node;
        }
        continue;
      } else if (key > node->pair.first && node->right) {
        node = node->right;
        if (key == node->pair.first) {
          return node;
        }
        continue;
      } else {
        break;
      }
    }

    return nullptr;
  }

  // Мой метод очистки
  void clearup(Node *node) {
    if (node == nullptr) {
      return;
    }

    if (node->left) {
      clearup(node->left);
    }

    if (node->right) {
      clearup(node->right);
    }

    delete node;
  }

  // Метод которы отвязывает наследника от родителая
  void unlinkparent(Node *node) {
    if (node->parent) {
      if (node->pair.first > node->parent->pair.first) {
        node->parent->right = nullptr;
      } else if (node->pair.first < node->parent->pair.first) {
        node->parent->left = nullptr;
      }

      node->parent = nullptr;
    } else {
      throw std::out_of_range("Error!!!");
    }
  }

  // Метод который похож на search, но он ищет ближайщий
  // parent key-а и возврощает этот самый parent
  Node *FindPossibleParent(const Key &key, Node *node) {
    if (node == nullptr) {
      return nullptr;
    }

    if (key < node->pair.first && node->left == nullptr) {
      return node;
    }

    if (key > node->pair.first && node->right == nullptr) {
      return node;
    }

    if (key < node->pair.first && node->left) {
      return FindPossibleParent(key, node->left);
    }

    if (key > node->pair.first && node->right) {
      return FindPossibleParent(key, node->right);
    }
    return nullptr;
  }

public:
  class Iterator;
  class ConstIterator;
  // Создает пустой словарь
  Map() : root(nullptr) {}

  // Конструктор для initializer_list
  Map(std::initializer_list<std::pair<Key, Value>> other) {
    Map<Key, Value> map;
    for (auto &value : other) {
      map[value.first] = value.second;
    }
    swap(map);
  }

  // Создает новый словарь, являющийся глубокой копией other [O(n)]
  //  Map<std::string, int> map;
  //  map["something"] = 69;
  //  map["anything"] = 199;
  //  Map<std::string, int> copied{map};
  //  copied["something"] == map["something"] == 69
  Map(const Map &other) {
    if (other.root) {
      root = new Node(nullptr, nullptr, nullptr);
      copy_tree(other.root, root);
      size_ = other.size_;
    } else {
      root = nullptr;
    }
  };

  // Move конструктор
  Map(Map &&other) {
    std::swap(root, other.root);
    std::swap(size_, other.size_);
  };

  // Перезаписывает текущий словарь словарем other
  Map &operator=(const Map &other) {
    Map tmp{other};
    std::swap(root, tmp.root);
    std::swap(size_, tmp.size_);

    return *this;
  };

  // Присвоивание перемещением
  Map &operator=(Map &&other) {
    Map tmp{std::move(other)};
    std::swap(root, tmp.root);
    std::swap(size_, tmp.size_);

    return *this;
  };

  // Очищает память словаря
  ~Map() { clear(); };

  // Возвращает итератор на первый элемент
  Iterator begin() {
    if (root) {
      Node *tmp = root;
      while (tmp->left) {
        tmp = tmp->left;
      }
      return Iterator(tmp);
    }
    return Iterator(nullptr);
  };

  // Возвращает const итератор на первый элемент
  ConstIterator begin() const {
    if (root) {
      Node *tmp = root;
      while (tmp->left) {
        tmp = tmp->left;
      }
      return ConstIterator(tmp);
    }
    return ConstIterator(nullptr);
  };

  // Возвращает итератор обозначающий конец контейнера
  Iterator end() {
    return Iterator(nullptr);
  };

  // Возвращает const итератор обозначающий конец контейнера
  ConstIterator end() const {
    return ConstIterator(nullptr);
  };

  // Возвращает размер словаря (сколько есть узлов)
  std::size_t size() const { return size_; };

  // Проверяет есть ли элемент с таким ключом в контейнере
  bool contains(const Key &key) const { return search(key, root); }

  // Возвращает элемент по ключу. Если в словаре нет элемента с таким ключом, то
  // бросает исключение std::out_of_range
  const Value &operator[](const Key &key) const {
    const Node *tmp = search(key, root);
    if (tmp) {
      return tmp->pair.second;
    }

    throw std::out_of_range("Error!!! Key is out of range!!!");
  };

  // Возвращает ссылку на элемент по ключу (позволяет менять элемент). Если в
  // словаре нет элемента с таким ключем, то создает его и устанавливает
  // дефолтное значение, после чего возвращает на него ссылку. map["something"]
  // = 75;
  Value &operator[](const Key &key) {
    if (root == nullptr) {
      Node *tmp = new Node(nullptr, nullptr, nullptr, key);
      root = tmp;
      size_++;
      return root->pair.second;
    }

    Node *tmp = search(key, root);
    if (tmp) {
      return tmp->pair.second;
    }
    delete tmp;

    Node *ntmp = FindPossibleParent(key, root);
    if (ntmp) {
      Node *node = new Node(nullptr, nullptr, ntmp);
      node->pair.first = key;
      size_++;
      if (key > ntmp->pair.first) {
        ntmp->right = node;
        return ntmp->right->pair.second;
      } else if (key < ntmp->pair.first) {
        ntmp->left = node;
        return ntmp->left->pair.second;
      }
    }

    throw std::out_of_range("Error!!!");
  };

  // Удаляет элемент по ключу и возвращает значение удаленного элемента
  // Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.erase(4) == true
  // c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //             {5, "five"}, {6,"six"  }
  //   }; результат после erase
  //
  // В начальном условии метод был const, но в этом случае мы
  // банально не сможем сделать size--.
  bool erase(const Key &key) {
    if (size_ == 1) {
      root = nullptr;
      size_ = 0;
      return true;
    }

    Node* tmp = search(key, root);

    if (tmp != nullptr) {
      Node *new_tmp = tmp;
      if (tmp->right) {
        tmp = tmp->right;
        while (tmp->left) {
          tmp = tmp->left;
        }
        if (tmp->right) {
          tmp->parent->left = tmp->right;
          tmp->right->parent = tmp->parent;
        }
      } else if (tmp->left) {
        tmp = tmp->left;
        while (tmp->right) {
          tmp = tmp->right;
        }
        if (tmp->left) {
          tmp->parent->right = tmp->left;
          tmp->left->parent = tmp->parent;
        }
      }
      std::swap(tmp->pair, new_tmp->pair);

      if (tmp == tmp->parent->left) {
        tmp->parent->left = nullptr;
      }

      else if (tmp == tmp->parent->right) {
        tmp->parent->right = nullptr;
      }

      delete tmp;
      size_--;
      return true;
    }

    return false;
  };

  // Меняет текуший контейнер с контейнером other
  void swap(Map &other) {
    std::swap(root, other.root);
    std::swap(size_, other.size_);
  };

  // Возвращает итератор на первый элемент который не меньше чем переданный
  // ключ. [O(h)]
  Iterator lower_bound(const Key &key) {
    Node *tmp = search(key, root);
    if (tmp) {
      Iterator it(tmp);
      return it;
    } else {
      Node *ntmp = FindPossibleParent(key, root);
      if (ntmp->pair.first > key) {
        Iterator it(ntmp);
        return it;
      } else if (ntmp->pair.first < key && ntmp->right) {
        Iterator it(ntmp);
        ++it;
        return it;
      }
    }
    throw std::out_of_range("Your key is larger than largest in Map!!!");
  };

  // Очищает контейнер [O(n)]
  // Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.clear;
  // c.size() == 0 //true;
  void clear() {
    clearup(root);
    root = nullptr; // Если этого не сделать, то в root будет мусор
    size_ = 0;
  };


  class Iterator {
    friend Map;

  private:
    Node *node;

  public:
    // Заполнить:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    Iterator(Node *tmp) : node(tmp) {}
    // Инкремент. Движение к следующему элементу.
    Iterator &operator++() {
      if (node->right) {
        node = node->right;
        while (node && node->left) {
          node = node->left;
        }
      } else {
        while (node->parent && node->parent->pair.first < node->pair.first) {
          node = node->parent;
        }
        node = node->parent;
      }
      return *this;
    };

    // Декремент. Движение к предыдущему элементу.
    Iterator &operator--() {
      if (node->left) {
        node = node->left;
        while (node && node->right) {
          node = node->right;
        }
      } else {
        while (node->parent && node->parent->pair.first > node->pair.first) {
          node = node->parent;
        }
        node = node->parent;
      }
      return *this;
    };

    // Проверка на неравенство двух итераторов.
    bool operator!=(const Iterator &other) { return node != other.node; };

    bool operator==(const Iterator &other) { return node == other.node; }

    // разыменование (как с указателями): *it = 42; или std::cout << *it;
    // (Это так не работает).
    //
    // В начальном условие возврощает std::pair<const Key, Value>, но
    // в этом случае прийдется переписовать весь код где заменяем pair
    // в node-ах, потому как pair у каждого node будет закрыт
    //
    // P.S. Коля я перепесал тесты чтобы без const работали
    std::pair<Key, Value> &operator*() { return node->pair; }

    std::pair<Key, Value> *operator->() { return &node->pair; }
  };
  class ConstIterator {
    friend Map;

  private:
    Iterator it;

  public:
    // Заполнить
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    ConstIterator(Iterator iter) : it(iter) {}

    ConstIterator &operator++() {
      ++it;
      return *this;
    };

    ConstIterator &operator--() {
      --it;
      return *this;
    };

    bool operator!=(const ConstIterator &other) { return it != other.it; };

    bool operator==(const ConstIterator &other) { return it == other.it; };

    // единственное отличие от Iterator, что возвращает константную ссылку
    const std::pair<Key, Value> &operator*() { return *it; };

    const std::pair<Key, Value> *operator->() { return &(*it); }
  };
};

void test_operator_brackets_simple() {
  Map<std::string, std::string> map;
  map["Nikolay"] = "teacher";
  assert(map["Nikolay"] == "teacher");
}

void test_operator_brackets_empty_string() {
  Map<std::string, std::string> map;
  map[""] = "";
  assert(map[""] == "");
}

void test_operator_brackets_key_not_exist() {
  Map<std::string, std::string> map;
  assert(map["Nikolay"] == "");
}

void test_operator_brackets_key_not_exist_const() {
  const Map<std::string, std::string> map;

  // проверка, что исключение будет выброшено
  bool exception_thrown{};
  try {
    assert(map["Nikolay"] == "");
  } catch (std::out_of_range) {
    exception_thrown = true;
  }
  assert(exception_thrown);
}

void test_contains() {
  Map<std::string, std::string> map;
  map["Nikolay"] = "teacher";
  assert(map.contains("Nikolay"));
}

void test_contains_key_not_exist() {
  Map<std::string, std::string> map;
  assert(!map.contains("Nikolay"));
}

void test_copy_constructor() {
  Map<int, std::string> map;
  map[2] = "some string";
  map[1] = "other string";
  Map<int, std::string> map2{map};
  assert(map[1] == map2[1]);
}

void test_move_constructor() {
  Map<std::string, std::string> map;
  map["first"] = "some text";
  map["second"] = "some other text";
  Map<std::string, std::string> moved_map{std::move(map)};

  assert(map.size() == 0);
  assert(moved_map.size() == 2);
  assert(moved_map["second"] == "some other text");
}

void test_operator_equal_r_value() {
  Map<std::string, int> map;
  map["first"] = 1;
  map["second"] = 2;
  Map<std::string, int> moved_map;
  moved_map = std::move(map);

  assert(map.size() == 0);
  assert(moved_map.size() == 2);
  assert(moved_map["first"] == 1);
}

void test_size() {
  Map<int, std::string> dict;
  dict[1] = "one";
  dict[2] = "two";
  dict[2] = "another two";
  dict[3] = "tree";
  assert(dict.size() == 3);
}

void test_clear() {
  Map<int, std::string> map;
  map[1] = "first";
  map[2] = "second";
  map.clear();

  assert(map.size() == 0);
  assert(!map.contains(1));
  assert(!map.contains(2));
}

void test_swap_with_empty_container() {
  Map<int, int> map;
  map[1] = 25;
  map[2] = 45;
  map[10] = 4512;
  Map<int, int> map_for_swap;
  map_for_swap.swap(map);

  assert(map.size() == 0);
  assert(map_for_swap.size() == 3);
  assert(map_for_swap[10] == 4512);
}

void test_swap() {
  Map<int, int> map;
  map[1] = 25;
  map[2] = 45;
  map[10] = 4512;
  Map<int, int> map_for_swap;
  map_for_swap[3] = 4564;
  map_for_swap[4] = 6121;
  map_for_swap[58] = 8888;
  map_for_swap[95] = 545656;
  map_for_swap.swap(map);

  assert(map.size() == 4);
  assert(map_for_swap.size() == 3);
  assert(map_for_swap[10] == 4512);
  assert(map[95] == 545656);
}

void test_erase_simple() {
  Map<std::string, int> map;
  map["first"] = 1;
  map["second"] = 2;

  assert(!map.erase("last"));
  assert(map.contains("first"));
}

void test_erase_returned_value() {
  Map<std::string, int> map;
  map["first"] = 1;
  map["second"] = 2;

  assert(map.erase("first"));
  assert(!map.contains("first"));
  assert(map.size() == 1);
}

void test_lower_bound() {
  Map<std::string, std::string> map;
  map["b"] = "second key";
  map["a"] = "first key";
  map["f"] = "third key";

  std::pair<std::string, std::string> expected{"b", "second key"};
  assert(*map.lower_bound("b") == expected);
}

void test_lower_bound_equal() {
  Map<std::string, std::string> map;
  map["b"] = "second key";
  map["a"] = "first key";
  map["f"] = "third key";
  auto it = map.end();

  std::pair<std::string, std::string> expected{"f", "third key"};
  assert(*map.lower_bound("c") == expected);
}

int main() {

  test_operator_brackets_simple();
  test_operator_brackets_empty_string();
  test_operator_brackets_key_not_exist();
  test_operator_brackets_key_not_exist_const();

  test_contains();
  test_contains_key_not_exist();

  test_copy_constructor();
  test_move_constructor();
  test_operator_equal_r_value();

  test_size();

  test_clear();

  test_swap_with_empty_container();
  test_swap();

  test_erase_simple();
  test_erase_returned_value();

  test_lower_bound();
  test_lower_bound_equal();

  Map<int, int> map;
  for (int i = 0; i < 20; ++i) {
    map[i] = i;
  }

  Map<int, int>::ConstIterator it1 = map.begin();
  ++it1;
  std::cout << it1->second << std::endl;

  map.erase(14);
  for (auto it = map.begin(); it != map.end(); ++it) {
    std::cout << it->first << " " << it->second << std::endl;
  }
}
