# Halide
a language for fast, portable computation on images and tensors

Halide是用C++作为宿主语言的一个图像处理相关的DSL(Domain Specified Language)语言，全称领域专用语言。主要的作用为在软硬层面上(与算法本身的设计无关)实现对算法的底层加速，我们有必要对其有一定的了解。因为不论是 传统的图像处理方法亦或是深度学习应用 都使用到了halide的思想。


[官网](https://halide-lang.org/)

[Halide_CVPR_intro pdf](https://halide-lang.org/assets/lectures/Halide_CVPR_intro.pdf)

[github](https://github.com/halide/Halide)

Halide is a programming language designed to make it easier to write high-performance image and array processing code on modern machines. Halide currently targets: 
* CPU architectures: X86, ARM, MIPS, Hexagon, PowerPC
* Operating systems: Linux, Windows, macOS, Android, iOS, Qualcomm QuRT
* GPU Compute APIs: CUDA, OpenCL, OpenGL, OpenGL Compute Shaders, Apple Metal, Microsoft Direct X 12


Halide’s answer: decouple algorithm from schedule 分离计算和调度

Algorithm: what is computed  

Schedule: where and when it’s computed 
 
* Easy for programmers to build pipelines simpliﬁes algorithm codeimproves modularity
* Easy for programmers to specify & explore optimizations fusion, tiling, parallelism, vectorization can’t break the algorithm
* Easy for the compiler to generate fast code
