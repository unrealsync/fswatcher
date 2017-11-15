package main

import (
	"fmt"
	"os"
	"syscall"
	"time"

	"github.com/unrealsync/fswatcher"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s {path-to-watch}\n", os.Args[0])
		syscall.Exit(1)
	}
	path := os.Args[1]
	if _, err := os.Stat(path); os.IsNotExist(err) {
		fmt.Fprintf(os.Stderr, "Cannot find path %s\n", path)
		syscall.Exit(1)
	}
	notifyChannel := make(chan string, 100)
	go fswatcher.RunWatcher(path, notifyChannel)
	for {
		select {
		case changedPath := <-notifyChannel:
			if changedPath == fswatcher.LOCAL_WATCHER_READY {
				fmt.Printf("%s\t\tWatcher ready\n", time.Now())
			} else {
				fmt.Printf("%s\t\t%s\n", time.Now(), changedPath)
			}
		}
	}
}
