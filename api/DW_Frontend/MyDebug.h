#ifndef __MYDEBUG_H__
#define __MYDEBUG_H__

#define ENABLE_DRIVER_DEBUG

#ifdef ENABLE_DRIVER_DEBUG
#define DriverDebugPrint      printk
#else
#define DriverDebugPrint
#endif

#endif