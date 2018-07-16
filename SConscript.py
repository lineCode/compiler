Import("*")
import os



#=======================================================#
#                   Helper Functions                    #
#=======================================================#

def get_all_subdirectories(root_dir):
    """
    Traverses and saves all subdirectories into a list
    @param root_dir : Starting directory to traverse
    @returns        : List of all directories
    """
    subdirs = [root_dir]
    for root, dirs, files in os.walk(root_dir):
        subdirs += [os.path.join(root, dir) for dir in dirs]
    return subdirs

#=======================================================#
#                       Constants                       #
#=======================================================#

GENERATED_DIR     = "generated"
GRAMMAR_FILE_NAME = "C"
ANLTR_STATIC_LIB  = "antlr-runtime"
LANGUAGE_NAME     = "Cmm"

AUTOGENERATED_FILES = [
    "BaseListener",
    "BaseVisitor",
    "Lexer",
    "Listener",
    "Parser",
    "Visitor",
]

# Add language name prefix
AUTOGENERATED_FILES[:] = ["{}/generated/{}{}.cpp".format(REPO_ROOT_DIR.abspath, LANGUAGE_NAME, src) for src in AUTOGENERATED_FILES]

#=======================================================#
#             Project Directories and Files             #
#=======================================================#

# Root directory of the spdlog submodule
SPDLOG_DIR = REPO_ROOT_DIR.Dir("src/modules/spdlog/include")

# Source directories that are specific to the project
PROJECT_SOURCE_DIRS = [
    "src/backend",
    "src/common",
    "src/intermediate",
    "src/utils",
]

# All header paths
CPPPATH = Flatten([
    GENERATED_DIR,
    "antlr4-runtime",
    PROJECT_SOURCE_DIRS,
])

# Discover all subdirectories for the header search paths
CPPPATH[:] = Flatten([get_all_subdirectories(REPO_ROOT_DIR.Dir(path).abspath) for path in CPPPATH] + ["src"])

# Glob all source files from the source directories
SOURCE_DIRS  = Flatten([get_all_subdirectories(REPO_ROOT_DIR.Dir(path).abspath) for path in PROJECT_SOURCE_DIRS] + ["src"])
SOURCE_FILES = Flatten([REPO_ROOT_DIR.Dir(d).glob("*.cpp") for d in SOURCE_DIRS])

# Add the spdlog directory to the header search path, after all processing is done without this directory
CPPPATH += [SPDLOG_DIR]

#=======================================================#
#                   Main Environment                    #
#=======================================================#

# C++ compile flags with warnings treated as errors
CXXFLAGS = [
    "-std=c++11",
    "-g",
    "-Wall",
    "-Werror",
    "-Wno-trigraphs",
]

# C++ compile flags with no warnings
CXXFLAGS_NO_WARNINGS = [
    "-std=c++11",
    "-g",
    "-Wno-trigraphs",
]

# Main environment
env = Environment(
    tools      = ["mingw", "g++"],
    ENV        = os.environ,
    CPPPATH    = CPPPATH,
    LINKFLAGS  = ["-static-libstdc++"],
    CXXCOMSTR  = "[Compiling $TARGET]",
    LINKCOMSTR = "[Linking $TARGET]",
    CXXFLAGS   = CXXFLAGS,
    LIBS       = [ANLTR_STATIC_LIB],
    LIBPATH    = [ANTLR_LIB_DIR],
)

def compile_without_warnings(env, cpp_file):
    """
    Compile a source file without warnings or errors
    @param env      : Environment used to compile
    @param cpp_file : *.cpp File() object
    """
    return env.Object(
        target   = os.path.splitext(cpp_file.name)[0],
        source   = cpp_file,
        CXXFLAGS = CXXFLAGS_NO_WARNINGS,
    )

def compile_with_warnings(env, cpp_file):
    """
    Compile a source file with all warnings and warnings treated as errors
    @param env      : Environment used to compile
    @param cpp_file : *.cpp File() object
    """
    return env.Object(
        target   = os.path.splitext(cpp_file.name)[0],
        source   = cpp_file,
        CXXFLAGS = CXXFLAGS,
    )

#=======================================================#
#                      Autogenerate                     #
#=======================================================#

BASH_SCRIPT_ARGUMENTS = {
    "SCRIPT"  : "scripts/build.sh",
    "GRAMMAR" : "{}.g4".format(LANGUAGE_NAME),
    "TARGET"  : "Cpp",
    "FILE"    : "{}.c".format(COMPILE),
}

ANTLR_FILES = env.Command(
    target = AUTOGENERATED_FILES,
    source = [
        REPO_ROOT_DIR.Dir("grammars").File("{}.g4".format(LANGUAGE_NAME))
    ],
    action = env.Action(
        " ".join([
            "bash",         # Call bash script
            "{SCRIPT}",     # Script name
            "-g {GRAMMAR}", # Grammar file name
            "-t {TARGET}",  # Target language
            "-f {FILE}",    # Source file to compile
        ]).format(**BASH_SCRIPT_ARGUMENTS),
        "Calling {} to autogenerate antlr code...".format(BASH_SCRIPT_ARGUMENTS["SCRIPT"]),
    ),
)

#=======================================================#
#                    Compile Sources                    #
#=======================================================#

# Compile all source files into object files
OBJECT_FILES = [   compile_with_warnings(env, cpp) for cpp in SOURCE_FILES] + \
               [compile_without_warnings(env, cpp) for cpp in ANTLR_FILES ]

# Link the object files together
LINK_EXE = env.Program(
    target  = "compiler",
    source  = OBJECT_FILES,
)

#=======================================================#
#                      SCons Set Up                     #
#=======================================================#

# Wait for bash script to finish generating files before compiling source files
Depends(OBJECT_FILES, ANTLR_FILES)

# Make sure the last builder is in the default targets list
Default(LINK_EXE)

# Always call bash script
AlwaysBuild(ANTLR_FILES)

# If given a source file to compile, run the compiler
if COMPILE is not None:
    # Run the compiler
    execute = env.Command(
        target = "execute",
        source = [LINK_EXE],
        action = "build\compiler samples/{}.c".format(COMPILE),
    )

    Default(execute)
