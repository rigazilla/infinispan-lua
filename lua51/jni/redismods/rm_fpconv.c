/* Infinispan: compile Redis deps/lua fpconv.c as its own C translation unit.
 * Internal dependency of lua_cjson. See rm_strbuf.c for rationale. */
#include "fpconv.c"
