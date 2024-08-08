set_xmakever("2.9.3")

set_project("PixLRenderer")
set_version("1.0.0")

set_allowedplats("windows", "linux")
set_allowedarchs("windows|x64", "linux|x64", "linux|x86_64")

add_rules("mode.debug", "mode.release")
set_languages("cxx20")
set_optimize("fastest")

local outputdir = "$(mode)-$(os)-$(arch)"

add_requires("glad v0.1.36", "glfw 3.4", "spdlog v1.9.0")

target("PixLRenderer")
    set_kind("static")

    set_targetdir("build/" .. outputdir .. "/PixLRenderer/bin")
    set_objectdir("build/" .. outputdir .. "/PixLRenderer/obj")

    add_files("PixLRenderer/Source/**.cpp")
    add_headerfiles("PixLRenderer/Include/**.hpp", "PixLRenderer/Include/**.inl")
    add_includedirs("PixLRenderer/Include/", {public = true})
    
    set_pcxxheader("PixLRenderer/Include/PixLRenderer/pch.hpp")

    add_packages("glad", "glfw", "spdlog", {public = true})
    
target("TestApplication")
    set_kind("binary")

    set_targetdir("build/" .. outputdir .. "/TestApplication/bin")
    set_objectdir("build/" .. outputdir .. "/TestApplication/obj")

    add_files("TestApplication/Source/**.cpp")
    add_headerfiles("TestApplication/Include/**.hpp")
    add_includedirs("TestApplication/Include/")
    
    add_deps("PixLRenderer")