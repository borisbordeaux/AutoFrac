#ifndef AUTOFRAC_LAYERMENU_H
#define AUTOFRAC_LAYERMENU_H

#include <vector>
#include <string>
#include <functional>
#include "layer.h"

class LayerMenu : public Layer {
public:
    LayerMenu();
    ~LayerMenu() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;

    template<typename T>
    void registerLayer(std::string name) {
        m_layers.push_back(std::make_pair(std::move(name), []() { return new T(); }));
    }

private:
    Layer* m_currentLayer;
    std::vector<std::pair<std::string, std::function<Layer*()>>> m_layers;
};

#endif //AUTOFRAC_LAYERMENU_H
