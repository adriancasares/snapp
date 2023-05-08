//
// Created by Adrian Casares on 5/7/23.
//

#ifndef SNAPP_INCLUDE_PACKAGE_PACKAGEEXTERNALPLUGIN_H_
#define SNAPP_INCLUDE_PACKAGE_PACKAGEEXTERNALPLUGIN_H_

#include <variant>

namespace SnappExternal {

    using DataValue = std::variant<int, double, bool, std::string>;

    class Plugin {
    public:
        virtual ~Plugin() = default;
        virtual DataValue run(const std::string& name, const std::vector<DataValue>& args) = 0;
    };

  extern "C" Plugin* createPlugin();


}
#endif //SNAPP_INCLUDE_PACKAGE_PACKAGEEXTERNALPLUGIN_H_
