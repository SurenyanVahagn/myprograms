#include <cassert>
#include <iostream>

template <class T> class SharedPtr {
public:
  // Создает пустой SharedPtr
  SharedPtr();

  // Создает новый объект для конкретного указателя
  SharedPtr(T *ptr);

  // Создает новый SharedPtr, который делит владение с other
  SharedPtr(const SharedPtr &other);

  // Перезаписывает текущий умный указатель с other, при этом делит владение
  SharedPtr &operator=(const SharedPtr &other);

  // Перезаписывает в текущий указатель указателем other(r-value)
  SharedPtr(SharedPtr &&other);

  // Присваивает текущему указателю указатель other
  SharedPtr &operator=(SharedPtr &&other);

  // Очищает память умного указателя
  ~SharedPtr();

  // Возвращает сырой указатель
  T *get() const;

  //Результат разыменования указателя
  T &operator*() const;

  // Чтобы можно было писать ptr->field
  T *operator->() const;

  // Возвращает количество SharedPtr, с которыми делит память сохранённый
  // указатель (включая самого себя)
  long use_count() const;

  // Проверяет, не равен ли сохраненный указатель нулю: if (ptr) или if(!ptr)
  operator bool() const;
};

template <class T> class WeakPtr {

public:
  // Создает пустой WeakPtr
  WeakPtr();

  // Создает новый WeakPtr, который делит владение с other
  WeakPtr(const WeakPtr &other);

  // Перезаписывает текущий WeakPtr с other
  WeakPtr &operator=(const WeakPtr &other);

  // Перезаписывает текущий указатель указателем other(r-value)
  WeakPtr(WeakPtr &&other);

  // Присваивает текущему указателю указатель  other
  WeakPtr &operator=(WeakPtr &&other);

  // Создает WeakPtr на SharedPtr (если other пуст, то текущий указатель тоже
  // должен быть пустым)
  WeakPtr(const SharedPtr<T> &other);

  // Перезаписывает SharedPtr в WeakPtr (если other пуст, то текущий указатель
  // тоже должен быть пустым)
  WeakPtr &operator=(const SharedPtr<T> &other);

  // Уничтожает WeakPtr.
  ~WeakPtr();

  // Показывет количество SharedPtr, указывающих на этот объект.
  long use_count() const;

  // Показывает, был ли удален управляемый объект .
  bool expired() const;

  // Создает новый SharedPtr, который разделяет права собственности на
  // управляемый объект. Если управляемого объекта нет, то возвращаемый
  // SharedPtr также пуст.
  // Важно: доступ к ресурсу осуществляется только через lock
  SharedPtr<T> lock() const;
};

void test_Constructors_SharedPtr() {

  SharedPtr<int> s1;
  assert(!s1);

  SharedPtr<int> s2{new int{10}};
  assert(s2.use_count() == 1);

  SharedPtr<int> s3{s2}; // copy

  assert(s2.use_count() == s3.use_count());
  assert(*s2.get() == *s3.get());

  SharedPtr<int> s4{SharedPtr<int>{new int{10}}}; // move

  assert(*s4.get() == 10);
}

void test_operator_assignment_l_value_SharedPtr() {

  SharedPtr<int> s1{new int{10}};
  SharedPtr<int> s2{new int{20}};
  s1 = s2;

  assert(*s1 == 20 && *s2 == 20);
}

void test_operator_assignment_r_value_SharedPtr() {

  SharedPtr<int> s1{new int{10}};
  SharedPtr<int> s2{s1};

  s1 = SharedPtr<int>{new int{2}};
  assert(*s1 == 2 && s1.use_count() == 1);

  s1 = std::move(s2);
  assert(*s1 == 10 && s1.use_count() == 1);
  assert(s2.use_count() == 0);
}

void test_use_count_SharedPtr() {

  SharedPtr<int> foo(new int{5});
  SharedPtr<int> bar(foo);
  assert(foo.use_count() == 2);
  assert(*foo.get() == 5);
}

void test_use_count_empty_SharedPtr() {
  SharedPtr<int> ptr{};
  assert(ptr.use_count() == 0);
}

void test_dereference_SharedPtr() {

  SharedPtr<int> foo(new int{5});
  assert(foo.use_count() == 1);
  assert(*foo == 5);

  struct basic_struct {
    int a;
  };
  SharedPtr<basic_struct> bar(new basic_struct{5});
  assert(bar->a == 5);
}



void test_operator_bool() {

  SharedPtr<int> p1(new int{100});
  SharedPtr<int> p2;

  assert(p1 == true);
  assert(p2 == false);
}

void test_Constructors_WeakPtr() {
  WeakPtr<int> w0{
    SharedPtr<int>(new int{100}) // SharedPtr тут временный объект, который сразу после этого удаляется.
  };

  assert(w0.expired());

  SharedPtr<int> p1{new int{42}};
  WeakPtr<int> w1{p1};
  
  assert(w1.use_count() == 1); // тут SharedPtr еще существует

  WeakPtr<int> w2{w1}; // copy
  WeakPtr<int> w3{std::move(w1)}; // move

  assert(w1.use_count() == 0); // moved
  assert(w2.use_count() == 1);
  assert(w3.use_count() == 1);
}

void test_operator_assignment_l_value_WeakPtr() {

  SharedPtr<int> s1{new int{10}};
  WeakPtr<int> w1{s1};
  WeakPtr<int> w2;
  w2 = w1;

  assert(w1.use_count() == 1);
  assert(w2.use_count() == 1);
}

void test_operator_assignment_r_value_Weakptr() {

  SharedPtr<int> s1{new int{10}};
  WeakPtr<int> w1{s1};
  WeakPtr<int> w2;
  w2 = std::move(w1);

  assert(w2.use_count() == 1);
  assert(w1.expired());

}

void test_use_count_WeakPtr() {

  SharedPtr<int> foo(new int{5});
  SharedPtr<int> bar(foo);

  WeakPtr<int> w1{foo};
  WeakPtr<int> w2{bar};

  assert(w1.use_count() == 2);
  assert(w2.use_count() == 2);
}

void test_lock() {
  WeakPtr<int> w1;
  {
    SharedPtr<int> s1{new int{42}};
    w1 = s1;

    assert(*w1.lock() == 42);

    SharedPtr<int> s2 = w1.lock();
    assert(*s2 == 42);
    assert(w1.use_count() == 2);
    assert(s1.use_count() == 2);
    assert(s2.use_count() == 2);
  } // s1 и s2 вышли из области видимости и отчистились. w1 еще существует
  assert(w1.expired());

  SharedPtr<int> s3 = w1.lock();  // должен создать пустой SharedPtr, потому что expired
  assert(!s3);
}


void test_lock_when_empty(){
  WeakPtr<int> empty;
  assert(!empty.lock());
}

int main() {

  test_Constructors_SharedPtr();
  test_operator_assignment_l_value_SharedPtr();
  test_operator_assignment_r_value_SharedPtr();
  test_use_count_SharedPtr();
  test_use_count_empty_SharedPtr();

  test_dereference_SharedPtr();
  test_operator_bool();

  test_Constructors_WeakPtr();
  test_operator_assignment_l_value_WeakPtr();
  test_operator_assignment_r_value_Weakptr();
  test_use_count_WeakPtr();

  test_lock();
  test_lock_when_empty();
}