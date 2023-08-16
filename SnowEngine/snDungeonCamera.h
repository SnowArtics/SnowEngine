#pragma once
#include <snScript.h>

namespace sn {
    class DungeonCamera :
        public Script
    {
    public:
        DungeonCamera();
        ~DungeonCamera();

    public:
        virtual void Update() override;

    private:
        std::pair<int, int> prevPlayerMapPos;
        std::pair<int, int> curPlayerMapPos;
    };

}