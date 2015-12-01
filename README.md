# FischertechnikSimulator

General timings:
* Each threadmill needs 5000ms for processing one item
* So each threadmill can be devided in zones, defined by the runtime per item
* Each lightbarrier will sense a item for one second, if the corresponding threadmill ist running without a stop

Light barrier positions (in runtime per item):
* 1: 0ms - 500ms (because u need to lay the item in the middle of the lightbarrier -> half transfer time)
* 2: 3000ms - 4000ms
* 3: 2000ms - 3000ms
* 4: 2000ms - 3000ms
* 5: 4000ms - 5000ms
