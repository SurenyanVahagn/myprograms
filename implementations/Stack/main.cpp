#include <iostream>
#include <vector>
#include <algorithm>


template <class T>
class Stack {
private:
std::vector<T> stack;

public:
  // Создает пустой стэк
  Stack()=default;
  

  // Создает новый стэк, являющийся глубокой копией списка other [O(n)]
  Stack(const Stack& other)=default;

  // Перезаписывает текущий стэк стэком other
  Stack& operator=(const Stack& other)=default;

  // Очищает память стэка
  ~Stack()=default;

  // Возвращает размер стэка (сколько памяти уже занято)
  std::size_t size() const{
    return stack.size();
  };

  // Проверяет является ли контейнер пустым
  bool empty() const{
    return stack.empty();
  };

  // Добавляет элемент на верх стэка.
  void push(const T& x)
  {
    stack.push_back(x);
  };

  // Получает элемент на верху стэка
  T& top(){
    return stack.back();
  };

  Stack<T> reverse(){
    Stack<T> tmp = *this;
    std::reverse(tmp.stack.begin(), tmp.stack.end());
    return tmp;
  };

  // Удаляет последний элемент стэка. Возвращает удаленный элемент.
  T pop(){
    T tmp = stack[stack.size()-1];
    stack.pop_back();
    return tmp;
  };

  // Меняет содержимое с другим стэком. s1.swap(s2);
  void swap(Stack& other){
    stack.swap(other.stack);
  };
};

int main() {
  Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.push(4);
  stack.push(5);

  std::cout << stack.empty() << std::endl;

  std::cout << stack.size() << std::endl;

  std::cout << stack.top() << std::endl;

  std::cout << stack.pop() << std::endl;

  std::cout << stack.size() << std::endl;

  Stack<int> stack2;
  stack2.push(25);
  stack2.push(12);
  stack2.push(33);

  stack.swap(stack2);

  std::cout << stack.size() << std::endl;

  std::cout << stack.top() << std::endl;

  Stack<int> tmp = stack.reverse();
  std::cout << tmp.top() << std::endl;

  Stack<int> stack3;
  std::cout << stack3.empty() << std::endl;
  stack3.push(25);
  std::cout << stack3.empty() << std::endl;
  
  
  return 0;
}