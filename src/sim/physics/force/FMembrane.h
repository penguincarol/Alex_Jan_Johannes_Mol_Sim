//
// Created by johnny on 09.01.23.
//
#pragma once

#include "ForceFunctorBase.h"

namespace sim::physics::force {
/**
 * calculate the force for all particles using the Lennard-Jones potential
 * */
class FMembrane : public ForceFunctorBase {
private:
    pair_fun_t pairFun;
    fpair_fun_t fpairFun;
    ForceFunctorBase* forceDelegate;
    double gGrav;

    void setPairFun();

public:
    /**
     * the created instance will take ownership of ff and will delete it upon deconstruction.
     * */
    FMembrane(
                       double st,
                       double et,
                       double dt,
                       double eps,
                       double sig,
                       double gG,
                       ParticleContainer &pc,
                       ForceFunctorBase* ff
    ) : ForceFunctorBase(st, et, dt, eps, sig, pc), forceDelegate(ff), gGrav(gG) {
        setPairFun();
    }

    ~FMembrane() override {
        delete forceDelegate;
    }

    void operator()() override;

    void setParticleContainer(ParticleContainer& pc) override;

    pair_fun_t& getForceFunction() override;

    fpair_fun_t getFastForceFunction() override;
};

} // sim::physics::force