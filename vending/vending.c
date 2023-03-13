#include <stdio.h>
#include <stdlib.h>

// var 13 -- X = 19
#define X 19

// possible states
typedef enum State {
  S0 = 0x00,
  S1,
  S2,
  S3,
  S4,
  S5,
  S6,
  S7,
  S8,
  S9,
  S10,
  S11,
  S12,
  S13,
  S14,
  S15,
  S16,
  S17,
  S18,
} State;

typedef struct statechange {
  const int input;            // input alphabet
  const State next_state;     // next_state
  const unsigned char output; // output alphabet
} statechange;

/*
output alphabet - 5 x 1 bit flag
check .xlsx table for output alphabet interpretation
*/

typedef struct FSM {
  const State state;                           // current state
  statechange (*Action)(struct FSM fsm_entry); // function for current state
  const statechange possible_state_changes[4]; // possible next states
} FSM;

char str_glob[5];

statechange announce_state(FSM fsm_entry);

void interpret_bin_flags(char flags);

int main(int argc, char **argv) {
  fprintf(stdout, "Rules:\ninput when asked 1 for 1 rub, 2 for 2 rub, 5 for 5"
                  "rub, X for 10 rub\nInput Q to exit\n");

  const FSM fsm[] = {{S0,
                      &announce_state,
                      {{'1', S1, 0b00000},
                       {'2', S2, 0b00000},
                       {'5', S5, 0b00000},
                       {'X', S10, 0b00000}}},
                     {S1,
                      &announce_state,
                      {{'1', S2, 0b00000},
                       {'2', S3, 0b00000},
                       {'5', S6, 0b00000},
                       {'X', S11, 0b00000}}},
                     {S2,
                      &announce_state,
                      {{'1', S3, 0b00000},
                       {'2', S4, 0b00000},
                       {'5', S7, 0b00000},
                       {'X', S12, 0b00000}}},
                     {S3,
                      &announce_state,
                      {{'1', S4, 0b00000},
                       {'2', S5, 0b00000},
                       {'5', S8, 0b00000},
                       {'X', S13, 0b00000}}},
                     {S4,
                      &announce_state,
                      {{'1', S5, 0b00000},
                       {'2', S6, 0b00000},
                       {'5', S9, 0b00000},
                       {'X', S14, 0b00000}}},
                     {S5,
                      &announce_state,
                      {{'1', S6, 0b00000},
                       {'2', S7, 0b00000},
                       {'5', S10, 0b00000},
                       {'X', S15, 0b00000}}},
                     {S6,
                      &announce_state,
                      {{'1', S7, 0b00000},
                       {'2', S8, 0b00000},
                       {'5', S11, 0b00000},
                       {'X', S16, 0b00000}}},
                     {S7,
                      &announce_state,
                      {{'1', S8, 0b00000},
                       {'2', S9, 0b00000},
                       {'5', S12, 0b00000},
                       {'X', S17, 0b00000}}},
                     {S8,
                      &announce_state,
                      {{'1', S9, 0b00000},
                       {'2', S10, 0b00000},
                       {'5', S13, 0b00000},
                       {'X', S18, 0b00000}}},
                     {S9,
                      &announce_state,
                      {{'1', S10, 0b00000},
                       {'2', S11, 0b00000},
                       {'5', S14, 0b00000},
                       {'X', S0, 0b10000}}},
                     {S10,
                      &announce_state,
                      {{'1', S11, 0b00000},
                       {'2', S12, 0b00000},
                       {'5', S15, 0b00000},
                       {'X', S0, 0b11000}}},
                     {S11,
                      &announce_state,
                      {{'1', S12, 0b00000},
                       {'2', S13, 0b00000},
                       {'5', S16, 0b00000},
                       {'X', S0, 0b10100}}},
                     {S12,
                      &announce_state,
                      {{'1', S13, 0b00000},
                       {'2', S14, 0b00000},
                       {'5', S17, 0b00000},
                       {'X', S0, 0b11100}}},
                     {S13,
                      &announce_state,
                      {{'1', S14, 0b00000},
                       {'2', S15, 0b00000},
                       {'5', S18, 0b00000},
                       {'X', S0, 0b10010}}},
                     {S14,
                      &announce_state,
                      {{'1', S15, 0b00000},
                       {'2', S16, 0b00000},
                       {'5', S0, 0b10000},
                       {'X', S0, 0b10001}}},
                     {S15,
                      &announce_state,
                      {{'1', S16, 0b00000},
                       {'2', S17, 0b00000},
                       {'5', S0, 0b11000},
                       {'X', S0, 0b10110}}},
                     {S16,
                      &announce_state,
                      {{'1', S17, 0b00000},
                       {'2', S18, 0b00000},
                       {'5', S0, 0b10100},
                       {'X', S0, 0b10101}}},
                     {S17,
                      &announce_state,
                      {{'1', S18, 0b00000},
                       {'2', S0, 0b10000},
                       {'5', S0, 0b11100},
                       {'X', S0, 0b11101}}},
                     {S18,
                      &announce_state,
                      {{'1', S0, 0b10000},
                       {'2', S0, 0b11000},
                       {'5', S0, 0b10010},
                       {'X', S0, 0b10011}}}};
  State state = S0;

  for (;;) {
    statechange next_state_info = fsm[state].Action(fsm[state]);
    interpret_bin_flags(next_state_info.output);
    state = next_state_info.next_state;
  }

  return 0;
}

statechange announce_state(FSM fsm_entry) {
  fprintf(stdout, "\nMachine in state S%d. %d rub placed.\nInput coin:\n",
          fsm_entry.state, fsm_entry.state);
  scanf_s("%1s", str_glob);
  State next_state;
  switch ((char)*str_glob) {
  case '1':
    return fsm_entry.possible_state_changes[0];
  case '2':
    return fsm_entry.possible_state_changes[1];
  case '5':
    return fsm_entry.possible_state_changes[2];
  case 'X':
    return fsm_entry.possible_state_changes[3];
  case 'Q':
    exit(0);
  default:
    fprintf(stdout, "Wrong input, no state transition happened. Try again\n");
    statechange self = {' ', fsm_entry.state, 0b00000};
    return self;
  };
}

void interpret_bin_flags(char flags) {
  if (flags & 0b10000)
    fprintf(stdout, "-----------------------\nPRODUCT SOLD\nCHANGE:\n");

  if (flags & 0b01000)
    fprintf(stdout, "1 RUB COIN\t");

  if (flags & 0b00100)
    fprintf(stdout, "2 RUB COIN\t");

  if (flags & 0b00010)
    fprintf(stdout, "2 x 2 RUB COIN\t");

  if (flags & 0b00001)
    fprintf(stdout, "5 RUB COIN");

  if (flags & 0b10000)
    fprintf(stdout, "\n-----------------------\n\n");
}