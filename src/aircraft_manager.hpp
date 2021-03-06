#pragma once

#include <memory>
#include "GL/opengl_interface.hpp"


class AircraftManager {
    private:

    public:
        void move_all_aircrafts(double delta_time)
        {
            for(auto it = GL::move_queue.begin() ; it != GL::move_queue.end();)
            {

                //if(!((*it)->move(std::min(delta_time/1000, (1000/60.f)))))
                auto& aircraft = *it;

                if(!(*it)->move(delta_time))
                {

                    //auto casted_item = dynamic_cast<const GL::Displayable*>(*it);

                    //it = GL::move_queue.erase(it);
                    //if(std::find(GL::display_queue.begin(), GL::display_queue.end(), casted_item) != GL::display_queue.end()) {
                    //    GL::display_queue.erase(std::remove(GL::display_queue.begin(), GL::display_queue.end(), casted_item));
                    //}            

                    it = GL::move_queue.erase(it);
                    delete(aircraft);
                }
                else 
                {
                    ++it;
                }
            }
        }
};