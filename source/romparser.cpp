#include "romparser.hpp"
#include "httploader.hpp"


RomParser::RomParser(std::string webAddress, std::string sys)
{

  HttpLoader httploader(webAddress, sys);
  httploader.Get(_rawOutput);
  
  GumboOutput* outputNode = gumbo_parse(_rawOutput.c_str());
  SearchForRomLinks(outputNode->root, &_romList, webAddress);
  gumbo_destroy_output(&kGumboDefaultOptions, outputNode);
   std::cout << "finished searching :)" << std::endl;
  
}

/* search for links with href tag and known file extension */
void RomParser::SearchForRomLinks(GumboNode* node, std::vector<romEntry_t>* list, std::string webAddress) 
{
  if (node->type != GUMBO_NODE_ELEMENT) 
  {
    // current node is not an HTML element
    return;
  }
  GumboAttribute* href;
  
  if (node->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) 
  {
      GumboNode* title = static_cast<GumboNode*>(node->v.element.children.data[0]);
      assert(title->type == GUMBO_NODE_TEXT || title->type == GUMBO_NODE_WHITESPACE);
      std::string tmpHref(href->value);
      std::string tmpTitle(title->v.text.text);

      if (tmpHref.find(".zip") != std::string::npos){
        romEntry_t entry = {webAddress + tmpHref, tmpTitle};       
        list->push_back(entry);
      }
      return;
  }
  
  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) 
  {  
    SearchForRomLinks(static_cast<GumboNode*>(children->data[i]), *&list, webAddress);
  }
}

/* search for specific game name in parsed rom list */
std::vector<romEntry_t> RomParser::SearchRom(std::string name)
{
  std::vector<romEntry_t> searchVector;

  for(std::vector<romEntry_t>::iterator it = _romList.begin(); it != _romList.end(); ++it) {
    if (it->name.find(name) != std::string::npos){
      searchVector.push_back(*it);
    }
  }
  return searchVector;
}