/**
 * \file feature_os_port.h
 *
 * \todo Document this header
 */

#ifndef _FEATURE_OS_PORT_H_
#define _FEATURE_OS_PORT_H_

#ifdef WIN32

#include <Windows.h>

#include <direct.h>

#define PATH_MAX MAX_PATH

#else

#include <fcntl.h>
#include <unistd.h>

#define MAX_PATH PATH_MAX

#endif

#endif
