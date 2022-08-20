#pragma once

#define GL_LOG_FILE "gl.log"
void log_reset();
void log_info(const char* message, ...);
void log_error(const char* message, ...);