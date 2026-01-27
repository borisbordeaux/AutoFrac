#include "app/layers/layerbcifs.h"
#include "app/layers/layereditfractal.h"
#include "app/layers/layermenu.h"
#include "core/application.h"

int main() {
    Core::ApplicationSpecification appSpec;
    appSpec.name = "AutoFrac";
    appSpec.windowSpec.width = 1920;
    appSpec.windowSpec.height = 1080;

    Core::Application application(appSpec);
    application.pushCacheLayer<LayerEditFractal>();
    application.pushLayer<LayerBcifs>(dynamic_cast<LayerEditFractal*>(application.cacheLayer()));
    application.pushLayer<LayerMenu>();
    application.run();
}
