#ifndef HYBRID2D3D_SPRITE_H
#define HYBRID2D3D_SPRITE_H

#include <godot_cpp/classes/sprite3d.hpp>

namespace godot
{

    class Hybrid2D3DSprite : public Sprite3D
    {
        GDCLASS(Hybrid2D3DSprite, Sprite3D)

    private:
        float sprite_height;

    protected:
        static void _bind_methods();

    public:
        Hybrid2D3DSprite();
        ~Hybrid2D3DSprite();

        void _process(double delta) override;

        // New Helper Property
        void set_sprite_height(float p_height);
        float get_sprite_height() const;
    };

} // namespace godot

#endif