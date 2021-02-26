#pragma once

#include <map>
#include <string>
#include "Inst.h"

#define FIRST_STEP   1
#define SECOND_STEP  2

#define DATA true
#define TEXT false

using std::string;
using std::map;

const string _LeerInteiro = "LeerInteiro:\nenter 4, 0\nmov dword [ EBP - 4 ], 0\nmov EAX, 3\nmov EBX, 0\nmov ECX, _digit_\nmov EDX, 5\nint 80h\nmov dword [EBP - 4], EAX\nmovzx EBX, byte [_digit_]\nsub ESI, ESI\nsub EAX, EAX\nsub ECX, ECX\ncmp EBX, dword 2dh\njne _loopLeerInteiro_\ninc ECX\n_loopLeerInteiro_:\ninc ESI\nmovzx EBX, byte [_digit_ + ECX]\ninc ECX\nsub EBX, 30h\ncmp EBX, 0\njl _checkNumber_\ncmp EBX, 9\njg _checkNumber_\nmul dword [_DEZ_]\nadd EAX, EBX\ncmp ESI, 3\njne _loopLeerInteiro_\n\n_checkNumber_:\ncmp ECX, ESI\nje _endLeerInteiro_\nneg dword EAX\n\n\n\n_endLeerInteiro_:\nmov EBX, dword [EBP + 8]\nmov dword [EBX], EAX\n\nmov EAX, dword [EBP - 4]\ncmp EAX, 5\njne _leaveLeerInteiro_\ndec EAX\n\n_leaveLeerInteiro_:\n\nleave\nret 4\n";
const string _EscreverInteiro = "EscreverInteiro:\nenter 4, 0\nmov [ EBP - 4 ], dword 0\nmov EAX, dword[ EBP + 8 ] \npush dword EAX\ncmp dword EAX, dword 0\njge _printDigits_\n\npush dword EAX\n\nmov dword[_digit_], 2dh\nmov dword EAX, 4\nmov dword EBX, 1\nmov dword ECX, _digit_\nmov dword EDX, 1\nint 80h\ninc dword [EBP - 4]\npop dword EAX\nneg EAX\n\n_printDigits_:\nmov ESI, -1\n\n_loop1_:\ncdq\nidiv dword[_DEZ_]\nadd dword EDX, 0x30\ninc ESI\ncmp dword EAX, 0\nje _continuePrint_\npush dword EDX\njmp _loop1_\n_continuePrint_:\ndec ESI\ninc dword[ EBP - 4 ]\nmov dword [ _digit_ ], dword EDX\nmov EAX, 4\nmov EBX, 1\nmov ECX, _digit_\nmov EDX,  1\nint 80h\npop dword EDX\ncmp ESI, 0\njge _continuePrint_\n\nadd dword EAX, dword[ EBP - 4 ]\nleave\nret 4\n";
const string _LeerChar = "LeerChar:\nenter 0, 0\nmov EAX, 3\nmov EBX, 0\nmov ECX, dword[ EBP + 8 ]\nmov EDX, 2\nint 80h\nmov EAX, 2\nleave\nret 4\n";
const string _EscreverChar = "EscreverChar:\nenter 0, 0\nmov EAX, 4\nmov EBX, 1\nmov ECX, dword[ EBP + 8 ]\nmov EDX, 1\nint 80h\nmov EAX, 1\nleave\nret 4\n";
const string _LeerString = "LeerString:\nenter 0, 0\nmov EAX, 3\nmov EBX, 0\nmov ECX, dword[ EBP + 12 ]\nmov EDX, dword[ EBP + 8 ]\nint 80h\nleave\nret 8\n";
const string _EscreverString = "EscreverString:\nenter 0, 0\nmov EAX, 4\nmov EBX, 1\nmov ECX, dword[ EBP + 12 ]\nmov EDX, dword[ EBP + 8 ]\nint 80h\nleave\nret 8\n";

const string _overflow = "jmp _stop_\n_overflow:\nmov EAX, 4\nmov EBX, 1\nmov ECX, _msgOverflow_\nmov EDX, _sizeMsgOverflow_\nint 80h\njmp _stop_\n";
const string _stop = "_stop_:\nmov EAX, 1\nmov EBX, 1\nint 80h\n";
const string _dataBss = "section .data\n_msgOverflow_ db \"Ocorreu Overflow!\",10\n_sizeMsgOverflow_ EQU $-_msgOverflow_\n_msgReading1_ db \"Foram lidos \"\n_sizeMsgReading1_ EQU $-_msgReading1_\n_msgReading2_ db \" caracteres.\",10\n_sizeMsgReading2_ EQU $-_msgReading2_\n_DEZ_ dd 10\nsection .bss \n _digit_ resd 2\n";

bool translator(string);
