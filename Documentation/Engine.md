# **Engine**

## **Game Engine**

Each frame is processed in Game.cpp
Framerate is limited in Game.cpp line 39.
Game will work as intented at any framerate.

### **Scenes (les state machine quoi)**

A Scene is discribe as a state. Each state can operate different updates method and drawing. The state is working with the GameObjectHandler.

### **Game Objects**

Each GameObject can be compared to a Unity GameObject and has similar properties.

### **Grid & match logic**

The Grid logic is build with multiple systems waiting for each others.
The matchFinder have function to calculate current matches on the grid in details, Current matches in the grid rapidly and can calculate future possible matches within one next turn.

---

## **Particles**

A huge system of this project is the particle system. You can create amazing effects rapidly and easily with the inGame editor.
Each particle system is a GameObject and is rendered with a vertexArray. Basically a list of points we give to SFML Renderer with a texture attached and some properties.

### **Particle Editor**

A particle editor is available when you press F6.

[Retour Doc](../README.md)
