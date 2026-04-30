#pragma once

class SoundCache {
public:
    static SoundCache& Instance() {
        static SoundCache instance;
        return instance;
    }
  
    
private: 
    struct Sound
    {
        void Init(std::string path)
        {
            m_se = std::make_shared<KdSoundEffect>();
            m_se->Load(path);
            m_seInst = m_se->CreateInstance(false);
            m_seInst->SetVolume(1.0f);
        }
        std::shared_ptr<KdSoundEffect>		m_se;
        std::shared_ptr<KdSoundInstance>	m_seInst;
    };

public:
    std::shared_ptr<KdSoundInstance> Get(const std::string& path) {

        auto it = cache.find(path);
        if (it != cache.end()) {
            return it->second->m_seInst;
        }
        auto sound = std::make_shared<Sound>();
        sound->Init(path);
        cache[path] = sound;
        return sound->m_seInst;
    }
private:
    std::unordered_map<std::string, std::shared_ptr<Sound>> cache;
   
};
#pragma once
