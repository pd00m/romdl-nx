// Include the most common headers from the C standard library
//#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string.h>

#include <assert.h>
#include <sys/stat.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include <curl/curl.h>
#include <sys/socket.h>
#include <libgumbo-parser/gumbo.h>

int download(const char *url);

static void search_for_rom_links(GumboNode* node) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    std::cout << href->value << " [] ";
  }
  
  
  /*GumboNode* link_text = (GumboNode *)node->v.element.children.data[1]];
      assert(link_text->type == GUMBO_NODE_TEXT || link_text->type == GUMBO_NODE_WHITESPACE);
      std::cout << link_text->v.text.text << std::endl;
  */
  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    GumboNode* child = (GumboNode *)children->data[i];
    const char* title;
    if (child->type == GUMBO_NODE_ELEMENT &&
        child->v.element.tag == GUMBO_TAG_A) {
      if (child->v.element.children.length != 1) {
        printf("<empty>");
      }
      GumboNode* title_text = (GumboNode *)child->v.element.children.data[0];
      assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
      //std::cout << title_text->v.text.text << endl;
      
    }
    
    search_for_rom_links(static_cast<GumboNode*>(children->data[i]));
  }
}

static void search_for_links(GumboNode* node) 
{
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    std::cout << href->value << std::endl;
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]));
  }
}

static void read_file(FILE* fp, char** output, int* length) 
{
  struct stat filestats;
  int fd = fileno(fp);
  fstat(fd, &filestats);
  *length = filestats.st_size;
  *output = (char *)malloc(*length + 1);
  int start = 0;
  int bytes_read;
  while ((bytes_read = fread(*output + start, 1, *length - start, fp))) {
    start += bytes_read;
  }
}

static const char* find_title(const GumboNode* root) 
{
  assert(root->type == GUMBO_NODE_ELEMENT);
  assert(root->v.element.children.length >= 2);

  const GumboVector* root_children = &root->v.element.children;
  GumboNode* head = NULL;
  for (int i = 0; i < root_children->length; ++i) {
    GumboNode* child = (GumboNode *)root_children->data[i];
    if (child->type == GUMBO_NODE_ELEMENT &&
        child->v.element.tag == GUMBO_TAG_HEAD) {
      head = child;
      break;
    }
  }
  assert(head != NULL);
  GumboVector* head_children = &head->v.element.children;
  for (int i = 0; i < head_children->length; ++i) {
    
    GumboNode* child = (GumboNode *)head_children->data[i];
    if (child->type == GUMBO_NODE_ELEMENT &&
        child->v.element.tag == GUMBO_TAG_TITLE) {
      if (child->v.element.children.length != 1) {
        return "<empty title>";
      }
      GumboNode* title_text = (GumboNode *)child->v.element.children.data[0];
      assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
      return title_text->v.text.text;
    }
  }
  return "<no title found>";
}



// Main program entrypoint
int main(int argc, char* argv[])
{
    consoleInit(NULL);
    socketInitializeDefault();

    Result rc = romfsInit();
    FILE* fp; 
    const char* path = "romfs:/folder/file.html";
    if (R_FAILED(rc))
    {
        printf("romfsInit: %08X\n", rc);
        consoleExit(NULL);
        return -1;
    }   
    else
    {
        printf("romfs Init Successful!\n");
        //fp = fopen(path, "r");
        
    
    }

    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in) 
    {
      std::cout << "File " << path << " not found!\n";
      exit(EXIT_FAILURE);
    }

    //download("http://www.thomas-bayer.com/sqlrest/");


    //printf("starting to parse...\n \n");
    



  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput* output = gumbo_parse(contents.c_str());
  search_for_rom_links(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);



/*
    char* input;
    int input_length;
    read_file(fp, &input, &input_length);
    GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions, input, input_length);
    const char* title = find_title(output->root);
    printf("%s\n", title);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    free(input);

*/



    // Main loop
    while (appletMainLoop()) 
    {
        
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown)
            break; // break in order to return to hbmenu

        // Your code goes here

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    //fclose(fp);
    romfsExit();
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}

int download(const char *url)
{
    CURL *curl = curl_easy_init();
    CURLcode res;

    
    if(curl) 
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url);   
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 			
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 

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
    return 0;
}
