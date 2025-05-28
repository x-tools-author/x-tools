defineReplace(x_git_get_latest_tag) {
    # Get the last tag.
    system(git describe --abbrev=0 --tags > $$1)
    !exists($$1) {
        error("Failed to get latest git tag.")
    }
    !system(git describe --abbrev=0 --tags | grep -q '^continuous$$') {
        !system(git describe --abbrev=0 --tags | grep -q '^$$') {
            !exists($$1) {
                system(date +%Y.%m.%d > $$1)
            }
        }
    }
}

defineReplace(x_git_get_latest_commit) {
    # Get the last commit.
    system(git log -1 --pretty=%H > $$1)
    !exists($$1) {
        error("Failed to get latest git commit.")
    }
}

defineReplace(x_git_get_latest_commit_time) {
    tmp = "111222333"
    return($$tmp)
}
