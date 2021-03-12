
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


#include "Model.h"


void Model::loadModelFromFile(const string &filepath) {
    std::string inputfile = filepath;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "../res";

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }
    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                mVertices.emplace_back(
                        attrib.vertices[3*idx.vertex_index+0],
                        attrib.vertices[3*idx.vertex_index+1],
                        attrib.vertices[3*idx.vertex_index+2]);
            }

            Triangle tri(this, {
                    (int)mVertices.size() - 3,
                    (int)mVertices.size() - 2,
                    (int)mVertices.size() - 1});
            mTriangles.emplace_back(tri);
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}