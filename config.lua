--
-- COMP3214 Coursework configuration
--
-- Name: Manibharathi Periasamymanjula
--
print("Config.lua executed")
GLFW = 3 -- use 2 (glfw 2), 3 (glfw 3) or 1 (freeglut, not tested yet)
GLew = true -- must use this.
IMAGE = 'stb_image' -- or "IMAGE = 'SOIL'" to  use the SOIL library
STD = 'c++11'
BOOST = false -- nil, false or true.
BULLET = false -- nil, false or true.
MODEL = false -- nil, false or true.
GUI = nil -- ImGui or  nil
FILES = nil -- nil or list of files. nil is equivalent to {'*.cpp'}.