#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>



/**
 * A type of data assumed in binary files.
 */
typedef float REAL;



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




int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cout << "\nUsage:\n" << argv[0] << " input_file.bin "
                 "n1=<number_of_columns> [h=<size_of_cell>] "
                 "[output_file.vts]\n" << std::endl;
    return 1;
  }

  const std::string binfile = argv[1];
  const int n_cols = atoi(argv[2]);
  double h = 1.0;
  if (argc > 3)
    h = atof(argv[3]);
  std::string vtsfile = file_stem(binfile) + ".vts";
  if (argc > 4)
    vtsfile = std::string(argv[4]);

  if (n_cols <= 0)
    throw std::runtime_error("n1 value (" + d2s(n_cols) + ") should be >0");
  if (h < 0)
    throw std::runtime_error("h value (" + d2s(h) + ") should be >0");

  int n_rows;
  double *values;
  read_binary(binfile, n_cols, n_rows, values);
  if (n_rows <= 0)
    throw std::runtime_error("n_rows in the file (" + d2s(n_rows) + ") should "
                             "be >0");

  write_vts(vtsfile, n_cols, n_rows, h, values);

  delete[] values;

  return 0;
}
