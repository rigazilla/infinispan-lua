/* Infinispan: compile Redis deps/lua lua_cjson.c as its own C translation unit.
 * ENABLE_CJSON_GLOBAL matches Redis's build flag so the `cjson` table is
 * registered as a global. See rm_strbuf.c for rationale. */
#define ENABLE_CJSON_GLOBAL
#include "lua_cjson.c"
