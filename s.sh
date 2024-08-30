#!/bin/bash

# Define the branch you are working on
BRANCH_NAME="master"

# Define the remote URL
REMOTE_URL="git@github.com:ajettesla/Previous_programs.git"

# Check the current branch
CURRENT_BRANCH=$(git branch --show-current)

# Function to commit and push
commit_and_push() {
    echo "Committing changes..."
    git add .
    git commit -m "Initial commit"
    echo "Pushing to remote repository..."
    git push --set-upstream origin "$BRANCH_NAME"
}

# Check if the branch exists and has commits
if [ "$CURRENT_BRANCH" = "$BRANCH_NAME" ]; then
    # Check if there are any commits on the branch
    if git rev-parse --quiet --verify "$BRANCH_NAME" > /dev/null; then
        echo "Branch $BRANCH_NAME exists and has commits."
        echo "Pushing changes..."
        git push --set-upstream origin "$BRANCH_NAME"
    else
        echo "Branch $BRANCH_NAME exists but has no commits."
        commit_and_push
    fi
else
    echo "You are on branch $CURRENT_BRANCH."
    # Check if there are any commits on the current branch
    if git rev-parse --quiet --verify "$CURRENT_BRANCH" > /dev/null; then
        echo "Current branch $CURRENT_BRANCH exists and has commits."
        echo "Switching to $BRANCH_NAME and pushing..."
        git checkout -b "$BRANCH_NAME"
        commit_and_push
    else
        echo "Current branch $CURRENT_BRANCH has no commits."
        commit_and_push
    fi
fi

# Add remote if not already set
if git remote get-url origin &>/dev/null; then
    echo "Remote repository already set."
else
    echo "Adding remote repository..."
    git remote add origin "$REMOTE_URL"
fi

# Verify the remote URL
echo "Current remote URL:"
git remote -v

echo "Script execution completed."

