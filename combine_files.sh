#!/usr/bin/awk -f

BEGIN {
    FS = ":"
    lastfile = ""
    filecount[1]
    filename[1]
}

{
    if($1 ~ /(\/usr\/lib)|(klee_src)/ ) {
        filepath = "external"
    } else {
        filepath = $0
    }
    if(filepath == lastfile) {
        filecount[length(filecount)] += 1
    } else {
        filecount[length(filecount)+1] = 1
        filename[length(filename)+1] = filepath
        lastfile = filepath
    }
}

END {
    for (i=1; i<=length(filename); ++i) {
        printf("%s → %s\n", filename[i], filecount[i])
    }
}
