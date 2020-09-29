#include <stdlib.h>
#include <criterion/criterion.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tape
// 0th value stored at 0th index        0 => 0
//+nth value stored at 2*nth index      1 => 2
//-nth value stored at -2*n+1 index    -1 => 1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Tape
{
    bool *vals;
    int length;
};

int posToIndex(int pos)
{
    if(pos == 0) return pos;
    else if(pos < 0) return -2 * pos - 1;
    else if(pos > 0) return 2 * pos;
}

int indexToPos(int index)
{
    if(index == 0) return index;
    else if(index % 2 == 0) return index / 2;
    else if(index % 2 != 0) return -(index + 1) / 2;
}

struct Tape *newTape()
{
    struct Tape *tape = malloc(sizeof(struct Tape));
    tape->vals = calloc(0, sizeof(bool));
    tape->length = 0;
    return tape;
}

void freeTape(struct Tape *tape)
{
    free(tape->vals);
    free(tape);
}

void set(struct Tape *tape, int pos, bool value)
{
    if(pos > tape->length || pos < -tape->length)
    {
        tape->vals = reallocarray(tape->vals, 2 * abs(pos) + 1, sizeof(bool));
        for(int i = 2 * tape->length + 1; i < 2 * abs(pos) + 1; i++) tape->vals[i] = false;
        tape->length = abs(pos);
    }
    tape->vals[posToIndex(pos)] = value;
}

void printTape(struct Tape *tape);

bool get(struct Tape *tape, int pos)
{
    // printTape(tape);
    // printf("From %d get\n", pos);
    if(pos > tape->length || pos < -tape->length)
    {
        tape->vals = reallocarray(tape->vals, 2 * abs(pos) + 1, sizeof(bool));
        for(int i = 2 * tape->length + 1; i < 2 * abs(pos) + 1; i++) tape->vals[i] = false;
        tape->length = abs(pos);
    }
    // printTape(tape);
    return tape->vals[posToIndex(pos)];
}

void printTape(struct Tape *tape)
{
    printf("Tape Length: %d\n", tape->length);
    for(int i = - tape->length; i <= tape->length; i++)
    {
        if(i == 0) printf(" %d ", tape->vals[posToIndex(i)]);
        else printf("%d", tape->vals[posToIndex(i)]);
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Tape Finished
////////////////////////////////////////////////////////////////////////////////////////////////////

bool getBitFromString(char *s, int position)
{
    if(position > 8 * strlen(s)) return false;
    char c = s[position / 8];
    int p = position % 8;
    return c >> p & 1;
}

void setBitToString(char *s, int position, bool value)
{
    int index = position / 8;
    char c = s[index];
    int p = position % 8;
    c += value << p;
    s[index] = c;
    // printf("%s\n", s);
}

char *boolfuck (char *code, char *in) 
{

    struct Tape *tape = newTape();
    int pointer = 0;

    char *out = calloc(1024, sizeof(char));
    int inputBitPosition = 0;
    int outputBitPosition = 0;
    int loop = 0;
    int i = 0;
    

    while(i < strlen(code))
    {
        char c = code[i];
        if(loop)
        {
            if(c == '[') loop++;
            if(c == ']') loop--;
        }
        else
        {
            if(c == '[' && get(tape, pointer) == 0) loop++;
            if(c == ']' && get(tape, pointer) == 1) loop--;
            if(c == '+') set(tape, pointer, !get(tape, pointer));
            if(c == ',') set(tape, pointer, getBitFromString(in, inputBitPosition)), inputBitPosition++;
            if(c == ';') setBitToString(out, outputBitPosition, get(tape, pointer)), outputBitPosition++;
            if(c == '>') pointer++;
            if(c == '<') pointer--;
        }
        if(!loop) i++; 
        else i += loop/abs(loop);
    }
    return out;
}

char* boolfuck (char *code, char *input);

Test (boolfuck, should_pass_all_the_tests_provided) {
    cr_assert (!strcmp (boolfuck (";;;+;+;;+;+;\
+;+;+;+;;+;;+;\
;;+;;+;+;;+;\
;;+;;+;+;;+;\
+;;;;+;+;;+;\
;;+;;+;+;+;;\
;;;;;+;+;;\
+;;;+;+;;;+;\
+;;;;+;+;;+;\
;+;+;;+;;;+;\
;;+;;+;+;;+;\
;;+;+;;+;;+;\
+;+;;;;+;+;;\
;+;+;+;", ""), "Hello, world!\n"), "Your interpreter did not work with the code example provided on the official website");
    cr_assert (!strcmp (boolfuck (">,>,>,>,>,>,>,>,<<<<<<<[>]+<[+<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]<<<<<<<<;>;>;>;>;>;>;>;<<<<<<<,>,>,>,>,>,>,>,<<<<<<<[>]+<[+<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]", "Codewars\xff"), "Codewars"), "Echo until byte(255) encountered");
    cr_assert (!strcmp (boolfuck (">,>,>,>,>,>,>,>,>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>;>;>;>;>;>;>;>;>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]>,>,>,>,>,>,>,>,>+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]", "Codewars"), "Codewars"), "Echo until byte(0) encountered");
    cr_assert (!strcmp (boolfuck (">,>,>,>,>,>,>,>,>>,>,>,>,>,>,>,>,<<<<<<<<+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]>[>]+<[+<]>>>>>>>>>[+]>[>]+<[+<]>>>>>>>>>[+]<<<<<<<<<<<<<<<<<<+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]>>>>>>>>>>>>>>>>>>>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]<<<<<<<<<<<<<<<<<<<<<<<<<<[>]+<[+<]>>>>>>>>>[+]>>>>>>>>>>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]<<<<<<<<<<<<<<<<<<+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]>>>>>>>>>>>>>>>>>>>;>;>;>;>;>;>;>;<<<<<<<<", "\x08\x09"), "\x48"), "Two numbers multiplier");
}
