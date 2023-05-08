#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "package/pluginLoader.h"
namespace Snapp::Native {

    FunctionValue createUseExternalFunction() {
        FunctionValue useExternalFunction;

        useExternalFunction.addOverload({
                                          DataType::Void,
                                          {},
                                          [](const std::vector<DataValue>& args) {
                                              std::cout << "useExternalFunction called" << std::endl;

                                              SnappExternal::Plugin* plugin = SnappExternal::PluginLoader::loadPlugin(std::get<std::string>(args[0]));

//                                              plugin->run(std::get<std::string>(args[1]),
//                                                         {
//                                                                    1
//                                                            });

                                              return std::nullopt;
                                          }
                                  });

        useExternalFunction.setAnyParameters(true);

        return useExternalFunction;
    }

}