// Copyright 2018 Brian J. Downs. All rights reserved.
// Use of this source code is governed by an BSD-style
// license that can be found in the LICENSE file.

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
 * log_t
 */
typedef struct {
    enum logger_format log_format;
} log_t;


/**
 * log_new creates a new pointer to a log_t structure
 */
log_t *log_new(enum logger_format lf);

/**
 * log_json receives a key and a value
 */
void log_json(const char* tag, char* message);

/*
 * log_standard receives a key and value
 */
void log_standard(const char* tag, char* message);

/**
 * log_to_stdout receives a key, value, and log type. Call the necessary logger with the 
 * given parameters.
 */
void log_to_stdout(int lf, const char* tag, char* message);

#endif  /* _LOG_H */
