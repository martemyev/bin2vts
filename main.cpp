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



int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cout << "\nUsage (options in brackets are optional):\n" << argv[0]
              << " input_file n1 [h] [output_file]\n";
    std::cout << "\nwhere\ninput_file = input binary file\nn1 = number of "
                 "columns of an array from the binary file\nh = size of a cell "
                 "of a structured grid (1 m by default)\noutput_file = VTS file"
                 " named stem(input_file)+'.vts' by default\n";
    std::cout << std::endl;
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
  double *values = nullptr;
  read_binary(binfile, n_cols, n_rows, values);
  if (n_rows <= 0)
    throw std::runtime_error("n_rows in the file (" + d2s(n_rows) + ") should "
                             "be >0");

  write_vts(vtsfile, n_cols, n_rows, h, values);

  delete[] values;

  return 0;
}
