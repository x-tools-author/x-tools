function onBytesRead(data) {
    jsRunner.xPrint("read: " + data);
    jsRunner.xWriteBytes(data);
}

while (!jsRunner.xIsInterruptionRequested()) {
    jsRunner.xWriteBytes("hello");
    jsRunner.xSleep(1000);
}