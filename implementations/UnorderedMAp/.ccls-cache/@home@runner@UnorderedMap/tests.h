#include <cassert>
#include <stdexcept>
#include <string>

#include "unordered_map.h"

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