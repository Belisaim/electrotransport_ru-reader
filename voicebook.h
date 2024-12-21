// voicebook.h

#define NICKS_N 128
#define L_NICK  64
#define L_VOICE 32

const unsigned Voices_n = 12;
const char *Voice[Voices_n] = {
  "anna", "artemiy",
  "arina", "yuriy -r 130",
  "elena", "mikhail",
  "irina -r 125", "pavel -r 115",
  "tatiana", "vitaliy",
  "victoria -r 125", "evgeniy-rus -r 130"
};

//-------------------------------------------
class VoiceBook {
  char nick[NICKS_N][L_NICK];
  const char *voice[NICKS_N];
public:
  unsigned nicks_n;
  VoiceBook () { nicks_n = 0; }
  const char *voice_of (const char *_nick);
};

///////////////////////////////////////////////////////
const char *VoiceBook::voice_of (const char *_nick) {
  for (int i = 0; i < nicks_n; i++)
    if (strcmp (nick[i], _nick) == 0)
      return voice[i];
  assert (L_NICK > strlen (_nick));
  strcpy (nick[nicks_n], _nick);
  voice[nicks_n] = Voice[nicks_n % Voices_n];
  assert (NICKS_N > nicks_n);
  return voice[nicks_n++];
}

