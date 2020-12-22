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
        printf("%s: %s\n", files[file], file)
    }
}
