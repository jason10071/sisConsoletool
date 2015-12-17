ANDROID_MAKE=ndk-build

all: BUILD_LINUX

BUILD_LINUX:
	@cd Linux && $(MAKE)
BUILD_ANDROID:
	@cd Android && $(ANDROID_MAKE)
CLEAN_LINUX:
	@cd Linux && $(MAKE) clean
CLEAN_ANDROID:
	@cd Android && $(ANDROID_MAKE) clean
CLEAN_LINUX_ALL:
	@cd Linux && $(MAKE) clean_all
CLEAN_ANDROID_ALL:
	@rm -vfr Android/libs Android/obj
clean: CLEAN_LINUX CLEAN_ANDROID
clean_all: CLEAN_LINUX_ALL CLEAN_ANDROID_ALL
