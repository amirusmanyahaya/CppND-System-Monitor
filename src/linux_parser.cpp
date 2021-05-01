#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value;
  string line;
  float memory_utilization{};
  std::ifstream file(kProcDirectory + kMeminfoFilename);
  if(file.is_open()){
    while(getline(file,line))
    {
      std::istringstream is(line);
      is >> key >> value;
      if(key == "MemTotal:" || key == "MemFree:"){
        memory_utilization += stof(value);
      }
    }
    
  }
  return memory_utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream file{kProcDirectory+kUptimeFilename};
  long up_time{0};
  long up_time_processor{0};
  if(file.is_open()){
    file >> up_time >> up_time_processor;
  }
  return up_time;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
	std::ifstream file{kProcDirectory+kUptimeFilename};
  	string cpu, user ,nice ,system ,idle,iowait;
    string line;
  	long sum{0};
  	if(file.is_open()){
    	getline(file,line);
      std::istringstream is(line);
      is >> cpu >> user >> nice >> system >> idle >> iowait;
      sum = stol(user) + stol(nice) + stol(system) + stol(idle) + stol(iowait);
    }
  return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
	long active_jiffies{0};
  std::ifstream file(kProcDirectory+to_string(pid)+kStatFilename);
  string component;
  if(file.is_open()){
    int i{0};
    while(file >> component){
      // break when the desired inputs are read
      if(i > 16){
        break;
      }
      // reads utime, stime, cutime and cstime
      if(i >= 13 && i <= 16){
        active_jiffies += stol(component);
      }
      ++i;
    }
  }
  return active_jiffies;
}
  

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
	std::ifstream file{kProcDirectory+kUptimeFilename};
  	string cpu, user ,nice ,system;
    string line;
  	long active_jiffies{0};
  	if(file.is_open()){
    	getline(file,line);
      std::istringstream is(line);
      is >> cpu >> user >> nice >> system;
      active_jiffies = stol(user) + stol(nice) + stol(system);
    }
  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::ifstream file{kProcDirectory+kUptimeFilename};
  	string cpu, user ,nice ,system ,idle,iowait;
    string line;
  	long idle_jiffies{0};
  	if(file.is_open()){
    	getline(file,line);
      std::istringstream is(line);
      is >> cpu >> user >> nice >> system >> idle >> iowait;
      idle_jiffies = stol(idle) + stol(iowait);
    }
  return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream file(kProcDirectory+kStatFilename);
  string line;
  vector<string> v  = {};
  if(file.is_open()){
    getline(file,line);
    std::istringstream is{line};
    std::istream_iterator<string> begin{is};
    std::istream_iterator<string> end;
    std::copy(begin,end,std::back_inserter(v));
  }
  return v;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
