# Godot Hybrid 2D/3D Renderer

A high-performance GDExtension for **Godot 4.x** designed to streamline the creation of **"HD-2D"** style games (mixing 2D pixel art with 3D environments). 

This plugin provides two custom nodes—`Hybrid2D3DSprite` and `Hybrid2D3DAnimatedSprite`—that solve the common headaches of using 2D sprites in a 3D world, such as scaling consistency, pivoting, and visibility culling errors.

---

## 🚀 Features

- **📏 Automatic Pixel-to-Meter Scaling** Stop manually tweaking `scale` properties! Just tell the node: *"This sprite is 32 pixels tall, and I want it to be 1.7 meters tall in the world."* The node calculates the correct pixel size automatically.

- **⚓ Smart Anchoring (Bottom-Center)** Standard 3D sprites pivot around their center, causing characters to "float" or clip into the ground.  
  **Hybrid2D3D** adds a `Bottom Center` anchor mode that ensures your character's feet always stay perfectly on the floor ($Y=0$) regardless of sprite size.

- **👁️ Flicker-Free Rendering (Custom AABB)** Solves the "vanishing sprite" bug where billboarded sprites disappear at the edge of the screen. We override the visibility box (AABB) to account for rotation, ensuring your character is always visible.

- **✨ HD-2D Defaults** Pre-configured for the pixel-art aesthetic:
  - **Billboard:** Y-Axis Enabled (Paper Mario / Octopath style)
  - **Filter:** Nearest Neighbor (Crisp pixels)
  - **Alpha:** Scissor Mode (Correct depth sorting and shadows)

---

## 📦 Installation

1. Download the latest release from the [Releases] page (or compile from source).
2. Copy the `bin/` folder and `hybrid2d3d.gdextension` file into your Godot project (e.g., `addons/hybrid2d3d/`).
3. Restart Godot.
4. You will now see `Hybrid2D3DSprite` and `Hybrid2D3DAnimatedSprite` in the "Create New Node" dialog.

---

## 🛠️ Usage

### 1. Basic Setup (Inspector)
After adding a `Hybrid2D3DAnimatedSprite` to your scene:

1. **Assign Frames:** Load your `SpriteFrames` resource standard `AnimatedSprite3D` way.
2. **Set Dimensions:**
   - `Sprite Height Px`: The height of your character in the texture (e.g., `32` or `64` px).
   - `Target 3D Height`: How tall the character should be in 3D units/meters (e.g., `1.7` m).
3. **Set Anchor:**
   - Change `Anchor Mode` to **Bottom Center**.

*Your sprite is now perfectly scaled and standing on the ground!*

### 2. Scripting (State Machines)

For precise control (e.g., in a State Machine), use the `set_pose()` API. This allows you to force a specific animation frame without fighting the internal timer logic.

```gdscript
@onready var sprite = $Hybrid2D3DAnimatedSprite

func _physics_process(delta):
    # Example: Lock to "Jump" animation, frame 2 (Apex of jump)
    sprite.set_pose("jump", 2)
```

## 🔧 API Reference

### `Hybrid2D3DSprite` (Static)
Inherits: `Sprite3D`

Used for static props, trees, or objects that do not require animation but need correct HD-2D scaling and visibility.

| Property           | Type    | Description                                                                                                                                                      |
| :----------------- | :------ | :--------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `sprite_height_px` | `float` | The height of the texture in pixels (e.g., `32.0`). Used to calculate the pixel-to-meter ratio.                                                                  |
| `target_3d_height` | `float` | The desired height of the object in 3D world units/meters (e.g., `1.7`).                                                                                         |
| `anchor_mode`      | `Enum`  | Controls the pivot point.<br>• `Center`: Standard 3D behavior.<br>• `Bottom Center`: Offsets the mesh so $(0,0,0)$ is at the bottom (feet), preventing clipping. |

---

### `Hybrid2D3DAnimatedSprite` (Animated)
Inherits: `AnimatedSprite3D`

Used for characters, enemies, or interactive objects that use `SpriteFrames`.

| Property / Method       | Type    | Description                                                                                                                                                                                      |
| :---------------------- | :------ | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `sprite_height_px`      | `float` | The height of the character in the sprite sheet in pixels.                                                                                                                                       |
| `target_3d_height`      | `float` | The desired height in 3D world units (meters).                                                                                                                                                   |
| `anchor_mode`           | `Enum`  | Controls the pivot point (`Center` or `Bottom Center`).                                                                                                                                          |
| `set_pose(anim, frame)` | `void`  | **Method.** Instantly sets the current animation and frame index, then stops playback. <br>Use this in State Machines to strictly control visuals via code (e.g., `sprite.set_pose("jump", 2)`). |

## 📝 Compiling from Source
Requirements:

- SCons
- Python 3+
- C++ Compiler (MSVC, GCC, or Clang)

## 📝 Compiling from Source

To fully install the extension (for both Editor use and Exporting), you must compile both targets.

**Requirements:**
- SCons
- Python 3+
- C++ Compiler (MSVC, GCC, or Clang)

```bash
# Clone the repo (recursive for godot-cpp)
git clone --recursive https://github.com/KavyaJP/Godot-HD2D-Renderer-GDExtension.git
cd Godot-HD2D-Renderer-GDExtension

# 1. Compile for the Editor (Debug) - REQUIRED to see nodes in Godot
scons platform=windows target=template_debug

# 2. Compile for Export (Release) - REQUIRED to export your game
scons platform=windows target=template_release

# (Replace 'windows' with 'linux' or 'macos' as needed)
```

## Credits

### Assets

Huge thanks to **Eris Esra** for the 8 Diagonal Character Pack, which allowed us to accelerate development and test 8-way directional logic without worrying about creating assets from scratch, and also made the testing more enjoyable with its amazing animations for jumping, walking, idle. Please support artists like her if you can. You can check it out [here](https://erisesra.itch.io/character-templates-pack)

### Development

Kavya Prajapati

## License

This project is licensed under [MIT License](LICENSE)