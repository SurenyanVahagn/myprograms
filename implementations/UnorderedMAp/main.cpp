#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <initializer_list>

template <class Key, class Value> class UnorderedMap {
public:
  class Iterator;
  class ConstIterator;
private:
  using ValueType = std::pair<Key, Value>;
  std::vector<std::list<ValueType>> data;
  std::size_t size_ = 0;

  void CollisionsCheck(){
    if(size_ >= 4*data.size()){
      reallocation();
    }
  }

 void reallocation(){
    int old_vec_size = data.size();
    data.resize(data.size()*2);
    for(int i = 0; i < old_vec_size; ++i){
      for(auto it = data[i].begin(); it != data[i].end();){
        int index = std::hash<Key>{}((*it).first);
        if((index % data.size()) >= old_vec_size){
          data[old_vec_size + i].push_back(*it);
          it = data[i].erase(it);
        }
        else{
          ++it;
        }
      }
    }
  }

  int Hash(const Key& key){
    return (std::hash<Key>{}(key))/data.size();
  }

  Iterator FindByHash(const Key &key, const int &hash){
    for(auto it = data[hash].begin(); it!=data[hash].end(); ++it){
      if(key == (*it).first){
        return Iterator(it, data, hash);    
      }
    }
  }

public:
  // Создает пустой словарь
  UnorderedMap(int capacity = 5) : data(capacity) {}

  // Создает новый UnorderedMap, являющийся глубокой копией other [O(n)]
  // UnorderedMap<std::string, int>  map;
  // map["something"] = 69;
  // map["anything"] = 199;
  // UnorderedMap<std::string, int> copied{map};
  // copied["something"] == map["something"] == 69
  UnorderedMap(const UnorderedMap &other) {
    data = other.data;
    size_ = other.size_;
  };

  // initializer_list
  UnorderedMap(std::initializer_list<std::pair<Key, Value>> other){
    UnorderedMap second;
    for(const auto& it : other){
      second[it.first] = it.second;
    }
    std::swap(data, second.data);
    std::swap(size_, second.size_);
  }

  // Конструктор перемещения
  UnorderedMap(UnorderedMap &&other) {
    std::swap(data, other.data);
    std::swap(size_, other.size_);
  };

  // Перезаписывает текущий словарь словарем other
  UnorderedMap &operator=(const UnorderedMap &other) {
    UnorderedMap tmp{other};
    std::swap(data, tmp.data);
    std::swap(size_, tmp.size_);

    return *this;
  };

  // Присваивание перемещением
  UnorderedMap &operator=(UnorderedMap &&other) {
    UnorderedMap tmp{std::move(other)};
    std::swap(data, tmp.data);
    std::swap(size_, tmp.size_);

    return *this;
  };

  ~UnorderedMap() = default;

  // Возвращает итератор на первый элемент
  Iterator begin() {
    for(int i = 0; i < data.size(); ++i){
      if(data[i].empty() != true){
        return Iterator(data[i].begin(), data, i);
      }
    }
    return Iterator(data);
  };

  // Возвращает константный итератор на первый элемент
  ConstIterator begin() const {
    return ConstIterator(begin());
  };

  // Возвращает итератор обозначающий конец контейнера (за последнимэлементом)
  Iterator end() {
    for(int i = data.size()-1; i>0; --i){
      if(data[i].empty() != true)
      return Iterator(data[i].end(), data, i);
    }
    return Iterator(data);
  };

  // // Возвращает константный итератор, обозначающий конец контейнера
  ConstIterator end() const {
    return ConstIterator(end());
  };

  // Возвращает размер UnorderedMap (сколько элементов добавлено)
  std::size_t size() const { return size_; };

  // Проверяет является ли UnorderedMap пустым
  bool empty() const { return size_ == 0; };

  // Возвращает элемент по ключу. Если отсутсвует, выбрасывает исключение
  const Value &operator[](Key key) const {
    int index = std::hash<Key>{}(key);
    for (const auto &it : data[index % data.size()]) {
      if (it.first == key) {
        return it.second;
      }
    }
    throw std::out_of_range("There is no key like yours!!!");
  };

  // Возвращает ссылку на элемент по Key (позволяет менять элемент). Если
  // элемент с таким ключом отсутствует, создает его и инициализирует дефолтным
  // значением map["something"] = 75;
  Value &operator[](Key key) {
    int index = std::hash<Key>{}(key);
    for (auto &it : data[index % data.size()]) {
      if (it.first == key) {
        return it.second;
      }
    }

    data[index % data.size()].push_back({key, Value{}});
    size_++;
    CollisionsCheck();
    return data[index % data.size()].back().second;
  };

  // Проверяет есть ли в контейнере элемент с таким Key
  bool contains(const Key &key) const {
    int index = std::hash<Key>{}(key);
    for (const auto &it : data[index % data.size()]) {
      if (it.first == key) {
        return true;
      }
    }
    return false;
  };

  // Возвращяет Итератор на элемент который ищем, если нет такого элемента
  // возвращает итератор на последный элемент
  Iterator find(const Key &key) {
    int index = std::hash<Key>{}(key);
    for(auto it = data[index % data.size()].begin(); it!=data[index % data.size()].end(); ++it){
      if(key == (*it).first){
        return Iterator(it, data, index % data.size());    
      }
    }
    return end();
  };

  // Добавляет новый элемент с ключем и значением, если нет уже существуюшего
  // элемента с таким же ключом Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.(5,"something");
  // возвращает false, потому что элмемент с key = 5 уже существует․
  bool insert(const Key &k, const Value &v) {
    if (contains(k)) {
      return false;
    }
    int index = std::hash<Key>{}(k);
    data[index % data.size()].push_back({k, v});
    size_++;
    CollisionsCheck();
    return true;
  };

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
  bool erase(const Key &key) {
    int index = std::hash<Key>{}(key);
    auto &collisions = data[index % data.size()];
    for (auto it = collisions.begin(); it != collisions.end(); ++it) {
      if (key == it->first) {
        collisions.erase(it);
        size_--;
        return true;
      }
    }
    return false;
  }

  class Iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    using ListIt = typename std::list<ValueType>::iterator;
    using Vector = std::vector<std::list<ValueType>>;

  private:
    Vector &data;  // ссылка на вектор
    ListIt listit; // итератор на лист
    int index = 0; // индекс вектора на котором находится listit в конкретный момент
    int max_index; // наибольший индекс не пустого листа, в комментах max
    int min_index; // наименьший индекс не пустого листа, в комментах min

  public:
    // конструктор на случай пустого вектора
    Iterator(Vector& vec) : data(vec), listit(data[data.size()-1].end()), index(data.size()-1), max_index(index), min_index(index) {}
    // обычный конструктор для begin() end() find
    Iterator(ListIt it, Vector& vec, int index_) : data(vec), listit(it), index(index_) {
      // через этот цикл нахожу в векторе первый непустой лист и прсиваиваю min его значение
      for(int i = 0; i < data.size(); ++i){
        if(data[i].empty() != true){
          min_index = i;
          break;
        }
      }
      // здесь уже поиск происходит с конца
      for(int i = data.size()-1; i>0; --i){
        if(data[i].empty() != true){
          max_index = i;
          break;
        }
      }
    } // P.S. Можно было сделать 4 конструктора эти 2 и еще +2. Один из тех двух сразу бы получал min а другой max

    Iterator &operator++() {
      ++listit;
      if(index == max_index) {}
      else{
        if(listit == data[index].end()){
          do{ // могу позволить сделать ++ сразу т.к. заню что index != max
            ++index;
          } while(data[index].empty() && index <= max_index); // пропускаем все index-ы в которых begin==end но при условии что index<max
          listit = data[index].begin();
        }
      }
      return *this;
    }

    Iterator &operator--() {
      if(index == min_index){
        if(listit != data[index].begin()){
          --listit; // идентично с ++
        }
      }
      else{
        if(listit == data[index].begin()){ // если к примеру после find сразу делаем --
          do{
            --index; // так же могу сразу -- делать т.к. index != min
          } while(data[index].empty() && index >= min_index); // аналогично с ++, только index>min
          listit = data[index].end(); 
          --listit; // чтобы указывал на элемент до end()
        }
        else{
          --listit; // если указывает к примеру где-то в середине листа то просто --
        }
      }
      return *this;
    };

    bool operator!=(const Iterator &other) { return listit != other.listit; };
    bool operator==(const Iterator &other) { return listit == other.listit; };

    // разыменование (как с указателями): *it = 42; или std::cout << *it;
    std::pair<Key, Value> &operator*() { return *listit; };

    std::pair<Key, Value> *operator->() { return &(*listit); };
  };
  class ConstIterator {
    friend UnorderedMap;
    // Заполнить
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

  private:
    Iterator it;
    

  public:
    ConstIterator(Iterator iter) : it(iter) {}

    ConstIterator &operator++() {
      ++it;
      return *this;
    }

    ConstIterator &operator--() {
      --it;
      return *this;
    };

    bool operator!=(const ConstIterator &other) { return it != other.it; };
    bool operator==(const ConstIterator &other) { return it == other.it; };

    // разыменование (как с указателями): *it = 42; или std::cout << *it;
    std::pair<Key, Value> &operator*() { return *it; };
  };
};

template <class K, class V>
UnorderedMap<K,V> merge(const UnorderedMap<K,V>& u1, const UnorderedMap<K,V>& u2){
  UnorderedMap<K,V> u3;
  for(auto it1 = u1.begin(), it2 = u2.begin();;){
    if(it1 != u1.end() && it2 != u2.end()){
      if(it1 != u1.end()){
        u3[(*it1).first] = (*it1).second;
        ++it1;
      }
      if(it2 != u2.end()){
        if(u3.contains((*it2).first) == false){
          u3[(*it2).first] = (*it2).second;
        }
        ++it2;
      }
    }
    else{
      break;
    }
  }
  return u3;
}

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

  assert(map.erase("first") == true);
  assert(map.contains("first") == false);
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
  assert(copied["anything"] == map["anything"]);
  assert(map.size() == copied.size());
}

void test_reallocation() {
  UnorderedMap<int, int> map;
  for (int i = 0; i < 80; ++i) {
    map[i] = i;
  }
  std::cout << map.size() << std::endl;

  auto *data = (std::vector<std::list<std::pair<int, int>>> *)&map;
  assert(data->size() == 40);
  assert((*data)[0].size() == 2); // for type `int` hash has perfect distribution
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

  test_reallocation();

  UnorderedMap<int, int> map;
  for(int i = 0; i < 40; ++i){
    map[i] = i;
  }

  auto it1 = map.begin();
  it1->second = 100;

  for(const auto& it : map){
    std::cout << it.first << " " << it.second << std::endl;
  }
}