// read_forum.cpp

#include "read_forum.h"

int main ()
{
  Forum f;

  f.find_str_with_date();
  f.txt.clear();
  f.find_str_with_date();
  f.find_up_empty_str();
  f.find_up_empty_str();
  f.up();
  f.get_nick ();

  do {
    f.get_voice();
    f.txt.clear();
    f.find_str_with_date();
    
    if (f.the_end) {
      f.up();
      f.up();
      f.say();
    }
    else {
      f.find_up_empty_str();
      f.find_up_empty_str();
      f.up();
      f.get_nick ();
      f.up();
      f.say();
    }
  } while (!f.the_end);

  fprintf (stderr, "\033[1;33mКонец.\033[0m\n");
  return 0;
}

