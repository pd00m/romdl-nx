/* HTTP loader class - read html files using HTTP methods/curl */
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <assert.h>

#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

class HttpLoader {
  private:
        std::string _webAddress;
        std::string _sys;
        
    public:
        HttpLoader(std::string, std::string);
        static size_t MemoryCallback(void*, size_t , size_t , void*);
        int Get(std::string&);
};