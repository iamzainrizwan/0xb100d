# 0xb100d

a redis-compatible in-memory key-value server in c++, benchmarked against real redis.

## requirements

CMake >= 3.20, g++ or clang++ for C++20
> hey! don't i need to install gtest?
no! CMake downloads it for us :p

## build/debug

project builds with cmake:

```bash
#configure cmake (once when you clone)
cmake -S . -B build
#if you're debugging:
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
#build it (redo every code edit)
cmake --build build
#test it
ctest --test-dir build
```
then, run the binary in `./build` with `./build/server`

as of right now, server listens on port `6380`
to talk to it, open a new terminal and `nc localhost 6380`

debug catches out-of-bounds, use-after-free, leaks, and undefined 
behaviour

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
