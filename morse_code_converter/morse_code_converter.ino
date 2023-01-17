/*
Morse Code Converter
Author: sunlu

Convert text into morse code and send it out via LED blink or the flash of 1602 screen. 
The text can be either pre-programmed into the code, or read from a simple keyboard.
*/

// configuration
const int unit_time = 100;        // unit time = 100ms
const int blank_letter = 3;       // blank between letters = 3 unit
const int blank_word = 7;         // blank between words = 7 unit 

// morse code table
const int tabMorse[][8] = {
  {0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // a, .-
  {0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}, // b, -...
  {0x03, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00}, // c, -.-.
  {0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, // d, -..
  {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // e, .
  {0x01, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00}, // f, ..-.
  {0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, // g, --.
  {0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}, // h, ....
  {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // i, ..
  {0x01, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00}, // j, .---
  {0x03, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}, // k, -.-
  {0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}, // l, .-..
  {0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // m, --
  {0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // n, -.
  {0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}, // o, ---
  {0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00}, // p, .--.
  {0x03, 0x03, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00}, // q, --.-
  {0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, // r, .-.
  {0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, // s, ...
  {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // t, -
  {0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}, // u, ..-
  {0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00}, // v, ...-
  {0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}, // w, .--
  {0x03, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00}, // x, -..-
  {0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00}, // y, -.--
  {0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}, // z, --..
  {0x01, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00}, // 1, .----
  {0x01, 0x01, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00}, // 2, ..---
  {0x01, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00}, // 3, ...--
  {0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00}, // 4, ....-
  {0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}, // 5, .....
  {0x03, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}, // 6, -....
  {0x03, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}, // 7, --...
  {0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00}, // 8, ---..
  {0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00}, // 9, ----.
  {0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00}, // 0, -----
  {0x01, 0x01, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00}, // ?, ..--..
  {0x03, 0x01, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00}, // !, -.-.--
  {0x01, 0x03, 0x01, 0x03, 0x01, 0x03, 0x00, 0x00}, // ., .-.-.-
  {0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00}, // ,, --..--
  {0x03, 0x01, 0x03, 0x01, 0x03, 0x01, 0x00, 0x00}, // ;, -.-.-.
  {0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00}  // :, ---...
};

// message
char msg[] = "Hello World!";

// user-defined functions
int checkTabMorse(char s){
  int i;
  switch (s)
  {
    case 'A':
    case 'a':
      i = 0;
      break;
    case 'B':
    case 'b':
      i = 1;
    break;
    case 'C':
    case 'c':
      i = 2;
      break;
    case 'D':
    case 'd':
      i = 3;
      break;
    case 'E':
    case 'e':
      i = 4;
      break;    
    case 'F':
    case 'f':
      i = 5;
      break;
    case 'G':
    case 'g':
      i = 6;
      break;
    case 'H':
    case 'h':
      i = 7;
      break;
    case 'I':
    case 'i':
      i = 8;
      break;
    case 'J':
    case 'j':
      i = 9;
      break;
    case 'K':
    case 'k':
      i = 10;
      break;
    case 'L':
    case 'l':
      i = 11;
      break;
    case 'M':
    case 'm':
      i = 12;
      break; 
    case 'N':
    case 'n':
      i = 13;
      break; 
    case 'O':
    case 'o':
      i = 14;
      break; 
    case 'P':
    case 'p':
      i = 15;
      break; 
    case 'Q':
    case 'q':
      i = 16;
      break;
    case 'R':
    case 'r':
      i = 17;
      break;
    case 'S':
    case 's':
      i = 18;
      break;
    case 'T':
    case 't':
      i = 19;
      break;
    case 'U':
    case 'u':
      i = 20;
      break;
    case 'V':
    case 'v':
      i = 21;
      break;
    case 'W':
    case 'w':
      i = 22;
      break;
    case 'X':
    case 'x':
      i = 23;
      break;
    case 'Y':
    case 'y':
      i = 24;
      break;
    case 'Z':
    case 'z':
      i = 25;
      break;
    case '1':
      i = 26;
      break;
    case '2':
      i = 27;
      break;
    case '3':
      i = 28;
      break;
    case '4':
      i = 29;
      break;
    case '5':
      i = 30;
      break;
    case '6':
      i = 31;
      break;
    case '7':
      i = 32;
      break;
    case '8':
      i = 33;
      break;
    case '9':
      i = 34;
      break;
    case '0':
      i = 35;
      break;
    case '?':
      i = 36;
      break;
    case '!':
      i = 37;
      break;
    case '.':
      i = 38;
      break;
    case ',':
      i = 39;
      break;
    case ';':
      i = 40;
      break;
    case ':':
      i = 41;
      break;
    default:
      i = 36;
  }
  return i;
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int msgLength;
  int count;
  int *indexMorseTable;
  /*read message*/
  // msg = 

  /*process message*/
  msgLength = sizeof(msg);
  for (count = 0; count < msgLength; count++){
    indexMorseTable = tabMorse[checkTabMorse(msg[count])];
  }


}

