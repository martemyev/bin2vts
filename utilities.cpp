#include "utilities.hpp"

#include <cstring>
#include <fstream>

/**
 * A type of data assumed in binary files.
 */
typedef float REAL;



int argcheck(int argc, char **argv, const char *arg)
{
  for(int i = 1; i < argc; ++i)
  {
    // strcmp returns 0 if the strings are equal
    if(strcmp(argv[i], arg) == 0)
      return(i);
  }
  return 0;
}



std::string file_name(const std::string &path)
{
  if (path == "") return path;
  // extract a filename
  const std::string fname = path.substr(path.find_last_of('/') + 1);
  return fname;
}



std::string file_stem(const std::string &path)
{
  if (path == "") return path;
  // get a file name from the path
  const std::string fname = file_name(path);
  // extract a stem and return it
  return fname.substr(0, fname.find_last_of('.'));
}



void read_binary(const std::string &filename, int n_cols, int &n_rows,
                 double* &values)
{
  std::ifstream in(filename.c_str(), std::ios::binary);
  if (!in)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  in.seekg(0, in.end); // jump to the end of the file
  int length = in.tellg(); // total length of the file in bytes
  if (length % n_cols != 0)
    throw std::runtime_error(
        "The number of bytes in the file " + filename + " is not divisible by "
        "the number of columns " + d2s(n_cols));
  if (length % sizeof(REAL) != 0)
    throw std::runtime_error(
        "The number of bytes in the file " + filename + " is not divisible by "
        "the size of a single value " + d2s(sizeof(REAL)));

  n_rows = length / sizeof(REAL) / n_cols;
  if (n_rows <= 0)
    throw std::runtime_error(
        "The number of rows in the file " + filename + " (" + d2s(n_rows) +
        ") should be >0");

  in.seekg(0, in.beg); // jump to the beginning of the file

  delete[] values; // just in case
  values = new double[n_cols*n_rows];

  REAL val;
  for (int i = 0; i < n_cols*n_rows; ++i)  // read element-by-element
  {
    in.read(reinterpret_cast<char*>(&val), sizeof(REAL)); // read a value
    values[i] = val; // convert it to a 'double' value
  }
}



void write_vts(const std::string &filename, int n_cols, int n_rows, double h,
               double *values)
{
  std::ofstream out(filename.c_str());
  if (!out)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  out << "<?xml version=\"1.0\"?>\n";
  out << "<VTKFile type=\"StructuredGrid\" version=\"0.1\">\n";
  out << "  <StructuredGrid WholeExtent=\"1 " << n_cols+1 << " 1 " << n_rows+1 << " 1 1\">\n";
  out << "    <Piece Extent=\"1 " << n_cols+1 << " 1 " << n_rows+1 << " 1 1\">\n";
  out << "      <CellData>\n";
  out << "        <DataArray type=\"Float64\" Name=\"data\" format=\"ascii\" NumberOfComponents=\"1\">\n";

  for (int i = 0; i < n_rows; ++i)
    for (int j = 0; j < n_cols; ++j)
      out << values[i*n_cols + j] << " ";

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </CellData>\n";
  out << "      <Points>\n";
  out << "        <DataArray type=\"Float64\" format=\"ascii\" NumberOfComponents=\"3\">\n";

  for (int i = 0; i < n_rows+1; ++i)
  {
    const double y = i*h;
    for (int j = 0; j < n_cols+1; ++j)
    {
      const double x = j*h;
      out << x << " " << y << " 0.0 ";
    }
  }

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </Points>\n";
  out << "    </Piece>\n";
  out << "  </StructuredGrid>\n";
  out << "</VTKFile>\n";
}
