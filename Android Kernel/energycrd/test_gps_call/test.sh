~/arm/bin/clang -pie intercept.c -o intercept
adb push intercept /data/local/tmp/.
adb shell "./data/local/tmp/intercept"
