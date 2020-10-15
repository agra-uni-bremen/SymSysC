#!/usr/bin/awk -f

BEGIN {
    FS = ":"
}

/^     \// {
    filepath=substr($1, 6)
    files[filepath] += 1
}

END {
    for (file in files) {
        printf("file: %s → %s\n", file, files[file])
    }
}
