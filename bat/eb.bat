@echo on
if [%1] == [] goto bldlog
    if not exist %1 goto done
    ifarch doeb %1 yy.
    vi yy
    goto done
:bldlog
    if not exist build.log goto done
    ifarch doeb build.log xx.
    vi xx
    goto done
:done
