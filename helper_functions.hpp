#include <string>

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

std::string urlDecode(std::string &SRC);
size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);

#endif