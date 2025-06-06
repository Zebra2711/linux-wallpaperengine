#pragma once

#include <glm/vec4.hpp>
#include <map>
#include <string>

#include "WallpaperEngine/Application/CApplicationContext.h"
#include "WallpaperEngine/Render/Drivers/Detectors/CFullScreenDetector.h"

using namespace WallpaperEngine::Application;

namespace WallpaperEngine::Application {
class CApplicationContext;
}

namespace WallpaperEngine::Render::Drivers {
class CVideoDriver;

namespace Detectors {
class CFullScreenDetector;
}

namespace Output {
class COutputViewport;

class COutput {
  public:
    COutput (CApplicationContext& context, CVideoDriver& driver);
    virtual ~COutput () = default;

    virtual void reset () = 0;

    int getFullWidth () const;
    int getFullHeight () const;

    virtual bool renderVFlip () const = 0;
    virtual bool renderMultiple () const = 0;
    virtual bool haveImageBuffer () const = 0;
    const std::map<std::string, COutputViewport*>& getViewports () const;
    virtual void* getImageBuffer () const = 0;
    virtual uint32_t getImageBufferSize () const = 0;
    virtual void updateRender () const = 0;

  protected:
    mutable int m_fullWidth = 0;
    mutable int m_fullHeight = 0;
    mutable std::map<std::string, COutputViewport*> m_viewports = {};
    CApplicationContext& m_context;
    CVideoDriver& m_driver;
};
} // namespace Output
} // namespace WallpaperEngine::Render::Drivers