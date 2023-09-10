#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include "../include/tools/file-operation.hpp"
#include "../third_party/CLI11.hpp"
#include "assimp/Exporter.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

void FindMeshInfo(const aiScene* scene, aiNode* node) {
    std::cout << node->mName.C_Str() << std::endl;
    if (0 == node->mNumMeshes) {
        return;
    }
    auto mesh = node->mMeshes;
}

void FindMesh(const aiScene* scene, aiNode* node) {
    FindMeshInfo(scene, node);
    for (unsigned int m = 0; m < node->mNumChildren; ++m) {
        FindMesh(scene, node->mChildren[m]);
    }
}

void ModelConverter(std::string input, std::string output, std::string type) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(input, aiProcess_Triangulate);
    if (nullptr == scene) {
        printf("read model %s failed.\n", input.c_str());
        return;
    }

    aiNode* rootNode = scene->mRootNode;
    FindMesh(scene, rootNode);
    Assimp::Exporter exporter;

    auto aiReturn = exporter.Export(scene, type, output);
    switch (aiReturn) {
        case aiReturn_SUCCESS:
            printf("convert model %s to %s success.\n", input.c_str(), output.c_str());
            break;
        case aiReturn_FAILURE:
            printf("convert model %s failed.\n", input.c_str());
            break;
        case aiReturn_OUTOFMEMORY:
            printf("out of memory while converting model %s.\n", input.c_str());
            break;
        default:
            printf("something wrong while converting model %s.\n", input.c_str());
            break;
    }
}

// Not all of them are right
const std::map<std::string, std::string> TYPE_SURRFIX = {
    {"collada", ".dae"},
    {"x", ".x"},
    {"stp", ".stp"},
    {"obj", ".obj"},
    {"objnomtl", ".obj"},
    {"stl", ".stl"},
    {"stlb", ".stl"},
    {"ply", ".ply"},
    {"plyb", ".ply"},
    {"3ds", ".3ds"},
    {"gltf", ".gltf"},
    {"glb", ".glb"},
    {"gltf2", ".gltf"},
    {"glb2", ".glb"},
    {"assbin", ".assbin"},
    {"assxml", ".assxml"},
    {"x3d", ".x3d"},
    {"fbx", ".fbx"},
    {"fbxa", ".fbx"},
    {"m3d", ".m3d"},
    {"m3da", ".m3d"},
    {"3mf", ".3mf"},
    {"pbrt", ".pbrt"},
    {"assjson", ".json"},
};

int main(int argc, char** argv) {
    CLI::App app{"model-converter v0.1 based on assimp"};
    // Descript parameters
    std::string input;
    std::string output;
    std::string type = "glb2";
    bool isDir = false;
    // Add flag
    app.add_option("-i,--input", input, "输入的模型文件路径或目录")->required();
    app.add_option("-o,--output", output, "输出的模型文件路径或目录")->required();
    app.add_option("-t,--type", type,
                   "输出模型的格式，类型包括:\n \
collada, x, stp, obj, objnomtl\n \
stl, stlb, ply, plyb, 3ds\n \
gltf2, glb2, gltf, glb, assbin \n \
assxml, x3d, fbx, fbxa, m3d, m3da\n \
3mf, pbrt, assjson");
    app.add_flag("-d,--dir", isDir, "传入的input和output对应的是否为目录，带上该flag说明是目录");

    CLI11_PARSE(app, argc, argv);

    std::cout << "input: " << input << std::endl;
    std::cout << "output: " << output << std::endl;
    std::cout << "type: " << type << std::endl;
    std::cout << "isDir " << isDir << std::endl;

    // DwgToGlb("../data/dxf/testgt.dxf", "../data/dxf/out1.glb", "glb2");

    if (isDir) {
        std::vector<std::string> files;
        auto count = ReadDir(input, files);
        std::cout << "count: " << count << std::endl;

        FixDirPath(&input);
        FixDirPath(&output);
        CheckFolder(output.c_str());

        for (auto f : files) {
            ModelConverter(input + f, output + f.substr(0, f.rfind(".")) + TYPE_SURRFIX.at(type), type);
        }
    } else {
        auto folder = output.substr(0, output.rfind("/") + 1);
        // std::cout << "folder: " << folder << std::endl;
        CheckFolder(folder.c_str());
        ModelConverter(input, output, type);
    }
    return 0;
}