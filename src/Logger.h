/**
 *  $Id: Logger.h 114 2007-10-04 04:57:03Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  A class for logging errors, to make it consistent, and to ensure that the
 *  system is exited if things get too bad.
**/
#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#define __FE_DEBUG_FILE "fe_debug.log"

#include "CompileConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "string.h"
#include "Object.h"

namespace fire_engine
{

/** The severity of the error. The application will exit on
    a CRITICAL error */
enum ESEVERITY
{
    ES_DEBUG     = 0x01,
    ES_LOW       = 0x02,
    ES_MEDIUM    = 0x03,
    ES_HIGH      = 0x04,
    ES_CRITICAL  = 0x05
};

class _FIRE_ENGINE_API_ Logger : public Object
{
    public:
        /**
         * Create an instance of the Logger.
         * @param debug    The file descriptor for DEBUG messages.
         * @param low      The file descriptor to write to for a LOW severity
         *                 error.
         * @param medium   The file descriptor to write to for a MEDIUM severity
         *                 error.
         * @param high     The file descriptor to write to for a HIGH severity
         *                 error.
         * @param critical The file descriptor to write to for a CRITICAL
         *                 severity error.
         * @return         A pointer to the singleton instance of the
         *                 Logger.
        **/
        static Logger * Create(FILE * debug = stdout,
            FILE * low      = stdout,
            FILE * medium   = stdout,
            FILE * high     = stderr,
            FILE * critical = stderr);

        /**
         * Get the instance of the Logger
         * @return A pointer to the singleton instance of the Logger
        **/
        static Logger * Get();

        /**
         * Change one of the file descriptors of the Logger
         * @param es  The severity for which the file descriptor should be
         *            changed.
         * @param out The new file descriptor.
        **/
        void setOutput(ESEVERITY es, FILE * out);

        /**
         * Set debugging mode
         * @param flag  Set to true to turn debugging on, false otherwise
        **/
        void setDebug(bool flag);

        /**
         * Log an error.
         * NOTE: The application *will* exit on a CRITICAL error
         * @param es  The severity of the error to log
         * @param src The source (ie class / file) where the error occured
         * @param fmt The actual text describing the error, can contain formats
        **/
        void log(ESEVERITY es, const string& src, const string fmt, ...) const;

    private:
		static Logger * m_instance; //! The singleton instance of the Logger
		bool m_debug_flag;    //! The flag to turn on/off debug mode
		FILE * m_debug_fd;    //! The file descriptor to write to, for DEBUG messages
		FILE * m_low_fd;      //! The file descriptor to write to, in case of a LOW error
		FILE * m_medium_fd;   //! The file descriptor to write to, in case of a MEDIUM error
		FILE * m_high_fd;     //! The file descriptor to write to, in case of a HIGH error
		FILE * m_critical_fd; //! The file descriptor to write to, in case of a CRITICAL error

        /**
         * The actual constructor, made private to ensure a single logger.
         * @param debug    The file descriptor, for DEBUG messages.
         * @param low      The file descriptor to write to for a LOW severity
         *                 error.
         * @param medium   The file descriptor to write to for a MEDIUM severity
         *                 error.
         * @param high     The file descriptor to write to for a HIGH severity
         *                 error.
         * @param critical The file descriptor to write to for a CRITICAL
         *                 severity error.
        **/
        Logger(FILE *, FILE *, FILE *, FILE *, FILE *);

        FILE * getFd(ESEVERITY es) const;

};

}

#endif
