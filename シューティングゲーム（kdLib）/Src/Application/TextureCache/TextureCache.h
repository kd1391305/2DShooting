#pragma once

class TextureCache {
public:
    static TextureCache& Instance() {
        static TextureCache instance;
        return instance;
    }

    std::shared_ptr<KdTexture> Get(const std::string& path) {

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
    std::unordered_map<std::string, std::shared_ptr<KdTexture>> cache;
};
