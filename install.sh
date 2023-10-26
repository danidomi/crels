#!/bin/bash

# Define the application name
app_name="crels"

# repo
branch="main"

# List of dependencies
dependencies=("gcc" "curl" "docker")

# Loop through the dependencies
for dependency in "${dependencies[@]}"; do
    if ! command -v "$dependency" &> /dev/null; then
        echo "$dependency is not installed. Please install it first."
        exit 1
    fi
done

curl -o "$app_name.zip" -sLJO "https://github.com/danidomi/$app_name/archive/refs/heads/$branch.zip"

# Unzip it
unzip "$app_name".zip

# cd a directory
cd "$app_name"-"$branch"

# Compile the main.c and src files
src=$(find src -type f -name "*.c")
gcc -o "$app_name" *.c $src -w

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Move the executable to a directory in your PATH (e.g., /usr/local/bin)
    mkdir -p ~/.local/bin
    mv "$app_name" ~/.local/bin/"$app_name"
else
    echo "Compilation failed. Please check for errors in your C code."
fi

rm -rf "$app_name"

export PATH="$HOME/.local/bin:$PATH"