#ifndef CUSTOM_H
#define CUSTOM_H

#include "lua.hpp"

int luaopen_jua(lua_State *L) {
    luaL_register(L, LUA_JAVALIBNAME, javalib);
    return 1;
}

static void luaJavaSetup(lua_State * L, JNIEnv * env, int lid) {
    lua_atpanic(L, &fatalError);
    luaJ_openlib(L, "");
    luaJ_openlib_call(L, LUA_JAVALIBNAME, &luaopen_jua);

    /* --- Infinispan: Redis parity --------------------------------------------
     * Open the same library set Redis exposes to EVAL scripts (see Redis
     * script_lua.c luaLoadLibraries) so Redis Lua scripts run unmodified:
     * base (above) + table, string, math, os + the data modules cjson,
     * cmsgpack, struct and bit.
     *
     * NOTE on `os`: this exposes only os.clock. Redis's deps/lua loslib.c is
     * patched so luaopen_os registers a sandboxed table (sandbox_syslib) with
     * clock only; the full os table (execute/exit/getenv/remove/rename/...) is
     * deliberately NOT registered. Opening `os` here therefore matches Redis's
     * sandbox exactly. `io` and `package` are intentionally not opened either.
     * ------------------------------------------------------------------------ */
    luaJ_openlib(L, "table");
    luaJ_openlib(L, "string");
    luaJ_openlib(L, "math");
    luaJ_openlib(L, "os");
    luaJ_openlib(L, "cjson");
    luaJ_openlib(L, "cmsgpack");
    luaJ_openlib(L, "struct");
    luaJ_openlib(L, "bit");

    lua_pushstring(L, JAVA_STATE_INDEX);
    lua_pushinteger(L, lid);
    lua_settable(L, LUA_REGISTRYINDEX);

    initMetaRegistry(L);
}

static int initLua51Bindings(JNIEnv * env) {
    return initBindings(env);
}

#endif /* !CUSTOM_H */
