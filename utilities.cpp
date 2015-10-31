#include "utilities.hpp"

#include <cstring>
#include <fstream>



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



void read_binary(const std::string &filename, int n_values, double *values)
{
  std::ifstream in(filename.c_str(), std::ios::binary);
  if (!in)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  in.seekg(0, in.end); // jump to the end of the file
  int length = in.tellg(); // total length of the file in bytes
  int size_value = length / n_values; // size (in bytes) of one value

  if (length % n_values != 0)
    throw std::runtime_error("The number of bytes in the file '" + filename +
                             "' is not divisible by the number of elements " +
                             d2s(n_values));

  in.seekg(0, in.beg); // jump to the beginning of the file

  if (size_value == sizeof(double))
  {
    in.read((char*)values, n_values*size_value); // read all at once

    if(n_values != static_cast<int>(in.gcount()))
      throw std::runtime_error("The number of successfully read elements is "
                               "different from the expected one");
  }
  else if (size_value == sizeof(float))
  {
    float val;
    for (int i = 0; i < n_values; ++i)  // read element-by-element
    {
      in.read((char*)&val, size_value); // read a 'float' value
      values[i] = val;                  // convert it to a 'double' value
    }
  }
  else
    throw std::runtime_error("Unknown size of an element (" + d2s(size_value) +
                             ") in bytes. Expected one is either sizeof(float) "
                             "= " + d2s(sizeof(float)) + ", or sizeof(double) "
                             "= " + d2s(sizeof(double)));
}



void write_vts_2D_XY(const std::string &filename, int nx, int ny, double h,
                     double *values)
{
  std::ofstream out(filename.c_str());
  if (!out)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  out << "<?xml version=\"1.0\"?>\n";
  out << "<VTKFile type=\"StructuredGrid\" version=\"0.1\">\n";
  out << "  <StructuredGrid WholeExtent=\"1 " << nx+1 << " 1 " << ny+1 << " 1 1\">\n";
  out << "    <Piece Extent=\"1 " << nx+1 << " 1 " << ny+1 << " 1 1\">\n";
  out << "      <CellData>\n";
  out << "        <DataArray type=\"Float64\" Name=\"data\" format=\"ascii\" NumberOfComponents=\"1\">\n";

  for (int i = 0; i < ny; ++i)
    for (int j = 0; j < nx; ++j)
      out << values[i*nx + j] << " ";

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </CellData>\n";
  out << "      <Points>\n";
  out << "        <DataArray type=\"Float64\" format=\"ascii\" NumberOfComponents=\"3\">\n";

  for (int i = 0; i < ny+1; ++i)
  {
    const double y = i*h;
    for (int j = 0; j < nx+1; ++j)
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



void write_vts_2D_XZ(const std::string &filename, int nx, int nz, double h,
                     double *values)
{
  std::ofstream out(filename.c_str());
  if (!out)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  out << "<?xml version=\"1.0\"?>\n";
  out << "<VTKFile type=\"StructuredGrid\" version=\"0.1\">\n";
  out << "  <StructuredGrid WholeExtent=\"1 " << nx+1 << " 1 1 1 " << nz+1 << "\">\n";
  out << "    <Piece Extent=\"1 " << nx+1 << " 1 1 1 " << nz+1 << "\">\n";
  out << "      <CellData>\n";
  out << "        <DataArray type=\"Float64\" Name=\"data\" format=\"ascii\" NumberOfComponents=\"1\">\n";

  for (int i = 0; i < nz; ++i)
    for (int j = 0; j < nx; ++j)
      out << values[i*nx + j] << " ";

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </CellData>\n";
  out << "      <Points>\n";
  out << "        <DataArray type=\"Float64\" format=\"ascii\" NumberOfComponents=\"3\">\n";

  for (int i = 0; i < nz+1; ++i)
  {
    const double z = i*h;
    for (int j = 0; j < nx+1; ++j)
    {
      const double x = j*h;
      out << x << " 0.0 " << z << " ";
    }
  }

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </Points>\n";
  out << "    </Piece>\n";
  out << "  </StructuredGrid>\n";
  out << "</VTKFile>\n";
}



void write_vts_3D(const std::string &filename, int nx, int ny, int nz, double h,
                     double *values)
{
  std::ofstream out(filename.c_str());
  if (!out)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  out << "<?xml version=\"1.0\"?>\n";
  out << "<VTKFile type=\"StructuredGrid\" version=\"0.1\">\n";
  out << "  <StructuredGrid WholeExtent=\"1 " << nx+1 << " 1 " << ny+1 << " 1 " << nz+1 << "\">\n";
  out << "    <Piece Extent=\"1 " << nx+1 << " 1 " << ny+1 << " 1 " << nz+1 << "\">\n";
  out << "      <CellData>\n";
  out << "        <DataArray type=\"Float64\" Name=\"data\" format=\"ascii\" NumberOfComponents=\"1\">\n";

  for (int iz = 0; iz < nz; ++iz)
    for (int iy = 0; iy < ny; ++iy)
      for (int ix = 0; ix < nx; ++ix)
      out << values[iz*ny*nx + iy*nx + ix] << " ";

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </CellData>\n";
  out << "      <Points>\n";
  out << "        <DataArray type=\"Float64\" format=\"ascii\" NumberOfComponents=\"3\">\n";

  for (int iz = 0; iz < nz+1; ++iz)
  {
    const double z = iz*h;
    for (int iy = 0; iy < ny+1; ++iy)
    {
      const double y = iy*h;
      for (int ix = 0; ix < nx+1; ++ix)
      {
        const double x = ix*h;
        out << x << " " << y << " " << z << " ";
      }
    }
  }

  out << "\n";
  out << "        </DataArray>\n";
  out << "      </Points>\n";
  out << "    </Piece>\n";
  out << "  </StructuredGrid>\n";
  out << "</VTKFile>\n";
}
