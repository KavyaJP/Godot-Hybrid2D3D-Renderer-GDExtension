#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# OUTPUT PATH: Updated to point to addons/hybrid2d3d/bin/
# This ensures the compiled libraries go directly into the plugin folder structure.

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "hybrid-2d-3d-demo/addons/hybrid2d3d/bin/libhybrid2d3d.{}.{}.framework/libhybrid2d3d.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "hybrid-2d-3d-demo/addons/hybrid2d3d/bin/libhybrid2d3d{}{}".format(
            env["suffix"], env["SHLIBSUFFIX"]
        ),
        source=sources,
    )

Default(library)
