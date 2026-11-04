# 0xb100d

a redis-compatible in-memory key-value server in c++, benchmarked against real redis.

## goals

- speak resp2, so `redis-cli` and `redis-benchmark` work against it unchanged
- one thread, non-blocking sockets and epoll, like redis itself
- handle 100s[?] of clients at once, with pipelined commands answered in order
- commands: ping, echo, get, set, del, exists, incr (more as they earn it)
- key expiry: expire, ttl, set ... px, with lazy expiry plus redis-style random sampling
- `maxmemory` with lru eviction, o(1) both ways
- persistence (stretch): an append-only file, replayed on restart
- modern c++ (c++20, cmake, googletest), sanitizers in the debug build
- every performance number measured against real redis on the same machine (ops/s, p50/p99), never a target
- profile with perf and a flame graph, and fix the top hotspots
