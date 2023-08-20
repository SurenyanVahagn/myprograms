#include <cstddef>
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
