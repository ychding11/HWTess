## Overview
- It is used to demo D3D11 HW Tessellation.
  - What is HW Tessellation ?
  - How to Control HW Tessellation by API and Shader ?
- It is to explore new algorithms taking advantage of HW Tessellation.
- No material, Lighting model involved.

## Design 
- A flexible design
  - It is easy to extend the capability of adding new features
- Data is independent of APP logic.
  - All Data Sources provide an unified interface. 
  - APP use this interface feed data into tessellation pipe.
  
## Control
All user control is by keyboard.

- ESC/q	: exit app.
- w		: wireframe mode.
- t     : increase tessellation factor.
- h     : apply height map.
- f     : fixed camera.
- d     : switch between diag modes.

## Demo 
Waiting for Update...

## References
-  [Tessellation in D3D11@GDC](https://www.gdcvault.com/play/1012740/Direct3D-11-In-Depth-Tutorial)
