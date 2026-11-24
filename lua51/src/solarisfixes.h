/* Solaris specific fixes.
 *
 * Copyright (c) 2009-Present, Redis Ltd.
 * All rights reserved.
 *
 * Licensed under your choice of (a) the Redis Source Available License 2.0
 * (RSALv2); or (b) the Server Side Public License v1 (SSPLv1); or (c) the
 * GNU Affero General Public License v3 (AGPLv3).
 */

/* Infinispan note: lua_cjson.c (from Redis deps/lua) includes this header via
 * a path relative to the Redis repo layout ("../../../src/solarisfixes.h").
 * Only deps/lua is vendored into this build, so we provide the header here at
 * the path that include resolves to (luajava/lua51/src/). The content is a
 * verbatim copy of Redis's src/solarisfixes.h; it is a no-op off Solaris. */

#if defined(__sun)

#if defined(__GNUC__)
#include <math.h>
#undef isnan
#define isnan(x) \
     __extension__({ __typeof (x) __x_a = (x); \
     __builtin_expect(__x_a != __x_a, 0); })

#undef isfinite
#define isfinite(x) \
     __extension__ ({ __typeof (x) __x_f = (x); \
     __builtin_expect(!isnan(__x_f - __x_f), 1); })

#undef isinf
#define isinf(x) \
     __extension__ ({ __typeof (x) __x_i = (x); \
     __builtin_expect(!isnan(__x_i) && !isfinite(__x_i), 0); })

#define u_int uint
#define u_int32_t uint32_t
#endif /* __GNUC__ */

#endif /* __sun */
