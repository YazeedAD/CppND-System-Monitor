#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

enum stat_enum {
  user_ = 1,
  nice_ = 2,
  system_ = 3,
  idle_ = 4,
  iowait_ = 5,
  irq_ = 6,
  softirq_ = 7,
  steal_ = 8
};
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  struct Mem {
    string Total = "MemTotal:";
    string Free = "MemFree:";
    string Buffers = "Buffers:";
    string Cached = "Cached:";
  } Mem;

  float MemTotal, MemFree, MemBuffers, MemCached, MemUsed = 0;
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == Mem.Total)
          MemTotal = stof(value);
        else if (key == Mem.Free)
          MemFree = stof(value);
        else if (key == Mem.Buffers)
          MemBuffers = stof(value);
        else if (key == Mem.Cached) {
          MemCached = stof(value);
          break;
        }
      }
    }
    stream.close();
    // Doc
    MemUsed = (MemTotal - MemFree - MemBuffers - MemCached) / MemTotal;
    return MemUsed;
  } else
    throw std::exception();
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;

    stream.close();

    return std::stol(uptime);

  } else
    throw std::exception();
}



// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line;
  string value;
  long jeffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value == "cpu")
        continue;
      jeffies += std::stol(value);
    }
    stream.close();

    return jeffies;

  } else
    throw std::exception();
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  string value;
  int parser_counter = 1;
  long active=0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value == "cpu")
        continue;

      switch (parser_counter) {
        case user_:
          active = std::stol(value);
          break;
        case nice_:
          active += std::stol(value);
          break;
        case system_:
          active += std::stol(value);
          break;
        case irq_:
          active += std::stol(value);
          break;
        case softirq_:
          active += std::stol(value);
          break;
        case steal_:
          active += std::stol(value);
          stream.close();
          return active;
        default:
          break;
      }
      parser_counter++;
    }
    return active;

  }
  else
    throw std::exception();
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  string value;
  int parser_counter = 1;
  long idle;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value == "cpu") continue;

      if (parser_counter == idle_)
        idle = std::stol(value);
      else if (parser_counter == iowait_) {
        idle += std::stol(value);
        break;
      }

      parser_counter++;
    }
    stream.close();

    return idle;

  } else
    throw std::exception();
}


// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string value;
  string key;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> value >> key;
      if (value != "procs_running")
        continue;

      stream.close();
      return std::stoi(key);
    }
  } else
    throw std::exception();
}


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid ) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE:  once you define the function
string LinuxParser::Ram(int pid ) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE:  once you define the function
string LinuxParser::Uid(int pid ) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE:  once you define the function
string LinuxParser::User(int pid ) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE:  once you define the function
long LinuxParser::UpTime(int pid ) { return 0; }