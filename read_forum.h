// read_forum.h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "voicebook.h"
#include "player.inc"
#include <string>
#include <vector>

using namespace std;

#define L 16000

class Forum {
  char s[L], buf[L + 256];
  char voice[L_VOICE];
  VoiceBook vb;
  FILE *pipe;
public:
  vector <string> txt;

  Forum () { pipe = NULL; }
  char *gets();
  void get_nick_and_voice ();
  void say ();
  void down() { txt.push_back (gets()); }
  void   up() { txt.pop_back(); }
  void find_str_with_first (const char *head);
  void find_str_with_date();
  void find_up_empty_str();
private:
  bool is_date_str();
  void Erase_User();
  void Erase_http();
  void Erase_Dobavleno();
  void Erase_Poslednee_redaktirovanie();
  void Erase_Citata ();
};

void Forum::get_nick_and_voice () {
  strcpy (s, txt.back().c_str());
  s[strlen (s) - 1] = '\0'; // delete last \n
  char *_nick = s;
  assert (L_VOICE > strlen (vb.voice_of (_nick))); strcpy (voice, vb.voice_of (_nick));
  printf ("\033[1;35m%s of %u (%s)\n\033[1;32m==================================-\033[0m\n", _nick, vb.nicks_n, voice);
  sprintf (buf, "LD_LIBRARY_PATH=/usr/local/lib /usr/local/bin/RHVoice-test -o - -p %s | %s > /dev/null 2>&1", voice, player);
  if (pipe) { pclose (pipe); pipe = NULL; }
  pipe = popen (buf, "w");
}

void Forum::say () {
  Erase_Citata ();
  int n = txt.size(), i;
  for (i = 0; i < n; i++) {
    strcpy (s, txt[i].c_str());
    Erase_User();
    Erase_http();
    Erase_Dobavleno();
    Erase_Poslednee_redaktirovanie();
    if (s[0] == '\n' || s[0] == '\0') continue;
    printf ("\033[1;33m--\033[0m %s", s);
    fprintf (pipe, "%s", s);
  }
}

char *Forum::gets() {
  *s = '\0'; fgets (s, L, stdin);
  if (strstr (s, "Перейти в") == s) { fprintf (stderr, "\033[1;33mКонец.\033[0m\n"); exit (0); }
  return s;
}

void Forum::find_str_with_first (const char *head) {
  do { txt.push_back (gets()); } while (strstr (s, head) != s);
}

void Forum::find_str_with_date() {
  do { down(); } while (!is_date_str());
}

bool Forum::is_date_str() {
  char *p;
  int dd, yy, hh, mm;
  if (!(p = strtok (s, " "))) return false;
  dd = atoi (p); if (!(dd >= 1 && dd <= 31)) return false;

  if (!(p = strtok (NULL, " "))) return false; // get month

  if (!(p = strtok (NULL, " "))) return false;
  yy = atoi (p); if (!(yy >= 1980 && yy <= 2100)) return false;

  if (!(p = strtok (NULL, " "))) return false; // в
  
  if (!(p = strtok (NULL, ":"))) return false;
  hh = atoi (p); if (!(hh >= 0 && hh <= 23)) return false;

  if (!(p = strtok (NULL, "\n"))) return false;
  mm = atoi (p); if (!(mm >= 0 && mm <= 59)) return false;

  return true;
}

void Forum::find_up_empty_str() {
  do { txt.pop_back(); } while (txt.back()[0] != '\n');
}

void Forum::Erase_User() {
  char *p;
  if (p = strstr (s, "[/user]")) {
    *p = '\0'; strcpy (buf, s); strcat (buf, p + 7); strcpy (s, buf);
  }
}

void Forum::Erase_http() {
  char *p1, *p2;
  if (p1 = strstr (s, "http")) {
    *p1 = '\0'; strcpy (buf, s); strcat (buf, "(ссылка)\n");
    if (p2 = strchr (p1 + 1, ' ')) {
      buf[strlen (buf) - 1] = '\0';  // delete last \n
      strcat (buf, p2);
    }
    strcpy (s, buf);
  }
}

void Forum::Erase_Dobavleno() {
  if (strstr (s, "Добавлено ") == s) *s = '\0';
}

void Forum::Erase_Poslednee_redaktirovanie() {
  if (strstr (s, "Последнее редактирование: ")) *s = '\0';
}

void Forum::Erase_Citata () {
  bool citata = false;
  int i, n = txt.size();
  for (i = 0; i < n; i++) {
    if (txt[i].find ("    Цит") == 0) citata = true;
    if (txt[i][0] != ' ' && txt[i][0] != '\n') citata = false;
    if (citata) txt[i][0] = '\0';
  }
}

