#include "ot_sender.h"

#include <stdlib.h>

#include "ge.h"

void sender_genS(SIMPLEOT_SENDER *s, unsigned char *S_pack) {
  int i;

  ge_p3 S, yS;

  //

  sc_random(s->y, 0);

  ge_scalarmult_base(&S, s->y); // S

  ge_p3_tobytes(S_pack, &S); // E^0(S)

  for (i = 0; i < 3; i++)
    ge_p3_dbl_p3(&S, &S); // 8S

  ge_p3_tobytes(s->S_pack, &S); // E_1(S)

  ge_scalarmult_vartime(&yS, s->y, &S);
  for (i = 0; i < 3; i++)
    ge_p3_dbl_p3(&yS, &yS); // 64T
  s->yS = yS;
}

void sender_keygen(SIMPLEOT_SENDER *s, unsigned char *Rs_pack,
                   unsigned char (*keys)[1][HASHBYTES]) {
  if (!sender_keygen_check(s, Rs_pack, keys)) {
    fprintf(stderr, "Error: point decompression failed\n");
    exit(-1);
  }
}

bool sender_keygen_check(SIMPLEOT_SENDER *s, unsigned char *Rs_pack,
                         unsigned char (*keys)[1][HASHBYTES]) {
  int i;

  ge_p3 P0;
  ge_p3 P1;
  ge_p3 Rs;
  ge_cached tmp;
  ge_p1p1 tmp2;

  //

  if (ge_frombytes_vartime(&Rs, Rs_pack) != 0) {
    return false;
  }

  for (i = 0; i < 3; i++)
    ge_p3_dbl_p3(&Rs, &Rs); // 64R^i

  ge_p3_tobytes(Rs_pack, &Rs); // E_2(R^i)

  ge_scalarmult_vartime(&P0, s->y, &Rs);        // 64yR^i
  ge_hash(keys[0][0], s->S_pack, Rs_pack, &P0); // E_2(yR^i)

  ge_p3_to_cached(&tmp, &P0);
  ge_sub(&tmp2, &s->yS, &tmp); // 64(T-yR^i)
  ge_p1p1_to_p3(&P1, &tmp2);
  ge_hash(keys[1][0], s->S_pack, Rs_pack, &P1); // E_2(T - yR^i)}

  return true;
}
