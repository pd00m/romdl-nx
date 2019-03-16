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


    // Main loop
    while (appletMainLoop()) 
    {
        
      // Scan all the inputs. This should be done once for each frame
      hidScanInput();

      // hidKeysDown returns information about which buttons have been
      // just pressed in this frame compared to the previous one
      u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
      

      if (kDown & KEY_PLUS)
      {
        break; // break in order to return to hbmenu
      }
/*          
      if (kDown & KEY_DUP)
      {
        consoleClear();
        if(entry > 0){
          --entry;
        }
        //std::cout << "Game: " << romList[entry].name << std::endl;
        //std::cout << "File: "<< romList[entry].reflink << std::endl;
      }
          
      if (kDown & KEY_DDOWN)
      {
        consoleClear();
        if(romList.size() > entry){
          ++entry;
        }
        //std::cout << "Game: " << romList[entry].name << std::endl;
        //std::cout << "File: "<< romList[entry].reflink << std::endl;
      }
*/          
      // Your code goes here

      // Update the console, sending a new frame to the display
      consoleUpdate(NULL);
    }

    
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}