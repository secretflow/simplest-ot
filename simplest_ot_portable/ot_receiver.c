#include "ot_receiver.h"

#include <stdlib.h>

#include "ge.h"

void portable_receiver_procS(SIMPLEOT_RECEIVER *r) {
  bool success = portable_receiver_procS_check(r);
  if (!success) {
    fprintf(stderr, "Error: point decompression failed\n");
    exit(-1);
  }
}

bool portable_receiver_procS_check(SIMPLEOT_RECEIVER *r) {
  int i;

  ge_p3 S;

  if (ge_frombytes_vartime(&S, r->S_pack) != 0) {
    return false;
  }

  for (i = 0; i < 3; i++) ge_p3_dbl_p3(&S, &S);  // 8S

  ge_p3_tobytes(r->S_pack, &S);  // E_1(S)
  r->S = S;

  return true;
}

void portable_receiver_rsgen(SIMPLEOT_RECEIVER *r, unsigned char *Rs_pack,
                             unsigned char c[1]) {
  ge_p1p1 P;
  ge_p3 P_tmp;
  ge_cached xB;

  sc_random(r->x, 1);
  ge_scalarmult_base(&r->xB, r->x);  // 8x^iB

  ge_p3_to_cached(&xB, &r->xB);

  ge_sub(&P, &r->S, &xB);  // 8S - 8x^iB
  ge_p1p1_to_p3(&P_tmp, &P);
  ge_p3_cmov(&r->xB, &P_tmp, c[0]);

  ge_p3_tobytes(Rs_pack, &r->xB);  // E^1(R^i)
}

void portable_receiver_keygen(SIMPLEOT_RECEIVER *r,
                              unsigned char (*keys)[HASHBYTES]) {
  int i;

  unsigned char Rs_pack[PACKBYTES];
  ge_p3 P;

  //

  for (i = 0; i < 3; i++) ge_p3_dbl_p3(&r->xB, &r->xB);
  ge_p3_tobytes(Rs_pack, &r->xB);  // E_2(R^i)

  ge_scalarmult_vartime(&P, r->x, &r->S);  // 64x^iS

  ge_hash(keys[0], r->S_pack, Rs_pack, &P);  // E_2(x^iS)
}
