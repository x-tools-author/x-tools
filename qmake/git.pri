defineReplace(x_git_get_latest_tag) {
    tmp = $$system(git describe --abbrev=0 --tags)
    # if tmp == "" or tmp == "continuous", using date info, such as: 2025.05.29
    isEqual($$tmp, "") | isEqual($$tmp, "continuous"): {
        date =
        win32: {
            date = $$system(git log -1 --format=%ad --date=format:%Y.%m.%d)
            date = $$system(powershell -Command "$d='$$date';$d -replace '(\d{4})\.0?(\d{1,2})\.0?(\d{1,2})','$1.$2.$3'")
        } else: {
            date = $$system(git log -1 --format=%ad --date=format:%Y.%-m.%d)
        }
        return($$date)
    }

    return($$tmp)
}

defineReplace(x_git_get_latest_commit) {
    tmp = $$system(git log -1 --pretty=%H)
    return($$tmp)
}

defineReplace(x_git_get_latest_commit_time) {
    tmp = $$system(git log -1 --format='%ad' --date='format:%Y.%m.%d-%H:%M:%S')
    return($$tmp)
}
