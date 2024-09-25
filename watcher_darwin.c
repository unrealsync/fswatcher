#include "_cgo_export.h"

// Watcher is based on fsnotifier for linux for IntelliJ IDEA
#define bool int
#define true 1
#define false 0

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <sys/stat.h>
#include <sys/time.h>

static void printChangesFunc(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]) {
	char **paths = eventPaths;
	struct timeval tv;

	int i;
	for (i = 0; i < numEvents; i++) {
		receiveChange(paths[i]);
	}
}

void initFSEvents(const char *path) {

	/* Define variables and create a CFArray object containing
	 CFString objects containing paths to watch.
	 */

	CFStringRef mypath = CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
	CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&mypath, 1, NULL);
	void *callbackInfo = NULL; // could put stream-specific data here.
	FSEventStreamRef stream;
	CFAbsoluteTime latency = 0.0; /* Latency in seconds */

	/* Create the stream, passing in a callback */
	stream = FSEventStreamCreate(NULL,
	                             &printChangesFunc,
	                             callbackInfo,
	                             pathsToWatch,
	                             kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
	                             latency,
	                             kFSEventStreamCreateFlagNone|kFSEventStreamCreateFlagNoDefer /* Flags explained in reference */
	                             );

	CFRelease(pathsToWatch);
	CFRelease(mypath);

	/* Create the stream before calling this. */
	FSEventStreamSetDispatchQueue(stream, dispatch_get_main_queue());
	FSEventStreamStart(stream);
}

void doRun (char *path) {

	struct stat tmp;

	if(stat(path, &tmp) != 0) {
		return;
	}
    receiveChange("Initialized"); // we need this to mark that fs changes thread is ready

	initFSEvents(path);
	CFRunLoopRun();
}
