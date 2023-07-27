/**
 *  \file apps/cppsas7bdat-ci.cpp
 *
 *  \brief Main file
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/version.hpp>
#include <cppsas7bdat/reader.hpp>
#include <cppsas7bdat/source/ifstream.hpp>
#include <cppsas7bdat/sink/print.hpp>
#include <cppsas7bdat/sink/csv.hpp>


void process_print(const std::string& _filename, long _n)
{
  cppsas7bdat::Reader reader(cppsas7bdat::datasource::ifstream(_filename.c_str()), cppsas7bdat::datasink::print(std::cout));
  while(_n != 0 && reader.read_row()) {
    if(_n > 0) --_n;
  }
}

std::string get_csv_filename(const std::string& _filename)
{
  auto ipos = _filename.rfind('.');
  if(ipos != _filename.npos) {
    return _filename.substr(0, ipos+1) + "csv";
  }
  return _filename + "csv";
}

void sas7bdat_to_csv(const std::string& _filename)
{
  const auto csv_filename = get_csv_filename(_filename);
  std::cout << _filename << std::endl;
  std::cout << csv_filename << std::endl;
	std::ofstream csv_os (csv_filename, std::ofstream::out);
	cppsas7bdat::Reader(
	  cppsas7bdat::datasource::ifstream(_filename.c_str()), cppsas7bdat::datasink::csv(csv_os)).read_all();
}

int main(const int argc, char* argv[])
{

  if(argc == 4 && (!std::string(argv[1]).compare("print"))) {
    const long int n = std::stoi(argv[2]);
    const std::string file = argv[3];
    process_print(file, n);
  } else if(argc == 3 && (!std::string(argv[1]).compare("convert_csv"))) {
    const std::string file = argv[2];
    sas7bdat_to_csv(file);
  }else{
    std::cout << "Try Again..." << std::endl;
    std::cout << "Options => {print <n_lines> <file_path>, convert_csv <file_path>}" << std::endl;
  }
  return 0;
}
