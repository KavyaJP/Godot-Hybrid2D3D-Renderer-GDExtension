#ifndef HYBRID2D3D_ANIMATED_SPRITE_H
#define HYBRID2D3D_ANIMATED_SPRITE_H

#include <godot_cpp/classes/animated_sprite3d.hpp>

namespace godot
{

    class Hybrid2D3DAnimatedSprite : public AnimatedSprite3D
    {
        GDCLASS(Hybrid2D3DAnimatedSprite, AnimatedSprite3D)

    private:
        float sprite_height;
        String base_animation; // Root name of the animation
        String last_direction_suffix; // Store the last known direction
        String _get_direction_suffix(double signed_angle, bool &out_flip); // Internal helper to get the angle between camera and player

    protected:
        static void _bind_methods();

    public:
        Hybrid2D3DAnimatedSprite();
        ~Hybrid2D3DAnimatedSprite();

        void _process(double delta) override;

        // New Helper Property
        void set_sprite_height(float p_height);
        float get_sprite_height() const;

        void set_base_animation(const String &p_name);
        String get_base_animation() const;
    };

} // namespace godot

#endif