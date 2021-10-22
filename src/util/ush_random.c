#include "stdlib.h"
#include "time.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "ush_random.h"

int ush_random_generate_cert(const char *seed) {
    ush_assert(seed);

    static unsigned int real_seed = 0;

    real_seed += (unsigned int)*seed;

    srand(real_seed + (unsigned int)time(NULL));
    int cert = rand();

    cert &= 0x7FFFFFFF; // set the sign-bit to 0

    ush_log(LOG_LVL_INFO, "cert gen 0x%08x", cert);

    return cert;
}
