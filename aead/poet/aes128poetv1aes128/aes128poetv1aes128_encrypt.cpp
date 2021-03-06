#include <stdlib.h>
#include <string.h>

#include "aes128poetv1aes128_encrypt.h"
#include "aes128poetv1aes128_poet.h"
#include "aes128poetv1aes128_api.h"

// CHANGE namespace moved due to includes
namespace Aes128poetv1aes128_raw {
int numRounds = -1;

int crypto_aead_encrypt(unsigned char *c,unsigned long long *clen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *ad,unsigned long long adlen,
            const unsigned char *nsec,
            const unsigned char *npub,
            const unsigned char *k)
{
  struct poet_ctx ctx;
  // CHANGE type casting added (because of c++)
  unsigned char *header = (unsigned char *) malloc((size_t) (adlen+CRYPTO_NPUBBYTES));
  unsigned char *tag = c+mlen;

  *clen=mlen+CRYPTO_NPUBBYTES;
  keysetup(&ctx, k);
  memcpy(header,ad,adlen);
  memcpy(header+adlen,npub,CRYPTO_NPUBBYTES);
  process_header(&ctx, header,adlen+CRYPTO_NPUBBYTES);
  encrypt_final(&ctx, m, mlen, c, tag);


  return 0;

}


int crypto_aead_decrypt(unsigned char *m,unsigned long long *mlen,
            unsigned char *nsec,
            const unsigned char *c,unsigned long long clen,
            const unsigned char *ad,unsigned long long adlen,
            const unsigned char *npub,
            const unsigned char *k)
{
  struct poet_ctx ctx;
  // CHANGE type casting added (because of c++)
  unsigned char *header = (unsigned char *) malloc((size_t) (adlen+CRYPTO_NPUBBYTES));
  unsigned char tag[CRYPTO_NPUBBYTES];
  int result;

  if(clen < CRYPTO_NPUBBYTES) return -1;
  *mlen = clen - CRYPTO_NPUBBYTES;

  keysetup(&ctx, k);
  memcpy(tag,c+(clen-CRYPTO_NPUBBYTES), CRYPTO_NPUBBYTES);

  memcpy(header,ad, adlen);
  memcpy(header+adlen, npub, CRYPTO_NPUBBYTES);
  process_header(&ctx, header, adlen+CRYPTO_NPUBBYTES);

  result = decrypt_final(&ctx, c, *mlen, tag,m);

  if (result) return -1;
  return 0;
}

} // namespace Aes128poetv1aes128_raw
