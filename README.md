# Semaphore Bake Off

Multithreaded bakery simulation in C using POSIX semaphores for shared resource synchronization.

## Overview

Multiple bakers compete in a shared kitchen, each running as its own POSIX thread. Every shared resource is protected by a counting or binary semaphore to prevent race conditions and deadlock. Each baker must complete all 5 recipes before the program finishes.

**Recipes:**
- Cookies
- Pancakes
- Homemade Pizza Dough
- Soft Pretzels
- Cinnamon Rolls

## Kitchen Resources

| Resource     | Count | Semaphore Type |
|--------------|-------|----------------|
| Mixer        | 2     | Counting        |
| Pantry       | 1     | Binary          |
| Refrigerator | 2     | Counting        |
| Bowl         | 3     | Counting        |
| Spoon        | 5     | Counting        |
| Oven         | 1     | Binary          |

Each ingredient (Flour, Sugar, Yeast, Baking Soda, Salt, Cinnamon, Egg, Milk, Butter) also has its own binary semaphore — only one baker may access a given ingredient at a time.

## How It Works

1. User specifies the number of bakers at runtime
2. All semaphores are initialized before any threads spawn
3. Each baker thread races to gather ingredients one at a time from the pantry or fridge
4. Once all ingredients are collected, the baker acquires a bowl, spoon, and mixer to mix
5. The baker then acquires the oven to bake and releases all equipment
6. Each baker completes all 5 recipes then announces they are finished

## Deadlock Avoidance

All bakers acquire resources in a consistent order: ingredients first (grabbed and released one at a time), then bowl → spoon → mixer, then oven. Consistent ordering eliminates circular wait, preventing deadlock.

## Ramsied Mechanic

Baker 1 is designated as the Ramsied baker. After grabbing the bowl, spoon, and mixer for any recipe, there is a 33% chance Baker 1 gets Ramsied — they release all equipment and restart the current recipe from the beginning. This is guaranteed to be possible on every run.

## Output

Each baker prints real-time status updates in a unique terminal color using ANSI escape codes, making concurrent output from multiple bakers easy to follow.

## Build & Run

```bash
gcc baker.c -o baker -lpthread
./baker
```

Then enter the number of bakers when prompted. Use 3 bakers to keep output readable.

> **Note:** Compiling on macOS will generate deprecation warnings for `sem_init` and `sem_destroy` as Apple has deprecated unnamed POSIX semaphores. These warnings do not affect functionality and will not appear when compiled on Linux.

## Course

CIS 452 — Operating Systems, Grand Valley State University
