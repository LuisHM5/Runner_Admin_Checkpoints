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
  static String readFile(fs::FS &fs, const char *path)
  {
    // cout << "Reading file: " << path << endl;
    // Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
      Serial.println("- failed to open file for reading");
      return String();
    }

    String fileContent = file.readString();
    file.close();

    return fileContent;
  }

  static void writeFile(fs::FS &fs, const char *path, const char *message)
  {
    // Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
      Serial.println("- failed to open file for writing");
      return;
    }

    if (file.print(message))
    {
      Serial.println("- file written");
    }
    else
    {
      Serial.println("- write failed");
    }

    file.close();
  }
};