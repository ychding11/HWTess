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
It may apply different effects to different mesh. An effect must contains vertex shader, pixel shader.
It may also contain geometry shader, hull shader and domain shader. Different effects may have different
constant buffers which need to update in each frame.

- class *Shader* serves the purpose of container of all kinds of shader instances in an effect file.
- class *ShaderContainer* is something like container of all instanced effect files.
- Shader objects are queried from those two container when rendering each frame.
- Constant buffer Update
  - All effects share the same constant buffer structure.
  - It simplify the design.

```
struct FrameParam
{
    DirectX::XMFLOAT4X4  cbWorld;
    DirectX::XMFLOAT4X4  cbViewProjection;
    DirectX::XMFLOAT3    cbCameraPosWorld;
    float cbTessellationFactor;
    int   cbWireframeOn;
    int   cbHeightMapOn;
    int   cbDiagType;
    float cbTexelCellU;
    float cbTexelCellV;
    float cbWorldCell;
};

```

### Render Option Management
Render option provides a way for user to instruct rendering process. Some options may involve 
render state change in GPU. Some may only affect code flow in CPU. I don't tell one from 
another at first stage. All GPU involved options will be updated as part of constant buffer.

- class *RenderOption* is designed to handle render options specified by user.
- It is implemented as a singleton pattern to ensure globally single instance. 

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
  
## Control
All user controls are done by key stroke.

- ESC/q	: exit app.
- w		: wire-frame mode.
- t     : increase tessellation factor.
- h     : apply height map.
- f     : fixed camera.
- d     : switch between diag modes.



## Demo 
Waiting for Update...    

![demo screenshot](./demo/demo.png)

## References
-  [Tessellation in D3D11@GDC](https://www.gdcvault.com/play/1012740/Direct3D-11-In-Depth-Tutorial)
