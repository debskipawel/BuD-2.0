# BuD 2.0

BuD is a Windows 3D graphics engine that I developed from scratch using C++ and DirectX 11. It provides an abstraction layer for writing window graphical applications, while also being very flexible and allowing for defining very custom shader pipelines in code.

BuD 2.0 comes with a few nifty optimizations that previous iteration of the engine did not have, like frustum culling and instanced rendering, which increased the performace of the engine significantly. 
Also, the engine natively supports multi-eye rendering, allowing for developing 3D (real 3D, with those funky red-blue glasses 3D) applications.

## BuD-CAD

The engine comes with a CAD application that I'm actively developing.

It allows for a simple geometric modelling in 3D space, using objects like points, curves, and surfaces - mainly focusing on surfaces and intersections between them. See short demo below:


https://github.com/debskipawel/BuD-2.0/assets/52053209/2142619f-dfee-4e37-b4d7-5f4845beaf9f


Some more examples below:

![C2_intersection](https://github.com/debskipawel/BuD-2.0/assets/52053209/6ba8b111-626f-47ff-abbd-3d214f75a71d)

![everything_intersection](https://github.com/debskipawel/BuD-2.0/assets/52053209/0fc37c94-1c6f-40c0-8f38-4bd844353388)


BuD-CAD also utilizes previously mentiones multi-eye rendering, allowing for modelling in anaglyph mode, using special glasses.

![watrdrop_anaglyph](https://github.com/debskipawel/BuD-2.0/assets/52053209/14a5c3a8-b791-41c2-802c-1f2b632aefc7)
