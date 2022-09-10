TEMPLATE = subdirs

os_linux {
	SUBDIRS += \
		./Config \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_android {
	SUBDIRS += \
		./Config \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_ios {
	SUBDIRS += \
		./Config \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_windows {
	SUBDIRS += \
		./Config \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_macx {
	SUBDIRS += \
		./Config \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_browser {
	SUBDIRS += \
		./Config \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else {
	error("Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser, os_baremetal")
}

CONFIG += ordered
