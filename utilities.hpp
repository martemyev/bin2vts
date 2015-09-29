#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>

//------------------------------------------------------------------------------
//
// d2s<T> - convert data of type T to string
//
//------------------------------------------------------------------------------
/**
 * Convert the data of any type which has oveloaded operator '<<' to string
 * @param data - the data
 * @param scientific - use scientific (exponential) format or not
 * @param precision - if scientific format is used, we can change the precision
 * @return data in string format
 */
template <typename T>
inline std::string d2s(T data,
                       bool scientific = false,
                       int precision = 6,
                       bool noperiod = false)
{
  std::ostringstream o;
  if (scientific)
  {
    o.setf(std::ios::scientific);
    o.precision(precision);
  }

  if (!(o << data))
    throw std::runtime_error("Bad conversion of data to string!");

  if (noperiod) // eliminate a period in case of 'double' numbers
  {
    std::string res = o.str(); // resulting string
    std::string::size_type pos = res.find('.');
    if (pos != std::string::npos)
      res.erase(pos, 1);
    return res;
  }

  return o.str();
}

//------------------------------------------------------------------------------
//
// expect, require and self checks
//
//------------------------------------------------------------------------------

#if defined(DEBUG_REC)
  #define expect(condition, message)  \
    if (!(condition))                 \
      requirement_fails(__FILE__,     \
                        __LINE__,     \
                        message)
#else
  // in release (or release-like) versions nothing happens
  #define expect(condition, message) { }
#endif // DEBUG_REC

#define require(condition, message) \
  if (!(condition))                 \
    requirement_fails(__FILE__,     \
                      __LINE__,     \
                      message)

/**
 * Throw an informative exception, if a requirement or an expectation fails.
 */
void requirement_fails(const char *file,
                       unsigned int line,
                       std::string message);

double get_wall_time();

std::string file_name(const std::string &path);

std::string file_path(const std::string &path);

std::string file_stem(const std::string &path);

std::string file_extension(const std::string &path);

bool file_exists(const std::string &path);

std::string absolute_path(const std::string &rel_path);

bool is_big_endian();
std::string endianness();

/**
 @brief Test for presence of a command line argument

 Compares a specified string to the array of standard Unix commnand line
 arguments. If the string  matches the i'th argument, it returns
 i, otherwise returns 0.

 @author R. Gibson

 @param argc - from command line, number of command line arguments
 @param argv - pointer to array of command line arguments
 @param arg - pointer to string to test for
 @return i, if the string  matches the i'th argument, 0 otherwise.
 */
int argcheck(int argc, char **argv, const char *arg);

/**
 * @brief Add some space to the given string so it has the requires length
 */
std::string add_space(const std::string &str, int length);

#endif // UTILITIES_HPP
