## Overview
- It is to demo D3D11 HW Tessellation, including.
  - What does HW Tessellation do ?
  - How to program HW Tessellation by API and Shader ?
  - Explore new graphics algorithms based on HW Tessellation.
- Material & Lighting is not the topic.

## Design 
It requires a flexible design, easy to extend the capability. It depends upon a deep understanding of rendering. 
- a unified interface to handle all kinds of mesh data
- coding on interface other than raw data. 
- collect and dispatch user's input efficiently.
- combination of different shader. 
  
### Mesh Data Management
- Data source, such as quad and bezier patch, is independent of application logic.
  - Data Sources should adhere a unified interface. 
  - Application codes based on this interface and feeds data into pipe.
  
  ![Class Diagram]()
  
### Shader Management
- It is free to map between effects and mesh.
- An effect must contain vertex shader and pixel shader. It may have other shaders such as Hull Shader, Domain Shader.
- Different effects share one common constant buffers which may be big in size. 

- class *Shader* serves the purpose of container of all kinds of shader instances in an effect file.
- class *ShaderContainer* is something like container of all instanced effect files.
- Shader objects are queried from those two container when rendering each frame.

### Render Option Management
Render option is a way to control rendering. Some may requires render state changes in GPU while some in CPU. I don't tell one from another.

- class *RenderOption* is to handle render options. 
- It applies singleton pattern to ensure globally single instance. 

```
struct RenderOption
{
    bool wireframeOn;
    bool diagModeOn;
    bool fixedCamera;
    int  heightMapOn;
    int  tessellateFactor;
    DiagType diagType;
    DirectX::XMMATRIX  world;

    RenderOption::RenderOption()
        : wireframeOn(false)
        , diagModeOn(false)
        , fixedCamera(false)
        , heightMapOn(1)
        , tessellateFactor(10)
        , diagType(eDiagNormal)
    { }

    static RenderOption& getRenderOption();
};

```
## Features
- an ImGUI based UI.
- cmake build.

## Control

- ESC/q	: exit app.

## Demo 

Update...


## References
-  [Tessellation in D3D11@GDC](https://www.gdcvault.com/play/1012740/Direct3D-11-In-Depth-Tutorial)
