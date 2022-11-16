/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include <iostream>
#include <Eigen>
#include "Particle.h"
#include "utils/ArrayUtils.h"
#include "io/Logging.h"

Particle::Particle(int type_arg) {
    type = type_arg;
    loggers::general->debug("Particle generated!");
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    x = {0, 0, 0};
    v = {0, 0, 0};
    m = 0;
}

Particle::Particle(const Particle &other) {
    x = other.x;
    v = other.v;
    f = other.f;
    old_f = other.old_f;
    m = other.m;
    type = other.type;
    loggers::general->debug("Particle generated by copy!");
}

// Todo: maybe use initializer list instead of copy?
Particle::Particle(const std::array<double, 3> &x_arg, const std::array<double, 3> &v_arg,
                   double m_arg, int type_arg) {
    x = {x_arg[0], x_arg[1], x_arg[2]};
    v = {v_arg[0], v_arg[1], v_arg[2]};
    m = m_arg;
    type = type_arg;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    loggers::general->debug("Particle generated!");
}

Particle::Particle(const Eigen::Vector3d &x_arg, const Eigen::Vector3d &v_arg, double m_arg, int type_arg) {
    x = {x_arg[0], x_arg[1], x_arg[2]};
    v = {v_arg[0], v_arg[1], v_arg[2]};
    m = m_arg;
    type = type_arg;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    loggers::general->debug("Particle generated!");
}

Particle::~Particle() {
    loggers::general->debug("Particle destructed!");
}

Particle::Particle(const Eigen::Vector3d &x_arg, const std::array<double, 3> &v_arg, double m_arg, int type_arg) {
    x = {x_arg[0], x_arg[1], x_arg[2]};
    v = {v_arg[0], v_arg[1], v_arg[2]};
    m = m_arg;
    type = type_arg;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    loggers::general->debug("Particle generated!");
}

const Eigen::Vector3d &Particle::getX() const { return x; }

const Eigen::Vector3d &Particle::getV() const { return v; }

const Eigen::Vector3d &Particle::getF() const { return f; }

const Eigen::Vector3d &Particle::getOldF() const { return old_f; }

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

void Particle::setType(const int tt) {
    this->type = tt;
}
void Particle::setM(const double mm){
    this->m = mm;
}
void Particle::setV(const Eigen::Vector3d &new_values){
    this->v = new_values;
}
void Particle::setX(const Eigen::Vector3d &new_values){
    this->x = new_values;
}

void Particle::setF(const Eigen::Vector3d &new_values) {
    this->f = new_values;
}

void Particle::setOldF(const Eigen::Vector3d &new_values) {
    this->old_f = new_values;
}

void Particle::add_to_F(const Eigen::Vector3d &summand) {
    this->f += summand;
}

void Particle::add_to_X(const Eigen::Vector3d &summand) {
    this->x += summand;
}

void Particle::add_to_V(const Eigen::Vector3d &summand) {
    this->v += summand;
}

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f
           << " old_f: " << old_f << " type: " << type;
    return stream.str();
}

bool Particle::operator==(const Particle &other) {
    return (x == other.x) and (v == other.v) and (f == other.f) and
           (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream &operator<<(std::ostream &stream, Particle &p) {
    stream << p.toString();
    return stream;
}
