#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

#ifdef __WIN32__
#include <windows.h>
#endif

int xavaMkdir(char *dir) {
	#if defined(__unix__)||defined(__APPLE__)
		return mkdir(dir, 0770);
	#else
		return mkdir(dir);
	#endif
}

int xavaGetConfigDir(char *configPath) {
	#if defined(__unix__)||defined(__APPLE__)
		char *configFile = "config";
		char *configHome = getenv("XDG_CONFIG_HOME");
	#elif defined(WIN)
		// editing files without an extension on windows is a pain
		char *configFile = "config.cfg";
		char *configHome = getenv("APPDATA");
	#endif
	// don't worry, this will never happen on windows unless you are running like 98
	if (configHome != NULL) {
		#ifdef __WIN32__
			sprintf(configPath,"%s\\%s\\", configHome, PACKAGE);
		#else
			sprintf(configPath,"%s/%s/", configHome, PACKAGE);
		#endif
	} else {
		configHome = getenv("HOME");
		if (configHome != NULL)
			sprintf(configPath,"%s/%s/%s/", configHome, ".config", PACKAGE);
		else return -1;
	}
	return 0;
}

const char *xavaGetInstallDir() {
	#ifdef __WIN32__
		// Windows uses widechars internally
		WCHAR wpath[MAX_PATH];
		char path[MAX_PATH];

		// Get path of where the executable is installed
		HMODULE hModule = GetModuleHandleW(NULL);
		GetModuleFileNameW(hModule, wpath, MAX_PATH);
		wcstombs(path, wpath, MAX_PATH);

		// Hardcoded things pain me, but writing a 100 line-long
		// string replace function for windows is a no-no.
		// This is supposed to find and replace %PROGRAM_NAME%.exe 
		// at the end of the string
		size_t executableNameSize = strlen(PACKAGE".exe");
		path[strlen(path)-executableNameSize] = '\0';
	#else 
		// everything non-windows is simple as fuck, go look at the mess above
		const char *path = PREFIX"/share/"PACKAGE"/";
	#endif
	return path;
}

unsigned long xavaSleep(unsigned long oldTime, int framerate) {
	unsigned long newTime = 0;
	if(framerate) {
	#ifdef WIN
		SYSTEMTIME time;
		GetSystemTime(&time);
		newTime = time.wSecond*1000+time.wMilliseconds;
		if(newTime-oldTime<1000/framerate&&newTime>oldTime)
			Sleep(1000/framerate-(newTime-oldTime));
		GetSystemTime(&time);
		return time.wSecond*1000+time.wMilliseconds;
	#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		newTime = tv.tv_sec*1000+tv.tv_usec/1000;
		if(oldTime+1000/framerate>newTime)
			usleep((1000/framerate+oldTime-newTime)*1000);
		gettimeofday(&tv, NULL);
		return tv.tv_sec*1000+tv.tv_usec/1000;
	#endif
	}
	#ifdef WIN
	Sleep(oldTime);
	#else
	usleep(oldTime*1000);
	#endif
	return 0;
}
