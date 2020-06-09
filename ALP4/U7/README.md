# N-body problem
Quelle: www.cs.berkeley.edu/~demmel/cs267_Spr09

```c
for each timestep t {
    for each particle i
        compute f(i), the force on i
    for each particle i
        update position and velocity of i using F = ma
    if (output step) Output new positions and velocities
}
```
