#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>


class KeyboardTraining{
private:
  int right_keys = 0; // Количество нажатий правилных клавиш
  int n_right_keys = 0; // Количество правильных клавиш одного предложения
  int wrong_keys = 0; // Неправильные нажатия
  int seconds = 1; // Время (считается в милисекундах 1 мс = 0.001 сек)
  int index = 0; // показвает на каком символе находится курсор
  bool flag = false; // если значение равно true значит предлажение полностью напечатано пользователем
  bool prtxt = false; // этот флаг показывает напичатал ли текст компьютер
  std::string word; // здесь хранится сгенерированое слово
  std::string sentence; // здесь предложение

  void print_text(){
    text_right_keys();
    text_wrong_keys();
    text_symb_min();
  }

  void text_right_keys(){
    move(LINES/2+5, COLS/2 - 20);
    addstr("RIGHT KEYS: ");
    refresh();
  }

  void text_wrong_keys(){
    move(LINES/2+6, COLS/2 - 20);
    addstr("WRONG KEYS: ");
    refresh();
  }

  void text_symb_min(){
    move(LINES/2+7, COLS/2 - 20);
    addstr("SYMB PER MIN: ");
    refresh();
  }

  // Функция генерирует "слово" - строку со случайными символами
  void generate_word(int length) {
    const char possible_chars[] = "abcdefghijklmnopqrstuvwxyz";
    srand(time(nullptr));
    for(int i = 0; i < length; ++i){
      word += possible_chars[rand()%26];
    }
  }

  void print_word(){
    for(int i = 0; i < word.size(); ++i){
      move(LINES/2, COLS/2 - (word.size()/2) + i);
      addch(word[i]);
      refresh();
    }
  }

  void generate_sentence(int count_of_words) {
    sentence.clear();
    const char possible_chars[] = "abcdefghijklmnopqrstuvwxyz";
    srand(time(nullptr));
    std::vector<std::string> words(count_of_words);
    for(int i = 0; i < count_of_words; ++i){
      int size_of_word = (rand() % 8) + 2; 
      for(int j = 0; j < size_of_word; ++j){
        words[i] += possible_chars[rand()%26];
      }
    }
    for(int i = 0; i < count_of_words; ++i){
      for(int j = 0; j < words[i].size(); ++j){
        sentence += words[i][j];
      }
      if(i != count_of_words-1){
        sentence += ' ';
      }
    }
  }

  void print_sentence(){
    for(int i = 0; i < sentence.size(); ++i){
      move(LINES/2, 5 + i);
      addch(sentence[i]);
      refresh();
    }
  }

  void clear_screen(){
    for(int i = 1; i < LINES-1; ++i){
      for(int j = 1; j < COLS-1; ++j){
        move(i,j);
        addch(' ');
        refresh();
      }
    }
  }
  // Эта функция должна печатать в терминале новое слово (и другую информацию, если нужно).    
  // typed_number - количество букв слова, которые уже нажаты и должны быть отображены 
  // звездочками.
  void update_screen(int typed_number) {
    if(index == sentence.size()){
      if(!want_new_word(typed_number)){
        return;
      }
    }
    if(!prtxt){
      print_sentence();
      prtxt = true;
    }
    if(sentence[index] == typed_number){
      if(sentence[index] != ' '){
        right_key();
        move(LINES/2, (index + 5));
        addch('*');
        refresh();
      }
      sentence[index] = '*';
      ++index;
    }
    else if(typed_number == ERR){
      ++seconds;
    }
    else if(sentence[index] != typed_number){
      wrong_key();
    }
  }

  bool want_new_word(int typed_number){
    if(flag == false){
      clear_screen();
      show_score_screen();
      flag = true;
    }
      
    if(typed_number == 'k'){
      clear_screen();
      flag = false;
      n_right_keys = 0;
      seconds = 1;
      generate_sentence((rand()%7) + 2);
      index = 0;
      prtxt = false;
      print_text();
      return true;
    }
    return false;
  }

  void right_key(){
    ++right_keys;
    ++n_right_keys;
    for(int i = 0; i < 5; ++i){
      move(LINES/2 + 5, COLS/2 - 5 + i);
      addch(' ');
      refresh();
    }
    std::string key = std::to_string(right_keys);
    for(int i = 0; i < key.size(); ++i){
      move(LINES/2 + 5, COLS/2 - 5 + i);
      addch(key[i]);
      refresh();
    }
  }

  void wrong_key(){
    ++wrong_keys;
    for(int i = 0; i < 5; ++i){
      move(LINES/2 + 6, COLS/2 - 5 + i);
      addch(' ');
      refresh();
    }
    std::string key = std::to_string(wrong_keys);
    for(int i = 0; i < key.size(); ++i){
      move(LINES/2 + 6, COLS/2 - 5 + i);
      addch(key[i]);
      refresh();
    }
  }

  void show_score_screen(){
    std::string str = "SYMBOLS PER MINUTE: ";
    for(int i = 0; i < str.size(); ++i){
      move(LINES/2, COLS/2 - 10 + i);
      addch(str[i]);
      refresh();
    }
    for(int i = 0; i < 5; ++i){
      move(LINES/2, COLS/2 + 15 + i);
      addch(' ');
      refresh();
    }
    int time = (n_right_keys*60 / (seconds/1000));
    std::string timen = std::to_string(time);
    for(int i = 0; i < timen.size(); ++i){
      move(LINES/2, COLS/2 + 15 + i);
      addch(timen[i]);
      refresh();
    }

    std::string eshutyun = "PRESS K FOR NEW SENTENCE";
    for(int i = 0; i < eshutyun.size(); ++i){
      move(LINES/2+1, COLS/2 - (eshutyun.size()/2) + i);
      addch(eshutyun[i]);
      refresh();
    }
  }

  
public:

  KeyboardTraining(){
    initscr(); // создает "экран"
    clear();   // отчищает его
    noecho(); // не выводит нажатые клавиши в консоль
    cbreak(); // не ждет enter от пользователя, а сразу считывает клавишу
    curs_set(0);          // прячет курсор
    timeout(1);
    keypad(stdscr, TRUE); // позволяет считывать доп клавиши типа стрелок
    border(0, 0, 0, 0, 0, 0, 0, 0); // Рисует границы
  }

  void start(){
    // std::string string_to_type = generate_word(10);
    // std::string string_to_type = generate_sentence(7);
    generate_sentence(7);
    print_text();
    while (true) {
      int key = getch();
      // TODO: write the code
      update_screen(key);
    }
  }

  ~KeyboardTraining(){
    endwin();
  }
};

int main(int argc, char **argv) {
  KeyboardTraining kboard;
  kboard.start();
  return 0;
}