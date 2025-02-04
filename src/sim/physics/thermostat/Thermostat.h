#pragma once

#include "data/Particle.h"
#include "data/ParticleContainer.h"
#include "defaults.h"
#include "io/output/Logging.h"

#include <vector>
#include <numeric>

enum ThermoMode {normalMode, pipeMode};

/**
 * @brief Thermostat is responsible for adjusting the temperature and determining whether temp-adjustment is wanted
 * 
 */
class Thermostat
{
private:
    /**reference to the current ParticleContainer; the reference is needed to Compute the current temperature, manipulate the particles, etc */
    ParticleContainer& pc;  
    /**variable, that gets increased in every simulation step. If it is large enough the Thermostat starts with the temperature-regulation process */
    unsigned int countSinceLastActivation{0};
    /**amount of iterations that have to pass before the Thermostat starts doing his work*/
    unsigned int countThreshold;
    /**Thermostat needs to know the number of dimenions for the temperature-computation*/
    unsigned int dims;
    /**Target temperature that the Thermostat tries to reach*/
    double Ttarget;
    /**determines the maximum temperature manipulation in one iteration step*/
    double deltaTemp;
    /**thermoMode determines whether we are simulating a pipe (with unmoving pipe particles) or any other simulation */
    const ThermoMode thermoMode;
    /**in pipe mode some particles get ignored in temperature calculation. Therefore this variable might not be activeParticles.size() (but in pipe sim no other particles get deleted. otherwise this doesn't work)*/
    size_t numberFlowingParticles;


public:
    /**
     * Initializes Thermostat sets the current Temperature to TInit
     * In case the TInit is already surpassed from the user input nothing happens
     * If no particle is initialized with "ordered" movement the current temperature gets set to TInit exactely
     * If particles have "ordered" movement already every Body gets set to (approximately) TInit in their respective system
     * Therefore in this case THE CURRENT TEMPERATURE IS UNEQUAL TO TInit!
     *
     * This might be confusing but hopefully as close as possible to the intended behaviour.
     *
     * @param particleContainer particle container
     * @param T_t target temperature
     * @param cT count of iterations between two thermostat actions
     * @param dimensions dimensionality of the simulation either 2 or 3
     * @param dT maximal difference of temperature the thermostat can produce in one time step
     * @param TInit starting temperature of the simulation
     * @param thermoEnable enable thermostat flag
     * @param tm thermostat mode either ThermoMode::normalMode for most simulations or ThermoMode::pipeMode for no y-Velocity scaling
     */
    explicit Thermostat(ParticleContainer& particleContainer, double T_t = default_t_target,
                        unsigned int cT = default_n_term, unsigned int dimensions = default_dims,
                        double dT = default_delta_temp, double TInit = default_t_init,
                        bool thermoEnable = default_therm, ThermoMode tm = ThermoMode::normalMode)
                        : pc(particleContainer), countThreshold(cT), dims(dimensions), Ttarget(T_t), deltaTemp(dT), thermoMode(tm) {
        if(tm == ThermoMode::normalMode){
            io::output::loggers::simulation->info("Using Thermostat: normal mode");
            numberFlowingParticles = pc.activeSize();
        }else{
            io::output::loggers::simulation->info("Using Thermostat: pipe mode");
            numberFlowingParticles=0;
            pc.runOnActiveData([&](auto&,auto&,auto&,auto&,std::vector<double> &m,auto&,auto,auto&,auto&,
                                   auto& activeParticles){
                for(auto a: activeParticles){
                    if(m[a] >= 0) numberFlowingParticles++;
                }
            });
        }

        if(thermoEnable) {
            //if currentTemp != 0 and TempToCreate != 0 this WILL distort the ordered movement and initialize the temperature a bit
            //inhomogenous
            auto curTemp{computeCurrentTemp()};
            if(curTemp > TInit){
                io::output::loggers::general->info("Since user defined Temperature is lower than user defined \"ordered\" movement no additional heat is generated");
                return ;
            }
            auto TempToCreate{TInit - curTemp};
            if (TempToCreate != 0.) { initializeBrownTemp(TempToCreate); }

                //normalize towards our intended temperature:
                //save real input parameters
                auto realDeltaTemp{dT};
                auto realTtarget{T_t};

                //abuse getCooking to normalize to intended TInit
                this->Ttarget = TInit;
                this->deltaTemp = std::numeric_limits<double>::infinity();

                this->getCooking();

                //set values to their intended parameters
                deltaTemp = realDeltaTemp;
                Ttarget = realTtarget;
            }

        }


    ~Thermostat() = default;

    void setParticleContainer(ParticleContainer& pc);

    /**
     * computes the current temperature relative to the Coordinate System of the Simulation
     * @return
     */
    double computeCurrentTemp();

    /**
     * @brief computes the Beta that should be applied
     * If T_target is not within reach, beta will depend on T_current and Delta_T
     * If T_target is withing reach, beta will depend on T_current and T_target
     * 
     * @return double 
     */
    double computeBeta();

    /**
     * @brief Notifies thermostat that a new iteration is being processed
     * Gets called in every iteration
     * 
     */
    void notify();

    /**
     * @brief This method does the actual heat-manipulation.
     * An alternative name for this method would be adjustTemp()
     * 
     */
    void getCooking();

    /**
     * @brief adds brownian motion with f_i = std::sqrt(TInit/m) to each particle
     * 
     * @param TInit 
     */
    void initializeBrownTemp(double TInit);
};

