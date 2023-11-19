# Run with: python3 build.py number_of_frames frames_per_second
import imageio
import os
import platform
import sys

OUTPUT_FILE_NAME = "output_animation.gif"
FRAMES_DIR = "frames"
num_frames = 2
fps = 2

# input validation
if len(sys.argv) < 2:
    print("Usage: python build.py number_of_frames frames_per_second")
    print("No value provided for number of frames. Using default: ", num_frames)
else:
    num_frames = int(sys.argv[1])

if len(sys.argv) < 3:
    print("No value provided for frames per second. Using default: ", fps)
else:
    fps = int(sys.argv[2])

if ((not isinstance(num_frames, int)) or num_frames < 1):
    print("Invalid value provided for number of frames: ", num_frames)
    num_frames = 2
    print("Using default number of frames: ", num_frames)

if ((not isinstance(fps, int)) or fps <= 0):
    print("Invalid value provided for frames per second: ", fps)
    fps = 2
    print("Using default frames per second: ", fps)

# create directory to store frames
if not os.path.exists(FRAMES_DIR):
    os.makedirs(FRAMES_DIR)

def convert_ppm_sequence_to_gif():
    frames = []
    for i in range(num_frames):
        image_filename = os.path.join(FRAMES_DIR, f'output_{i}.ppm')
        if os.path.exists(image_filename):
            frames.append(imageio.v2.imread(image_filename))
    imageio.mimsave(OUTPUT_FILE_NAME, frames, duration=(1000/fps))

# (1)==================== COMMON CONFIGURATION OPTIONS ======================= #
COMPILER="g++ -std=c++17"   # The compiler we want to use 
                                #(You may try g++ if you have trouble)
SOURCE="./src/*.cpp"    # Where the source code lives
EXECUTABLE="project"        # Name of the final executable
# ======================= COMMON CONFIGURATION OPTIONS ======================= #

# (2)=================== Platform specific configuration ===================== #
# For each platform we need to set the following items
ARGUMENTS=""            # Arguments needed for our program (Add others as you see fit)
INCLUDE_DIR=""          # Which directories do we want to include.
LIBRARIES=""            # What libraries do we want to include

if platform.system()=="Linux":
    ARGUMENTS="-D LINUX" # -D is a #define sent to preprocessor
    INCLUDE_DIR="-I ./include/ -I ./../common/thirdparty/glm/"
    LIBRARIES="-lSDL2 -ldl"
elif platform.system()=="Darwin":
    ARGUMENTS="-D MAC" # -D is a #define sent to the preprocessor.
    INCLUDE_DIR="-I ./include/ -I/Library/Frameworks/SDL2.framework/Headers -I./../common/thirdparty/old/glm"
    LIBRARIES="-F/Library/Frameworks -framework SDL2"
elif platform.system()=="Windows":
    COMPILER="g++ -std=c++17" # Note we use g++ here as it is more likely what you have
    ARGUMENTS="-D MINGW -std=c++17 -static-libgcc -static-libstdc++" 
    INCLUDE_DIR="-I./include/ -I./../common/thirdparty/old/glm/"
    EXECUTABLE="project.exe"
    LIBRARIES="-lmingw32 -lSDL2main -lSDL2 -mwindows -mconsole"
# (2)=================== Platform specific configuration ===================== #

# (3)====================== Building the Executable ========================== #
# Build a string of our compile commands that we run in the terminal
compileString=COMPILER+" "+ARGUMENTS+" -o "+EXECUTABLE+" "+" "+INCLUDE_DIR+" "+SOURCE+" "+LIBRARIES
# Print out the compile string
# This is the command you can type
print("============v (Command running on terminal) v===========================")
print("Compilng on: "+platform.system())
print(compileString)
print("========================================================================")
# Run our command
compile_result = os.system(compileString)
# ========================= Building the Executable ========================== #

# If compilation failed, exit build.py
if compile_result != 0:
    print("Compilation failed. Exiting...")
    sys.exit(1)

# If executable exists, run the program to produce .ppm frames
if os.path.exists(EXECUTABLE):
    if platform.system() == "Windows":
        os.system(f'{EXECUTABLE} {num_frames}')
    else:
        os.system(f'./{EXECUTABLE} {num_frames}')

# Convert .ppm frames to .gif
convert_ppm_sequence_to_gif()

# Why am I not using Make?
# 1.)   I want total control over the system. 
#       Occassionally I want to have some logic
#       in my compilation process (like searching for missing files).
# 2.)   Realistically our projects are 'small' enough 
#       this will not matter.
# 3.)   Feel free to implement your own make files or autogenerate it from this
#       script
# 4.)   It is handy to know Python

