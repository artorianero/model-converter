# Introduction

一个基于assimp(https://github.com/assimp/assimp)的模型格式转换工具。

# Usage

```sh
model-converter v0.1 based on assimp
Usage: bin/model-converter [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -i,--input TEXT REQUIRED    输入的模型文件路径或目录
  -o,--output TEXT REQUIRED   输出的模型文件路径或目录
  -t,--type TEXT              输出模型的格式，类型包括:
                               collada, x, stp, obj, objnomtl
                               stl, stlb, ply, plyb, 3ds
                               gltf2, glb2, gltf, glb, assbin 
                               assxml, x3d, fbx, fbxa, m3d, m3da
                               3mf, pbrt, assjson
  -d,--dir                    传入的input和output对应的是否为目录，带上该flag说明是目录
```

# Reference 

> https://blog.csdn.net/g0415shenw/article/details/124583012
> https://blog.csdn.net/qq_41006901/article/details/115555842