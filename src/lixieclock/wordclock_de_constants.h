//https://github.com/c3e/wordclock/blob/master/wordclock/wordclock.ino
#define M_ES 0
#define M_IST 1
#define M_FUENF 2
#define M_ZEHN 3
#define M_ZWANZIG 4
#define M_VIERTEL 5
#define M_VOR 6
#define M_NACH 8
#define M_HALB 7
#define M_UHR 21
#define H_EINS 11
#define H_ZWEI 12
#define H_DREI 13
#define H_VIER 14
#define H_FUENF 10
#define H_SECHS 15
#define H_SIEBEN 17
#define H_ACHT 16
#define H_NEUN 9
#define H_ZEHN 19
#define H_ELF 20
#define H_ZWOELF 18

const int clockWords[22][10] = {
  {0,-1,-1,-1,-1,-1,-1,-1,-1,-1}, // es 0
  {1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  // ist 1
  {2,3,-1,-1,-1,-1,-1,-1,-1,-1},  // fuenf 2
  {7,8,-1,-1,-1,-1,-1,-1,-1,-1},  // zehn 3
  {4,5,6,-1,-1,-1,-1,-1,-1,-1}, //  zwanzig 4
  {9,10,11,-1,-1,-1,-1,-1,-1,-1},  // viertel 5
  {12,-1,-1,-1,-1,-1,-1,-1,-1,-1},  // vor 6
  {13,14,-1,-1,-1,-1,-1,-1,-1,-1},  // nach 8
  {15,16,-1,-1,-1,-1,-1,-1,-1,-1},  // halb 7
  {17,18,-1,-1,-1,-1,-1,-1,-1,-1},  // neun 9 
  {19,20,-1,-1,-1,-1,-1,-1,-1,-1},  // fuenf 10
  {23,24,-1,-1,-1,-1,-1,-1,-1,-1},  // eins 11
  {21,22,-1,-1,-1,-1,-1,-1,-1,-1},  // zwei 12
  {25,26,-1,-1,-1,-1,-1,-1,-1,-1},  // drei 13
  {27,28,-1,-1,-1,-1,-1,-1,-1,-1},  // vier 14
  {31,32,-1,-1,-1,-1,-1,-1,-1,-1},  // sechs 15
  {29,30,-1,-1,-1,-1,-1,-1,-1,-1},  // acht 16
  {33,34,35,-1,-1,-1,-1,-1,-1,-1},  // sieben 17
  {36,37,38,-1,-1,-1,-1,-1,-1,-1}, // zwölf 18
  {41,42,-1,-1,-1,-1,-1,-1,-1,-1},  // zehn 19
  {40,-1,-1,-1,-1,-1,-1,-1,-1,-1},  // elf 20
  {39,-1,-1,-1,-1,-1,-1,-1,-1},  // uhr 21
};


int getHourWord(const int _h, const int _m)
{
  switch(_h)
  {
    case 12:
    case 0:
      return H_ZWOELF;
      
      break;
    case 1:
    case 13:
      // special case to get sigular @ "ein uhr"
      if (_m < 5)
      {
        return H_EINS;
      } else
      {
        return H_EINS;
      }
      break;
    case 2:
    case 14:
      return H_ZWEI;
      break;
    case 3:
    case 15:
      return H_DREI;
      break;
    case 4:
    case 16:
      return H_VIER;
      break;
    case 5:
    case 17:
      return H_FUENF;
      break;
    case 6:
    case 18:
      return H_SECHS;
      break;
    case 7:
    case 19:
      return H_SIEBEN;
      break;
    case 8:
    case 20:
      return H_ACHT;
      break;
    case 9:
    case 21:
      return H_NEUN;
      break;
    case 10:
    case 22:
      return H_ZEHN;
      break;
    case 11:
    case 23:
      return H_ELF;
      break;
  }
}

int getMinuteWord(const int _m)
{
  /*
  get word for current "minute"
  */
  switch(_m)
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return M_UHR;
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      return M_FUENF;
     // set_word(M_NACH);
      break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      return M_ZEHN;
     // set_word(M_NACH);
      break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
      return M_VIERTEL;
      //set_word(M_NACH);
      break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
      return M_ZWANZIG;
     // set_word(M_NACH);
      break;
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
      return M_FUENF;
      //set_word(M_VOR);
      break;
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
      return M_HALB;
      break;
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
      return M_FUENF;
   //   set_word(M_NACH);
    //  set_word(M_HALB);
      break;
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
      return M_ZWANZIG;
    //  set_word(M_VOR);
      break;
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
      return M_VIERTEL;
    //  set_word(M_VOR);
      break;
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
      return M_ZEHN;
    //  set_word(M_VOR);
      break;
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
      return M_FUENF;
     // set_word(M_VOR);
      break;
  }
}