#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

struct ObjectOptions {
    std::unordered_set<int> groups;
    // "dont-fade", "dont-enter", "no-effects",
    // "no-touch", "no-particles", "no-glow"
    std::array<bool, 6> toggles = {
        false // :3
    };
    float scaleX = 1.0f, scaleY = 1.0f;
};

auto mod = Mod::get();

class $modify(Editor, LevelEditorLayer) {
    struct Fields {
        std::unordered_map<int, ObjectOptions> defaultObjectOptions;

        bool advancedDefaultOptions = mod->getSettingValue<bool>("advanced-options");
        bool dontFade = mod->getSettingValue<bool>("dont-fade");
        bool dontEnter = mod->getSettingValue<bool>("dont-enter");
        bool noGlow = mod->getSettingValue<bool>("no-glow");
    };

    bool init(GJGameLevel* p0, bool p1) {
        if (!LevelEditorLayer::init(p0, p1)) return false;

        if (m_fields->advancedDefaultOptions) parseDefaultObjectOptions();

        return true;
    }

    GameObject* createObject(int p0, CCPoint p1, bool p2) {
        GameObject* obj = LevelEditorLayer::createObject(p0, p1, p2);

        if (!obj) return obj;

        auto fields = m_fields.self();

        obj->m_isDontFade = fields->dontFade;
        obj->m_isDontEnter = fields->dontEnter;
        obj->m_hasNoGlow = fields->noGlow;

        auto& defaultObjectOptions = fields->defaultObjectOptions;
        if (!fields->advancedDefaultOptions || defaultObjectOptions.empty()) return obj;

        if (defaultObjectOptions.contains(0)) updateObjectWithObjectOptions(obj, defaultObjectOptions.at(0));
        if (defaultObjectOptions.contains(p0)) updateObjectWithObjectOptions(obj, defaultObjectOptions.at(p0));

        return obj;
    }

    void updateObjectWithObjectOptions(GameObject* obj, const ObjectOptions& options) {
        for (auto group : options.groups) obj->addToGroup(group);

        obj->m_isDontFade = options.toggles[0];
        obj->m_isDontEnter = options.toggles[1];
        obj->m_hasNoEffects = options.toggles[2];
        obj->m_isNoTouch = options.toggles[3];
        obj->m_hasNoParticles = options.toggles[4];
        obj->m_hasNoGlow = options.toggles[5];

        obj->setScaleX(options.scaleX);
        obj->m_scaleX = options.scaleX;
        obj->setScaleY(options.scaleY);
        obj->m_scaleY = options.scaleY;
    }

    void parseDefaultObjectOptions() {
        auto& defaultObjectOptions = m_fields->defaultObjectOptions;

        auto path = (mod->getConfigDir() / "entries.json").string(); // i hope this is safe im spamming checks cuz im paranoid at bad at this
        if (!std::filesystem::exists(path)) {
            Notification::create("(Default Object Options) invalid filepath, try 'entries.json' :3", NotificationIcon::Error)->show();
            return;
        }

        std::ifstream file(path);
        auto parseJson = matjson::parse(file);
        if (!parseJson.isOk()) {
            Notification::create("(Default Object Options) failed to parse :3", NotificationIcon::Error)->show();
            return;
        }
        auto json = parseJson.unwrap();
        if (!json.isObject()) return;

        std::array<const std::string, 6> toggleKeys = {
            "dont-fade", "dont-enter", "no-effects",
            "no-touch", "no-particles", "no-glow"
        };

        for (auto [id, value] : json) {
            auto objectID = std::strtol(id.c_str(), nullptr, 10);
            if (objectID == 0) continue;
            ObjectOptions objectOptions;
            if (value.contains("groups") && value["groups"].isArray()) {
                auto groups = value["groups"].asArray().unwrap();
                for (auto group : groups) {
                    if (group.isNumber()) objectOptions.groups.insert(group.asDouble().unwrap());
                }
            }
            if (value.contains("toggles")) {
                auto toggles = value["toggles"];
                int i = 0;
                for (auto key : toggleKeys) {
                    if (toggles.contains(key) && toggles[key].isBool()) {
                        objectOptions.toggles[i] = toggles[key].asBool().unwrap();
                    }
                    i++;
                }
            }
            if (value.contains("scale") && value["scale"].isArray()) {
                auto scaleArray = value["scale"].asArray().unwrap();
                int i = 0;
                for (auto scaleValue : scaleArray) {
                    if (!scaleValue.isNumber()) break;
                    auto scale = scaleValue.asDouble().unwrap();
                    if (scaleArray.size() == 1) {
                        objectOptions.scaleX = scale;
                        objectOptions.scaleY = scale;
                        break;
                    }
                    else {
                        if (i == 0) objectOptions.scaleX = scale;
                        else objectOptions.scaleY = scale;
                    }
                    i++;
                }
            }
            defaultObjectOptions.insert({objectID, objectOptions});
        }
    }
};