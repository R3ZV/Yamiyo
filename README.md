# Yamiyo

Yamiyo is a Vampire Survival clone that aims to add mechanics from ARPGS like
Path Of Exile.

# Notes

This project uses Zig as its build system, if you download the zig binary it should
work out of the box if you have SDL3 installed on your system as it dynamically links it.

Master branch contains the OOP implementation if you wish to see the DOD
implementation head over to the DOD git branch.

I noticed that with 100.000 entities with OOP implementation there is
an average of 2.0 FPS whereas with DOD there are 2.55 FPS which is
a 27.5% increase in performance.

# References
- [ECS](https://austinmorlan.com/posts/entity_component_system/)
- [Grid Hashing](https://matthias-research.github.io/pages/tenMinutePhysics/11-hashing.pdf)
