//
// Created by Adrian Casares on 5/7/23.
//

#ifndef SNAPP_INCLUDE_PACKAGE_PLUGINLOADER_H_
#define SNAPP_INCLUDE_PACKAGE_PLUGINLOADER_H_

#include <string>
#include "package/plugin.h"

namespace SnappExternal {

    class PluginLoader {
    public:
        static Plugin* loadPlugin(const std::string& path);


    };

}
#endif //SNAPP_INCLUDE_PACKAGE_PLUGINLOADER_H_
