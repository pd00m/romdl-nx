#include "httploader.hpp"


HttpLoader::HttpLoader(std::string webAddress, std::string sys){
    // socketInitializeDefault(); ???
    _webAddress = webAddress;
    _sys = sys;
}

size_t HttpLoader::MemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = static_cast<char*>(realloc(mem->memory, mem->size + realsize + 1));
  if(ptr == NULL) {
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

int HttpLoader::Get(std::string &rawOutput)
{
  const char *url = _webAddress.c_str();
  const char *sys = _sys.c_str();

  CURL *curl = curl_easy_init();
  CURLcode res;

  struct MemoryStruct chunk;
 
  chunk.memory = static_cast<char*>(malloc(1));  
  chunk.size = 0;

  if(curl) 
  {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(curl, CURLOPT_URL, url);   
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 			
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, MemoryCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&chunk)); 

      res = curl_easy_perform(curl);

      curl_easy_cleanup(curl);
  }
  else{
      return -1;
  }

  if(res != CURLE_OK)
  {
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
          return -1;
  }
  else
  {
    std::string htmlStr(chunk.memory);
    rawOutput = htmlStr;
  }
  return 0;
}