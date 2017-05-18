// +build linux

#include "_cgo_export.h"

// Watcher is based on fsnotifier for linux for IntelliJ IDEA
#define bool int
#define true 1
#define false 0

#include "watcher_linux/fsnotifier.h"
#include "watcher_linux/util.c"
#include "watcher_linux/inotify.c"
#include "watcher_linux/main.c"
