//
// Created by alex on 28.11.2022.
//

#pragma once

#include "sim/physics/PhysicsFunctorBase.h"


namespace sim::physics::bounds {
    /**
     * Interface for bounds functors.
     * Provides further functionality for periodic bounds.
     * */
    template<sim::physics::bounds::side>
    class BoundsFunctorBase : public PhysicsFunctorBase {
    protected:
        bool enableOMP;
    public:
        virtual ~BoundsFunctorBase() = default;

        /**
         * Constructor for utilization by subclasses
         * @param st start time
         * @param et end time
         * @param dt delta time
         * @param eps epsilon
         * @param sig sigma
         * @param pc particle container
         * @param eOMP enable OMP flag
         */
        BoundsFunctorBase(double st, double et, double dt, double eps, double sig, ParticleContainer &pc, bool eOMP)
                : PhysicsFunctorBase(st, et, dt, eps, sig, pc), enableOMP(eOMP) {}

        /**Do nothing, sub classes should still override*/
        void operator()() override {}

        virtual bool isPeriodic() {return false;}

        /**Constructs halo in periodic bound. By default does nothing. Has to be overridden.*/
        virtual void generateHalo() {}

        /**Calculates the force for this halo side in periodic bounds. Has to be overridden.*/
        virtual void calcHaloForce() {}
    };
} //sim::physics::bounds