--
-- COMP3214 automatic build script.
--
-- jnc 9/3/17
--
-- This may be out of date.
-- addd error trap, evaluating config.lua
--
solution "COMP3214_CW"
    configurations { "Release" }
        language "C++"
        includedirs{ "./include"}
        buildoptions {"-fpermissive"}
    project "demo"
        kind "ConsoleApp"
        GLFW = 3
        GLew = true
        IMAGE = "stb_image"
        STD = nil
        BULLET = false
        BOOST = false
        FILES = nil
        -- load 'config.lua'
        if os.isfile("config.lua") then
            if pcall(dofile, "config.lua") then
                print("No error in config.lua")
            else
                print("error interpreting config.lua\nYour configuration is not correct\nparameters set to default values")
                GLFW = 3
                GLew = true
                IMAGE = "stb"
                STD = nil
                BULLET = false
                BOOST = false
                FILES = nil
            end
            print("Loaded")
        else
            print('Default values used')
        end
        -- print out active paarameters.
        print("GLFW", GLFW)
        print('GLew', GLew)
        print("Image loader", IMAGE)
        print('C++ version', STD)
        print("Bullet", BULLET)
        if not FILES then
            FILES = {'*.cpp'}
        end
        print('Files are:')
        for i, f in ipairs(FILES) do
            print('    ', f)
        end
        print("--")
        -- c++ 11
        if STD == "gnu++11" then
            print("GNU++ 11")
            buildoptions{"-std=gnu++11"}
            print{'gnu++ 11'}
        elseif STD == "c++11" then
            print("C++ 11")
            buildoptions{"-std=c++11"}
            print{'c++ 11'}
        else
            print("Standard C++ defaults")
        end
        -- Bullet Library
        if BULLET then
            print("Using Bullet")
            links{"BulletDynamics"}
            links{"BulletCollision"}
            links{"LinearMath"}
            -- links{"BulletSoftBody"}
        end
        -- OpenGL interface to create drawing context
        if GLFW == 3 then
            print("Using GLFW V3.")
            links{"glfw3"}
        elseif GLFW == 2 then
            print("Using GLFW V2.")
            links{"glfw"}
        elseif GLFW == 1 then
            print("using freeglut")
            links{"freeglut"}
        else
            print("No GLFW guideance")
        end
        -- OpenGL function mapping
        if GLew then
            print("GLew loader")
            links{ "mingw32", "glew32", "gdi32", "opengl32", "stdc++"}
        else
            print("No alternative to GLew supported")
        end
        -- Image loading
        if IMAGE == "stb_image" then
            print("stb used as image loader")
        elseif IMAGE == "SOIL" then
            print("Using SOIL")
            links{"SOIL"}
        end
        print("Linking these files")
        for i, f in ipairs(FILES) do
        print('    ', f)
            files {f}
        end
        print("--")
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }   
