# 🚗 2D Top-Down Car Racing Game

A simple yet engaging **2D top-down car racing game** built using **C++**, **OpenGL**, and **GLUT**.  
This project was developed as part of the **Computer Graphics Lab (CSE422)** at **Daffodil International University**.

---

## 📖 Project Overview
This game simulates a moving road environment where the player controls a car, avoids enemy vehicles, and scores points by surviving longer.  
It demonstrates fundamental computer graphics concepts such as:
- Orthographic projection
- Geometric primitives
- DDA line drawing algorithm
- Real-time animation
- Collision detection
- Event-driven programming

---

## 🎮 Features
- **Player Control**: Move left/right using arrow keys.
- **Enemy Cars**: Randomly generated with varying speeds and lane positions.
- **Collision Detection**: Axis-Aligned Bounding Box (AABB) logic.
- **Scoring System**: Points increase as enemies pass without collision.
- **Restart Mechanism**: Press `R` to restart after game over.
- **Graphics**: Road, lane markings, cars, trees, and background rendered using OpenGL primitives.
- **Animation**: Smooth motion via GLUT timer functions (~16ms updates).

---

## 🛠️ Tech Stack
- **Language**: C++
- **Graphics Library**: OpenGL
- **Utility Toolkit**: GLUT
- **Algorithms**: DDA Line Drawing, AABB Collision Detection

---

## 📐 System Design
The project follows a modular design:
- **Input Module**: Handles keyboard events.
- **Game Logic Module**: Manages movement, spawning, scoring, and collisions.
- **Rendering Module**: Draws all game objects using OpenGL primitives.
- **Animation Module**: Timer-based updates for smooth gameplay.

---

## 🖥️ User Interface
- **Player Car**: Colored rectangle with windshield.
- **Enemy Cars**: Differently colored rectangles.
- **Road & Lanes**: Gray road with dashed white lane markings (DDA algorithm).
- **Background**: Green grass and scrolling trees.
- **Score Display**: Top-left corner.
- **Game Over Message**: Center screen with restart instructions.

---

## 📊 Performance
- Lightweight and efficient 2D rendering.
- Double buffering prevents flickering.
- Optimized enemy spawning and background scrolling.
- Runs smoothly on standard personal computers.

---

## ✅ Project Outcomes
- Fully functional 2D car racing game.
- Practical implementation of graphics algorithms and animation.
- Hands-on experience with OpenGL and GLUT.
- Academic project suitable for demonstrations and evaluations.

---

## ⚠️ Limitations
- Basic shapes and solid colors (no textures or sprites).
- No sound or background music.
- Simple enemy behavior.

---

## 🚀 Future Work
- Add textured graphics and sprites.
- Integrate sound effects and background music.
- Introduce multiple levels and difficulty modes.
- Extend to 3D graphics with shaders.

---

## 👨‍💻 Authors
- **Dip Sarker**
- **Md. Sakib Khan**
- **Mahadi Hasan**

---

## 📜 License
This project is developed for academic purposes.  
Feel free to use and modify for learning and educational demonstrations.
