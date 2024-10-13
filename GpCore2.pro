TEMPLATE = subdirs

os_linux {
	SUBDIRS += \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_android {
	SUBDIRS += \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_ios {
	SUBDIRS += \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_windows {
	SUBDIRS += \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_macx {
	SUBDIRS += \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else:os_browser {
	SUBDIRS += \
		./GpUtils \
		./GpReflection \
		./GpTasks
} else {
    error("Unknown OS")
}

CONFIG += ordered
