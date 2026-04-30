#pragma once

class SoundCache {
public:
    static SoundCache& Instance() {
        static SoundCache instance;
        return instance;
    }
  
    struct Sound
    {

    };

    std::shared_ptr<Sound> Get(const std::string& path) {

        auto it = cache.find(path);
        if (it != cache.end()) {
            return it->second;
        }
        auto tex = std::make_shared<KdTexture>();
        tex.get()->Load(path);
        cache[path] = tex;
        return tex;
    }

   

private:
    std::unordered_map<std::string, std::shared_ptr<Sound>> cache;
};
#pragma once
