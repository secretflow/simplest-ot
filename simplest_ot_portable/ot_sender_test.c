#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "ot_sender.h"

#include "cpucycles.h"
#include "network.h"
#include "ot_config.h"

void ot_sender_test(SIMPLEOT_SENDER *sender, int newsockfd) {
  int i, k;
  unsigned char S_pack[PACKBYTES];
  unsigned char Rs_pack[PACKBYTES];
  unsigned char keys[2][1][HASHBYTES];

  //

  sender_genS(sender, S_pack);
  writing(newsockfd, S_pack, sizeof(S_pack));

  //

  for (i = 0; i < NOTS; i += 1) {
    reading(newsockfd, Rs_pack, sizeof(Rs_pack));

    sender_keygen(sender, Rs_pack, keys);

    //

    if (VERBOSE) {
      printf("%4d-th sender keys:", i);

      for (k = 0; k < HASHBYTES; k++)
        printf("%.2X", keys[0][0][k]);
      printf(" ");
      for (k = 0; k < HASHBYTES; k++)
        printf("%.2X", keys[1][0][k]);
      printf("\n");
    }
  }
}

int main(int argc, char *argv[]) {
  int sockfd;
  int newsockfd;
  int rcvbuf = BUFSIZE;
  int reuseaddr = 1;

  long long t = 0;

  SIMPLEOT_SENDER sender;

  //

  if (argc != 2) {
    fprintf(stderr, "usage %s port\n", argv[0]);
    exit(-1);
  }

  //

  sockfd = server_listen(atoi(argv[1]));
  newsockfd = server_accept(sockfd);

  if (setsockopt(newsockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf)) !=
      0) {
    perror("ERROR setsockopt");
    exit(-1);
  }
  if (setsockopt(newsockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
                 sizeof(reuseaddr)) != 0) {
    perror("ERROR setsockopt");
    exit(-1);
  }

  t -= cpucycles_amd64cpuinfo();

  ot_sender_test(&sender, newsockfd);

  t += cpucycles_amd64cpuinfo();

  //

  if (!VERBOSE)
    printf("[n=%d] Elapsed time:  %lld cycles\n", NOTS, t);

  shutdown(newsockfd, 2);
  shutdown(sockfd, 2);

  //

  return 0;
}
