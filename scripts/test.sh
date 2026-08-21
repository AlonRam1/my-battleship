#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

pkill my-game-server

kitty bash -c "\"$PROJECT_DIR/build/server/my-game-server\" \"$arg\"; exec bash" &
sleep 1 
kitty bash -c "\"$PROJECT_DIR/build/client/my-game-client\" \"$arg\"; exec bash" &
kitty bash -c "\"$PROJECT_DIR/build/client/my-game-client\" \"$arg\"; exec bash"



