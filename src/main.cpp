#include "demo.h"

void error(const std::string& message) {
    printf("%s\n", message.c_str());
    throw std::runtime_error(message);
}

int main() {
    Vk_Demo demo;
    demo.run();
    return 0;
}
