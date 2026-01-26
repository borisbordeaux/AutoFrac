#include "app/layers/layerbcifs.h"
#include "app/layers/layermenu.h"
#include "app/layers/layertexture.h"
#include "core/application.h"

int main() {
    Core::ApplicationSpecification appSpec;
    appSpec.name = "AutoFrac";
    appSpec.windowSpec.width = 1920;
    appSpec.windowSpec.height = 1080;

    Core::Application application(appSpec);
    application.pushLayer<LayerBcifs>();
    application.pushLayer<LayerMenu>();
    application.pushCacheLayer<LayerTexture>();
    application.run();
}
