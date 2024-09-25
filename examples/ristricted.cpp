#include <iostream>
#include <string>
#include "structopt.hpp"

struct Cli {
 struct New : structopt::sub_command {
  std::string projectName;
 };

 // sub commands
 New new_;
};

STRUCTOPT(Cli::New, projectName);
STRUCTOPT(Cli, new_);

int main(int argc, char** argv) {
 try {
 const auto cli = structopt::app("main").parse<Cli>(argc, argv);

 // Print out parsed arguments:
  std::cout << "New project name: " << cli.new_.projectName << "\n";

 } catch (const structopt::exception &e) {
  std::cerr << e.what() << std::endl;
  std::cerr << e.help();
 }
 return 0;
}