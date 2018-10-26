## Overview
- It is used to demo D3D11 HW Tessellation.
  - What is HW Tessellation ?
  - How to Control HW Tessellation by API and Shader ?
- It is to explore new algorithms taking advantage of HW Tessellation.
- No material, Lighting model involved.

## Design 
- A flexible design
  - It is easy to extend the capability of adding new features
  
### Mesh Data Management
- Data source such quad, bezier patch is independent of APP logic.
  - All Data Sources provide an unified interface. 
  - APP use this interface feed data into tessellation pipe.
  
### Shader Management
It may apply different effects to different mesh. An effect must contains vertex shader, pixel shader.
It may also contain geometry shader, hull shader and domain shader. 
- class *Shader* serves the purpose of container of all kinds of shader instances in an effect file.
- class *ShaderContainer* is something like container of all instanced effect files.
- Shader objects are queried from those two container when rendering each frame.


### Render Option Management
- class *RenderOption* is designed to handle render options specified by user.
- It is implemented as a singleton pattern to ensure globally single instance. 
  
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

## References
-  [Tessellation in D3D11@GDC](https://www.gdcvault.com/play/1012740/Direct3D-11-In-Depth-Tutorial)
