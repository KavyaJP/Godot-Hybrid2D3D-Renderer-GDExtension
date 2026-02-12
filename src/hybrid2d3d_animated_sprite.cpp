#include "hybrid2d3d_animated_sprite.h"
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

Hybrid2D3DAnimatedSprite::Hybrid2D3DAnimatedSprite()
{
    set_billboard_mode(BaseMaterial3D::BILLBOARD_FIXED_Y);
    set_texture_filter(BaseMaterial3D::TEXTURE_FILTER_NEAREST);
    set_alpha_cut_mode(SpriteBase3D::ALPHA_CUT_DISCARD);
    set_alpha_scissor_threshold(0.5);
    set_centered(true);

    sprite_height = 32.0;
    set_offset(Vector2(0, sprite_height * 0.5));

    // Default base animation
    base_animation = "idle";
    last_direction_suffix = "_s";
}

Hybrid2D3DAnimatedSprite::~Hybrid2D3DAnimatedSprite()
{
}

void Hybrid2D3DAnimatedSprite::set_sprite_height(float p_height)
{
    sprite_height = p_height;
    Vector2 current = get_offset();
    set_offset(Vector2(current.x, sprite_height * 0.5));
}

float Hybrid2D3DAnimatedSprite::get_sprite_height() const
{
    return sprite_height;
}

// NEW: Set the base animation (e.g., "walk")
void Hybrid2D3DAnimatedSprite::set_base_animation(const String &p_name)
{
    base_animation = p_name;
    // We don't force update here; _process will pick it up next frame
}

String Hybrid2D3DAnimatedSprite::get_base_animation() const
{
    return base_animation;
}

void Hybrid2D3DAnimatedSprite::_process(double delta)
{
    // 1. Get Camera and Parent
    Camera3D *camera = get_viewport()->get_camera_3d();
    Node3D *parent = Object::cast_to<Node3D>(get_parent());
    if (!camera || !parent)
        return;

    // 2. Calculate Vectors (Flattened to XZ plane)
    Vector3 cam_pos = camera->get_global_position();
    Vector3 parent_pos = parent->get_global_position();

    // Vector from Player to Camera
    Vector3 to_camera = (cam_pos - parent_pos).normalized();

    // Player's Forward Vector (-Z is forward in Godot)
    Vector3 forward = -parent->get_global_transform().basis.get_column(2).normalized();

    // 3. Calculate Signed Angle
    // atan2(x, z) gives angle from North (Z-axis).
    // We want the relative angle between where player is facing vs where camera is.
    float angle_to_cam = atan2(to_camera.x, to_camera.z);
    float angle_player = atan2(forward.x, forward.z);

    float angle = angle_to_cam - angle_player;

    // Normalize to -PI to +PI
    if (angle < -Math_PI)
        angle += 2 * Math_PI;
    if (angle > Math_PI)
        angle -= 2 * Math_PI;

    // 4. Get Target Suffix and Flip State
    bool should_flip = false;
    String suffix = _get_direction_suffix(angle, should_flip);

    // 5. Construct Target Animation Name
    String target_anim = base_animation + suffix;

    // 6. MIRRORING LOGIC:
    // If the target animation (e.g., "walk_w") doesn't exist...
    Ref<SpriteFrames> frames = get_sprite_frames();
    if (frames.is_valid())
    {
        if (!frames->has_animation(target_anim))
        {
            // ...check if the OPPOSITE animation exists (e.g., "walk_e")
            String mirrored_anim = "";
            bool force_flip = false;

            if (suffix == "_w")
            {
                mirrored_anim = base_animation + "_e";
                force_flip = true;
            }
            else if (suffix == "_nw")
            {
                mirrored_anim = base_animation + "_ne";
                force_flip = true;
            }
            else if (suffix == "_sw")
            {
                mirrored_anim = base_animation + "_se";
                force_flip = true;
            }

            // If the mirror exists, use it!
            if (mirrored_anim != "" && frames->has_animation(mirrored_anim))
            {
                target_anim = mirrored_anim;
                should_flip = force_flip;
            }
        }
    }

    // 7. Apply Animation & Flip
    if (frames.is_valid() && frames->has_animation(target_anim))
    {
        if (get_animation() != target_anim)
        {
            play(target_anim);
        }

        // Only apply flip if it's different (avoids unnecessary updates)
        if (is_flipped_h() != should_flip)
        {
            set_flip_h(should_flip);
        }
    }
}

// Helper to calculate raw direction based on angle
String Hybrid2D3DAnimatedSprite::_get_direction_suffix(double angle, bool &out_flip)
{
    double deg = Math::rad_to_deg(angle);
    out_flip = false; // Default: Don't flip

    // Note: The angles might need fine-tuning depending on your exact camera setup
    // But generally: 0 = Back (North), 180 = Front (South)

    // Front (South)
    if (deg >= 157.5 || deg < -157.5)
        return "_s";

    // Front-Right (South-East)
    if (deg >= 112.5 && deg < 157.5)
        return "_se";

    // Right (East)
    if (deg >= 67.5 && deg < 112.5)
        return "_e";

    // Back-Right (North-East)
    if (deg >= 22.5 && deg < 67.5)
        return "_ne";

    // Back (North)
    if (deg >= -22.5 && deg < 22.5)
        return "_n";

    // Back-Left (North-West)
    if (deg >= -67.5 && deg < -22.5)
        return "_nw";

    // Left (West)
    if (deg >= -112.5 && deg < -67.5)
        return "_w";

    // Front-Left (South-West)
    if (deg >= -157.5 && deg < -112.5)
        return "_sw";

    return "_s";
}

void Hybrid2D3DAnimatedSprite::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_sprite_height", "height"), &Hybrid2D3DAnimatedSprite::set_sprite_height);
    ClassDB::bind_method(D_METHOD("get_sprite_height"), &Hybrid2D3DAnimatedSprite::get_sprite_height);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprite_height", PROPERTY_HINT_NONE, "suffix:px"), "set_sprite_height", "get_sprite_height");

    // NEW: Bind the Base Animation property
    ClassDB::bind_method(D_METHOD("set_base_animation", "base_animation"), &Hybrid2D3DAnimatedSprite::set_base_animation);
    ClassDB::bind_method(D_METHOD("get_base_animation"), &Hybrid2D3DAnimatedSprite::get_base_animation);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "base_animation"), "set_base_animation", "get_base_animation");
}