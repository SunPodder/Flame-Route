Memory usage (kb) from `ps`

Ubuntu:
-----------------------------
Initial | After | Iterations
-----------------------------
2132 | 2232 | 1000
2128 | 2228 | 1000
2132 | 2216 | 1000
2132 | 2460 | 5000
2100 | 2600 | 5000

Arch:
-----------------------------
Initial | After | Iterations
-----------------------------
2148 | 2508 | 1000
2240 | 2316 | 1000
2240 | 2472 | 1000
2148 | 2680 | 5000
2240 | 2540 | 5000

Android:
-----------------------------
Initial | After | Iterations
-----------------------------
2640 | 2640 | 1000
2592 | 2680 | 5000

Github CI (Ubuntu):
-----------------------------
Initial | After | Iterations
-----------------------------
2360 | 4320 | 1000
2400 | 4384 | 10000
2468 | 4644 | 100000
2568 | 4260 | 500000

Repl.it:
-----------------------------
Initial | After | Iterations
-----------------------------
2924 | 4772 | 1000
2856 | 4724 | 10000


Not considering this as a critical memory leak, as the memory increment freezes at a certain point on every system. Still further investigation should be made.

Here are some suspicious warnings from Valgrind, though if I remember correctly they're there in a previous version without this leaks.

# Valgrind:
- Use of uninitialised value of size 8
- Conditional jump or move depends on uninitialised value(s)
- Uninitialised value was created by a stack allocation

