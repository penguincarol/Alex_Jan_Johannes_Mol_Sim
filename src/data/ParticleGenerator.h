#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include "Body.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "Membrane.h"

#include <Eigen>
#include <list>

namespace ParticleGenerator{
    extern int bodyID;
    extern int particleID;

    /**
     * @brief generates  Cuboid with the parameters defined in the Body
     * Body.shape has to be Cuboid
     * Feeds the generated particles into @param buffer
     * @param body is the desired body
     * @param v_bolz is the thermal friction
     * @param dims is the dimension of the data
     */
    void generateCuboid(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims, double sigma, double epsilon);

    /**
     * @brief generates Sphere at said point
     * Body.shape has to be sphere
     * Feeds the generated particles into @param buffer
     * @param body is the desired body 
     * @param v_bolz is the thermal friction
     * @param dims is the dimension of the data
     */
    void generateSphere(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims, double sigma, double epsilon);


    /**
     * @brief generates a Membrane at said point
     * Body.shape has to be membrane
     * One of the dimensions has to be 1
     * Feeds the generated particles into @param buffer
     * Feeds the generated Membranes into @param membranes
     * Note that the Membrane classes are useless without the Particles in the buffer since it mostly stores references to particles
     * @param body
     * @param v_bolz
     * @param buffer
     * @param membranes
     * @param dims
     * @param sigma
     * @param epsilon
     */
    void generateMembrane(struct Body& body, double v_bolz, std::list<Particle>& buffer, std::list<Membrane>& membranes, int dims, double sigma, double epsilon);

    /**
     * @brief generates Particle at said point
     * 
     * @param x 
     * @param v 
     * @param m
     *
     * and stores it in @param buffer.
     */
    void generateParticle(Eigen::Vector3d& x, Eigen::Vector3d& v, double m, std::list<Particle>& buffer, double sigma, double epsilon);

    /**
     * Retrieves an global unique ID for a body.
     * ID 0 is reserved for single particles.
     * */
    int getNextBodyID();

    /**
     * Retrieves an global unique ID for a particle.
     * */
    int getNextParticleID();
};
