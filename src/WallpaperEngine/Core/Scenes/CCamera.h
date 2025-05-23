#pragma once

#include "WallpaperEngine/Core/Core.h"

namespace WallpaperEngine::Core::Scenes {
using json = nlohmann::json;

class CCamera {
  public:
    static const CCamera* fromJSON (const json::const_iterator& data);

    const glm::vec3& getCenter () const;
    const glm::vec3& getEye () const;
    const glm::vec3& getUp () const;

  protected:
    CCamera (glm::vec3 center, glm::vec3 eye, glm::vec3 up);

  private:
    const glm::vec3 m_center;
    const glm::vec3 m_eye;
    const glm::vec3 m_up;
};
} // namespace WallpaperEngine::Core::Scenes
