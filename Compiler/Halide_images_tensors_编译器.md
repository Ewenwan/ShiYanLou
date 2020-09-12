# Halide
a language for fast, portable computation on images and tensors

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
