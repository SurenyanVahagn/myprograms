#include <cstddef>
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
  bool erase(const Key &key) const;

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

    // разыменование (как с указателями): *it = 42; или std::cout << *it;
    std::pair<const Key, Value> &operator*();
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
    const std::pair<const Key, Value> &operator*();
  };
};
