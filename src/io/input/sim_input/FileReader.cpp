/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"
#include "io/output/Logging.h"
#include "data/Membrane.h"


#include <cstdlib>
#include <fstream>
#include <iostream>

namespace io::input {
    FileReader::FileReader() = default;

    FileReader::~FileReader() = default;

    void FileReader::readFile(const char *filename, std::list<Particle> &particles, std::list<Membrane> &,  std::unordered_map<io::input::names, std::string> & ) {
        std::array<double, 3> x;
        std::array<double, 3> v;
        double m;
        int num_particles = 0;

        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open()) {

            getline(input_file, tmp_string);
            io::output::loggers::general->debug("Read line: {}", tmp_string);

            while (tmp_string.empty() or tmp_string[0] == '#') {
                getline(input_file, tmp_string);
                io::output::loggers::general->debug("Read line: {}", tmp_string);
            }

            std::istringstream numstream(tmp_string);
            numstream >> num_particles;
            io::output::loggers::general->debug("Reading {}", num_particles);
            getline(input_file, tmp_string);
            io::output::loggers::general->debug("Read line: {}", tmp_string);

            for (int i = 0; i < num_particles; i++) {
                std::istringstream datastream(tmp_string);

                for (auto &xj: x) {
                    datastream >> xj;
                }
                for (auto &vj: v) {
                    datastream >> vj;
                }
                if (datastream.eof()) {
                    io::output::loggers::general->error("Error reading file: eof reached unexpectedly reading from line {}", i);
                    exit(-1);
                }
                datastream >> m;
                particles.emplace_back(x, v, m);

                getline(input_file, tmp_string);
                io::output::loggers::general->debug("Read line: {}", tmp_string);
            }
        } else {
            io::output::loggers::general->error("Error: could not open file {}", filename);
            exit(-1);
        }
    }
} // io::input