~/arm/bin/clang -pie syscallstest.c -o syscalltest
adb push syscalltest /data/local/tmp/.
adb shell "./data/local/tmp/syscalltest"
