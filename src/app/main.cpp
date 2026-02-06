#include "app/layers/layerbcifs.h"
#include "app/layers/layereditfractal.h"
#include "app/layers/layermenu.h"
#include "core/application.h"

int main() {
    Core::ApplicationSpecification appSpec;
    appSpec.name = "AutoFrac";
    appSpec.initLayout = "../res/layout/imgui.ini";
    appSpec.windowSpec.width = 1920;
    appSpec.windowSpec.height = 1080;
    appSpec.windowSpec.samples = 16;

    Core::Application application(appSpec);
    application.pushCacheLayer<LayerEditFractal>();
    application.pushLayer<LayerBcifs>(dynamic_cast<LayerEditFractal*>(application.getCacheLayer(0)));
    application.pushLayer<LayerMenu>();
    application.run();
}
