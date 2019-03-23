/* ROM parser calss - scrape roms from html input stream */
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <assert.h>

#include <libgumbo-parser/gumbo.h>

struct romEntry_t {
  std::string reflink;
  std::string name;
};


class RomParser{
    private:
        std::string _webAddress;
        std::string _sys;
        std::string _rawOutput;
        std::vector<romEntry_t> _romList;
        

        static void SearchForRomLinks(GumboNode*, std::vector<romEntry_t>*, std::string);
        
    public:
        std::vector<romEntry_t> RomList;

        RomParser(std::string, std::string);
        std::vector<romEntry_t> SearchRom(std::string);
        bool DownloadRom(romEntry_t);
};