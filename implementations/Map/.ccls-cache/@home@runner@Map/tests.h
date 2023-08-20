#include <cassert>
#include <stdexcept>
#include <string>

#include "map.h"

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
  bool exception_thrown{};

  // проверка, что исключение будет выброшено
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

  std::pair<const std::string, std::string> expected{"b", "second key"};
  assert(*map.lower_bound("b") == expected);
}

void test_lower_bound_equal() {
  Map<std::string, std::string> map;
  map["b"] = "second key";
  map["a"] = "first key";
  map["f"] = "third key";
  auto it = map.end();

  std::pair<const std::string, std::string> expected{"f", "third key"};
  assert(*map.lower_bound("c") == expected);
}