#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <stdexcept>
#include <sstream>

/**
 * Convert the data of any type which has oveloaded operator '<<' to string
 * @return data in string format
 */
template <typename T>
inline std::string d2s(T data)
{
  std::ostringstream o;
  if (!(o << data))
    throw std::runtime_error("Bad conversion of data to string!");
  return o.str();
}

/**
 * @brief Test for presence of a command line argument
 * Compares a specified string to the array of standard Unix commnand line
 * arguments. If the string  matches the i'th argument, it returns i, otherwise
 * returns 0.
 * @author R. Gibson
 * @param argc - from command line, number of command line arguments
 * @param argv - pointer to array of command line arguments
 * @param arg - pointer to string to test for
 * @return i, if the string  matches the i'th argument, 0 otherwise.
 */
int argcheck(int argc, char **argv, const char *arg);

/**
 * Get (extract) a file name from the given path.
 * @param path - a name of a file under interest including the path
 * @return a string representing a name of the file. For example:
 * @verbatim
   file_name("/home/user/file.dat") = "file.dat"
 * @endverbatim
 */
std::string file_name(const std::string &path);

/**
 * Extract a stem from a filename with a path.
 * @param path - a name of a file under interest including the path
 * @return a string which represents the name of the file without an extension -
 * only a stem of the file. For example:
 * @verbatim
   stem("/home/user/file.dat") = "file"
 * @endverbatim
 */
std::string file_stem(const std::string &path);

/**
 * Read an array of floating-point numbers in double precision from a binary
 * file, where the numbers are stored as REAL numbers (it may represent a single
 * or a double precision).
 * @param filename - name of the binary file
 * @param n_cols - number of columns in the binary file (it represents an array
 * of values)
 * @param n_rows - based on the size of the binary file, size of a single value
 * and the number of columns, the number of rows is computed
 * @param values - an array of values in double precision. The array is
 * allocated in this function, so it has to be freed on the caller's end.
 */
void read_binary(const std::string &filename, int n_cols, int &n_rows,
                 double* &values);

/**
 * Write a VTS file (it's a VTK format for structured grid).
 * @param filename - name of the .vts file
 * @param n_cols - number of columns in the array of values to be written (can
 * be considered as the number of cells of the structured grid in x-direction)
 * @param n_rows - number of rows in the array of values to be written (can
 * be considered as the number of cells of the structured grid in y-direction)
 * @param h - the structured grid is supposed to be square, and this is the cell
 * size
 * @param values - the array of values to be written in the file
 */
void write_vts(const std::string &filename, int n_cols, int n_rows, double h,
               double *values);

#endif // UTILITIES_HPP
