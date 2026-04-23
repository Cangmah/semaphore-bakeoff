# Semaphore Bake Off

Multithreaded bakery simulation in C using POSIX semaphores for shared resource synchronization.

## Overview

Multiple bakers compete in a shared kitchen, each running as its own thread. Every shared resource (mixers, ovens, pantry, refrigerators, bowls, spoons) is protected by a counting or binary semaphore to prevent race conditions and deadlock.

Each baker must complete all 5 recipes:
- Cookies
- Pancakes
- Homemade Pizza Dough
- Soft Pretzels
- Cinnamon Rolls

## Kitchen Resources

| Resource     | Count |
|--------------|-------|
| Mixer        | 2     |
| Pantry       | 1     |
| Refrigerator | 2     |
| Bowl         | 3     |
| Spoon        | 5     |
| Oven         | 1     |

## How It Works

1. User specifies the number of bakers at runtime
2. Each baker runs as a separate thread and races to acquire ingredients and equipment
3. Ingredients are grabbed one at a time from the pantry or refrigerator (semaphore-protected)
4. Once all ingredients are collected, the baker acquires a bowl, spoon, and mixer to mix
5. The baker then waits for the oven to bake
6. One baker is designated to be randomly "Ramsied" — they drop all semaphores and restart the current recipe from scratch
7. Each baker prints real-time status updates in a unique color
8. Program ends when all bakers have completed all recipes

## Concepts Demonstrated

- POSIX threads (`pthreads`)
- POSIX semaphores
- Counting vs binary semaphores
- Deadlock avoidance via consistent resource ordering
- Shared memory / shared state between threads
- ANSI terminal color output

## Build & Run

```bash
make
./bakeoff
```

## Course

CIS 452 — Operating Systems, Grand Valley State University
