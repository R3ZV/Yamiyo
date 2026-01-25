# Yamiyo

Yamiyo is a Vampire Survival clone that aims to add mechanics from ARPGS like
Path Of Exile.

# Optimization journey

Note that all the measurements from now on will be based on the release build
i.e. compiling with `-Doptimize=ReleaseFast` and 100.000 entities.

Without any optimizations just the DOD code we reach a 5.8 FPS, if we alos
add multithreading to our code we reach about 15.8 FPS a 172% increase in
performance.

Another optimization was changeing the spatial hash algorithm to instead
of using a normal hash map to use a just a normal array for locality of data.

As well as removing as many allocations as possible that happened before.

With these changes we went from 15.8 FPS to 30 FPS, almost doubled our FPS.

# References
- [ECS](https://austinmorlan.com/posts/entity_component_system/)
- [Grid Hashing](https://matthias-research.github.io/pages/tenMinutePhysics/11-hashing.pdf)
