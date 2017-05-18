// +build freebsd

package fswatcher

/*
extern void doRun(char *path);
extern void free(void *pointer);
*/
import "C"

const (
	LOCAL_WATCHER_READY = "Initialized"
)

//export receiveChange
func receiveChange(path *C.char) {
}

func RunWatcher(path string, channelForNotifications chan string) {
}
