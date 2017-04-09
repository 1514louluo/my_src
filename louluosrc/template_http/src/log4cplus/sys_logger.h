#ifndef _SYS_LOGGER_H
#define _SYS_LOGGER_H

#ifdef LOGGER
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

/*INIT_LOGGER is called in main(), for load conf file!*/
#define INIT_LOGGER(log_conf_path) 	log4cplus::PropertyConfigurator::doConfigure(log_conf_path);
/*ALLOC_LOGGER is used in c function*/
#define ALLOC_LOGGER(name) log4cplus::Logger logger=log4cplus::Logger::getInstance(#name);

/*DECL_LOGGER and IMPL_LOGGER is used in c++ class*/
#define DECL_LOGGER(logger) static log4cplus::Logger logger;
#define IMPL_LOGGER(classname, logger) log4cplus::Logger classname::logger = log4cplus::Logger::getInstance(#classname);

/*----------------------------------------------------------------*/
#ifndef _DEBUG

#ifdef LOG4CPLUS_DEBUG
#undef LOG4CPLUS_DEBUG
#define LOG4CPLUS_DEBUG(a,b)
#endif

#ifdef LOG4CPLUS_INFO
#undef LOG4CPLUS_INFO
#define LOG4CPLUS_INFO(a,b)
#endif

#endif // #endif _DEBUG
/*----------------------------------------------------------------*/
#else  // #ifndef LOGGER
#define _LOG4CPLUS_LOGGING_MACROS_HEADER_
#define LOG4CPLUS_DEBUG(a,b)
#define LOG4CPLUS_INFO(a,b)
#define LOG4CPLUS_WARN(a,b)
#define LOG4CPLUS_ERROR(a,b)
#define LOG4CPLUS_FATAL(a,b)

#define INIT_LOGGER(log_conf_path) 
#define ALLOC_LOGGER(name)
#define DECL_LOGGER(logger)
#define IMPL_LOGGER(classname, logger)

#endif // #endif LOGGER
#endif
