#include "hybrid2d3d_sprite.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

Hybrid2D3DSprite::Hybrid2D3DSprite()
{
    // Defaults for HD-2D look
    set_billboard_mode(BaseMaterial3D::BILLBOARD_FIXED_Y);
    set_texture_filter(BaseMaterial3D::TEXTURE_FILTER_NEAREST);
    set_alpha_cut_mode(SpriteBase3D::ALPHA_CUT_DISCARD);
    set_alpha_scissor_threshold(0.5);
    set_centered(true);

    // Default height 32px -> Offset 16px
    sprite_height = 32.0;
    set_offset(Vector2(0, sprite_height * 0.5));
}

Hybrid2D3DSprite::~Hybrid2D3DSprite()
{
}

void Hybrid2D3DSprite::set_sprite_height(float p_height)
{
    sprite_height = p_height;
    Vector2 current = get_offset();
    set_offset(Vector2(current.x, sprite_height * 0.5));
}

float Hybrid2D3DSprite::get_sprite_height() const
{
    return sprite_height;
}

void Hybrid2D3DSprite::_process(double delta)
{
    // Placeholder for 8-way directional logic
}

void Hybrid2D3DSprite::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_sprite_height", "height"), &Hybrid2D3DSprite::set_sprite_height);
    ClassDB::bind_method(D_METHOD("get_sprite_height"), &Hybrid2D3DSprite::get_sprite_height);
    
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprite_height", PROPERTY_HINT_NONE, "suffix:px"), "set_sprite_height", "get_sprite_height");
}