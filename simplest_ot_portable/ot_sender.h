#ifndef OT_SENDER_H
#define OT_SENDER_H

#include <stdbool.h>
#include <stdio.h>

#include "ge.h"
#include "ot_config.h"
#include "sc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ot_sender {
  unsigned char S_pack[PACKBYTES];
  unsigned char y[32];
  ge_p3 yS;
};

typedef struct ot_sender SIMPLEOT_SENDER;

void portable_sender_genS(SIMPLEOT_SENDER *, unsigned char *);
void portable_sender_keygen(SIMPLEOT_SENDER *, unsigned char *,
                            unsigned char (*)[1][HASHBYTES]);
bool portable_sender_keygen_check(SIMPLEOT_SENDER *, unsigned char *,
                                  unsigned char (*)[1][HASHBYTES]);

#ifdef __cplusplus
}
#endif

#endif  // ifndef OT_SENDER_H
