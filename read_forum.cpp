// read_forum.cpp

#include "read_forum.h"

int main ()
{
  Forum f;

  f.find_str_with_date();
  f.txt.clear();

NextText:
  f.find_str_with_date();
  f.find_up_empty_str();
  f.find_up_empty_str();
  f.up();
  f.get_nick_and_voice();
  f.txt.clear();
  f.find_str_with_first ("    Понравилось");
  f.up();
  f.up();
  f.say();
  f.txt.clear();
  goto NextText;
}

