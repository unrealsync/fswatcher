// +build darwin

package fswatcher

//#cgo CFLAGS: -x objective-c
//#cgo LDFLAGS: -framework CoreFoundation -framework CoreServices
//extern void doRun(char *path);
import "C"

const (
	LOCAL_WATCHER_READY = "Initialized"
)

var fschanges chan string

//export receiveChange
func receiveChange(path *C.char) {
	change := C.GoString(path)
	fschanges <- change
}

func RunWatcher(path string, channelForNotifications chan string) {
	fschanges = channelForNotifications
	p := C.CString(path)
	C.doRun(p)
}
