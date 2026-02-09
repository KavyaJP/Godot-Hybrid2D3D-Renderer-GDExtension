#ifndef HD2D_SPRITE_H
#define HD2D_SPRITE_H

#include <godot_cpp/classes/geometry_instance3d.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/color.hpp>

namespace godot
{
    class HD2DSprite : public GeometryInstance3D
    {
        GDCLASS(HD2DSprite, GeometryInstance3D)

    private:
        // Sprite properties
        Vector2 sprite_size;
        bool pixel_perfect;
        float pixel_size;
        bool billboard_enabled;
        Color modulate;
        float alpha_scissor_threshold;

        // Animation properties
        Ref<SpriteFrames> sprite_frames;
        String animation;
        int frame;
        float frame_progress;
        bool playing;
        double speed_scale;

        // Flip properties
        bool flip_h;
        bool flip_v;

        // Mesh and material
        Ref<ArrayMesh> mesh;
        Ref<StandardMaterial3D> material;

        // Internal methods
        void _update_mesh();
        void _update_material();

    protected:
        static void _bind_methods();

    public:
        HD2DSprite();
        ~HD2DSprite();

        // Size management
        void set_sprite_size(const Vector2 &p_size);
        Vector2 get_sprite_size() const;

        // Pixel perfect rendering
        void set_pixel_perfect(bool p_enabled);
        bool get_pixel_perfect() const;

        void set_pixel_size(float p_size);
        float get_pixel_size() const;

        // Billboard mode
        void set_billboard_enabled(bool p_enabled);
        bool get_billboard_enabled() const;

        // Color Modulation
        void set_modulate(const Color &p_color);
        Color get_modulate() const;

        // Alpha Scissor
        void set_alpha_scissor_threshold(float p_threshold);
        float get_alpha_scissor_threshold() const;

        // SpriteFrames Management
        void set_sprite_frames(const Ref<SpriteFrames> &p_frames);
        Ref<SpriteFrames> get_sprite_frames() const;

        // Animation Control
        void set_animation(const String &p_name);
        String get_animation() const;

        void set_frame(int p_frame);
        int get_frame() const;

        void set_playing(bool p_playing);
        bool is_playing() const;

        // Flip Control
        void set_flip_h(bool p_flip);
        bool get_flip_h() const;

        void set_flip_v(bool p_flip);
        bool get_flip_v() const;

        // Godot lifecycle
        void _ready();
        void _process(double delta);
    };

} // namespace godot

#endif // HD2D_SPRITE_H