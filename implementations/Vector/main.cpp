#include <iostream>

template <class T> class Vector {
private:
  size_t size_;
  size_t capacity_;
  T* vector;

public:
  // Создает вектор размера arr_size заполненный дефолтными значениями типа T
  Vector(std::size_t arr_size = 0):size_(arr_size),capacity_(arr_size*2+1),vector(new T[capacity_]{}){};

  Vector(int* arr, size_t arr_size):Vector(arr_size)
  {
    for(int i=0; i<size_; ++i){
      vector[i]=arr[i];
    }
  };

  // Создает новый вектор, являющийся глубокой копией вектора other
  Vector(const Vector &other):size_(other.size),capacity_(other.capacity),vector(new T[other.capacity_]){
    for (size_t i = 0; i < other.size_; ++i) 
    {
      vector[i] = other.vector[i];
    }
    //Vector<T> vector = other;
  };

  // Перезаписывает текущий вектор вектором other
  // Vector v1{5}, v2{10};
  // v1 = v2;
  // std::cout << v1.size() == 10 << std::endl // True
  Vector &operator=(const Vector &other)
  {
    delete[] vector;
    vector = new T[other.capacity_];
      
    for (size_t i = 0; i < other.size_; ++i) 
    {
      vector[i] = other.vector[i];
    }
      
    size_ = other.size_;
    capacity_ = other.capacity_;
    return *this;
  };

  // Очищает память вектора
  ~Vector(){
    delete[] vector;
  };

  // Возвращает размер вектора (сколько памяти уже занято)
  std::size_t size(){
    return size_;
  };

  // Проверяет является ли контейнер пустым
  bool empty(){
    return size_ == 0;
  };

  // Возвращает размер выделенной памяти
  std::size_t capacity(){
    return capacity_;
  };

  // Возвращает копию элемента по индексу
  T operator[](std::size_t index) const{
    return vector[index];
  };

  // Возвращает ссылку на элемент по индексу (позволяет менять элемент, типа
  // v[5] = 42;)
  T &operator[](std::size_t index){
    return vector[index];
  };

  // Перевыделяет память:
  // * выделяет новый кусок памяти размером равным new_capacity,
  // * копирует столько старых элементов, сколько влезает,
  // * удаляет старый кусок памяти.
  void resize(std::size_t new_capacity){
    if(new_capacity<capacity_)
    {
      size_ = new_capacity;
    }
    capacity_ = new_capacity;
    T* tmp = vector;
    vector = new T[capacity_];
    for (size_t i = 0; i < size_; ++i)
    {
      vector[i] = tmp[i];
    } 
    delete[] tmp;
  };

  // Добавляет элемент в конец вектора. Если нужно перевыделяет память
  void push_back(const T &x){
    if (size_ >= capacity_)
    {
      resize(capacity_*2);
    }
    vector[size_++] = x;
  };

  // Удаляет последний элемент вектора и возвращает его.
  T pop_back(){
    T tmp = vector[size_-1];
    size_--;
    return tmp;
  };

  // Очищает вектор (выделенная память остает выделенной)
  void clear(){
    size_=0;
  };

  // Вставляет новый элемент value на место pos.
  // [1, 2, 3].insert(1, 42) -> [1, 42, 2, 3]
  void insert(size_t pos, T value){
    push_back(value);
    for(size_t i=size_-1; i>pos; --i)
      {
        std::swap(vector[i],vector[i-1]);
      }
  };

  // Удаляет элемент с идексом pos. Возвращает удаленный элемент.
  // [1, 2, 3].erase(1) -> [1, 3] (return 2)
  T erase(size_t pos)
  {
    T tmp = vector[pos];
    for (size_t i = pos + 1; i < size_; ++i) {
    		vector[i - 1] = vector[i];
    }
    --size_;
    return tmp;
  };
};
template <class T>
std::ostream& operator<<(std::ostream &out, Vector<T> &instance){
  for(int i=0; i<instance.size(); ++i){
    out << instance[i] << " ";
  }
  return out;
};

int main() 
{
  Vector<int> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);
  vector.push_back(4);
  vector.push_back(5);

  std::cout<<vector.size()<<std::endl;

  std::cout<<vector.pop_back()<<std::endl;
  
  std::cout<<vector.pop_back()<<std::endl;
  std::cout<<vector.size()<<std::endl;

  std::cout<<vector.erase(1)<<std::endl;

  vector.push_back(38);
  vector.push_back(132);
  vector.push_back(43);
  vector.push_back(551);

  vector.insert(1,100);

  std::cout<<vector.erase(1)<<std::endl;

  vector.resize(15);
  std::cout<<vector.size()<<std::endl;

  std::cout<<vector.empty()<<std::endl;

  

  
  int* arr = new int[5] {0, 1, 2, 3, 4};
  Vector<int> v{arr, 5};
  std::cout << v << std::endl;
  return 0;
}