/* Infinispan: compile Redis deps/lua strbuf.c as its own C translation unit.
 * These Redis modules are not C++-clean and strbuf.h has no include guard, so
 * they cannot be amalgamated into the C++ all.cc TU. Compiling each as a
 * separate C object (as Redis itself does via its Makefile) is the correct
 * approach. The source is resolved from the Redis checkout via the 'lua51/src'
 * header dir (-I), so it always tracks the vendored deps/lua. */
#include "strbuf.c"
