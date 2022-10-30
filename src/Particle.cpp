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

Particle::Particle(int type_arg) {
  type = type_arg;
  std::cout << "Particle generated!" << std::endl;
  f = {0., 0., 0.};
  old_f = {0., 0., 0.};
}

Particle::Particle(const Particle &other) {
  x = other.x;
  v = other.v;
  f = other.f;
  old_f = other.old_f;
  m = other.m;
  type = other.type;
  std::cout << "Particle generated by copy!" << std::endl;
}

// Todo: maybe use initializer list instead of copy?
Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg, int type_arg) {
  x = { x_arg[0], x_arg[1], x_arg[2] };
  v = { v_arg[0], v_arg[1], v_arg[2] };
  m = m_arg;
  type = type_arg;
  f = {0., 0., 0.};
  old_f = {0., 0., 0.};
  std::cout << "Particle generated!" << std::endl;
}

Particle::~Particle() { std::cout << "Particle destructed!" << std::endl; }

const Eigen::Vector3d &Particle::getX() const { return x; }

const Eigen::Vector3d &Particle::getV() const { return v; }

const Eigen::Vector3d &Particle::getF() const { return f; }

const Eigen::Vector3d &Particle::getOldF() const { return old_f; }

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

void Particle::setF(const Eigen::Vector3d& new_values){
  this->f = new_values;
}

void Particle::setOldF(const Eigen::Vector3d& new_values){
  this->old_f = new_values;
}

void Particle::add_to_F(const Eigen::Vector3d& summand){
    this->f += summand;
}

void Particle::add_to_X(const Eigen::Vector3d& summand){
    this->x += summand;
}

void Particle::add_to_V(const Eigen::Vector3d& summand){
    this->v += summand;
}

std::string Particle::toString() const {
  std::stringstream stream;
  stream << "Particle: X:" << x << " v: " << v << " f: " << f
         << " old_f: " << old_f << " type: " << type;
  return stream.str();
}

bool Particle::operator==(Particle &other) {
  return (x == other.x) and (v == other.v) and (f == other.f) and
         (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream &operator<<(std::ostream &stream, Particle &p) {
  stream << p.toString();
  return stream;
}
