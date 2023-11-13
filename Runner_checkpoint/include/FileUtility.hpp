#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
class FileUtility
{

public:
  // Function to read the content of a file
  static string readFile(const char *filename)
  {
    string content = "";

    // Create an input file stream
    std::ifstream file(filename);

    // Check if the file is open
    if (file.is_open())
    {
      // Read the file line by line
      std::string line;
      while (getline(file, line))
      {
        // Append each line to the content string
        content += line.c_str();
        content += "\n"; // Add a newline character if needed
      }

      // Close the file
      file.close();
    }
    else
    {
      cout << "Error opening file " << filename << endl;
    }

    return content;
  }
};