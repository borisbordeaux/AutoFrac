#include "app/layers/layermenu.h"
#include "core/application.h"
#include "app/layers/voidlayer.h"

int main() {

    Core::ApplicationSpecification appSpec;
    appSpec.name = "AutoFrac";
    appSpec.windowSpec.width = 1920;
    appSpec.windowSpec.height = 1080;

    Core::Application application(appSpec);
    application.pushLayer<VoidLayer>();
    application.pushLayer<LayerMenu>();
    application.run();
}