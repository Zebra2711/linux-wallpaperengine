#pragma once

#include "WallpaperEngine/Render/Drivers/Output/COutput.h"
#include "WallpaperEngine/Input/CMouseInput.h"
#include "WallpaperEngine/Input/CInputContext.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace WallpaperEngine::Application {
class CWallpaperApplication;
}

namespace WallpaperEngine::Input {
class CInputContext;
class CWaylandMouseInput;
}

namespace WallpaperEngine::Render::Drivers {
namespace Detectors {
class CFullScreenDetector;
}

class CVideoDriver {
  public:
    explicit CVideoDriver (CWallpaperApplication& app, Input::CMouseInput& mouseInput);
    virtual ~CVideoDriver () = default;

    /**
     * @return The current output in use
     */
    [[nodiscard]] virtual Output::COutput& getOutput () = 0;
    /**
     * @return The time that has passed since the driver started
     */
    [[nodiscard]] virtual float getRenderTime () const = 0;
    /**
     * @return If a close was requested by the OS
     */
    virtual bool closeRequested () = 0;
    /**
     * @param size The new size for the window
     */
    virtual void resizeWindow (glm::ivec2 size) = 0;
    /**
     * @param positionAndSize The new size and position of the window
     */
    virtual void resizeWindow (glm::ivec4 positionAndSize) = 0;
    /**
     * Shows the window created by the driver
     */
    virtual void showWindow () = 0;
    /**
     * Hides the window created by the driver
     */
    virtual void hideWindow () = 0;
    /**
     * @return The size of the framebuffer available for the driver
     */
    [[nodiscard]] virtual glm::ivec2 getFramebufferSize () const = 0;
    /**
     * @return The number of rendered frames since the start of the driver
     */
    [[nodiscard]] virtual uint32_t getFrameCounter () const = 0;
    /**
     * @param name
     * @return GetProcAddress for this video driver
     */
    [[nodiscard]] virtual void* getProcAddress (const char* name) const = 0;
    /**
     * Process events on the driver and renders a frame
     */
    virtual void dispatchEventQueue () = 0;
    /**
     * @return The app that owns this driver
     */
    [[nodiscard]] CWallpaperApplication& getApp () const;

    /**
     * @return The input context in use by this driver
     */
    [[nodiscard]] Input::CInputContext& getInputContext ();

  private:
    /** App that owns this driver */
    CWallpaperApplication& m_app;
    Input::CInputContext m_inputContext;
};
} // namespace WallpaperEngine::Render::Drivers