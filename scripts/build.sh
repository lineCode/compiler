
#======================================================#
#                   Script Variables                   #
#======================================================#

# Echo colors
RED='\033[0;31m'
LIGHT_BLUE='\033[1;36m'
NO_COLOR='\033[0m'

# Variables
SCRIPT_NAME="$0"
ANTLR="java -Xmx500M org.antlr.v4.Tool"
LANGUAGE="C"
GRAMMAR="$LANGUAGE.g4"
TARGET="Java"
OUTPUT_DIR="generated"
SAMPLE_C_FILE="hello_world.c"

# Flags
GENERATE_PARSE_TREE=false
COMPILE=false

#======================================================#
#                     Variable Paths                   #
#======================================================#

JAVA_JDK_PATH="C:/Program Files/Java/jdk-9/bin"
ANTLR_JAR_PATH="C:/Users/JP/Desktop/compiler/libs"
ANTLR_JAR_FILE_NAME="antlr-4.7-complete.jar"
GRAMMAR_PATH="grammars"
SAMPLES_PATH="samples"

# Set up class path for the Java compiler
export CLASSPATH=".;$JAVA_JDK_PATH;$ANTLR_JAR_PATH/$ANTLR_JAR_FILE_NAME;$OUTPUT_DIR;"

#======================================================#
#                    Helper Functions                  #
#======================================================#

function print_menu()
{
    echo "----------------------------------------------------------------------------------"
    echo "| This script generates parse trees for a sample source file                     |"
    echo "| 1. Autogenerates the ANTLR Java target for the specified grammar file          |"
    echo "| 2. Compiles the autogenerated code                                             |"
    echo "| 3. Runs the ANTLR Java test rig to display the parse tree                      |"
    echo "----------------------------------------------------------------------------------"
    echo "| Option | Explanation                                                           |"
    echo "----------------------------------------------------------------------------------"
    echo "|   -c   | If specified, compile the autogenerated code                          |"
    echo "|   -g   | Specify the grammar file                                              |"
    echo "|   -f   | Specify the sample C file to parse                                    |"
    echo "|   -t   | Specify the target language (Cpp | Java)                              |"
    echo "|   -p   | If specified, generates the parse tree diagram                        |"
    echo "----------------------------------------------------------------------------------"
    echo ""
}

function path_checks()
{
    # Map path names to path values
    declare -A ALL_PATHS
    ALL_PATHS=(
        ["JAVA_JDK_PATH"]="$JAVA_JDK_PATH"
        ["ANTLR_JAR_PATH"]="$ANTLR_JAR_PATH"
        ["GRAMMAR_PATH"]="$GRAMMAR_PATH"
        ["SAMPLES_PATH"]="$SAMPLES_PATH"
    )

    for path in "${!ALL_PATHS[@]}"; do
        if [ ! -d "${ALL_PATHS[$path]}" ]; then
            echo -e "${RED}[ERROR] The path chosen for \$$path does not exist : ${ALL_PATHS[$path]}"
            print_menu
            exit
        fi
    done
}

function parse_args()
{
    while (( "$#" )); do
        case $1 in
            "-c" )
                shift
                COMPILE=true
                ;;
            "-g" )
                shift
                GRAMMAR=$1
                ;;
            "-f" )
                shift
                SAMPLE_C_FILE=$1
                ;;
            "-t" )
                shift
                TARGET=$1
                ;;
            "-p" )
                shift
                GENERATE_PARSE_TREE=true
                ;;
              *  )
                echo -e "${RED}[ERROR] Unrecognized argument : $var"
                print_menu
                exit
                ;;
        esac
        shift;
    done

    echo "Executing script    : $SCRIPT_NAME"
    echo "Chosen grammar file : $GRAMMAR"
    echo "Language selected   : $TARGET"
    echo "Output directory    : $OUTPUT_DIR"
    echo "Sample source file  : $SAMPLE_C_FILE"
    echo ""
}

function run_script()
{
    echo "Generating lexer and parser code..."
    $ANTLR $GRAMMAR_PATH/$GRAMMAR -Dlanguage=$TARGET -visitor -o $OUTPUT_DIR

    if [ "$TARGET" == "Java" ]; then

        # Compile autogenerated code
        if [ "$COMPILE" == true ]; then
            echo "Compiling java code..."
            javac $OUTPUT_DIR/*.java -d $OUTPUT_DIR/java
        fi

        if [ "$GENERATE_PARSE_TREE" == true ]; then
            # Generate parse tree
            echo "Generating parse tree..."
            java org.antlr.v4.gui.TestRig $LANGUAGE compilationUnit -gui < $SAMPLES_PATH/$SAMPLE_C_FILE
        fi

    elif [ "$TARGET" == "Cpp" ]; then

        # Compile autogenerated code
        if [ "$COMPILE" == true ]; then

            # Glob header directories and combine into one string
            # Does not work if directories have spaces in them but not the case here
            ALL_INCLUDES_STRING=""
            INCLUDE_DIRS=(`find antlr4-runtime -type d`)
            for dir in "${INCLUDE_DIRS[@]}"; do
                echo $dir
                ALL_INCLUDES_STRING="$ALL_INCLUDES_STRING -I$dir "
            done

            # Compile autogenerated code
            echo "Compiling C++ code..."
            GPP_COMMAND="g++ -std=c++11 -w -fPIC $ALL_INCLUDES_STRING $OUTPUT_DIR/*.cpp -c"
            echo "$GPP_COMMAND"
            $GPP_COMMAND
            
        fi

    else

        echo -e "${RED}[ERROR] The language chosen is not recognized : $TARGET"
    
    fi
}

#======================================================#
#                     Execute Script                   #
#======================================================#

# Sanity check
path_checks

# Print pretty menu
print_menu

# Parser command line arguments
parse_args "$@"

# Execute antlr, java compiler, then antlrs diagram generator
run_script
