/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

const uint8_t TextFontBitmaps[] PROGMEM = {
  0x00, 0xFA, 0x00, 0xB6, 0x80, 0x2F, 0xD5, 0xFA, 0x00, 0x0B, 0xC4, 0x7A, 
  0x00, 0x6D, 0x74, 0x45, 0xD6, 0xC0, 0x4A, 0xA4, 0xBA, 0x50, 0xC0, 0x2A, 
  0x49, 0x11, 0x00, 0x88, 0x92, 0x54, 0x00, 0xAB, 0xAA, 0x00, 0x21, 0x3E, 
  0x42, 0x00, 0x60, 0xE0, 0x80, 0x04, 0xA5, 0x20, 0x69, 0x99, 0x99, 0x60, 
  0x75, 0x54, 0x00, 0x69, 0x12, 0x48, 0xF0, 0x69, 0x12, 0x19, 0x60, 0x11, 
  0x95, 0x2F, 0x88, 0x40, 0xF8, 0xE9, 0x19, 0x60, 0x69, 0x8E, 0x99, 0x60, 
  0xF1, 0x12, 0x44, 0x40, 0x69, 0x96, 0x99, 0x60, 0x69, 0x97, 0x19, 0x60, 
  0x88, 0x00, 0x40, 0x60, 0x2A, 0x22, 0x00, 0xE3, 0x80, 0x88, 0xA8, 0x00, 
  0x69, 0x12, 0x40, 0x40, 0x31, 0x2B, 0x6B, 0xB9, 0x03, 0x80, 0x69, 0x9F, 
  0x99, 0x90, 0xE9, 0x9E, 0x99, 0xE0, 0x69, 0x88, 0x89, 0x60, 0xF4, 0x63, 
  0x18, 0xC7, 0xC0, 0xF8, 0x8E, 0x88, 0xF0, 0xF8, 0x8E, 0x88, 0x80, 0x69, 
  0x8B, 0x99, 0x60, 0x99, 0x9F, 0x99, 0x90, 0xFE, 0x00, 0x11, 0x11, 0x99, 
  0x60, 0x8C, 0xA9, 0x8A, 0x4A, 0x20, 0x88, 0x88, 0x88, 0xF0, 0x83, 0x8E, 
  0xAC, 0x98, 0x30, 0x60, 0x80, 0x8E, 0x6B, 0x38, 0xC6, 0x20, 0x74, 0x63, 
  0x18, 0xC5, 0xC0, 0xE9, 0x9E, 0x88, 0x80, 0x74, 0x63, 0x1A, 0xC9, 0xA0, 
  0xE9, 0x9E, 0x99, 0x90, 0x69, 0x86, 0x19, 0x60, 0xF9, 0x08, 0x42, 0x10, 
  0x80, 0x99, 0x99, 0x99, 0x60, 0x8C, 0x62, 0xA5, 0x10, 0x80, 0x83, 0x06, 
  0x4C, 0x9A, 0xB5, 0x51, 0x00, 0x8C, 0x54, 0x45, 0x46, 0x20, 0x8C, 0x62, 
  0xA2, 0x10, 0x80, 0xF8, 0x44, 0x44, 0x43, 0xE0, 0xEA, 0xAB, 0x12, 0x24, 
  0x48, 0x00, 0xD5, 0x57, 0x54, 0x00, 0xF0, 0x90, 0x61, 0x79, 0x70, 0x88, 
  0xE9, 0x99, 0xE0, 0x69, 0x89, 0x60, 0x11, 0x79, 0x99, 0x70, 0x69, 0xF8, 
  0x70, 0x2B, 0xA4, 0x90, 0x00, 0x79, 0x97, 0x1E, 0x88, 0xE9, 0x99, 0x90, 
  0xBE, 0x00, 0x45, 0x56, 0x88, 0x9A, 0xCA, 0x90, 0xAA, 0xA4, 0x00, 0xD5, 
  0x6B, 0x18, 0x80, 0xE9, 0x99, 0x90, 0x69, 0x99, 0x60, 0xE9, 0x9E, 0x88, 
  0x79, 0x97, 0x11, 0xBA, 0x48, 0x00, 0x78, 0x61, 0xE0, 0x4B, 0xA4, 0x88, 
  0x00, 0x99, 0x99, 0x60, 0x8C, 0x62, 0xA2, 0x00, 0x8C, 0x6B, 0x55, 0x00, 
  0x8A, 0x88, 0xA8, 0x80, 0x99, 0x97, 0x1E, 0xE5, 0x4E, 0x00, 0x29, 0x44, 
  0x91, 0xFE, 0x00, 0x89, 0x14, 0x94
};

const GFXglyph TextFontGlyphs[] PROGMEM = {
  {     0,   1,   1,   2,    0,    0 }   // ' '
 ,{     1,   1,   7,   2,    0,   -7 }   // '!'
 ,{     3,   3,   3,   4,    0,   -7 }   // '"'
 ,{     5,   5,   5,   6,    0,   -6 }   // '#'
 ,{     9,   3,   9,   4,    0,   -7 }   // '$'
 ,{    13,   5,   8,   6,    0,   -7 }   // '%'
 ,{    18,   4,   7,   5,    0,   -7 }   // '&'
 ,{    22,   1,   3,   2,    0,   -7 }   // '''
 ,{    23,   3,   9,   4,    0,   -7 }   // '('
 ,{    27,   3,   9,   4,    0,   -7 }   // ')'
 ,{    31,   5,   3,   6,    0,   -6 }   // '*'
 ,{    34,   5,   5,   6,    0,   -5 }   // '+'
 ,{    38,   2,   2,   2,    0,   -1 }   // ','
 ,{    39,   3,   1,   4,    0,   -3 }   // '-'
 ,{    40,   1,   1,   2,    0,   -1 }   // '.'
 ,{    41,   3,   8,   4,    0,   -7 }   // '/'
 ,{    44,   4,   7,   5,    0,   -7 }   // '0'
 ,{    48,   2,   7,   3,    0,   -7 }   // '1'
 ,{    51,   4,   7,   5,    0,   -7 }   // '2'
 ,{    55,   4,   7,   5,    0,   -7 }   // '3'
 ,{    59,   5,   7,   6,    0,   -7 }   // '4'
 ,{    64,   4,   7,   5,    0,   -7 }   // '5'
 ,{    68,   4,   7,   5,    0,   -7 }   // '6'
 ,{    72,   4,   7,   5,    0,   -7 }   // '7'
 ,{    76,   4,   7,   5,    0,   -7 }   // '8'
 ,{    80,   4,   7,   5,    0,   -7 }   // '9'
 ,{    84,   1,   5,   2,    0,   -5 }   // ':'
 ,{    86,   2,   6,   3,    0,   -5 }   // ';'
 ,{    88,   3,   5,   4,    0,   -5 }   // '<'
 ,{    91,   3,   3,   4,    0,   -4 }   // '='
 ,{    93,   3,   5,   4,    0,   -5 }   // '>'
 ,{    96,   4,   7,   5,    0,   -7 }   // '?'
 ,{   100,   6,   7,   7,    0,   -7 }   // '@'
 ,{   106,   4,   7,   5,    0,   -7 }   // 'A'
 ,{   110,   4,   7,   5,    0,   -7 }   // 'B'
 ,{   114,   4,   7,   5,    0,   -7 }   // 'C'
 ,{   118,   5,   7,   6,    0,   -7 }   // 'D'
 ,{   123,   4,   7,   5,    0,   -7 }   // 'E'
 ,{   127,   4,   7,   5,    0,   -7 }   // 'F'
 ,{   131,   4,   7,   5,    0,   -7 }   // 'G'
 ,{   135,   4,   7,   5,    0,   -7 }   // 'H'
 ,{   139,   1,   7,   2,    0,   -7 }   // 'I'
 ,{   141,   4,   7,   5,    0,   -7 }   // 'J'
 ,{   145,   5,   7,   6,    0,   -7 }   // 'K'
 ,{   150,   4,   7,   5,    0,   -7 }   // 'L'
 ,{   154,   7,   7,   8,    0,   -7 }   // 'M'
 ,{   161,   5,   7,   6,    0,   -7 }   // 'N'
 ,{   166,   5,   7,   6,    0,   -7 }   // 'O'
 ,{   171,   4,   7,   5,    0,   -7 }   // 'P'
 ,{   175,   5,   7,   6,    0,   -7 }   // 'Q'
 ,{   180,   4,   7,   5,    0,   -7 }   // 'R'
 ,{   184,   4,   7,   5,    0,   -7 }   // 'S'
 ,{   188,   5,   7,   6,    0,   -7 }   // 'T'
 ,{   193,   4,   7,   5,    0,   -7 }   // 'U'
 ,{   197,   5,   7,   6,    0,   -7 }   // 'V'
 ,{   202,   7,   7,   8,    0,   -7 }   // 'W'
 ,{   209,   5,   7,   6,    0,   -7 }   // 'X'
 ,{   214,   5,   7,   6,    0,   -7 }   // 'Y'
 ,{   219,   5,   7,   6,    0,   -7 }   // 'Z'
 ,{   224,   2,   8,   3,    0,   -7 }   // '['
 ,{   226,   3,   7,   4,    0,   -7 }   // '\'
 ,{   230,   2,   8,   3,    0,   -7 }   // ']'
 ,{   232,   3,   2,   4,    0,   -7 }   // '^'
 ,{   234,   4,   1,   5,    0,    0 }   // '_'
 ,{   235,   2,   2,   3,    0,   -7 }   // '`'
 ,{   236,   4,   5,   5,    0,   -5 }   // 'a'
 ,{   239,   4,   7,   5,    0,   -7 }   // 'b'
 ,{   243,   4,   5,   5,    0,   -5 }   // 'c'
 ,{   246,   4,   7,   5,    0,   -7 }   // 'd'
 ,{   250,   4,   5,   5,    0,   -5 }   // 'e'
 ,{   253,   3,   7,   4,    0,   -7 }   // 'f'
 ,{   257,   4,   6,   5,    0,   -5 }   // 'g'
 ,{   260,   4,   7,   5,    0,   -7 }   // 'h'
 ,{   264,   1,   7,   2,    0,   -7 }   // 'i'
 ,{   266,   2,   8,   3,    0,   -7 }   // 'j'
 ,{   268,   4,   7,   5,    0,   -7 }   // 'k'
 ,{   272,   2,   7,   3,    0,   -7 }   // 'l'
 ,{   275,   5,   5,   6,    0,   -5 }   // 'm'
 ,{   279,   4,   5,   5,    0,   -5 }   // 'n'
 ,{   282,   4,   5,   5,    0,   -5 }   // 'o'
 ,{   285,   4,   6,   5,    0,   -5 }   // 'p'
 ,{   288,   4,   6,   5,    0,   -5 }   // 'q'
 ,{   291,   3,   5,   4,    0,   -5 }   // 'r'
 ,{   294,   4,   5,   5,    0,   -5 }   // 's'
 ,{   297,   3,   7,   4,    0,   -7 }   // 't'
 ,{   301,   4,   5,   5,    0,   -5 }   // 'u'
 ,{   304,   5,   5,   6,    0,   -5 }   // 'v'
 ,{   308,   5,   5,   6,    0,   -5 }   // 'w'
 ,{   312,   5,   5,   6,    0,   -5 }   // 'x'
 ,{   316,   4,   6,   5,    0,   -5 }   // 'y'
 ,{   319,   3,   5,   4,    0,   -5 }   // 'z'
 ,{   322,   3,   8,   4,    0,   -7 }   // '{'
 ,{   325,   1,   7,   2,    0,   -7 }   // '|'
 ,{   327,   3,   8,   4,    0,   -7 }   // '}'
};

const GFXfont TextFont PROGMEM = {
(uint8_t  *)TextFontBitmaps, (GFXglyph *)TextFontGlyphs, 0x20, 0x7e, 8};