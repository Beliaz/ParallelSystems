# stencil
| Dimension | 1 | 2 | 3 |
|---|---|---|---|
| 1 Thread | 59800 ms | 64095 ms | 42001 ms | 2616 ms |
| Speedup | 1x | 1x | 1x |
| 2 Threads | 30643 ms | 32343 ms | 21038 ms | 32327 ms |
| Speedup | 1,95x | 1,98x | 2x |
| 4 Threads | 16296 ms | 16419 ms | 10577 ms | 432170 ms |
| Speedup | 3,67x | 3,90x | 3,97x |
| 8 Threads | 8471 ms | 8883 ms | 5491 ms | 432170 ms |
| Speedup | 7,06x | 7,22x | 7,65x |

The runs did include the following parameters

| Dimension | 1D Size | Total Elements | Border1 | Border2 | Border3 | Border4 | Border5 | Border 6 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | 38.000 | 38.000 | 273 | 1000 |
| 2 | 220 | 48.400 | 273 | 1000 | 2000 | 10 |
| 3 | 140 | 2.744.000 | 273 | 1000 | 2000 | 10 | 200 | 8000 |