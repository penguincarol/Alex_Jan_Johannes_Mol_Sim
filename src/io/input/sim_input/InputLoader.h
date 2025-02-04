//
// Created by alex on 29.10.2022.
//

#pragma once

#include "defaults.h"
#include "data/Particle.h"
#include "data/Membrane.h"
#include "io/input/arg_names.h"

#include <list>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace io::input {

    /**
     * Generic Input file loader.
     * Basic interface to load particles.
     * @param LOAD is a function that loads raw data from @param LOCATOR into the list.
     * */
    template<typename LOCATOR, void (*LOAD)(LOCATOR, std::list<Particle> &, std::list<Membrane> &,
                                            std::unordered_map<io::input::names, std::string> &)>
    class InputLoader {
    private:
        std::list<Particle> buffer;
        std::list<Membrane> membranes;
        LOCATOR locator;
        std::unordered_map<io::input::names, std::string> &arg_map;
    public:
        explicit InputLoader(LOCATOR loc, std::unordered_map<io::input::names, std::string> &map) : locator(loc),
                                                                                               arg_map(map) {}

        InputLoader(const InputLoader &i) = delete;

        /**
         * Calls the loader again and stores all particles the internal buffer.
         * */
        void reload() {
            LOAD(locator, buffer, membranes, arg_map);
        }

        /**
         * Populates @param buf with the stored particles and afterwards clears the internal buffer.
         * */
        void getParticles(std::vector<Particle> &buf) {
            while (!buffer.empty()) {
                buf.template emplace_back(buffer.front());
                buffer.pop_front();
            }
        }

        /**
         *
         */
        void getMembranes(std::vector<Membrane>& membr){
            while(!membranes.empty()){
                membr.template emplace_back(membranes.front());
                membranes.pop_front();
            }
        }


        /**
         * Updates the locator with l
         * */
            void setLocator(LOCATOR l) {
            locator = l;
        }
    };

} // io::input

