#include "hd2d_sprite.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

HD2DSprite::HD2DSprite()
{
    // Initialize default values
    sprite_size = Vector2(1.0, 1.0);
    pixel_perfect = false;
    pixel_size = 0.01;
    billboard_enabled = true;

    // Create mesh and material
    mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
    material = Ref<StandardMaterial3D>(memnew(StandardMaterial3D));

    // Configure material for sprite rendering
    material->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
    material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
    material->set_cull_mode(BaseMaterial3D::CULL_DISABLED); // Double-sided
    material->set_billboard_mode(BaseMaterial3D::BILLBOARD_ENABLED);
}

HD2DSprite::~HD2DSprite()
{
    // Cleanup handled by Ref<>
}

void HD2DSprite::_bind_methods()
{
    // Texture properties
    ClassDB::bind_method(D_METHOD("set_texture", "texture"), &HD2DSprite::set_texture);
    ClassDB::bind_method(D_METHOD("get_texture"), &HD2DSprite::get_texture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");

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
}

void HD2DSprite::_update_mesh()
{
    mesh->clear_surfaces();

    // Create a quad mesh for the sprite
    PackedVector3Array vertices;
    PackedVector2Array uvs;
    PackedInt32Array indices;

    float half_width = sprite_size.x * 0.5;
    float half_height = sprite_size.y * 0.5;

    // Define quad vertices (centered at origin)
    vertices.push_back(Vector3(-half_width, -half_height, 0));
    vertices.push_back(Vector3(half_width, -half_height, 0));
    vertices.push_back(Vector3(half_width, half_height, 0));
    vertices.push_back(Vector3(-half_width, half_height, 0));

    // UV coordinates
    uvs.push_back(Vector2(0, 1)); // Bottom-left
    uvs.push_back(Vector2(1, 1)); // Bottom-right
    uvs.push_back(Vector2(1, 0)); // Top-right
    uvs.push_back(Vector2(0, 0)); // Top-left

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

    // Set the mesh to this GeometryInstance3D
    set_base(mesh->get_rid());
}

void HD2DSprite::_update_material()
{
    if (texture.is_valid())
    {
        material->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, texture);
    }

    // Update billboard mode
    if (billboard_enabled)
    {
        material->set_billboard_mode(BaseMaterial3D::BILLBOARD_ENABLED);
    }
    else
    {
        material->set_billboard_mode(BaseMaterial3D::BILLBOARD_DISABLED);
    }
}

// Texture management
void HD2DSprite::set_texture(const Ref<Texture2D> &p_texture)
{
    texture = p_texture;
    _update_material();
}

Ref<Texture2D> HD2DSprite::get_texture() const
{
    return texture;
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
    // Future: Add pixel-perfect snapping logic here
}