#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
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
    MemUsed = (MemTotal - MemFree - MemBuffers - MemCached) / MemTotal;
    return MemUsed;
  } else
    throw std::exception();
}

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

long LinuxParser::Jiffies() {
  string line;
  string value;
  long jeffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value == "cpu") continue;
      jeffies += std::stol(value);
    }
    stream.close();

    return jeffies;

  } else
    throw std::exception();
  return 0;
}

long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() {
  string line;
  string value;
  int parser_counter = 1;
  long active = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value == "cpu") continue;

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

  } else
    throw std::exception();
}

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
float LinuxParser::CpuUtilization(int pid) {
  string pid_dir = to_string(pid);
  string line;
  string value;
  long utime;
  long stime;
  long cutime;
  long cstime;
  long starttime;
  long hertez;
  long total_time;
  long seconds;
  float cpu_usage;
  long upt = LinuxParser::UpTime();
  int parser_counter = 1;
  std::ifstream stream(kProcDirectory + pid_dir + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value && parser_counter <= starttime_) {
      switch (parser_counter++) {
        case utime_:
          utime = std::stol(value);
          break;
        case stime_:
          stime = std::stol(value);
          break;
        case cutime_:
          cutime = std::stol(value);
          break;
        case cstime_:
          cstime = std::stol(value);
          break;
        case starttime_:
          starttime = std::stol(value);
          break;
        default:
          break;
      }
    }
    stream.close();
    hertez = sysconf(_SC_CLK_TCK);
    total_time = utime + stime + cutime + cstime;
    total_time= total_time/hertez;

    seconds = (upt) - (starttime/hertez);
    cpu_usage = (float)total_time/(float)seconds;
    if (cpu_usage > 1)
      return 0;
    return cpu_usage;
  } else
    throw std::exception();
  return 0;
}

int LinuxParser::TotalProcesses() { return 0; }

int LinuxParser::RunningProcesses() {
  string line;
  string value;
  string key;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> value >> key;
      if (value != "procs_running") continue;

      stream.close();
      return std::stoi(key);
    }
  } else
    throw std::exception();

  return 0;
}


string LinuxParser::Command(int pid) {
  string command;
  string pid_dir = to_string(pid);
  std::ifstream stream(kProcDirectory + pid_dir + kCmdlineFilename);

  if (stream.is_open()) {
    getline(stream, command);
    stream.close();

    return command;
  } else
    throw std::exception();
}


string LinuxParser::Ram(int pid) {
  string pid_dir = to_string(pid);
  long mem_MB;
  string line;
  string value;
  string key;
  std::ifstream stream(kProcDirectory + pid_dir + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> value >> key;
      if (value != "VmSize:") continue;
      stream.close();
      mem_MB = std::stol(key) / 1000;
      return to_string(mem_MB);
    }
  } else
    throw std::exception();
  return string("!");
}


string LinuxParser::Uid(int pid) {
  string pid_dir = to_string(pid);
  string line;
  string value;
  string key;
  std::ifstream stream(kProcDirectory + pid_dir + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> value >> key;
      if (value != "Uid:") continue;

      stream.close();
      return key;
    }
  } else
    throw std::exception();
  return string("!");
}


string LinuxParser::User(int pid) {
  string line;
  string token;
  std::ifstream stream(kPasswordPath);
  string name;
  int parser_counter = 1;

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      parser_counter = 1;
      while (getline(linestream, token, ':') && parser_counter <= 3) {
        std::istringstream tokenStream(token);
        switch (parser_counter++) {
          case name_:
            name = token;
            break;
          case password_:
            break;
          case uid_:
            if (token == Uid(pid)) {
              stream.close();
              return name;
            }
            break;
          default:
            break;
        }
      }
    }
  } else
    throw std::exception();
  return string("!");
}

long LinuxParser::UpTime(int pid) {
  string pid_dir = to_string(pid);
  string line;
  string value;
  int parser_counter = 1;
  std::ifstream stream(kProcDirectory + pid_dir + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (parser_counter == utime_) {
        stream.close();
        return std::stol(value) / sysconf(_SC_CLK_TCK);
      }

      parser_counter++;
    }

  } else
    throw std::exception();
  return 0;
}