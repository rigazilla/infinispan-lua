package party.iroiro.luajava.lua51;

import org.junit.jupiter.api.Test;
import party.iroiro.luajava.Lua;

/**
 * Verifies Redis parity for the Infinispan lua51 native: a freshly created
 * {@link Lua51} state must expose the Redis data modules (cjson, cmsgpack,
 * struct, bit) as ready-to-use globals WITHOUT any {@code openLibrary()} call
 * (i.e. luaJavaSetup auto-opens them), so Redis Lua scripts run unmodified.
 *
 * <p>Assertions are expressed as embedded Lua {@code assert(...)}; a failure
 * raises a {@code LuaException} from {@link Lua#run(String)} and fails the test.
 */
public class RedisModulesTest {

    /** All four modules present as globals with no openLibrary() call. */
    @Test
    public void redisModulesAutoOpened() {
        try (Lua L = new Lua51()) {
            L.run("assert(type(cjson)    == 'table', 'cjson missing')");
            L.run("assert(type(cmsgpack) == 'table', 'cmsgpack missing')");
            L.run("assert(type(struct)   == 'table', 'struct missing')");
            L.run("assert(type(bit)      == 'table', 'bit missing')");
        }
    }

    /** Each module actually works at runtime. */
    @Test
    public void redisModulesWork() {
        try (Lua L = new Lua51()) {
            // cjson round-trip
            L.run("assert(cjson.encode({1,2,3}) == '[1,2,3]')");
            L.run("assert(cjson.decode('[1,2,3]')[2] == 2)");
            // cmsgpack round-trip (single and multiple values)
            L.run("assert(cmsgpack.unpack(cmsgpack.pack(42)) == 42)");
            L.run("local a,b = cmsgpack.unpack(cmsgpack.pack('x', true)); assert(a == 'x' and b == true)");
            // struct pack/unpack (big-endian uint32)
            L.run("assert(#struct.pack('>I4', 42) == 4)");
            L.run("assert(struct.unpack('>I4', struct.pack('>I4', 42)) == 42)");
            // bit (LuaBitOp)
            L.run("assert(bit.band(6, 3) == 2)");
            L.run("assert(bit.tohex(255) == '000000ff')");
        }
    }

    /**
     * Verifies that {@code os.clock} is the ONLY accessible {@code os} method.
     *
     * <p>Redis's deps/lua loslib.c is patched so {@code luaopen_os} registers a
     * sandboxed table ({@code sandbox_syslib}) containing only {@code os.clock};
     * the full os table (execute/exit/getenv/remove/rename/tmpname/setlocale/
     * time/date/difftime) is deliberately not registered. Since this build
     * vendors Redis's loslib.c verbatim, the dangerous os functions are absent,
     * matching Redis's sandbox. This test guards that posture.
     */
    @Test
    public void onlyOsClockIsAccessible() {
        try (Lua L = new Lua51()) {
            L.run("assert(type(os.clock) == 'function', 'os.clock should exist')");
            // These must be nil if os is restricted to only os.clock.
            L.run("assert(os.execute   == nil, 'os.execute should NOT be accessible')");
            L.run("assert(os.exit      == nil, 'os.exit should NOT be accessible')");
            L.run("assert(os.getenv    == nil, 'os.getenv should NOT be accessible')");
            L.run("assert(os.remove    == nil, 'os.remove should NOT be accessible')");
            L.run("assert(os.rename    == nil, 'os.rename should NOT be accessible')");
            L.run("assert(os.tmpname   == nil, 'os.tmpname should NOT be accessible')");
            L.run("assert(os.setlocale == nil, 'os.setlocale should NOT be accessible')");
            L.run("assert(os.time      == nil, 'os.time should NOT be accessible')");
            L.run("assert(os.date      == nil, 'os.date should NOT be accessible')");
            L.run("assert(os.difftime  == nil, 'os.difftime should NOT be accessible')");
        }
    }

    /** Library set matches what Redis exposes to EVAL scripts. */
    @Test
    public void redisParityLibrarySet() {
        try (Lua L = new Lua51()) {
            // Present (Redis exposes these)
            L.run("assert(type(string) == 'table' and type(table) == 'table')");
            L.run("assert(type(math) == 'table' and type(os) == 'table')");
            // Not opened, matching Redis sandbox
            L.run("assert(io == nil, 'io must not be open')");
            L.run("assert(package == nil, 'package must not be open')");
        }
    }
}
