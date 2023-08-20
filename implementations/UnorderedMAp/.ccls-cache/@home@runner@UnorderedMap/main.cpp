#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

template <class Key, class Value> class UnorderedMap {
public:
  class Iterator;
  class ConstIterator;
  // Создает пустой словарь
  UnorderedMap();

  // Создает новый UnorderedMap, являющийся глубокой копией other [O(n)]
  // UnorderedMap<std::string, int>  map;
  // map["something"] = 69;
  // map["anything"] = 199;
  // UnorderedMap<std::string, int> copied{map};
  // copied["something"] == map["something"] == 69
  UnorderedMap(const UnorderedMap &other);

  // Конструктор перемещения
  UnorderedMap(UnorderedMap &&other);

  // Перезаписывает текущий словарь словарем other
  UnorderedMap &operator=(const UnorderedMap &other);

  // Присваивание перемещением
  UnorderedMap &operator=(UnorderedMap &&other);

  ~UnorderedMap();

  // Возвращает итератор на первый элемент
  Iterator begin();

  // Возвращает константный итератор на первый элемент
  ConstIterator begin() const;

  // Возвращает итератор обозначающий конец контейнера (за последним элементом)
  Iterator end();

  // Возвращает константный итератор, обозначающий конец контейнера
  ConstIterator end() const;

  // Возвращает размер UnorderedMap (сколько элементов добавлено)
  std::size_t size() const;

  // Проверяет является ли UnorderedMap пустым
  bool empty() const;

  // Возвращает элемент по ключу. Если отсутсвует, выбрасывает исключение
  const Value &operator[](Key key) const;

  // Возвращает ссылку на элемент по Key (позволяет менять элемент). Если
  // элемент с таким ключом отсутствует, создает его и инициализирует дефолтным
  // значением map["something"] = 75;
  Value &operator[](Key key);

  // Проверяет есть ли в контейнере элемент с таким Key
  bool contains(const Key &key) const;

  // Возвращяет Итератор на элемент который ищем, если нет такого элемента
  // возвращает итератор на последный элемент
  Iterator find(const Key &key);

  // Добавляет новый элемент с ключем и значением, если нет уже существуюшего
  // элемента с таким же ключом Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.(5,"something");
  // возвращает false, потому что элмемент с key = 5 уже существует․
  bool insert(const Key &k, const Value &v);

  // Удаляет элемент по ключу и возвращает результат операции
  // UnorderedMap<int, std::string> c =
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
  bool erase(const Key &key);

  class Iterator {

  public:
    // Заполнить
    // using iterator_category = ? ;
    // using value_type =        ? ;
    // using difference_type =   ? ;
    // using pointer =           ? ;
    // using reference =         ? ;

    Iterator();

    Iterator &operator++();
    Iterator &operator--();

    bool operator!=(const Iterator &other);
    bool operator==(const Iterator &other);

    // Разыменовывает указатель: std::cout << it->second;
    std::pair<Key, Value> &operator->();

    // Возвращает значение итератора: *it = {"travel", 42};
    std::pair<Key, Value> &operator*();
  };
  class ConstIterator {
    Iterator it;

  public:
    // Заполнить
    // using iterator_category = ? ;
    // using value_type =        ? ;
    // using difference_type =   ? ;
    // using pointer =           ? ;
    // using reference =         ? ;

    ConstIterator();

    ConstIterator &operator++();
    ConstIterator &operator--();

    bool operator!=(const ConstIterator &other);
    bool operator==(const ConstIterator &other);

    // единственное отличие от Iterator, что возвращает константную ссылку
    const std::pair<Key, Value> &operator*();
  };
};

void test_operator_brackets_simple() {
  UnorderedMap<std::string, std::string> map;
  map["Nikolay"] = "teacher";
  assert(map["Nikolay"] == "teacher");
}

void test_operator_brackets_empty_string() {
  UnorderedMap<std::string, std::string> map;
  map[""] = "";
  assert(map[""] == "");
}

void test_operator_brackets_key_not_exist() {
  UnorderedMap<std::string, std::string> map;
  assert(map["Nikolay"] == "");
}

void test_operator_brackets_key_not_exist_const() {
  const UnorderedMap<std::string, std::string> map;
  bool exception_thrown{};

  // проверка, что исключение будет выброшено
  try {
    assert(map["Nikolay"] == "");
  } catch (std::out_of_range) {
    exception_thrown = true;
  }
  assert(exception_thrown);
}

void test_contains_simple() {
  UnorderedMap<std::string, std::string> map;
  map["Nikolay"] = "teacher";
  assert(map.contains("Nikolay"));
}

void test_contains_false() {
  UnorderedMap<std::string, std::string> map;
  assert(!map.contains("Nikolay"));
}

void test_find() {
  UnorderedMap<std::string, std::string> map;
  map["Nikolay"] = "teacher";
  map["John"] = "programmer";

  assert(map.find("Nikolay") != map.end());
}

void test_find_not_found() {
  UnorderedMap<std::string, std::string> map;
  map["Nikolay"] = "teacher";
  map["John"] = "programmer";

  assert(map.find("Alex") == map.end());
}

void test_insert_false() {
  UnorderedMap<int, std::string> c;
  c[1] = "one";
  c[2] = "two";
  c[3] = "tree";
  c[4] = "four";
  c[5] = "five";
  assert(c.insert(5, "something") == false);
}

void test_insert_accessed() {
  UnorderedMap<int, std::string> c;
  c[1] = "one";
  c[2] = "two";
  c[3] = "tree";
  c[4] = "four";
  c[5] = "five";
  bool res = c.insert(7, "something");
  assert(res == true);
}

void test_erase_simple() {
  UnorderedMap<std::string, int> map;
  map["first"] = 1;
  map["second"] = 2;

  assert(map.erase("last") == false);
  assert(map.contains("first") == true);
}

void test_erase_returned_value() {
  UnorderedMap<std::string, int> map;
  map["first"] = 1;
  map["second"] = 2;

  assert(map.erase("first") == false);
  assert(map.contains("first") == true);
  assert(map.size() == 1);
}

void test_empty_simple() {
  UnorderedMap<int, std::string> dict;
  dict[1] = "one";
  assert(dict.empty() == false);
}

void test_empty() {
  UnorderedMap<int, std::string> dict;
  assert(dict.empty() == true);
}

void test_size() {
  UnorderedMap<int, std::string> dict;
  dict[1] = "one";
  dict[2] = "two";
  dict[3] = "tree";
  assert(dict.size() == 3);
}

void test_size_empty_container() {
  UnorderedMap<int, std::string> dict;
  dict[1] = "one";
  dict[2] = "two";
  dict[3] = "tree";
  for (int i = 1; i < 4; ++i) {
    dict.erase(i);
  }
  assert(dict.size() == 0);
}

void test_operator_equal_r_value() {
  UnorderedMap<std::string, int> map;
  map["first"] = 1;
  map["second"] = 2;
  UnorderedMap<std::string, int> moved_map;
  moved_map = std::move(map);

  assert(map.empty());
  assert(moved_map["first"] == 1);
}

void test_move_constructor() {
  UnorderedMap<std::string, std::string> map;
  map["first"] = "some text";
  map["second"] = "some other text";

  UnorderedMap<std::string, std::string> moved_map{std::move(map)};

  assert(map.empty());
  assert(moved_map.size() == 2);
  assert(moved_map["first"] == "some text");
}

void test_copy_constructor() {
  UnorderedMap<std::string, int> map;
  map["something"] = 69;
  map["anything"] = 199;
  UnorderedMap<std::string, int> copied{map};

  assert(copied["something"] == map["something"]);
  assert(copied["anything"] == map["anuthing"]);
  assert(map.size() == copied.size());
}

int main() {
  test_operator_brackets_simple();
  test_operator_brackets_empty_string();
  test_operator_brackets_key_not_exist();
  test_operator_brackets_key_not_exist_const();

  test_contains_simple();
  test_contains_false();

  test_find();
  test_find_not_found();

  test_insert_false();
  test_insert_accessed();

  test_erase_simple();
  test_erase_returned_value();

  test_empty_simple();
  test_empty();

  test_size();
  test_size_empty_container();

  test_operator_equal_r_value();
  test_move_constructor();
  test_copy_constructor();
}