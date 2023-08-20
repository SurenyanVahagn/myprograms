#include <iostream>  
#include <initializer_list>

template <class T>
class List {
private:
  struct node
  {
    struct node *prev;
    struct node *next;
    T value;

    node(node* prev_=nullptr,node* next_ = nullptr,T value_ = 0): prev(prev_),next(next_),value(value_) {}

  };

  node *head = nullptr;
  node *tail = nullptr;

  std::size_t size_=0;

public:
  class Iterator;
  // Создает список размера count заполненный дефолтными значениями типа T
  List(std::size_t count = 0){
    for(int i = count; i!=0; --i)
      {
        push_back(T{});
      }
  };

  List(std::initializer_list<T> other){
		for (auto &value : other)
		{
			push_back(value);
		}
  };
  
  // Создает новый список, являющийся глубокой копией списка other [O(n)]
  List(const List& other){
    node *tmp = other.head;
    while(tmp!=0){
      push_back(tmp->data);
      tmp=tmp->next;
    }
    size_=other.size_;
  };
  
  // Перезаписывает текущий список списком other
  // List l1{5}, l2{10};
  // l1 = l2;
  // std::cout << l1.size() == 10 << std::endl // True
  List& operator=(const List& other){
    if(this->head != other.head && other.head!=0){
      // head=nullptr;
      // tail=nullptr;
      std::size_t list_size=size_-1;
      for(int i=0; i<list_size; ++i){
        pop_back();
      }
      delete head;
      size_=0;
      node *tmp= other.head;
      while(tmp!=0){
        push_back(tmp->value);
        tmp=tmp->next;
      }
      size_=other.size_;
    }
    return *this;
  };
  
  // Очищает память списка [O(n)]
  ~List(){
    while(head!=tail)
      {
        node *tmp=head->next;
        delete head;
        head=tmp;
      }
    delete head;
  };
  
  // Возвращает размер списка (сколько памяти уже занято)
  std::size_t size(){
    return size_;
  };
  
  // Проверяет является ли контейнер пустым
  bool empty(){
    return size_==0;
  };
  
  // Возвращает итератор на первый элемент
  // https://en.cppreference.com/w/cpp/container/list/begin
  Iterator begin(){
    return Iterator(head);
  };
  
  // Возвращает итератор обозначающий конец контейнера
  Iterator end(){
    return Iterator(tail->next);
  };
  
  // Возвращает копию элемента по индексу
  T operator[](std::size_t index) const{
    if(size_ ==0 || index >= size_ ){
      throw std::runtime_error("Error");
    }
    node *tmp = head;
    for(int i = 0;i < size_-1;++i){
      if(i != index){
        tmp = tmp-> next;
      }
      else{
        break;
      }
    }
    return tmp -> value;
  };
  
  // Возвращает ссылку на элемент по индексу (позволяет менять элемент, типа
  // v[5] = 42;)
  T& operator[](std::size_t index){
    if(size_ ==0 || index >= size_ ){
      throw std::runtime_error("Error");
    }
    node* tmp = head;
    for(int i = 0;i < size_-1;++i){
      if(i != index){
        tmp = tmp -> next;
      }
      else{
        break;
      }
    }
    return tmp -> value;
    };
  
  // Добавляет элемент в конец списока.
  void push_back(const T& x){
    node *tmp = new node(tail,nullptr,x);
    if(tail==nullptr){
      head=tmp;
    }
    else{
      tail->next=tmp;
    }
    tail=tmp;
    size_++;
  };
  
  // Добавляет элемент в начало списока.
  void push_front(const T& x){
    node* tmp = new node(nullptr,head,x);
    if(head==nullptr){
      tail=tmp;
    }
    else{
      head->prev=tmp;
    }
    head=tmp;
    size_++;
  }
  
  // Удаляет последний элемент списока.
  T pop_back(){
    if (size_ == 0){
      throw std::runtime_error("Error");
    }
    T val;
    node *tmp;
    tmp=tail;
    tail = tail->prev;
    tail -> next = nullptr;
    val = tmp->value;
    delete tmp;
    size_--;
    return val;
  };
  
  // Удаляет первый элемент списока.
  T pop_front(){
    if(size_==0)
    {
      throw std::runtime_error("Error");
    }
    T val;
    node *tmp;
    tmp=head;
    head = head->next;
    head->prev = nullptr;
    val = tmp->value;
    delete tmp;
    size_--;
    return val;
  };
  
  
  // Вставляет новый элемент value перед элементом, на который указывает it.
  //     it
  //     v
  // [1, 2, 3].insert(it, 42) -> [1, 42, 2, 3]
  void insert(Iterator it, T value){
    if( it.current  == head){
      push_front(value);
    }
    else if(it.current == tail->next){
      push_back(value);
    }
    else{
      node *tmp = new node(it.current->prev,it.current,value);
      it.current->prev=tmp;
      --it;
      --it;
      it.current->next=tmp;
      size_++;
    }
  };
  
  //     it
  //     v
  // [1, 2, 3].erase(it) -> [1, 3] (return 2)
  T erase(Iterator it){
    if (it.current == nullptr) {
      throw std::runtime_error("Error");
    } else if (it.current == head) {
      return pop_front();
    } else if (it.current == tail) {
      return pop_back();
    }
    T val;
    node *tmp = it.current;
    --it;
    it.current->next = tmp->next;
    ++it;
    it.current->prev = tmp->prev;
    val = tmp->value;
    delete tmp;
    size_--;
    return val;
  };

  void erase(Iterator start, Iterator end){
    // start.current->prev->next=end.current->next;
    // end.current->next->prev=start.current->prev;
    while(start!=end){
      List<T>:: Iterator it = start;
      ++start;
      erase(it);
    }
  }

  // Класс, который позволяет итерироваться по контейнеру.
  // Я указал минимальный набор операций
  class Iterator {
  friend class List;
  private:
    node *current;
    // Инкремент. Движение к следующему элементу. ++it
  public: 
    Iterator(node *tmp): current(tmp){}

    Iterator& operator++(){
      Iterator it(current);
      current=current->next;
      return *this;
    };
    
    // Декремент. Движение к предыдущему элементу. --it
    Iterator& operator--(){
      Iterator it(current);
      current=current->prev;
      return *this;
    };
    
    // Проверка на неравенство двух итераторов. Обычно используется it !=
    // container.end().
    bool operator!=(const Iterator& other){
      return current != other.current;
    };
    
    // разыменование (как с указателями): *it = 42; или std::cout << *it;
    T& operator*(){
      return current->value;
    };
  };
  friend std::ostream& operator<< (std::ostream &out, const List<T> &instance);
};

template <class T>
std::ostream& operator<< (std::ostream &out, List<T> &instance){
  std::size_t list_size = instance.size();
  for(int i=0; i<list_size; ++i){
    out << instance[i] << " ";
  }
  return out;
};

int main() {
  List<int> list;
  std::cout << list.empty() << std::endl;
  list.push_front(10);
  list.push_back(24);
  list.push_back(30);
  list.push_back(55);
  list.push_back(77);
  list.push_back(14);
  list.push_back(65);
  std::cout << list.empty() << std::endl;
  std::cout << list.size() << std::endl;
  std::cout << list[0]<< std::endl;
  list.pop_front();
  list.pop_back();
  std::cout << list[0]<< std::endl;
  std::cout << list.size() << std::endl;
  List<int>:: Iterator it = list.begin();
  ++it;
  ++it;
  ++it;
  list.insert(it,5);
  std::cout << list.size() << std::endl;
  std::cout << list.erase(it) << std::endl;
  std::cout << list.size() << std::endl;

  List<int> list_1;
  List<int> list_2;
  list_1.push_front(10);
  list_1.push_back(11);
  list_1.push_back(12);
  list_2.push_front(13);
  list_2.push_back(14);
  list_2.push_back(15);
  list_1=list_2;
  std::cout << list_1 << std::endl;
  std::cout << list_1[0]<< std::endl;
  std::cout << list[3]<< std::endl;
  
  std::cout << list << std::endl;
  std::cout << list_1 << std::endl;

  List<int> listf;
  listf.push_front(10);
  listf.push_back(24);
  listf.push_back(30);
  listf.push_back(55);
  listf.push_back(77);
  listf.push_back(14);
  listf.push_back(65);
  listf.push_back(34);
  listf.push_back(27);
  listf.push_back(114);
  std::cout << listf.size() << std::endl;
  List<int>:: Iterator it1 = listf.begin();
  //std::advance(it1, 1);
  List<int>:: Iterator it2 = listf.begin();
  //std::advance(it2, 3);
  ++it1;
  ++it2;
  ++it2;
  ++it2;
  ++it2;
  listf.erase(it1,it2);
  std::cout << listf.size() << std::endl;


  
  List<int> l0{1,2,3,4,5};
  auto start_it = l0.begin();
  ++start_it;
  // std::advance(start_it, 1); // на 2ку
  auto end_it = l0.begin();
  ++end_it;
  ++end_it;
  ++end_it;
  // std::advance(end_it, 3); // на 4ку

  l0.erase(start_it, end_it);
  std::cout << l0 << std::endl; // 1 4 5 
  return 0;
}