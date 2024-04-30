#pragma once

namespace OStim {
    class SexToyReloadListener {
    public:
        /**
        * gets called once on post post load and again every time the player thread starts
        * all previously registered toys will be wiped right before this gets called
        * so always register all your toys whenever this gets called
        * 
        * gets called from an asynchronous thread so you don't have to worry too much about performance
        * still it shouldn't take ten seconds for toys to set up after a scene starts
        */
        virtual void reloadToys() = 0;
    };
}