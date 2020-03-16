#!/bin/bash
set -e

tmux new -s mpc_ros
tmux send-keys 'echo "1"'
tmux rename-window 'First'

tmux select-window -t mpc_ros:0
tmux send-keys 'echo "2"'
