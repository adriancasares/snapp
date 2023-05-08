//
// Created by Adrian Casares on 5/7/23.
//

#include <iostream>
#include "../include/package/plugin.h"

class TestPlugin : public SnappExternal::Plugin {
  public:
    SnappExternal::DataValue run(const std::string& name, const std::vector<SnappExternal::DataValue>& args) override {
        std::cout << "TestPlugin::run called" << std::endl;
        return 1;
    }
};

extern "C" SnappExternal::Plugin* createPlugin() {
  return new TestPlugin();
}