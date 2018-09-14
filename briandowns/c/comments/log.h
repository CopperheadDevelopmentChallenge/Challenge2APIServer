#ifndef _LOG_H
#define _LOG_H

/**
 * logger_format
 */
enum logger_format {
    standard,
    json,
};

/**
 * log_json receives a key and a value. The value is the HTTP request header which
 * which is then broken up into relevant pieces and added to the JSON log.
 */
void log_json(const char* tag, char* message);

/*
 * log_standard receives a key and value. The value is the HTTP request header which
 * is then broken up into relevant pieces and added to the log buffer.
 */
void log_standard(const char* tag, char* message);

/**
 * log_to_stdout receives a key, value, and log type. Call the necessary logger with the 
 * given parameters.
 */
void log_to_stdout(int lf, const char* tag, char* message);

#endif
