#include <iostream>
#include <list>

template <class T>
class Queue {
private:
  std::list<T> list;
public:
  // Создает пустую очередь
  Queue()=default;

  Queue(std::initializer_list<T> q) : list(q) {}

  // Создает новую очередь, являющююся глубокой копией очереди other [O(n)]
  Queue(const Queue& other)=default;

  // Перезаписывает текущую очередь очередью other
  Queue& operator=(const Queue& other)=default;

  // Создает новую очередь перемещая существующую
  Queue(Queue&& other)=default;

  // Перезаписывает текущую очередь очередью other
  Queue& operator=(Queue&& other)=default;


  // Очищает память очереди
  ~Queue()=default;

  // Возвращает размер очереди (сколько памяти уже занято)
  std::size_t size() const{
    return list.size();
  };

  // Проверяет является ли контейнер пустым
  bool empty() const{
    return list.empty();
  };

  // Получает ссылку на первый элемент очереди
  T& front(){
    return list.front();
  };

  // Получает ссылку на последний элемент очереди
  T& back(){
    return list.back();
  };

  // Добавляет элемент в конец очереди.
  void push(const T& x){
    list.push_back(x);
  };

  // Удаляет элемент из начала очереди. Возвращает удаленный элемент.
  T pop(){
    T tmp = list.front();
    list.pop_front();
    return tmp;
  };

  // Меняет содержимое с другой очередью. q1.swap(q2);
  void swap(Queue& other){
    list.swap(other.list);
  };

  void dump_from(Queue<T>& q){
    size_t size= q.size();
    for(int i=0; i<size; ++i)
      {
        push(q.pop());
      }
  };

friend std::ostream& operator<<(std::ostream &out,const Queue<T> &list_s);
};
template <class T>
std::ostream& operator<<(std::ostream &out, Queue<T> &list_s){
  std::size_t size_=list_s.size();
  for(int i=0; i<size_; ++i)
    {
      T x = list_s.back();
      out << list_s.pop() << " ";
      list_s.push(x);
    }
  return out;
}


int main() {
  Queue<int> q0{1,2,3,4};
  std::cout << q0.back() << std::endl;
  std::cout << q0.size() << std::endl;
  Queue<int> q1{5, 6};
  q0.dump_from(q1); // q0 == {1, 2, 3, 4, 5, 6}; q1 == {}
  std::cout << q0.back() << std::endl;
  std::cout << q1.size() << std::endl;

  std::cout << q0 << std::endl;
  std::cout << q0.size() << std::endl;
  
  Queue<int> list;
  
  std::cout << list.empty() << std::endl;
  list.push(9);
  std::cout << list.empty() << std::endl;
  list.push(8);
  list.push(11);
  list.push(23);
  std::cout << list.size() << std::endl;
  std::cout << list.front() << std::endl;
  std::cout << list.back() << std::endl;
  std::cout << list.pop() << std::endl;

  Queue<int> list2;

  list.swap(list2);

  std::cout << list.empty() << std::endl;
  std::cout << list2.empty() << std::endl;
  

  
  return 0;
}