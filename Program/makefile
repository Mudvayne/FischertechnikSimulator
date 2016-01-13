objectFiles: model.c nativeLibrary.c program.c programAutomat.c programDebug.c programPanic.c programRunning.c simLightBarrier.c simPusher.c simTool.c simTreadmill.c timeUtil.c
	gcc -c model.c nativeLibrary.c program.c programAutomat.c programDebug.c programPanic.c programRunning.c programUtil.c programStart.c simLightBarrier.c simPusher.c simTool.c simTreadmill.c timeUtil.c -DWITH_MAKE_FILE -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.65-3.b17.fc22.x86_64/include/ -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.65-3.b17.fc22.x86_64/include/linux/ -fPIC 

simulatorLibrary: objectFiles
	gcc model.o nativeLibrary.o program.o programAutomat.o programDebug.o programPanic.o programRunning.o programUtil.o programStart.o simLightBarrier.o simPusher.o simTool.o simTreadmill.o timeUtil.o -DWITH_MAKE_FILE -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.65-3.b17.fc22.x86_64/include/ -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.65-3.b17.fc22.x86_64/include/linux/ -o libWuber.so -shared -lc -fPIC
