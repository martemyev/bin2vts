#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "utilities.hpp"

/**
 * Depending on the compiler, this new nullptr keyword may or may not be
 * supported.
 */
#ifndef nullptr
  #define nullptr NULL
#endif

using namespace std;


int main(int argc, char **argv)
{
  if (argc == 1 || argcheck(argc, argv, "-h") || argcheck(argc, argv, "--help"))
  {
    cout << "\nUsage (options in brackets are optional):\n" << argv[0] <<
            " -i input_file"
            " -dim dimension"
            " -nx n_values_x "
            "[-ny n_values_y"
            " -nz n_values_z"
            " -sx cell_size_x"
            " -sy cell_size_y"
            " -sz cell_size_z"
            " -o output_file]\n"
            "\nwhere\n"
            "input_file = input binary file\n"
            "dimension = 2 or 3\n"
            "n_values_x = number of values along x-direction\n"
            "n_values_y = number of values along y-direction\n"
            "n_values_z = number of values along z-direction (for 3D or for 2D in XZ plane)\n"
            "cell_size_x = size of a cell of a structured grid along x-direction (1 m by default)\n"
            "cell_size_y = size of a cell of a structured grid along y-direction (1 m by default)\n"
            "cell_size_z = size of a cell of a structured grid along z-direction (1 m by default)\n"
            "output_file = VTS file named stem(input_file)+'.vts' by default\n";
    cout << endl;
    return 1;
  }

  string binfile = "";
  int dimension = 0;
  int nx = 0, ny = 0, nz = 0;
  double hx = 1.0, hy = 1.0, hz = 1.0;
  string vtsfile = "";

  try
  {
    int pos;
    if (pos = argcheck(argc, argv, "-i"))
      binfile = string(argv[pos+1]);
    else throw runtime_error("Input binary file is not provided");

    if (pos = argcheck(argc, argv, "-dim"))
      dimension = atoi(argv[pos+1]);
    else throw runtime_error("Dimension is not provided");
    if (dimension != 2 && dimension != 3)
      throw runtime_error("Dimension (" + d2s(dimension) + ") must be 2 or 3");

    if (pos = argcheck(argc, argv, "-nx"))
      nx = atoi(argv[pos+1]);
    else throw runtime_error("n_values_x (-nx) is not provided");
    if (nx <= 0)
      throw runtime_error("n_values_x (" + d2s(nx) + ") must be >0");

    if (pos = argcheck(argc, argv, "-ny"))
    {
      ny = atoi(argv[pos+1]);
      if (ny <= 0)
        throw runtime_error("n_values_y (" + d2s(ny) + ") must be >0");
    }

    if (pos = argcheck(argc, argv, "-nz"))
    {
      nz = atoi(argv[pos+1]);
      if (nz <= 0)
        throw runtime_error("n_values_z (" + d2s(nz) + ") must be >0");
    }

    if (ny == 0 && nz == 0)
      throw runtime_error("n_values_y (-ny) or n_values_z (-nz) must be provided");

    if (dimension == 3 && (ny == 0 || nz == 0))
      throw runtime_error("For 3D n_values_y (-ny) and n_values_z (-nz) must be provided");

    if (dimension == 2 && (ny != 0 && nz != 0))
      throw runtime_error("For 2D n_values_y (-ny) or n_values_z (-nz) (not both) must be provided");

    if (pos = argcheck(argc, argv, "-sx"))
      hx = atof(argv[pos+1]);

    if (pos = argcheck(argc, argv, "-sy"))
      hy = atof(argv[pos+1]);

    if (pos = argcheck(argc, argv, "-sz"))
      hz = atof(argv[pos+1]);

    if (pos = argcheck(argc, argv, "-o"))
      vtsfile = string(argv[pos+1]);
    else
      vtsfile = file_stem(binfile) + ".vts";

    int n_values;
    if (dimension == 3) n_values = nx*ny*nz;
    else n_values = nx * (ny ? ny : nz);

    double *values = new double[n_values];

    read_binary(binfile, n_values, values);

    if (dimension == 2)
    {
      if (ny)
        write_vts_2D_XY(vtsfile, nx, ny, hx, hy, values);
      else
        write_vts_2D_XZ(vtsfile, nx, nz, hx, hz, values);
    }
    else
      write_vts_3D(vtsfile, nx, ny, nz, hx, hy, hz, values);

    delete[] values;
  }
  catch (const std::exception& e)
  {
    cout << "\n" << e.what() << "\n" << endl;
  }
  catch (...)
  {
    cout << "\nUnknown exception!\n" << endl;
  }
}
