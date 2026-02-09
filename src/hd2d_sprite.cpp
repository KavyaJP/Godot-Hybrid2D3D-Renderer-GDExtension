#include "hd2d_sprite.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/texture2d.hpp>
// ADDED: Required for PackedStringArray
#include <godot_cpp/variant/packed_string_array.hpp>

using namespace godot;

HD2DSprite::HD2DSprite()
{
    // Initialize default values
    sprite_size = Vector2(1.0, 1.0);
    pixel_perfect = false;
    pixel_size = 0.01;
    billboard_enabled = true;
    modulate = Color(1.0, 1.0, 1.0, 1.0);
    alpha_scissor_threshold = 0.5;

    // Animation defaults
    animation = "default";
    frame = 0;
    frame_progress = 0.0;
    playing = false;
    speed_scale = 1.0;

    // Flip defaults
    flip_h = false;
    flip_v = false;

    // Create mesh and material
    mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
    material = Ref<StandardMaterial3D>(memnew(StandardMaterial3D));

    // Configure material for HD-2D style rendering
    material->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA_SCISSOR);
    material->set_alpha_scissor_threshold(alpha_scissor_threshold);
    material->set_albedo(modulate);
    material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
    material->set_cull_mode(BaseMaterial3D::CULL_DISABLED);
    material->set_texture_filter(BaseMaterial3D::TEXTURE_FILTER_NEAREST);
    material->set_billboard_mode(BaseMaterial3D::BILLBOARD_ENABLED);
}

HD2DSprite::~HD2DSprite()
{
    // Cleanup handled by Ref<>
}

void HD2DSprite::_bind_methods()
{
    // SpriteFrames properties
    ClassDB::bind_method(D_METHOD("set_sprite_frames", "sprite_frames"), &HD2DSprite::set_sprite_frames);
    ClassDB::bind_method(D_METHOD("get_sprite_frames"), &HD2DSprite::get_sprite_frames);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sprite_frames", PROPERTY_HINT_RESOURCE_TYPE, "SpriteFrames"), "set_sprite_frames", "get_sprite_frames");

    // Animation Controls
    ClassDB::bind_method(D_METHOD("set_animation", "name"), &HD2DSprite::set_animation);
    ClassDB::bind_method(D_METHOD("get_animation"), &HD2DSprite::get_animation);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "animation"), "set_animation", "get_animation");

    ClassDB::bind_method(D_METHOD("set_frame", "frame"), &HD2DSprite::set_frame);
    ClassDB::bind_method(D_METHOD("get_frame"), &HD2DSprite::get_frame);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "frame"), "set_frame", "get_frame");

    ClassDB::bind_method(D_METHOD("set_playing", "playing"), &HD2DSprite::set_playing);
    ClassDB::bind_method(D_METHOD("is_playing"), &HD2DSprite::is_playing);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "is_playing");

    // Size properties
    ClassDB::bind_method(D_METHOD("set_sprite_size", "size"), &HD2DSprite::set_sprite_size);
    ClassDB::bind_method(D_METHOD("get_sprite_size"), &HD2DSprite::get_sprite_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "sprite_size"), "set_sprite_size", "get_sprite_size");

    // Pixel perfect properties
    ClassDB::bind_method(D_METHOD("set_pixel_perfect", "enabled"), &HD2DSprite::set_pixel_perfect);
    ClassDB::bind_method(D_METHOD("get_pixel_perfect"), &HD2DSprite::get_pixel_perfect);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "pixel_perfect"), "set_pixel_perfect", "get_pixel_perfect");

    ClassDB::bind_method(D_METHOD("set_pixel_size", "size"), &HD2DSprite::set_pixel_size);
    ClassDB::bind_method(D_METHOD("get_pixel_size"), &HD2DSprite::get_pixel_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pixel_size"), "set_pixel_size", "get_pixel_size");

    // Billboard properties
    ClassDB::bind_method(D_METHOD("set_billboard_enabled", "enabled"), &HD2DSprite::set_billboard_enabled);
    ClassDB::bind_method(D_METHOD("get_billboard_enabled"), &HD2DSprite::get_billboard_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "billboard_enabled"), "set_billboard_enabled", "get_billboard_enabled");

    // Visual properties
    ADD_GROUP("Visual", "");
    ClassDB::bind_method(D_METHOD("set_modulate", "color"), &HD2DSprite::set_modulate);
    ClassDB::bind_method(D_METHOD("get_modulate"), &HD2DSprite::get_modulate);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");

    ClassDB::bind_method(D_METHOD("set_alpha_scissor_threshold", "threshold"), &HD2DSprite::set_alpha_scissor_threshold);
    ClassDB::bind_method(D_METHOD("get_alpha_scissor_threshold"), &HD2DSprite::get_alpha_scissor_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "alpha_scissor_threshold", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_alpha_scissor_threshold", "get_alpha_scissor_threshold");

    // Flipping properties
    ClassDB::bind_method(D_METHOD("set_flip_h", "flip_h"), &HD2DSprite::set_flip_h);
    ClassDB::bind_method(D_METHOD("get_flip_h"), &HD2DSprite::get_flip_h);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");

    ClassDB::bind_method(D_METHOD("set_flip_v", "flip_v"), &HD2DSprite::set_flip_v);
    ClassDB::bind_method(D_METHOD("get_flip_v"), &HD2DSprite::get_flip_v);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");
}

void HD2DSprite::_update_mesh()
{
    mesh->clear_surfaces();

    // Create a quad mesh for the sprite
    PackedVector3Array vertices;
    PackedVector2Array uvs;
    PackedInt32Array indices;

    float half_width = sprite_size.x * 0.5;
    float full_height = sprite_size.y;

    // Define quad vertices (Bottom-Center Pivot)
    vertices.push_back(Vector3(-half_width, 0.0, 0));         // Bottom-Left
    vertices.push_back(Vector3(half_width, 0.0, 0));          // Bottom-Right
    vertices.push_back(Vector3(half_width, full_height, 0));  // Top-Right
    vertices.push_back(Vector3(-half_width, full_height, 0)); // Top-Left

    // UV Logic with Flipping
    // Normal: 0 to 1
    // Flip H: 1 to 0
    float left = flip_h ? 1.0 : 0.0;
    float right = flip_h ? 0.0 : 1.0;
    float bottom = flip_v ? 0.0 : 1.0;
    float top = flip_v ? 1.0 : 0.0;

    // Apply UVs matching the vertex order (BL, BR, TR, TL)
    uvs.push_back(Vector2(left, bottom));  // Bottom-Left vertex gets (Left, Bottom) UV
    uvs.push_back(Vector2(right, bottom)); // Bottom-Right vertex gets (Right, Bottom) UV
    uvs.push_back(Vector2(right, top));    // Top-Right vertex gets (Right, Top) UV
    uvs.push_back(Vector2(left, top));     // Top-Left vertex gets (Left, Top) UV

    // Two triangles to form the quad
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // Build the mesh
    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = vertices;
    arrays[Mesh::ARRAY_TEX_UV] = uvs;
    arrays[Mesh::ARRAY_INDEX] = indices;

    mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
    mesh->surface_set_material(0, material);

    set_base(mesh->get_rid());
}

void HD2DSprite::_update_material()
{
    Ref<Texture2D> current_texture;

    if (sprite_frames.is_valid() && sprite_frames->has_animation(animation))
    {
        int frame_count = sprite_frames->get_frame_count(animation);
        if (frame_count > 0)
        {
            if (frame >= frame_count)
                frame = 0;
            current_texture = sprite_frames->get_frame_texture(animation, frame);
        }
    }

    if (current_texture.is_valid())
    {
        material->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, current_texture);
    }

    if (billboard_enabled)
        material->set_billboard_mode(BaseMaterial3D::BILLBOARD_ENABLED);
    else
        material->set_billboard_mode(BaseMaterial3D::BILLBOARD_DISABLED);
}

// Size management
void HD2DSprite::set_sprite_size(const Vector2 &p_size)
{
    sprite_size = p_size;
    _update_mesh();
}

Vector2 HD2DSprite::get_sprite_size() const
{
    return sprite_size;
}

// Pixel perfect rendering
void HD2DSprite::set_pixel_perfect(bool p_enabled)
{
    pixel_perfect = p_enabled;
}

bool HD2DSprite::get_pixel_perfect() const
{
    return pixel_perfect;
}

void HD2DSprite::set_pixel_size(float p_size)
{
    pixel_size = p_size;
}

float HD2DSprite::get_pixel_size() const
{
    return pixel_size;
}

// Billboard mode
void HD2DSprite::set_billboard_enabled(bool p_enabled)
{
    billboard_enabled = p_enabled;
    _update_material();
}

bool HD2DSprite::get_billboard_enabled() const
{
    return billboard_enabled;
}

// Godot lifecycle
void HD2DSprite::_ready()
{
    _update_mesh();
    _update_material();
}

void HD2DSprite::_process(double delta)
{
    if (playing && sprite_frames.is_valid() && sprite_frames->has_animation(animation))
    {
        double speed = sprite_frames->get_animation_speed(animation);
        bool is_looping = sprite_frames->get_animation_loop(animation);
        int frame_count = sprite_frames->get_frame_count(animation);

        if (frame_count > 1)
        {
            frame_progress += speed * speed_scale * delta;

            if (frame_progress >= 1.0)
            {
                int frames_to_advance = (int)frame_progress;
                frame += frames_to_advance;
                frame_progress -= frames_to_advance;

                if (frame >= frame_count)
                {
                    if (is_looping)
                    {
                        frame = frame % frame_count;
                    }
                    else
                    {
                        frame = frame_count - 1;
                        playing = false;
                    }
                }
                _update_material();
            }
        }
    }
}

void HD2DSprite::set_modulate(const Color &p_color)
{
    modulate = p_color;
    if (material.is_valid())
    {
        material->set_albedo(modulate);
    }
}

Color HD2DSprite::get_modulate() const
{
    return modulate;
}

void HD2DSprite::set_alpha_scissor_threshold(float p_threshold)
{
    alpha_scissor_threshold = p_threshold;
    if (material.is_valid())
    {
        material->set_alpha_scissor_threshold(alpha_scissor_threshold);
    }
}

float HD2DSprite::get_alpha_scissor_threshold() const
{
    return alpha_scissor_threshold;
}

void HD2DSprite::set_sprite_frames(const Ref<SpriteFrames> &p_frames)
{
    if (sprite_frames == p_frames)
        return;

    sprite_frames = p_frames;

    if (sprite_frames.is_valid())
    {
        if (!sprite_frames->has_animation(animation))
        {
            // FIX: Use PackedStringArray instead of StringList
            PackedStringArray anim_list = sprite_frames->get_animation_names();
            if (anim_list.size() > 0)
            {
                animation = anim_list[0];
            }
        }
    }
    _update_material();
}

Ref<SpriteFrames> HD2DSprite::get_sprite_frames() const
{
    return sprite_frames;
}

void HD2DSprite::set_animation(const String &p_name)
{
    if (animation == p_name)
        return;
    animation = p_name;
    frame = 0;
    frame_progress = 0.0;
    _update_material();
}

String HD2DSprite::get_animation() const
{
    return animation;
}

void HD2DSprite::set_frame(int p_frame)
{
    frame = p_frame;
    _update_material();
}

int HD2DSprite::get_frame() const
{
    return frame;
}

void HD2DSprite::set_playing(bool p_playing)
{
    playing = p_playing;
}

bool HD2DSprite::is_playing() const
{
    return playing;
}

void HD2DSprite::set_flip_h(bool p_flip)
{
    flip_h = p_flip;
    _update_mesh(); // Rebuild mesh to swap UVs
}

bool HD2DSprite::get_flip_h() const
{
    return flip_h;
}

void HD2DSprite::set_flip_v(bool p_flip)
{
    flip_v = p_flip;
    _update_mesh();
}

bool HD2DSprite::get_flip_v() const
{
    return flip_v;
}