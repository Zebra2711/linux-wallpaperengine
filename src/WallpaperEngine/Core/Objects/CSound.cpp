#include "CSound.h"

#include <utility>
#include "WallpaperEngine/Core/CObject.h"
#include "WallpaperEngine/Logging/CLog.h"

using namespace WallpaperEngine::Core::Objects;

CSound::CSound (
    std::shared_ptr <const Core::CProject> project, const CUserSettingBoolean* visible, int id, std::string name,
    const CUserSettingVector3* origin, const CUserSettingVector3* scale, const CUserSettingVector3* angles,
    bool repeat, std::vector<std::string> sounds, std::vector<int> dependencies
) :
    CObject (project, visible, id, std::move(name), origin, scale, angles, std::move(dependencies)),
    m_repeat (repeat),
    m_sounds (std::move(sounds)) {}

const WallpaperEngine::Core::CObject* CSound::fromJSON (
    std::shared_ptr <const Core::CProject> project, const json& data, const CUserSettingBoolean* visible,
    int id, const std::string& name, const CUserSettingVector3* origin, const CUserSettingVector3* scale,
    const CUserSettingVector3* angles, std::vector<int> dependencies
) {
    // TODO: PARSE AUDIO VOLUME
    std::vector<std::string> sounds;
    const auto sound_it = jsonFindRequired (data, "sound", "Sound information not present");

    if (!sound_it->is_array ())
        sLog.exception ("Expected sound list on element ", name);

    for (const auto& cur : (*sound_it))
        sounds.push_back (cur);

    return new CSound (
        project,
        visible,
        id,
        name,
        origin,
        scale,
        angles,
        jsonFindDefault<std::string> (data, "playbackmode", "") == "loop",
        sounds,
        std::move(dependencies)
    );
}

const std::vector<std::string>& CSound::getSounds () const {
    return this->m_sounds;
}

bool CSound::isRepeat () const {
    return this->m_repeat;
}
