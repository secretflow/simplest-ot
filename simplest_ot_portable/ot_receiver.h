#ifndef OT_RECEIVER_H
#define OT_RECEIVER_H

#include <stdbool.h>
#include <stdio.h>

#include "ge.h"
#include "ot_config.h"
#include "sc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ot_receiver {
  unsigned char S_pack[PACKBYTES];
  ge_p3 S;

  // temporary

  ge_p3 xB;
  unsigned char x[32];
};

typedef struct ot_receiver SIMPLEOT_RECEIVER;

void portable_receiver_procS(SIMPLEOT_RECEIVER *);
bool portable_receiver_procS_check(SIMPLEOT_RECEIVER *);
void portable_receiver_rsgen(SIMPLEOT_RECEIVER *, unsigned char *,
                             unsigned char[1]);
void portable_receiver_keygen(SIMPLEOT_RECEIVER *,
                              unsigned char (*)[HASHBYTES]);

#ifdef __cplusplus
}
#endif

#endif  // ifndef OT_RECEIVER_H
