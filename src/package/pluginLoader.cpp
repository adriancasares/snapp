//
// Created by Adrian Casares on 5/7/23.
//

#include "package/pluginLoader.h"
#include "package/plugin.h"
#include <iostream>
#include <dlfcn.h>
#include <fstream>

namespace SnappExternal {

    Plugin* PluginLoader::loadPlugin(const std::string& path) {
      typedef Plugin* (*createPluginFunc)();
      char* filename = new char[path.length() + 1];
        strcpy(filename, path.c_str());

      #ifdef _WIN32
      HMODULE handle = LoadLibraryA(filename);
      if (!handle) {
        return nullptr;
      }
      #else
      void* handle = dlopen(filename, RTLD_NOW);

      std::ifstream source(filename);

      if (!handle) {
        std::cout << "Error: " << dlerror() << std::endl;
        return nullptr;
      }
      #endif
      createPluginFunc create = reinterpret_cast<createPluginFunc>(dlsym(handle, "createPlugin"));
      if (!create) {
        return nullptr;
      }
      return create();
    }

}