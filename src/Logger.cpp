/**
 *  $Id: Logger.cpp 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the Logger class, as defined in Logger.h
**/
#include "Logger.h"

namespace fire_engine
{
Logger * Logger::m_instance = 0;
//------------------------------------------------------------------------------
Logger::Logger(FILE * debug, FILE * low, FILE * medium,
    FILE * high, FILE * critical)
    : m_debug_flag(true),
      m_debug_fd(debug),
      m_low_fd(low),
      m_medium_fd(medium),
      m_high_fd(high),
      m_critical_fd(critical)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Logger");
#endif
}
//------------------------------------------------------------------------------
Logger * Logger::Create(FILE * debug, FILE * low, FILE * medium,
    FILE * high, FILE * critical)
{
	if (m_instance == 0)
		m_instance = new Logger(debug, low, medium, high, critical);
	return m_instance;
}
//------------------------------------------------------------------------------
Logger * Logger::Get()
{
    return m_instance;
}
//------------------------------------------------------------------------------
void Logger::log(ESEVERITY es, const String& src, const String fmt, ...) const
{
	FILE * fd = getFd(es);
	va_list list;
	va_start(list, fmt);
	if (es == ES_DEBUG)
	{
		if (m_debug_flag)
		{
			fprintf(fd, "[DEBUG - %s]: ", src.c_str());
			vfprintf(fd, fmt.c_str(), list);
			fprintf(fd, ".\n");
		}
	}
	else
	{
		fprintf(fd, "%s: ", src.c_str());
		vfprintf(fd, fmt.c_str(), list);
		fprintf(fd, ".\n");
	}
	va_end(list);
	if (es == ES_CRITICAL)
		exit(EXIT_FAILURE);
}
//------------------------------------------------------------------------------
void Logger::setOutput(ESEVERITY es, FILE * out)
{
    switch (es)
    {
        case ES_DEBUG:
            m_debug_fd = out;
            break;

        case ES_LOW:
            m_low_fd = out;
            break;

        case ES_MEDIUM:
            m_medium_fd = out;
            break;

        case ES_HIGH:
            m_high_fd = out;
            break;

        case ES_CRITICAL:
            m_critical_fd = out;
            break;
    }
}
//------------------------------------------------------------------------------
void Logger::setDebug(bool flag)
{
    m_debug_flag = flag;
}
//------------------------------------------------------------------------------
FILE * Logger::getFd(ESEVERITY es) const
{
	FILE * fd = 0;
	switch (es)
	{
		case ES_DEBUG:
			fd = m_debug_fd;
			break;
		case ES_LOW:
			fd = m_low_fd;
			break;
		case ES_MEDIUM:
			fd = m_medium_fd;
			break;
		case ES_HIGH:
			fd = m_high_fd;
			break;
		case ES_CRITICAL:
			fd = m_critical_fd;
			break;
	}
	return fd;
}
}
