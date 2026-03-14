# hell2077 — learning OpenGL in C++
*(the name is inspired by livinamuk, even tho it's not a game engine, lol)*

I started this with a little C++ knowledge. This repo tracks
my progress through learnopengl.com, chapter by chapter.
Each folder is a self-contained stage. Compile and run
any of them independently.|

---

## How to run any stage

1. Open the folder in Visual Studio
2. Build `main.cpp` 
3. Run — a window should open

Each stage is standalone. No shared dependencies between folders.

---

## What I've figured out so far

**VAO / VBO** — the VAO remembers how your vertex data
is laid out. The VBO holds the actual data. You bind the
VAO once, and OpenGL knows how to read your buffer from
that point on.

**EBO** — lets you reuse vertices instead of repeating them.
Two triangles that share an edge only need 4 vertices, not 6.

**Vertex shader** — runs per vertex. Moves points into
screen space using matrix math. This is where linear algebra
lives in graphics.

**Fragment shader** — runs per pixel. Takes interpolated
values from the vertex shader and outputs a color. The
`out` in one becomes the `in` in the other.

**Uniforms** — how your C++ code talks to the shader at
runtime. Set them with `glUniform*` after calling
`glUseProgram`.

---

## Stack

- C++ — learning as I go
- OpenGL 4.6
- GLFW — window and input
- GLAD — loads OpenGL functions
- Visual Studio 2022

---

## Reference

learnopengl.com by Joey de Vries
