// Include the most common headers from the C standard library
//#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <thread>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include "romparser.hpp"
#include <sys/socket.h>


// Main program entrypoint
int main(int argc, char* argv[])
{
  consoleInit(NULL);

  std::cout << "nx-romDl v0.1" << std::endl;
  std::cout << "parsing NES rom list..." << std::endl;
  consoleUpdate(NULL);

  socketInitializeDefault(); // Only works from here
  RomParser romParserNes("http://thesecretromsite/", "NES");

  consoleUpdate(NULL);

  while (appletMainLoop()) 
  {
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);     

    if (kDown & KEY_PLUS)
    {
      break;
    }
    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}