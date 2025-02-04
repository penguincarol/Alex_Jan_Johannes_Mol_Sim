//
// Created by alex on 10.01.2023.
//
#pragma once

#include <vector>
#include <memory>

#include "ForceFunctorBase.h"
#include "types.h"
#include "FGravity.h"
#include "FGravityCells.h"
#include "FGravityOMP.h"
#include "FGravityCellsOMP.h"
#include "FLennardJones.h"
#include "FLennardJonesCells.h"
#include "FLennardJonesOMP.h"
#include "FLennardJonesCellsOMP.h"
#include "FGlobalGravity.h"
#include "FMembrane.h"
#include "FMembranePull.h"

namespace sim::physics::force {
    /**
     * Takes care of all force calculation.
     * */
    class ForceHandler {
    private:
        /**performs the main force calculation (LJ/GRAV using LC|OMP)*/
        std::shared_ptr<ForceFunctorBase> primaryFF;
        /**storage for all additional force functors (glob grav, mem, mem pull)*/
        std::vector<std::shared_ptr<ForceFunctorBase>> secondaryFF;

    public:
        /**
         * @param t primary force type: either lennard jones or gravity
         * @param eLC enable linked cell flag
         * @param eOMP enable OMP flag
         * @param eGrav enable global gravity flag
         * @param eMem enable membrane simulation flag
         * @param eMemPull enable membrane pull flag
         * @param gG0 global gravity x0 direction
         * @param gG1 global gravity x1 direction
         * @param gG2 global gravity x2 direction
         * @param st start time
         * @param et end time
         * @param dt delta time
         * @param eps epsilon
         * @param sig sigma
         * @param pc particle container
         * */
        ForceHandler(type t, bool eLC, bool eOMP, bool eGrav, bool eMem, bool eMemPull,
                     double gG0, double gG1, double gG2,
                     double st, double et, double dt, double eps, double sig, ParticleContainer &pc) {
            //generate prim FF
            if (t == gravity && !eLC && !eOMP) { primaryFF = std::make_shared<FGravity>(st, et, dt, eps, sig, pc);}
            if (t == gravity &&  eLC && !eOMP) { primaryFF = std::make_shared<FGravityCells>(st, et, dt, eps, sig, pc);}
            if (t == gravity && !eLC &&  eOMP) { primaryFF = std::make_shared<FGravityOMP>(st, et, dt, eps, sig, pc);}
            if (t == gravity &&  eLC &&  eOMP) { primaryFF = std::make_shared<FGravityCellsOMP>(st, et, dt, eps, sig, pc);}
            if (t == lennardJones && !eLC && !eOMP) { primaryFF = std::make_shared<FLennardJones>(st, et, dt, eps, sig, pc);}
            if (t == lennardJones &&  eLC && !eOMP) { primaryFF = std::make_shared<FLennardJonesCells>(st, et, dt, eps, sig, pc);}
            if (t == lennardJones && !eLC &&  eOMP) { primaryFF = std::make_shared<FLennardJonesOMP>(st, et, dt, eps, sig, pc);}
            if (t == lennardJones &&  eLC &&  eOMP) { primaryFF = std::make_shared<FLennardJonesCellsOMP>(st, et, dt, eps, sig, pc);}
            if (!primaryFF) {
                io::output::loggers::simulation->error("Fatal: Failed to construct primary ForceFunctor!");
                exit(-1);
            }
            //generate secondary FFs
            if (eGrav) secondaryFF.emplace_back(std::make_shared<FGlobalGravity>(st, et, dt, eps, sig, gG0, gG1, gG2,pc));
            if (eMem) secondaryFF.emplace_back(std::make_shared<FMembrane>(st, et, dt, eps, sig, pc));
            if (eMem && eMemPull) secondaryFF.emplace_back(std::make_shared<FMembranePull>(st, et, dt, eps, sig, pc));
        }

        /**
         * Performs all registered force calculations.
         * */
        void operator()(){
            (*primaryFF)();
            for (unsigned long i {0}; i < secondaryFF.size(); i++) (*secondaryFF[i])();
        }

        /**
         * Returns the primary function that calculates the force between two particles
         * */
        pair_fun_t& getForceFunction() {
            return primaryFF->getForceFunction();
        }

        /**
         * Returns the fast version of the primary pairwise force function
         * */
        fpair_fun_t getFastForceFunction() {
            return primaryFF->getFastForceFunction();
        }

        /**
         * Returns the fast version of the primary pairwise force function, with the second particle being temporary
         * */
        fpair_fun_alt_t getFastForceAltFunction() {
            return primaryFF->getFastForceAltFunction();
        }

        /**
         * Returns the fast version of the primary pairwise force function, with the second particle being temporary and no write back
         * */
        fpair_fun_ret_t getFastForceRetFunction(){
            return primaryFF->getFastForceRetFunction();
        }

        /**
         * Sets the particle container for all FFs.
         * */
         void setParticleContainer(ParticleContainer& pc) {
             primaryFF->setParticleContainer(pc);
             for (unsigned long i {0}; i < secondaryFF.size(); i++) secondaryFF[i]->setParticleContainer(pc);
         }
    };
} // sim::physics::force

