#include "hybrid2d3d_animated_sprite.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

Hybrid2D3DAnimatedSprite::Hybrid2D3DAnimatedSprite()
{
    // 1. HD-2D Rendering Defaults
    set_billboard_mode(BaseMaterial3D::BILLBOARD_FIXED_Y);
    set_texture_filter(BaseMaterial3D::TEXTURE_FILTER_NEAREST);
    set_alpha_cut_mode(SpriteBase3D::ALPHA_CUT_DISCARD);
    set_alpha_scissor_threshold(0.5);
    set_centered(true);

    // 2. Default Values
    sprite_height_px = 32.0;
    target_3d_height = 1.7;

    // 3. Apply Initial Calculations
    _update_pixel_size();
}

Hybrid2D3DAnimatedSprite::~Hybrid2D3DAnimatedSprite()
{
}

void Hybrid2D3DAnimatedSprite::_update_pixel_size()
{
    if (sprite_height_px > 0.0)
    {
        // FORMULA: Real World Meters / Pixel Height = Size of 1 Pixel
        double new_pixel_size = target_3d_height / sprite_height_px;
        set_pixel_size(new_pixel_size);
    }
}

void Hybrid2D3DAnimatedSprite::set_sprite_height_px(float p_height)
{
    sprite_height_px = p_height;
    _update_pixel_size();
    notify_property_list_changed();
}

float Hybrid2D3DAnimatedSprite::get_sprite_height_px() const
{
    return sprite_height_px;
}

void Hybrid2D3DAnimatedSprite::set_target_3d_height(float p_height)
{
    target_3d_height = p_height;
    _update_pixel_size();
    notify_property_list_changed();
}

float Hybrid2D3DAnimatedSprite::get_target_3d_height() const
{
    return target_3d_height;
}

void Hybrid2D3DAnimatedSprite::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_sprite_height_px", "height"), &Hybrid2D3DAnimatedSprite::set_sprite_height_px);
    ClassDB::bind_method(D_METHOD("get_sprite_height_px"), &Hybrid2D3DAnimatedSprite::get_sprite_height_px);

    ClassDB::bind_method(D_METHOD("set_target_3d_height", "height"), &Hybrid2D3DAnimatedSprite::set_target_3d_height);
    ClassDB::bind_method(D_METHOD("get_target_3d_height"), &Hybrid2D3DAnimatedSprite::get_target_3d_height);

    ADD_GROUP("Hybrid 2D3D Scaling", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprite_height_px", PROPERTY_HINT_RANGE, "1,512,1,or_greater,suffix:px"), "set_sprite_height_px", "get_sprite_height_px");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "target_3d_height", PROPERTY_HINT_RANGE, "0.1,10.0,0.01,or_greater,suffix:m"), "set_target_3d_height", "get_target_3d_height");
}