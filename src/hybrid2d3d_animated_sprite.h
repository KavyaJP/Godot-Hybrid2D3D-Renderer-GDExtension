#ifndef HYBRID2D3D_ANIMATED_SPRITE_H
#define HYBRID2D3D_ANIMATED_SPRITE_H

#include <godot_cpp/classes/animated_sprite3d.hpp>

namespace godot
{

    class Hybrid2D3DAnimatedSprite : public AnimatedSprite3D
    {
        GDCLASS(Hybrid2D3DAnimatedSprite, AnimatedSprite3D)

    private:
        float sprite_height_px;
        float target_3d_height;

        // Helper to auto-calculate pixel size
        void _update_pixel_size();

    protected:
        static void _bind_methods();

    public:
        Hybrid2D3DAnimatedSprite();
        ~Hybrid2D3DAnimatedSprite();

        void set_sprite_height_px(float p_height);
        float get_sprite_height_px() const;

        void set_target_3d_height(float p_height);
        float get_target_3d_height() const;
    };

} // namespace godot

#endif