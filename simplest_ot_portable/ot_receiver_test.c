#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ot_receiver.h"

#include "cpucycles.h"
#include "network.h"
#include "ot_config.h"
#include "randombytes.h"

void ot_receiver_test(SIMPLEOT_RECEIVER *receiver, int sockfd) {
  int i, k;

  unsigned char Rs_pack[PACKBYTES];
  unsigned char keys[1][HASHBYTES];
  unsigned char cs[1];

  //

  reading(sockfd, receiver->S_pack, sizeof(receiver->S_pack));
  receiver_procS(receiver);

  //

  for (i = 0; i < NOTS; i += 1) {
    randombytes(&cs[0], sizeof(cs));
    cs[0] &= 1;

    if (VERBOSE)
      printf("%4d-th choose bit = %d\n", i, cs[0]);

    receiver_rsgen(receiver, Rs_pack, cs);

    writing(sockfd, Rs_pack, sizeof(Rs_pack));

    receiver_keygen(receiver, keys);

    //

    if (VERBOSE) {
      printf("%4d-th reciever key:", i);

      for (k = 0; k < HASHBYTES; k++)
        printf("%.2X", keys[0][k]);
      printf("\n");
    }
  }
}

int main(int argc, char *argv[]) {
  int sockfd;
  int sndbuf = BUFSIZE;
  int flag = 1;

  long long t = 0;

  SIMPLEOT_RECEIVER receiver;

  //

  if (argc != 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(-1);
  }

  //

  client_connect(&sockfd, argv[1], atoi(argv[2]));

  if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(int)) != 0) {
    perror("ERROR setsockopt");
    exit(-1);
  }
  if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) != 0) {
    perror("ERROR setsockopt");
    exit(-1);
  }

  t -= cpucycles_amd64cpuinfo();

  ot_receiver_test(&receiver, sockfd);

  t += cpucycles_amd64cpuinfo();

  //

  if (!VERBOSE)
    printf("[n=%d] Elapsed time:  %lld cycles\n", NOTS, t);

  shutdown(sockfd, 2);

  //

  return 0;
}
