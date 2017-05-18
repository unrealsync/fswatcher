// +build linux

package fswatcher

/*
extern void doRun(char *path);
extern void free(void *pointer);
*/
import "C"

import (
	"unsafe"
)

const (
	LOCAL_WATCHER_READY = "Initialized"
)

var fschanges chan string

//export receiveChange
func receiveChange(path *C.char) {
	fschanges <- C.GoString(path)
}

func RunWatcher(path string, channelForNotifications chan string) {
	fschanges = channelForNotifications
	p := C.CString(path)
	C.doRun(p)
	C.free(unsafe.Pointer(p))
}
