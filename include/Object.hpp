#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <sstream>
#include <string>
#include <vector>

struct Vertex {
    float x, y, z; // position coordinates
    float s, t; // texture coordinate
    float nx, ny, nz; // vertex normal

    // Constructor: no args
    Vertex() { }   

    // Constructor: position & texture
    Vertex(
        float _x, float _y, float _z,
        float _s, float _t,
        float _nx, float _ny, float _nz): 
        x(_x), y(_y), z(_z), 
        s(_s), t(_t),
        nx(_nx), ny(_ny), nz(_nz) { }

    // Determing equality of vertices based on all fields
    bool operator== (const Vertex &rhs) {
        return
            (x == rhs.x) && 
            (y == rhs.y) && 
            (nx == rhs.nx) && 
            (ny == rhs.ny) && 
            (nz == rhs.nz);

            /* Textures are out of scope for this assignment
            (z == rhs.z) && 
            (s == rhs.s) && 
            (t == rhs.t);
            */
    }
};

struct Object {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices; // faces
    std::string mtlFilePath; // path to material file
    std::string mapKdPPMFilePath; // path to diffuse color map file

    // Constructor
    Object(std::string filename);
    // Destructor
    ~Object();

    /**
     * Converts vector of Vertex to vector of float vertex positions.
     * Facilitates conversion of Vertex struct to GUfloat.
    */
    std::vector<float> getVerticesAsFloats();

    /**
     * Converts vector of Vertex to vector of float vertex positions
     * and vertext textures.
     * Facilitates conversion of Vertex struct to GUfloat.
    */
    std::vector<float> getVerticesWithTexturesAsFloats();

    /**
     * Parses material file at given path to retrieve and store PPM information.
    */
    void parseMtl(std::string dir, std::string mtlFileName);

    /**
     * Searches for matching vertex in existing vector to determine whether to add
     * the given vertex or use an existing one.
    */
   void storeOrSkip(Vertex vToAdd);

    /**
     * Saves Object data to file.
     * For testing purposes only.
    */
    void saveObj(std::string outputFileName);

    /**
     * Returns list of triangles (sets of 3 vertices).
    */
    std::vector<std::vector<Vertex>> getFaces() const;

    /**
     * Tokenizes given string using given delimiter.
    */
    inline std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> tokens;
        std::stringstream stream(str);
        std::string value;

        while (std::getline(stream, value, delimiter[0])) {
            if (!value.empty()){
                tokens.push_back(value);
            }
        }

        return tokens;
    }

    inline std::string getDirectory(std::string filepath) {
        std::string dir = "";
        size_t lastSlashPos = filepath.find_last_of("/");
        if (lastSlashPos != std::string::npos) {
            dir = filepath.substr(0, lastSlashPos);
            dir.append("/");
        }
        return dir;
    }
};

#endif