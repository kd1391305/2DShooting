#pragma once

class SoundCache {
public:
    static SoundCache& Instance() {
        static SoundCache instance;
        return instance;
    }
  
    //渡すサウンドデータの状態
    enum SoundState
    {
        None,           //指定なし
        NotPlaying,     //再生していない
        Playing,        //再生中
    };


private: 
    struct Sound
    {
        void Init(std::string path)
        {
            m_se = std::make_shared<KdSoundEffect>();
            m_se->Load(path);
            std::shared_ptr<KdSoundInstance> inst;
            inst = m_se->CreateInstance(false);
            inst->SetVolume(1.0f);
            m_seInstList.push_back(inst);
        }

        //再生用のインスタンスを追加する
        std::shared_ptr<KdSoundInstance> Add()
        {
            std::shared_ptr<KdSoundInstance> inst;
            inst = m_se->CreateInstance(false);
            inst->SetVolume(1.0f);
            m_seInstList.push_back(inst);
            return inst;
        }
        std::shared_ptr<KdSoundEffect>		m_se;
        std::vector<std::shared_ptr<KdSoundInstance>>	m_seInstList;
    };

    

public:
    std::shared_ptr<KdSoundInstance> Get(const std::string& path, const SoundState state = SoundState::None) {

        auto it = cache.find(path);
        //SEデータが見つかったなら、
        if (it != cache.end()) 
        {
            if (state == SoundState::None)
            {
                return it->second->m_seInstList.front();
            }
            //再生していないインスタンスを探す
            else if (state == SoundState::NotPlaying)
            {
                for (auto& se : it->second->m_seInstList)
                {
                    if (!se->IsPlay())
                    {
                        return se;
                    }
                }
            }
            //再生中のインスタンスを探す
            else if (state == SoundState::Playing)
            {
                for (auto& se : it->second->m_seInstList)
                {
                    if (se->IsPlay())
                    {
                        return se;
                    }
                }
            }

            //stateで指定した状態のインスタンスが存在しなかったら、インスタンス作成し、追加する
            return it->second->Add();
        }

        //SEデータが見つからなかったら
        //新しくSEデータを作成
        auto sound = std::make_shared<Sound>();
        sound->Init(path);
        cache[path] = sound;
        return sound->m_seInstList.back();
    }
private:
    std::unordered_map<std::string, std::shared_ptr<Sound>> cache;
   
};
#pragma once
