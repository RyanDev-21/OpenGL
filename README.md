# OpenGL Learning Project

A C-based OpenGL project demonstrating 3D rendering with textured cubes, custom camera system, and shader programming.

![Demo](Demo.gif)

## Features

- **3D Cube Rendering**: 12 textured cubes arranged in 3D space with rotation
- **Custom Camera System**: FPS-style camera with mouse look and smooth movement
- **Dual Texturing**: Two texture channels (container and face) with blending
- **Dynamic Shaders**: Vertex and fragment shaders with model/view/projection matrices
- **Window Controls**: Resizable window with proper viewport updates

## Controls

| Key | Action |
|-----|--------|
| `ESC` | Close window |
| `J` | Adjust texture blend |
| Mouse | Look around |
| Scroll | Zoom in/out |

## Dependencies

- GLFW3
- GLEW
- GLM (cglm)
- Glad
- STB Image

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Project Structure

```
src/
├── main.c           # Entry point, rendering loop
├── camera/          # Custom 3D camera system
├── shader/          # Shader loading and uniform management
├── rectangle/       # Rectangle/cube shaders
├── triangle/        # Triangle shaders
└── assets/          # Textures (container.jpg, awesomeface.png)
```

## Technical Details

- OpenGL 3D rendering with depth testing
- Perspective projection with configurable FOV
- View matrix with camera rotation (yaw/pitch)
- Model matrices with per-cube translation and rotation
- Texture filtering: Linear (texture1) and Nearest (texture2)
