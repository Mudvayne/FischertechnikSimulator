#Java-Simulator/Front-End
Readme for this Java Simulator Front-End thingy...

##Backends
There are different Backends available.

### Simple
A Java Backend.
Purpose: Test Java-Simulator.

### C
Integrates the C Program into the Simulator. JavaSimulator writes the Sensor conditions and reads Actor states.
Make sure to provide the program as shared library called `libWuber.so` in Linux OS and `Wuber.dll` in Windows.
Also make sure that the library provide the NativeLibrary-JNI Interface.
 
Purpose: Test C-Program in simulated Environment. 

### USB
Communicates with the Embedded Device via a TCP-Protocol.
Make sure to configure `hostname`, `port` and `brickUID`
 
Purpose: Visualize the current State from real Fischertechnik construction site. Also handles the Diagnostic-State. 

##Ways to configure and operate this Program

###In Window
Via EN-Keyboard.

|Key|Result|
|---|------|
| **components** |
| `SPACE` | Add new component to Simulator|
| `1 - 0` | Removes component with equivalent number|
| **States** |
|`z`| PANIC_SWITCH |
|`x`| DIAGNOSTIC |
|`c`| START |
|`v`| RUNNING |
|`b`| REST |
|`n`| STOP |
| **Simulation/Diagnostic** |
|`p`| Toggle Panic Switch |
|`a`| Toggle Treadmill 0 |
|`q`| Backward Pusher 0 |
|`w`| Stop Pusher 0 |
|`e`| Forward Pusher 0 |
|`r`| Toggle Treadmill 1 |
|`t`| Toggle Treadmill 2 |
|`d`| Toggle Tool 0 |
|`f`| Toggle Tool 1 |
|`y`| Backward Pusher 1 |
|`u`| Stop Pusher 1 |
|`i`| Forward Pusher 1 |
|`j`| Toggle Treadmill 3 |

###As Properties
Via -D Param on JVM start-up

| key | description | datatype | default|
|-----|-------------|----------|--------|
| **General** |
| `model` | Desired Model, can be `USB`, `C` or `Simple`. | `String` | C |
| `delta` | Sleep-time for gameloop in ms | `int` | 10 |
| **USB Model related** |
| `hostname` | Hostname for Brick Deamon | `String` | localhost |
| `port` | Port from Brick Deamon | `int` | 4223 |
| `timeoutInMS` | Timeout for not replied messages | `int` | 1000 |
| `brickUID` | The encoded BrickUID | `Long` | 3568702513 <- Group_01 MasterBrick |
| **C Model related** |
| `java.library.path` | path to shared library program called `libWuber.so` or `Wuber.dll` | `String` | n/a |
| **Simulation related** |
| `treadmillSpeed` | Speed for treadmills in pixel/milliseconds | `Float` | 0.064 |
| `pusherSpeed` | Speed for Pusher in pixel/milliseconds | `Float` | 0.0667 |
| `minComponentDistance` | A min distance for new Components | `Float` | 85.0 |


###As \*.properties File
To add components on start-up expand components propertie in file stored in jar ./components.properties

For each component include x,y coordinates, which are separated with ','. To add more than one component separate with '|'. 
Example:
components = 860,300|860,130
