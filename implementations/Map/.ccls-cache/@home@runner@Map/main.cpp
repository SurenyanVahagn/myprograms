#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

template <class Key, class Value> class Map {
public:
  class Iterator;
  class ConstIterator;
  // Создает пустой словарь
  Map();

  // Создает новый словарь, являющийся глубокой копией other [O(n)]
  //  Map<std::string, int> map;
  //  map["something"] = 69;
  //  map["anything"] = 199;
  //  Map<std::string, int> copied{map};
  //  copied["something"] == map["something"] == 69
  Map(const Map &other);

  // Move конструктор
  Map(Map &&other);

  // Перезаписывает текущий словарь словарем other
  Map &operator=(const Map &other);

  // Присвоивание перемещением
  Map &operator=(Map &&other);
  // Очищает память словаря
  ~Map();

  // Возвращает итератор на первый элемент
  Iterator begin();

  // Возвращает const итератор на первый элемент
  ConstIterator begin() const;

  // Возвращает итератор обозначающий конец контейнера
  Iterator end();

  // Возвращает const итератор обозначающий конец контейнера
  ConstIterator end() const;

  // Возвращает размер словаря (сколько есть узлов)
  std::size_t size() const;

  // Проверяет есть ли элемент с таким ключом в контейнере
  bool contains(const Key &key) const;

  // Возвращает элемент по ключу. Если в словаре нет элемента с таким ключом, то
  // бросает исключение std::out_of_range
  const Value &operator[](const Key &key) const;

  // Возвращает ссылку на элемент по ключу (позволяет менять элемент). Если в
  // словаре нет элемента с таким ключем, то создает его и устанавливает
  // дефолтное значение, после чего возвращает на него ссылку. map["something"]
  // = 75;
  Value &operator[](const Key &key);

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
  bool erase(const Key &key);

  // Меняет текуший контейнер с контейнером other
  void swap(Map &other);

  // Возвращает итератор на первый элемент который не меньше чем переданный
  // ключ. [O(h)]
  Iterator lower_bound(const Key &key);

  // Очищает контейнер [O(n)]
  // Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.clear;
  // c.size() == 0 //true;
  void clear();

  class Iterator {
  public:
    // Заполнить:
    // using iterator_category = ? ;
    // using value_type =        ? ;
    // using difference_type =   ? ;
    // using pointer =           ? ;
    // using reference =         ? ;

    Iterator();
    // Инкремент. Движение к следующему элементу.
    Iterator &operator++();

    // Декремент. Движение к предыдущему элементу.
    Iterator &operator--();

    // Проверка на неравенство двух итераторов.
    bool operator!=(const Iterator &other);

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
}
