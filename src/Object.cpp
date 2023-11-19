#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Object.hpp"

/** 
 * Constructor
 * Given filename (relative path), parses file data to create Object.
*/
Object::Object(std::string filename) {
    std::ifstream inFile;
    inFile.open(filename);

    if (inFile.is_open()) {

        std::string line;

        std::vector<float> vec_s;
        std::vector<float> vec_t;
        std::vector<float> vec_nx;
        std::vector<float> vec_ny;
        std::vector<float> vec_nz;

        while (getline(inFile, line)) {
            
            if (line.length() >= 2 && line.at(0) != '#') { // skip blank & comment lines
                std::stringstream stream(line);
                std::string linePrefix;
                stream >> linePrefix;
                
                if (linePrefix == "v") {
                    Vertex v;
                    stream >> v.x;
                    stream >> v.y;
                    stream >> v.z;
                    vertices.push_back(v);
                } else if (linePrefix == "vt") {
                    float s, t;
                    stream >> s;
                    stream >> t;
                    vec_s.push_back(s);
                    vec_t.push_back(t);
                } else if (linePrefix == "vn") {
                    float nx, ny, nz;
                    stream >> nx;
                    stream >> ny;
                    stream >> nz;
                    vec_nx.push_back(nx);
                    vec_ny.push_back(ny);
                    vec_nz.push_back(nz);
                } else if (linePrefix == "f") {
                    std::string face;
                    while (stream >> face) {
                        // tokenize face data
                        std::vector<std::string> tokens = split(face, "//");
                        
                        unsigned int v_idx = stoi(tokens.at(0)) - 1;
                        unsigned int vt_idx = stoi(tokens.at(1)) - 1;
                        unsigned int vn_idx = stoi(tokens.at(1)) - 1;

                        bool hasVn = false;
                        bool hasVt = false;

                        if (face.find("//") != std::string::npos) { // v//n
                            hasVn = true;
                        } else if (tokens.size() <= 2) { // v/vt
                            hasVt = true;

                        } else { // v/vt/vn
                            hasVt = true;
                            hasVn = true;

                            vn_idx = stoi(tokens.at(2)) - 1;
                        }
                        
                        // get postion from existing vertex
                        Vertex v = vertices.at(v_idx);

                        float s = 0;
                        float t = 0;

                        if (hasVt) {
                            s = vec_s.at(vt_idx);
                            t = vec_t.at(vt_idx);
                        }

                        float nx = 0;
                        float ny = 0;
                        float nz = 0;

                        // store normals if available
                        if (hasVn) {
                            nx = vec_nx.at(vn_idx);
                            ny = vec_ny.at(vn_idx);
                            nz = vec_nz.at(vn_idx);
                        }

                        // create vertex
                        Vertex* vToAdd = new Vertex(
                            v.x, v.y, v.z,
                            s, t,
                            nx, ny, nz
                        );

                        storeOrSkip(*vToAdd); 

                        delete vToAdd;       
                    }
                } else if (linePrefix == "mtllib") {
                    /* material file out of scope for this assignment
                    // find and parse material file
                    std::string mtlFileName;
                    stream >> mtlFileName;
                    
                    // create path to mtlib, assuming same directory
                    std::string dir = getDirectory(filename);
                    parseMtl(dir, mtlFileName);
                    */
                }
            }
        }
    } else {
        std::stringstream errorMsg;
        errorMsg << "Filepath does not exist: " << filename << ". Please ensure given path is relative to program execution location.";
        throw std::runtime_error(errorMsg.str());
    }

    inFile.close();
}

/**
 * Destructor
*/
Object::~Object() { }

/**
 * Converts vector of Vertex to vector of float vertex positions.
 * Facilitates conversion of Vertex struct to GUfloat.
*/
std::vector<float> Object::getVerticesAsFloats() {
    const std::vector<Vertex>& input = vertices;
    std::vector<float> floatVertices;

    floatVertices.reserve(input.size() * 3);

    for (const Vertex& v : input) {
        floatVertices.push_back(v.x);
        floatVertices.push_back(v.y);
        floatVertices.push_back(v.z);
    }

    return floatVertices;
}

/**
 * Converts vector of Vertex to vector of float vertex positions
 * and vertext textures.
 * Facilitates conversion of Vertex struct to GUfloat.
*/
std::vector<float> Object::getVerticesWithTexturesAsFloats() {
    const std::vector<Vertex>& input = vertices;
    std::vector<float> floatVertices;

    floatVertices.reserve(input.size() * 3);

    for (const Vertex& v : input) {
        floatVertices.push_back(v.x);
        floatVertices.push_back(v.y);
        floatVertices.push_back(v.z);
        floatVertices.push_back(v.s);
        floatVertices.push_back(v.t);
    }

    return floatVertices;
}

/**
 * Parses meterial file at given path to retrieve and store PPM information.
*/
void Object::parseMtl(std::string dir, std::string mtlFileName) {
    mtlFilePath = dir + mtlFileName;

    std::ifstream inFile;
    inFile.open(mtlFilePath);

    if (inFile.is_open()) {

        std::string line;

        while (getline(inFile, line)) {
            
            if (line.length() >= 2 && line.at(0) != '#') { // skip blank & comment lines
                std::stringstream stream(line);
                std::string linePrefix;
                stream >> linePrefix;

                if (linePrefix == "map_Kd") {
                    // save diffuse color map file path
                    std::string ppmFileName;
                    stream >> ppmFileName;
                    mapKdPPMFilePath = dir + ppmFileName;
                    return;
                }
            }
        }
    } else {
        std::cerr << "Filepath does not exist: " << mtlFilePath << std::endl;
        std::cerr << "Please ensure .obj, .mtl, and .ppm files are in the same directory." << std::endl;
    }

    inFile.close();
}

void Object::storeOrSkip(Vertex vToAdd) {
    for (unsigned int i = 0; i < vertices.size(); i++) {
        if (vToAdd == vertices.at(i)) {
            indices.push_back(i);
            return;
        }
    }
    
    vertices.push_back(vToAdd);
    indices.push_back(vertices.size() - 1);
    
}

void Object::saveObj(std::string outputFileName) {
    std::ofstream outFile;
    outFile.open(outputFileName);

    for (auto v: vertices) {
        outFile << "v " << v.x << ' ' << v.y << ' ' << v.z << std::endl;
        outFile << "vn " << v.nx << ' ' << v.ny << ' ' << v.nz << std::endl;
        outFile << "vt " << v.s << ' ' << v.t << std::endl;
    }
    outFile.close();
}

std::vector<std::vector<Vertex>> Object::getFaces() const {
    std::vector<std::vector<Vertex>> faces;

    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 < indices.size()) { // ensure 3+ indices left
            std::vector<Vertex> triangle;
            triangle.push_back(vertices[indices[i]]);
            triangle.push_back(vertices[indices[i + 1]]);
            triangle.push_back(vertices[indices[i + 2]]);
            faces.push_back(triangle);
        }
    }

    return faces;
}